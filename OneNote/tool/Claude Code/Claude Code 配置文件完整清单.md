
------------------------------
2026-7-30

## Claude Code 配置文件完整清单

## 1. 软件行为与控制（Settings & MCP）

* ~/.claude/settings.json（全局偏好设置）
* .claude/settings.json（项目级共享设置）
* .claude/settings.local.json（项目本地覆盖设置）
* ~/.claude.json（全局 MCP 工具配置）
* .mcp.json（项目级共享 MCP 工具配置）

## 2. 子代理与自定义技能（Agents & Skills）

* `~/.claude/agents/<agent-name>.md`（全局自定义 Agent）
* `.claude/agents/<agent-name>.md`（项目级自定义 Agent）
* `~/.claude/skills/<skill-name>/SKILL.md`（全局自定义技能）
* `.claude/skills/<skill-name>/SKILL.md`（项目级自定义技能）
* `~/.claude/commands/*.md`（全局简易斜杠命令）
* `.claude/commands/*.md`（项目级简易斜杠命令）

## 3. 指令、规范与规则（Instructions & Rules）

* `~/.claude/CLAUDE.md`（全局代码规范）
* CLAUDE.md（项目级代码规范与构建指令）
* CLAUDE.local.md（项目本地私有规范）
* `.claude/rules/*.md`（项目模块化/路径特定规则）

## 4. 自动记忆（Auto-Memory）

* `~/.claude/memory/`（跨项目长期记忆目录）
* `~/.claude/projects/<project-id>/memory/MEMORY.md`（项目动态自动记忆索引表）
* `~/.claude/projects/<project-id>/memory/*.md`（项目动态自动记忆明细笔记）
