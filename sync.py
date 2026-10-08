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
            resp.raise_for_status()
            data = resp.json()
            if "errors" in data:
                raise RuntimeError(data["errors"])
            return data
        except Exception as e:
            if attempt == retries:
                raise
            time.sleep(backoff * attempt)
    return {}

# GraphQL query to fetch recent submissions (Accepted only)
SUBMISSIONS_QUERY = """
query recentSubmissions($username: String!, $offset: Int!) {
  recentSubmissions(userSlug: $username, offset: $offset) {
    lastKey
    submissions {
      id
      titleSlug
      title
      statusDisplay
      lang
      timestamp
      timeComplexity
      memoryComplexity
      runtime
      memory
      beatRate
      __typename
    }
  }
}
"""

USERNAME = os.getenv("LEETCODE_USERNAME", "Aryanbhadani123")

def fetch_all_accepted() -> List[Dict[str, Any]]:
    all_submissions = []
    offset = 0
    while True:
        data = graphql_query(SUBMISSIONS_QUERY, {"username": USERNAME, "offset": offset})
        recent = data.get("data", {}).get("recentSubmissions", {})
        submissions = recent.get("submissions", [])
        if not submissions:
            break
        for sub in submissions:
            if sub.get("statusDisplay") == "Accepted":
                all_submissions.append(sub)
        offset = recent.get("lastKey")
        if not offset:
            break
    return all_submissions

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
        problem_dir = Path("problems") / f"{sub_id:04d}-{slug}"
        problem_dir.mkdir(parents=True, exist_ok=True)
        sol_path = problem_dir / f"solution.{ext}"
        sol_path.write_text(code)
        readme_path = problem_dir / "README.md"
        readme_content = f"""# {sub_id}. {title}\n\n- **Difficulty:** Unknown (LeetCode does not expose in this API)\n- **Language:** {lang}\n- **LeetCode URL:** https://leetcode.com/problems/{slug}/\n\n## Solution\n\n```{lang}\n{code}\n```\n\n"""
        readme_path.write_text(readme_content)
        new_ids.append(sub_id)
    if new_ids:
        tracked.update(new_ids)
        save_tracker(sorted(tracked))
        print(f"Added {len(new_ids)} new submissions.")
    else:
        print("No new accepted submissions.")

if __name__ == "__main__":
    main()
