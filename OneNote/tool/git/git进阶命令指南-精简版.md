## 1. 彻底搞懂三种 git reset 的控制范围
当你需要撤销提交或修改历史时，reset 是最核心的命令。它的三个核心参数，本质上是在控制代码在 Repository、Index/Stage 和 Working Directory 之间的流动：
```
                      [Repository]  --→  [Index/Stage]  --→  [Working Directory]
--soft 影响到：            这里
--mixed 影响到：                            这里
--hard 影响到：                                                  这里
```

* git reset --soft HEAD~1
* 作用范围：只回滚本地库（Repository）。
   * 细节效果：代码和修改完全保留。回滚掉的那次提交的代码，会自动放入 Index/Stage（已暂存状态），可以直接再次 commit。
   * 最佳场景：刚提交完发现 commit 信息写错了，或者想把多次提交合并为一次。
* git reset --mixed HEAD~1（默认参数）
* 作用范围：回滚本地库（Repository）和 暂存区（Index/Stage）。
   * 细节效果：代码修改仍然保留在硬盘上，但它们会从暂存区被踢出来，变成 Working Directory（未暂存状态）。
* git reset --hard HEAD~1
* 作用范围：全面毁灭。同时回滚 Repository、Index/Stage 和 Working Directory。
   * 细节效果：没有任何商量余地。所有没锁进本地库的修改全部被直接删除，代码彻底回到上一个版本的状态。
   * 安全提示：在 Working Directory 中未提交的代码一旦被 --hard 冲掉，无法找回。

------------------------------
## 2. 分支合并的艺术：merge vs rebase
UI 上的合并通常只有一键，但命令行能让你精准控制项目历史树（Graph）的走向。

* git merge --no-ff <branch>
* 细节：禁用“快进合并”（Fast-forward）。
   * 效果：即使目标分支没有分叉，Git 也会强制生成一个全新的 Merge Commit 节点。
   * 优势：在历史记录（git log --graph）中，能绝对清晰地看到这条特性分支从出生到消亡的完整弧线。
* git rebase <base-branch>
* 细节：改变当前分支的基底（变基）。
   * 效果：把你当前分支的 commit 先摘下来放一边，把目标分支最新的代码拉过来，再把你刚才摘下来的 commit 一个一个重新按顺序贴在最顶端。
   * 优势：消除类似 Merge branch 'main' into... 这种无意义的噪音提交，让整个大项目的历史演进图呈现一条完美的直线。
   * 黄金法则：绝不要在公共主分支（如公共的 main 或 master）上使用 rebase，只能在你自己的本地私有特性分支上使用。

------------------------------
## 3. 精准救场与空间隔离
这是高阶开发者最依赖的两个“神级”操作，用于精确复制或临时转移代码。

* git cherry-pick <commit-id>
* 细节：精准提取某一个特定的 commit。
   * 效果：不需要合并一整条分支，仅仅把另一个分支上的某一个 commit 的改动（Diff）复制过来，应用在当前分支并生成一个新的 commit。
   * 场景：你在 feature-A 分支写了一个紧急修复，现在想把这个修复同步到 production 分支，但你不想带上 feature-A 里面其他没写完的代码。
* git stash（临时储藏区）
* git stash save "说明信息"：把当前 Working Directory 和 Index/Stage 里所有改到一半、不能提交的代码一把抓走，塞进一个完全独立的堆栈空间（Stash）。你的工作区瞬间变干净，回到上次 commit 的状态。
   * git stash list：查看储藏室里现在存了多少个半成品大箱子。
   * git stash pop：把最近一次存进 Stash 的代码重新倒回你的 Working Directory，并把这个记录从储藏室删掉。
   * git stash apply stash@{1}：应用指定的第 1 号储藏记录，但不在储藏室里删除它。

------------------------------
## 4. 终极后悔药：git reflog
在 UI 界面中，如果你误删了分支或者执行了错误的 reset --hard，通常会不知所措。但在命令行里，所有的“擦除”都有迹可循。

* git reflog
* 细节：记录你这台电脑上 HEAD 指针的每一次移动。
   * 效果：不仅记录正常的 commit，连你切换分支、执行 reset、执行 rebase 的每一次动作都会被按时间顺序强制曝光。
   * 如何自救：输入 git reflog，找到你犯错前一秒钟的那个操作对应的 Hash 值（例如 HEAD@{2}），然后执行：
   
   git reset --hard <commit-id>
   
   本地项目会瞬间时光倒流，完美回到犯错前的状态。

------------------------------
## 5. 进阶代码审查与修正
用命令行做代码上线前的最后把关，比 UI 更严谨。

* git diff --cached
* 细节：对比 Index/Stage（已暂存） 与上一次 commit 之间的差异。
   * 核心用法：在你敲下 git commit 前，用它来做最后一次自我代码审查（Code Review），确保塞进快递盒里的代码没有任何错漏。
* git commit --amend
* 细节：直接修补上一次的 commit。
   * 核心用法：如果你刚刚完成了一次 commit，突然发现漏掉了一个文件，或者 commit 注释写错了。你可以把漏掉的文件 git add 到 Index/Stage，然后执行这个命令。它会把新改动无缝吸收到上一次的 commit 节点里，不会在历史记录中产生恶心的“修复错别字”等垃圾节点。

