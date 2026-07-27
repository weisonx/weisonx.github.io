## 一、Git 配置的深度定制

### 1. 全局配置优化

```bash
# 设置默认编辑器（推荐 VSCode）
git config --global core.editor "code --wait"

# 设置差异工具
git config --global diff.tool vscode
git config --global difftool.vscode.cmd "code --wait --diff $LOCAL $REMOTE"

# 设置合并工具
git config --global merge.tool vscode
git config --global mergetool.vscode.cmd "code --wait $MERGED"

# 设置别名（提高效率）
git config --global alias.co checkout
git config --global alias.br branch
git config --global alias.ci commit
git config --global alias.st status
git config --global alias.unstage 'reset HEAD --'
git config --global alias.last 'log -1 HEAD'
git config --global alias.visual '!gitk'
git config --global alias.lg "log --graph --oneline --all --decorate"

# 设置自动换行处理
git config --global core.autocrlf input  # macOS/Linux
git config --global core.autocrlf true   # Windows

# 设置颜色输出
git config --global color.ui auto
```

### 2. 项目级配置

```bash
# 为特定项目设置不同的用户名/邮箱
git config user.name "Work Name"
git config user.email "work@company.com"

# 设置特定的 SSH 密钥
git config core.sshCommand "ssh -i ~/.ssh/work_rsa"

# 忽略文件权限变化
git config core.fileMode false

# 设置代理
git config http.proxy http://proxy.company.com:8080
git config https.proxy http://proxy.company.com:8080
```

## 二、Git Hooks - 自动化工作流

### 1. 常用 Hook 示例

```bash
# 在 .git/hooks/ 目录下创建 hook 文件

# pre-commit: 提交前运行测试
#!/bin/sh
# .git/hooks/pre-commit
npm test
if [ $? -ne 0 ]; then
    echo "Tests failed, commit aborted"
    exit 1
fi

# commit-msg: 检查提交信息格式
#!/bin/sh
# .git/hooks/commit-msg
commit_msg=$(cat "$1")
if ! echo "$commit_msg" | grep -qE "^(feat|fix|docs|style|refactor|test|chore):"; then
    echo "Commit message must start with type: feat|fix|docs|style|refactor|test|chore"
    exit 1
fi

# post-merge: 合并后自动安装依赖
#!/bin/sh
# .git/hooks/post-merge
changed=$(git diff HEAD@{1} --name-only | grep "package.json")
if [ -n "$changed" ]; then
    npm install
fi
```

### 2. 使用 Husky 管理 Hooks（推荐）

```bash
# 安装 Husky
npm install husky --save-dev

# 初始化
npx husky install

# 创建 hook
npx husky add .husky/pre-commit "npm test"
npx husky add .husky/commit-msg "npx commitlint --edit $1"

# 在 package.json 中添加
{
  "scripts": {
    "prepare": "husky install"
  }
}
```

## 三、Git 高级操作技巧

### 1. 交互式 Rebase 的高级用法

```bash
# 合并多个 commit 为一个
git rebase -i HEAD~5
# 在编辑器中：
# pick abc123 First commit
# squash def456 Second commit  # 合并到上一个
# squash ghi789 Third commit   # 合并到上一个
# fixup jkl012 Fix typo        # 合并但丢弃提交信息

# 重新排序 commit
git rebase -i HEAD~4
# 调整行的顺序即可

# 拆分一个 commit 为多个
git rebase -i HEAD~3
# 将 pick 改为 edit
# 然后使用 git reset HEAD^ 撤销提交
# 再分多次 add 和 commit

# 只 rebase 特定范围的 commit
git rebase --onto main feature-branch~5 feature-branch
```

### 2. Cherry-pick 的精妙用法

```bash
# 挑选单个 commit
git cherry-pick abc123

# 挑选多个连续的 commit
git cherry-pick abc123..def456

# 挑选多个不连续的 commit
git cherry-pick abc123 def456 ghi789

# 只应用修改，不创建 commit
git cherry-pick -n abc123

# 添加 cherry-pick 标记
git cherry-pick -x abc123

# 解决冲突后继续
git cherry-pick --continue

# 放弃 cherry-pick
git cherry-pick --abort
```

### 3. Bisect - 二分查找 bug

```bash
# 开始二分查找
git bisect start

# 标记当前版本为 bad
git bisect bad

# 标记一个已知的好版本
git bisect good v1.0

# Git 会自动 checkout 中间版本
# 测试后标记
git bisect good  # 如果当前版本没问题
git bisect bad   # 如果当前版本有问题

# 找到第一个 bad commit 后
git bisect reset

# 自动化二分查找
git bisect start HEAD v1.0
git bisect run npm test
git bisect reset
```

## 四、Git 工作流高级模式

### 1. Git Flow 完整实现

```bash
# 初始化 Git Flow
git flow init

# 开始新功能
git flow feature start user-auth

# 完成功能
git flow feature finish user-auth

# 开始发布
git flow release start v1.0.0

# 完成发布
git flow release finish v1.0.0

# 开始热修复
git flow hotfix start critical-bug

# 完成热修复
git flow hotfix finish critical-bug
```

### 2. 子模块管理

```bash
# 添加子模块
git submodule add https://github.com/user/library.git libs/library

# 初始化子模块
git submodule init

# 更新子模块
git submodule update

# 递归更新所有子模块
git submodule update --init --recursive

# 克隆包含子模块的项目
git clone --recursive https://github.com/user/project.git

# 更新子模块到最新
git submodule foreach git pull origin main

# 删除子模块
git submodule deinit libs/library
git rm libs/library
```

### 3. Worktree - 同时处理多个分支

```bash
# 创建新的工作目录
git worktree add ../project-feature feature-branch

# 创建并切换到新分支的工作目录
git worktree add -b new-feature ../project-new-feature main

# 列出所有工作目录
git worktree list

# 移除工作目录
git worktree remove ../project-feature

# 清理过期的工作目录
git worktree prune
```

## 五、Git 性能优化

### 1. 大文件处理

```bash
# 安装 Git LFS
git lfs install

# 跟踪大文件类型
git lfs track "*.psd"
git lfs track "*.zip"

# 查看跟踪的文件类型
git lfs track

# 迁移现有的大文件
git lfs migrate import --include="*.psd" --everything

# 查看 LFS 状态
git lfs status
```

### 2. 仓库优化

```bash
# 压缩仓库
git gc --aggressive --prune=now

# 重新打包
git repack -a -d --depth=250 --window=250

# 清理未引用的对象
git prune

# 检查仓库大小
git count-objects -vH

# 找出大文件
git rev-list --objects --all | git cat-file --batch-check='%(objecttype) %(objectname) %(objectsize) %(rest)' | awk '/^blob/ {print $3, $4}' | sort -rn | head -10
```

## 六、调试与故障排除

### 1. 深入调试

```bash
# 查看文件的历史变更
git blame file.txt

# 忽略空白变更
git blame -w file.txt

# 查看某行代码的变更历史
git log -L 1,10:file.txt

# 查找字符串的变更历史
git log -S "functionName" -- source/

# 查找正则表达式匹配
git log -G "TODO|FIXME" -- source/

# 查看某个 commit 的完整上下文
git show abc123
```

### 2. 恢复丢失的数据

```bash
# 查看所有引用（包括已删除的）
git reflog show

# 恢复丢失的 commit
git checkout HEAD@{2}

# 恢复已删除的分支
git branch recover-branch HEAD@{3}

# 恢复已删除的 stash
git stash apply stash@{2}

# 查找丢失的对象
git fsck --lost-found
```

## 七、Git 与其他工具集成

### 1. 与 CI/CD 集成

```yaml
# .github/workflows/ci.yml
name: CI
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
        with:
          fetch-depth: 0  # 获取完整历史
      - name: Run tests
        run: |
          git log --oneline
          npm test
```

### 2. 与 IDE 集成

```json
// .vscode/settings.json
{
  "git.enableSmartCommit": true,
  "git.confirmSync": false,
  "git.autofetch": true,
  "gitlens.advanced.blame.customArguments": ["-w"],
  "gitlens.historyExplorer.enabled": true
}
```

## 八、安全最佳实践

### 1. 敏感信息管理

```bash
# 使用 .gitignore 忽略敏感文件
echo ".env" >> .gitignore
echo "*.key" >> .gitignore
echo "config/credentials.yml" >> .gitignore

# 使用 git-secrets 防止提交敏感信息
git secrets --install
git secrets --register-aws
git secrets --add 'password\s*=\s*.+'

# 清理历史中的敏感信息
git filter-branch --force --index-filter \
  "git rm --cached --ignore-unmatch .env" \
  --prune-empty --tag-name-filter cat -- --all
```

### 2. 签名验证

```bash
# 生成 GPG 密钥
gpg --full-generate-key

# 配置 Git 使用 GPG
git config --global user.signingkey YOUR_KEY_ID
git config --global commit.gpgsign true
git config --global tag.gpgsign true

# 签名提交
git commit -S -m "Signed commit"

# 签名标签
git tag -s v1.0 -m "Signed tag"

# 验证签名
git verify-commit HEAD
git verify-tag v1.0
```

## 九、终极技巧

### 1. 一键清理

```bash
# 清理所有本地已合并的分支
git branch --merged | grep -v "\*\|main\|develop" | xargs -n 1 git branch -d

# 清理远程已删除的分支
git remote prune origin

# 清理所有 stash
git stash clear

# 一键清理所有
alias git-cleanup='git branch --merged | grep -v "\*\|main\|develop" | xargs -n 1 git branch -d && git remote prune origin && git gc --aggressive'
```

### 2. 批量操作

```bash
# 批量修改历史中的作者信息
git filter-branch --env-filter '
OLD_EMAIL="old@email.com"
CORRECT_NAME="New Name"
CORRECT_EMAIL="new@email.com"
if [ "$GIT_COMMITTER_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_COMMITTER_NAME="$CORRECT_NAME"
    export GIT_COMMITTER_EMAIL="$CORRECT_EMAIL"
fi
if [ "$GIT_AUTHOR_EMAIL" = "$OLD_EMAIL" ]
then
    export GIT_AUTHOR_NAME="$CORRECT_NAME"
    export GIT_AUTHOR_EMAIL="$CORRECT_EMAIL"
fi
' --tag-name-filter cat -- --branches --tags
```
