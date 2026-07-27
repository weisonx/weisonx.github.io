## 一、远程仓库核心概念

### 术语解释
- **Remote (远程仓库)**：托管在服务器上的仓库副本，通常是 `origin`
- **Upstream (上游)**：你 fork 的原始仓库，通常命名为 `upstream`
- **Tracking Branch (跟踪分支)**：本地分支与远程分支的关联关系
- **Fetch (获取)**：从远程下载数据，但不合并
- **Pull (拉取)**：Fetch + Merge
- **Push (推送)**：将本地提交上传到远程

## 二、远程仓库管理命令

### 1. `git remote` - 管理远程连接

```bash
# 查看所有远程仓库
git remote -v
# 输出示例：
# origin  https://github.com/user/repo.git (fetch)
# origin  https://github.com/user/repo.git (push)

# 添加远程仓库
git remote add origin https://github.com/user/repo.git

# 删除远程仓库
git remote remove origin

# 重命名远程仓库
git remote rename origin upstream

# 查看远程仓库详细信息
git remote show origin
```

### 2. `git fetch` - 获取远程更新

```bash
# 获取所有远程分支的更新
git fetch origin

# 获取特定分支
git fetch origin main

# 获取所有远程仓库（如果有多个）
git fetch --all

# 获取并删除远程已不存在的分支引用
git fetch --prune
# 或简写
git fetch -p
```

### 3. `git pull` - 拉取并合并

```bash
# 基本用法（fetch + merge）
git pull origin main

# 使用 rebase 而不是 merge
git pull --rebase origin main

# 设置默认行为为 rebase
git config --global pull.rebase true

# 只拉取不合并（相当于 fetch）
git pull --no-commit

# 如果本地有未提交的修改
git stash
git pull
git stash pop
```

### 4. `git push` - 推送本地更新

```bash
# 基本推送
git push origin main

# 推送并设置 upstream
git push -u origin main
# 之后可以直接用 git push

# 推送所有分支
git push origin --all

# 推送标签
git push origin --tags

# 删除远程分支
git push origin --delete feature-branch
# 或
git push origin :feature-branch

# 强制推送（危险！）
git push --force origin main
# 更安全的强制推送
git push --force-with-lease origin main
```

## 三、分支与远程的交互

### 1. 创建远程跟踪分支

```bash
# 拉取远程分支并创建本地跟踪分支
git checkout -b local-branch origin/remote-branch

# 或使用更简洁的方式
git switch -c local-branch origin/remote-branch

# 直接 checkout 远程分支（自动创建跟踪分支）
git checkout origin/feature-branch
# Git 会提示：Switched to a new branch 'feature-branch'
```

### 2. 查看分支关系

```bash
# 查看所有分支（包括远程）
git branch -a

# 查看跟踪关系
git branch -vv

# 查看远程分支
git branch -r

# 查看哪些分支已合并到当前分支
git branch --merged

# 查看哪些分支未合并
git branch --no-merged
```

### 3. 同步远程分支

```bash
# 删除本地已合并的远程跟踪分支
git remote prune origin

# 删除本地已删除的远程分支的跟踪
git fetch -p

# 清理所有本地不存在的远程分支引用
git fetch --prune origin
```

## 四、常见远程操作场景

### 场景1：Fork 工作流

```bash
# 1. Fork 项目后，克隆自己的 fork
git clone https://github.com/your-username/project.git
cd project

# 2. 添加上游仓库
git remote add upstream https://github.com/original-owner/project.git

# 3. 查看远程仓库
git remote -v
# origin    https://github.com/your-username/project.git (fetch)
# origin    https://github.com/your-username/project.git (push)
# upstream  https://github.com/original-owner/project.git (fetch)
# upstream  https://github.com/original-owner/project.git (push)

# 4. 同步上游更新
git fetch upstream
git checkout main
git merge upstream/main
git push origin main

# 5. 创建功能分支
git checkout -b feature-branch

# 6. 提交并推送到自己的 fork
git add .
git commit -m "feat: add new feature"
git push -u origin feature-branch

# 7. 在 GitHub 上创建 Pull Request
```

### 场景2：多人协作

```bash
# 1. 拉取最新代码
git pull --rebase origin main

# 2. 创建功能分支
git checkout -b feature/login

# 3. 开发过程中，定期同步主分支
git fetch origin
git rebase origin/main

# 4. 解决冲突后继续开发
# 5. 推送功能分支
git push -u origin feature/login

# 6. 创建 PR 后，如果 review 需要修改
git add .
git commit --amend
git push --force-with-lease origin feature/login

# 7. PR 合并后，删除本地和远程分支
git checkout main
git pull
git branch -d feature/login
git push origin --delete feature/login
```

### 场景3：紧急修复线上问题

```bash
# 1. 从主分支创建 hotfix 分支
git checkout main
git pull
git checkout -b hotfix/critical-bug

# 2. 修复并提交
git add .
git commit -m "fix: critical security vulnerability"

# 3. 推送到远程
git push -u origin hotfix/critical-bug

# 4. 创建 PR 并合并到 main

# 5. 同步到其他分支
git checkout develop
git merge main
git push origin develop
```

## 五、远程操作的高级技巧

### 1. 配置多个远程仓库

```bash
# 添加第二个远程仓库
git remote add backup https://github.com/backup/repo.git

# 推送到多个远程
git push origin main
git push backup main

# 或一次性推送
git push --all origin
git push --all backup
```

### 2. 使用 SSH 密钥

```bash
# 生成 SSH 密钥
ssh-keygen -t ed25519 -C "your_email@example.com"

# 添加密钥到 SSH agent
eval "$(ssh-agent -s)"
ssh-add ~/.ssh/id_ed25519

# 添加公钥到 GitHub/GitLab

# 使用 SSH URL 克隆
git clone git@github.com:user/repo.git
```

### 3. 处理推送冲突

```bash
# 当推送被拒绝时
git push origin main
# ! [rejected] main -> main (fetch first)

# 解决方案1：先拉取再推送
git pull --rebase origin main
git push origin main

# 解决方案2：强制推送（仅在你确定时使用）
git push --force-with-lease origin main
```

### 4. 查看远程仓库的详细信息

```bash
# 查看远程仓库的 URL
git remote get-url origin

# 修改远程仓库的 URL
git remote set-url origin https://new-url.com/repo.git

# 查看远程分支的详细信息
git ls-remote origin

# 查看远程仓库的 HEAD
git remote show origin
```

## 六、最佳实践与注意事项

### 1. 推送前检查
```bash
# 检查是否有未提交的修改
git status

# 检查本地分支是否落后于远程
git fetch
git status

# 运行测试
npm test  # 或其他测试命令
```

### 2. 安全的强制推送
```bash
# 使用 --force-with-lease 而不是 --force
git push --force-with-lease origin branch-name

# 这个选项会检查远程分支是否被其他人更新过
# 如果远程分支有新的提交，推送会被拒绝
```

### 3. 避免常见错误
```bash
# 错误：直接推送到 main 分支
# 正确：使用 PR/MR 流程

# 错误：强制推送覆盖他人工作
# 正确：先沟通，使用 --force-with-lease

# 错误：推送未测试的代码
# 正确：运行测试后再推送
```

### 4. 团队协作规范
```bash
# 1. 保持提交历史清晰
git rebase -i HEAD~3  # 合并提交

# 2. 使用有意义的提交信息
git commit -m "feat: add user authentication"
git commit -m "fix: resolve login timeout issue"

# 3. 定期同步主分支
git fetch origin
git rebase origin/main

# 4. 及时删除已合并的分支
git branch -d feature-branch
git push origin --delete feature-branch
```

## 七、常用命令速查表

| 命令 | 说明 |
|------|------|
| `git remote -v` | 查看远程仓库 |
| `git fetch origin` | 获取远程更新 |
| `git pull origin main` | 拉取并合并 |
| `git push origin main` | 推送到远程 |
| `git push -u origin branch` | 推送并设置上游 |
| `git push --force-with-lease` | 安全强制推送 |
| `git remote add origin url` | 添加远程仓库 |
| `git remote remove origin` | 删除远程仓库 |
| `git fetch -p` | 获取并清理远程分支 |
| `git branch -a` | 查看所有分支 |

远程仓库操作是 Git 协作的核心，掌握了这些命令，你就能在各种团队协作场景中游刃有余。记住：**先 fetch 再 rebase，push 前检查，force push 要谨慎**。
