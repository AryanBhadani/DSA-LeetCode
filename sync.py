#!/usr/bin/env python3

"""sync.py - Sync accepted LeetCode submissions to the repository.

This script is intended to run in a GitHub Actions environment where the
following secrets are provided as environment variables:

- LEETCODE_SESSION
- LEETCODE_CSRF_TOKEN

It reads ``synced_submissions.json`` to determine which submissions have already
been imported, queries the LeetCode private API for the user's accepted
submissions, and writes any new solutions into ``problems/<id>-<slug>/``.

The script is deliberately tolerant:
* Missing fields (e.g., ``topicTags``) are handled gracefully.
* API pagination and transient network errors use simple retry logic.
* No secrets are printed or committed.
"""

import os
import json
import time
import sys
from pathlib import Path
from typing import List, Dict, Any

import requests

# Constants
BASE_URL = "https://leetcode.com/graphql"
HEADERS = {
    "Content-Type": "application/json",
    "Accept": "application/json",
    "Referer": "https://leetcode.com",
    "Origin": "https://leetcode.com",
    "User-Agent": "Mozilla/5.0 (compatible; leetcode-sync/1.0)",
    "Cookie": "",
}

# Load auth cookies from secrets
session = os.getenv("LEETCODE_SESSION")
csrf = os.getenv("LEETCODE_CSRF_TOKEN")
if not session or not csrf:
    print("LeetCode authentication secrets are missing.", file=sys.stderr)
    sys.exit(1)

HEADERS["Cookie"] = f"LEETCODE_SESSION={session}; csrftoken={csrf}"
HEADERS["x-csrftoken"] = csrf

# Helper to perform GraphQL queries with retries
def graphql_query(query: str, variables: Dict[str, Any] = None, retries: int = 3, backoff: int = 2) -> Dict[str, Any]:
    payload = {"query": query, "variables": variables or {}}
    for attempt in range(1, retries + 1):
        try:
            resp = requests.post(BASE_URL, json=payload, headers=HEADERS, timeout=10)
            # Log HTTP status (do not expose secrets)
            print(f"HTTP response status: {resp.status_code}", file=sys.stderr)
            if resp.status_code != 200:
                # Truncate response body for safety
                snippet = resp.text[:500]
                print(f"Non‑200 response body (truncated): {snippet}", file=sys.stderr)
                resp.raise_for_status()
            data = resp.json()
            if "errors" in data:
                msgs = [e.get("message", "<no message>") for e in data.get("errors", [])]
                print(f"GraphQL errors: {', '.join(msgs)}", file=sys.stderr)
                raise RuntimeError(data["errors"])
            return data
        except Exception as e:
            if attempt == retries:
                # Propagate error so the workflow fails
                raise
            time.sleep(backoff * attempt)
    return {}

# GraphQL query to fetch a page of submissions (accepted only)
SUBMISSION_LIST_QUERY = """
query submissionList($offset: Int!, $limit: Int!, $lastKey: String, $questionSlug: String) {
  submissionList(offset: $offset, limit: $limit, lastKey: $lastKey, questionSlug: $questionSlug) {
    lastKey
    hasNext
    submissions {
      id
      statusDisplay
      lang
      timestamp
      title
      titleSlug
    }
  }
}
"""

def fetch_all_accepted() -> List[Dict[str, Any]]:
    """Fetch all accepted submissions using the paginated `submissionList` query.
    Returns a flat list of submission dicts.
    """
    submissions: List[Dict[str, Any]] = []
    offset = 0
    page_size = 20
    last_key = None
    while True:
        variables = {
            "offset": offset,
            "limit": page_size,
            "lastKey": last_key,
            "questionSlug": "",
        }
        try:
            data = graphql_query(SUBMISSION_LIST_QUERY, variables)
        except Exception as e:
            print(f"Error fetching submissions page at offset {offset}: {e}", file=sys.stderr)
            break
        result = data.get("data", {}).get("submissionList")
        if result is None:
            print("Error: submissionList data missing – possible auth issue or API change", file=sys.stderr)
            break
        subs = result.get("submissions")
        if subs is None:
            print("Warning: submissions list is null – possible authentication issue or API change", file=sys.stderr)
            break
        for sub in subs:
            if sub.get("statusDisplay") == "Accepted":
                submissions.append(sub)
        if not result.get("hasNext"):
            break
        last_key = result.get("lastKey")
        offset += page_size
        time.sleep(0.5)
    return submissions

def load_tracker() -> List[int]:
    tracker_path = Path("synced_submissions.json")
    if tracker_path.is_file():
        try:
            return json.loads(tracker_path.read_text())
        except Exception:
            return []
    return []

def save_tracker(ids: List[int]):
    Path("synced_submissions.json").write_text(json.dumps(ids, indent=2))

def main():
    tracked = set(load_tracker())
    submissions = fetch_all_accepted()
    new_ids = []
    for sub in submissions:
        sub_id = sub.get("id")
        if sub_id in tracked:
            continue
        slug = sub.get("titleSlug")
        title = sub.get("title")
        lang = sub.get("lang") or "txt"
        # Map language to file extension
        ext_map = {
            "cpp": "cpp",
            "java": "java",
            "python3": "py",
            "python": "py",
            "c": "c",
            "csharp": "cs",
            "javascript": "js",
            "typescript": "ts",
            "go": "go",
            "rust": "rs",
        }
        ext = ext_map.get(lang.lower(), "txt")
        # Fetch the solution code for this submission
        code_query = """
        query submissionDetail($id: Int!) {
          submissionDetail(submissionId: $id) {
            code
            __typename
          }
        }
        """
        try:
            detail = graphql_query(code_query, {"id": sub_id})
            code = detail.get("data", {}).get("submissionDetail", {}).get("code", "")
        except Exception as e:
            print(f"Failed to fetch code for submission {sub_id}: {e}", file=sys.stderr)
            continue
        if not code:
            continue
        # Determine the official problem number (frontendQuestionId) via a secondary query
        question_number = None
        question_query = """
        query questionData($titleSlug: String!) {
          question(titleSlug: $titleSlug) {
            frontendQuestionId
            __typename
          }
        }
        """
        try:
            qdata = graphql_query(question_query, {"titleSlug": slug})
            question_number = qdata.get("data", {}).get("question", {}).get("frontendQuestionId")
        except Exception as e:
            print(f"Failed to fetch question number for {slug}: {e}", file=sys.stderr)
        # Fallback: use submission ID if we cannot get the problem number
        if not question_number:
            question_number = str(sub_id)
        # Build the problem directory name using the question number
        dir_name = f"{question_number}-{slug}"
        problem_dir = Path("problems") / dir_name
        # If the directory already exists (imported previously), we skip creating a new one
        if problem_dir.is_dir():
            # Mark this submission as synced to avoid reprocessing, but do not treat as new
            tracked.add(sub_id)
            continue
        # Ensure directory exists (may already contain previous solution)
        problem_dir.mkdir(parents=True, exist_ok=True)
        # Determine solution file path
        sol_path = problem_dir / f"solution.{ext}"
        # Decide whether we need to write (i.e., content differs)
        should_commit = True
        if sol_path.is_file():
            existing_code = sol_path.read_text()
            if existing_code == code:
                # No change in solution; skip committing for this submission
                should_commit = False
        if should_commit:
            sol_path.write_text(code)
            # Write (or overwrite) README with metadata
            readme_path = problem_dir / "README.md"
            readme_content = f"""# {question_number}. {title}\n\n- **Difficulty:** Unknown (LeetCode does not expose in this API)\n- **Language:** {lang}\n- **LeetCode URL:** https://leetcode.com/problems/{slug}/\n\n## Solution\n\n```{lang}\n{code}\n```\n\n"""
            readme_path.write_text(readme_content)
            new_ids.append(sub_id)
        else:
            # Mark as synced without creating a commit
            tracked.add(sub_id)
    # Save tracker irrespective of new submissions to ensure existing folders are recorded
    if tracked:
        save_tracker(sorted(tracked))
    if new_ids:
        print(f"Added {len(new_ids)} new submissions.")
    else:
        print("No new accepted submissions.")

if __name__ == "__main__":
    main()
