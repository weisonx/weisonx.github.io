# What I Run Before Reading Any Code

`git log --format=format: --name-only --since="1 year ago" | sort | uniq -c | sort -nr | head -20`

`git shortlog -sn --no-merges`

`git log -i -E --grep="fix|bug|broken" --name-only --format='' | sort | uniq -c | sort -nr | head -20`

`git log --format='%ad' --date=format:'%Y-%m' | sort | uniq -c`

`git log --oneline --since="1 year ago" | grep -IE 'revert|hotfix|emergency|rollback'`
