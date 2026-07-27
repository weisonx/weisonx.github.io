### 一、核心概念与术语（必须理解）

- **Working Directory (工作区)**：你当前能看到的、正在编辑的文件目录。
- **Staging Area (暂存区 / Index)**：一个中间区域，用于暂存你准备提交的修改。`git add` 就是把修改放到这里。
- **Local Repository (本地仓库)**：`.git` 目录，包含了所有版本历史和元数据。`git commit` 就是把暂存区的内容永久保存到本地仓库。
- **Remote Repository (远程仓库)**：托管在服务器（如 GitHub、GitLab）上的仓库。`git push` 和 `git pull` 就是与它交互。
- **Commit (提交)**：一个快照，记录了项目在某个时间点的状态。每个 commit 有唯一的 SHA-1 hash。
- **Branch (分支)**：一个可移动的指针，指向某个 commit。`main` 或 `master` 是默认分支。
- **HEAD**：一个特殊的指针，指向你当前所在的 branch 或 commit。它代表“你现在在哪里”。
- **Stash (贮藏)**：一个临时存储区域，用于保存工作区中未提交的修改，让你可以切换到其他分支或做其他操作，之后再恢复。

---

### 二、基础命令的进阶细节

#### 1. `git add` - 把修改放入 Staging Area

- **`git add <file>`**：暂存单个文件。
- **`git add .`**：暂存当前目录下所有**新文件、修改文件、删除文件**。
- **`git add -A`**：与 `git add .` 类似，但范围是整个工作区（包括上级目录），更推荐使用。
- **`git add -p`**（或 `--patch`）：**交互式暂存**。Git 会逐块（hunk）显示你的修改，让你选择是否暂存。这对于只提交部分修改非常有用。
  - 按 `y`暂存该块，`n` 跳过，`s` 分割成更小的块，`e` 手动编辑该块。

#### 2. `git commit` - 创建一个新的 Commit

- **`git commit -m "message"`**：最常用，直接带提交信息。
- **`git commit`**：不带 `-m` 会打开默认编辑器（如 Vim）让你写多提交信息。**这是好习惯**，因为好的提交信息应该包含标题和正文。
- **`git commit -a -m "message"`**：**跳过 Staging Area**，直接提交所有已跟踪文件的修改。**慎用**，容易误提交不需要的文件。
- **`git commit --amend`**：**修改最近一次 commit**。它会将当前 Staging Area 的内容合并到上一个 commit 中，并允许你修改提交信息。**注意：如果已经 push 到远程，需要 force push，非常危险。**

#### 3. `git status` - 查看当前状态

- **`git status`**：显示工作区和暂存区的状态。
- **`git status -s`**（或 `--short`）：**简洁模式**。输出两列，第一列是暂存区状态，第二列是工作区状态。
  - `??`：未跟踪
  - `M`：已修改
  - `A`：已暂存
  - `D`：已删除

#### 4. `git log` - 查看 Commit 历史

- **`git log`**：默认显示所有 commit 的 hash、作者、日期、信息。
- **`git log --oneline`**：一行显示一个 commit，只显示 hash 和标题。
- **`git log --graph`**：以 ASCII 图形显示分支合并历史，非常直观。
- **`git log -p`**：显示每个 commit 的详细 diff。
- **`git log --author="yourname"`**：只看某个作者的 commit。
- **`git log --since="2 weeks ago"`**：只看最近两周的 commit。

---

### 三、进阶命令与核心场景

#### 1. `git branch` - 分支管理

- **`git branch`**：列出本地所有分支，当前分支前有 `*`。
- **`git branch <branch-name>`**：创建一个新分支（基于当前 HEAD）。
- **`git branch -d <branch-name>`**：删除本地分支（安全删除，会检查是否已合并）。
- **`git branch -D <branch-name>`**：强制删除本地分支（即使未合并）。
- **`git branch -m <old-name> <new-name>`**：重命名分支。

#### 2.git checkout` / `git switch` - 切换分支或恢复文件

- **`git checkout <branch-name>`**：切换到已有分支。
- **`git checkout -b <branch-name>`**：创建并切换到新分支（等价于 `git branch + git checkout`）。
- **`git checkout -- <file>`**：**丢弃工作区中某个文件的修改**，恢复到最近一次 commit 或 staging area 的状态。**危险操作，不可恢复。**
- **`git switch <branch-name>`**：Git 2.23+ 引入的新命令，专门用于切换分支，语义更清晰。
- **`git switch -c <branch-name>`**：创建并切换。

#### 3. `git merge` - 合并分支

- **`git merge <branch-name>`**：将指定分支合并到当前分支。
- **Fast-forward merge**：如果当前分支是目标分支的直接上游，Git 会直接移动指针，不产生新的 merge commit。
- **Three-way merge**：如果两个分支有分叉，Git 会创建一个新的 merge commit，将两个分支的修改合并。
- **Merge conflict**：当两个分支修改了同一个文件的同一部分时，Git 无法自动合并，需要你手动解决冲突。解决后 `git add` 并 `git commit`。

#### 4. `git rebase` - 变基（重写历史）

- **`git rebase <branch-name>`**：将当前分支的 commits “移植”到目标分支的顶端。**核心思想：让历史更线性、更干净。**
- **交互式变基 `git rebase -i HEAD~n`**：**极其强大**。允许你编辑、合并、删除、重排最近 n 个 commits。
  - `pick`：使用该 commit
  - `reword`：修改提交信息
  - `squash`：将该 commit 合并到上一个 commit
  - `fixup`：类似 squash，但丢弃该 commit 的提交信息
  - `drop`：删除该 commit
- **Golden Rule of Rebasing**：**永远不要 rebase 已经 push 到远程仓库且别人正在使用的 commits。** 这会改变历史，导致灾难。

#### 5. `git stash` - 贮藏未完成的修改

- **`git stash`**：将工作区和暂存区的所有未提交修改保存到一个栈中，工作区恢复到干净状态。
- **`git stash pop`**：将栈顶的 stash 恢复到工作区，并从栈中移除。
- **`git stash apply`**：将栈顶的 stash 恢复到工作区，但保留在栈中（可多次应用）。
- **`git stash list`**：查看所有 stash 列表。
- **`git stash drop stash@{n}`**：删除指定的 stash。
- **`git stash save "message"`**：带描述地贮藏。

#### 6. `git reset` - 撤销操作（危险）

- **`git reset --soft HEAD~1`**：**软重置**。将 HEAD 指向上一个 commit，但**保留工作区和暂存区的修改**。相当于撤销了 commit，但修改还在暂存区。
- **`git reset --mixed HEAD~1`**（默认）：**混合重置**。将 HEAD 指向上一个 commit，**保留工作区修改，但清空暂存区**。相当于撤销了 commit 和 add。
- **`git reset --hard HEAD~1`**：**硬重置**。将 HEAD 指向上一个 commit，**同时丢弃工作区和暂存区的所有修改**。**极其危险，不可恢复。**
- **`git reset <file>`**：将文件从暂存区移除（相当于撤销 `git add`），但保留工作区修改。

#### 7. `git revert` - 安全的撤销

- **`git revert <commit-hash>`**：**创建一个新的 commit，该 commit 会撤销指定 commit 的修改。** 这是**安全**的撤销方式，因为它不改变历史，只是追加一个新的 commit。适用于已经 push 到远程的 commit。

#### 8. `git remote` - 远程仓库管理

- **`git remote -v`**：查看所有远程仓库的 URL。
- **`git remote add origin <url>`**：添加一个名为 `origin` 的远程仓库。
- **`git remote remove origin`**：移除远程仓库。
- **`git remote rename origin upstream`**：重命名远程仓库。

#### 9. `git push` / `git pull` - 与远程交互

- **`git push origin main`**：将本地 `main` 分支推送到远程 `origin` 的 `main` 分支。
- **`git push -u origin main`**：推送并设置 upstream（上游分支），之后可以直接用 `git push`。
- **`git push --force`**（或 `-f`）：**强制推送**。会覆盖远程仓库的历史。**极其危险**，除非你非常确定自己在做什么（例如，rebase 后需要强制推送）。
- **`git pull`**：等价于 `git fetch` + `git merge`。从远程拉取最新代码并合并到当前分支。
- **`git pull --rebase`**：拉取代码后使用 rebase 而不是 merge，让历史更线性。

---

### 四、实战场景与命令组合

1.  **场景：开始一个新功能**
    ```bash
    git checkout main
    git pull
    git checkout -b feature/new-feature
    ```

2.  **场景：完成一个功能，提交并推送**
    ```bash
    git add .
    git commit -m "feat: add new feature"
    git push -u origin feature/new-feature
    ```

3.  **场景：需要切换到另一个分支，但当前修改不想提交**
    ```bash
    git stash
    git checkout other-branch
    # 做完其他事情后
    git checkout feature/new-feature
    git stash pop
    ```

4.  **场景：修改最近一次 commit 的信息或内容**
    ```bash
    git add .
    git commit --amend -m "new message"
    ```

5.  **场景：合并多个 commits 为一个（交互式变基）**
    ```bash
    git rebase -i HEAD~3
    # 在编辑器中，将后两个 pick 改为 squash 或 fixup
    ```

6.  **场景：撤销一个已经 push 的 commit**
    ```bash
    git revert <commit-hash>
    git push
    ```

7.  **场景：解决合并冲突**
    ```bash
    git merge feature-branch
    # 出现冲突，编辑文件解决
    git add .
    git commit
    ```

---

### 五、学习建议

1.  **多用 `--help`**：`git <command> --help` 会打开官方文档，非常详细。
2.  **可视化工具辅助**：在命令行学习时，可以同时打开 `git log --graph --oneline --all` 或使用 `tig`、`lazygit` 等 TUI 工具，直观看到分支和 commit 的变化。
3.  **刻意练习**：创建一个测试仓库，故意制造冲突、rebase、reset 等场景，理解每个命令的效果。
4.  **理解 Git 的数据结构**：Git 本质上是一个**有向无环图 (DAG)**，每个 commit 是一个节点，分支是指针。理解这个模型，所有命令都变得清晰。

从 UI 到，最大的转变是**对操作的理解从“点击按钮”变为“执行命令”**。当你开始用 `git rebase -i` 整理历史、用 `git stash` 管理上下文、用 `git reset` 和 `git revert` 精确控制版本时，你就真正进阶了。

记住：**Git 是工具，不是魔法。** 理解了它的核心模型，任何操作都变得可控。
