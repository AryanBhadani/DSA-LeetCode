# 🛠️ Repository Setup

## 1️⃣ Create the GitHub repository
Follow the steps in the **Create the GitHub repository** section of the previous message.

## 2️⃣ Add GitHub Actions secrets
- `LEETCODE_SESSION` – copy from your browser cookies after logging into LeetCode.
- `LEETCODE_CSRF_TOKEN` – copy from the same cookie store.
- `GIT_EMAIL` – your verified GitHub email address.

## 3️⃣ Initial push
```powershell
cd C:/Users/vikas/.gemini/antigravity/scratch/DSA-LeetCode

git init

git add .

git -c user.name="AryanBhadani" -c user.email="${{ secrets.GIT_EMAIL }}" commit -m "Initial import of existing LeetCode solutions"

git remote add origin https://github.com/AryanBhadani/DSA-LeetCode.git

git push -u origin main
```

## 4️⃣ How the sync works
The `sync.py` script:
- Reads `synced_submissions.json` to know which LeetCode IDs are already imported.
- Calls LeetCode’s private GraphQL API (authenticated with the two cookies) to fetch **all** accepted submissions.
- Saves each new solution under `problems/<id>-<slug>/solution.<ext>` and creates a minimal `README.md` with metadata.
- Updates `synced_submissions.json`.

The GitHub Actions workflow runs this script every **6 hours** (cron `0 */6 * * *`) and can also be triggered manually via the **Run workflow** button.

## 5️⃣ No empty commits
The workflow only commits when `git diff-index --quiet HEAD` reports changes, guaranteeing that your contribution graph reflects real work.

---

Happy coding! 🎉
