# Git 常用命令速查表

## 1. 概述

Git 分布式版本控制系统的常用命令快速参考。

## 2. 核心语法

### 配置

```bash
git config --global user.name "Your Name"
git config --global user.email "you@example.com"
git config --global core.editor vim
```

### 初始化与克隆

```bash
git init                    # 初始化仓库
git clone <url>             # 克隆远程仓库
git clone <url> <dir>       # 克隆到指定目录
```

### 基本操作

```bash
git status                  # 查看状态
git add <file>              # 添加到暂存区
git add .                   # 添加所有更改
git commit -m "message"     # 提交
git commit --amend          # 修改上次提交
```

### 分支管理

```bash
git branch                  # 列出本地分支
git branch <name>           # 创建分支
git checkout <branch>       # 切换分支
git checkout -b <branch>    # 创建并切换
git merge <branch>          # 合并分支
git branch -d <branch>      # 删除分支
```

### 远程操作

```bash
git remote -v               # 查看远程仓库
git remote add origin <url> # 添加远程仓库
git push origin main        # 推送到远程
git pull origin main        # 拉取并合并
git fetch origin            # 拉取（不合并）
```

### 查看历史

```bash
git log                     # 查看提交历史
git log --oneline           # 简洁模式
git log --graph             # 图形化显示
git diff                    # 查看差异
git diff <branch1> <branch2> # 比较分支
```

### 撤销与回退

```bash
git restore <file>          # 撤销工作区更改
git restore --staged <file> # 取消暂存
git reset HEAD~1            # 回退一个提交
git reset --hard HEAD~1     # 回退并丢弃更改
git revert <commit>         # 创建撤销提交
```

### 暂存工作

```bash
git stash                   # 暂存当前工作
git stash list              # 列出暂存
git stash pop               # 恢复并删除
git stash apply             # 恢复但保留
```

### 标签

```bash
git tag                     # 列出标签
git tag v1.0.0              # 创建轻量标签
git tag -a v1.0.0 -m "msg"  # 创建附注标签
git push origin --tags      # 推送标签
```

## 3. 代码示例

### 典型工作流

```bash
# 1. 克隆项目
git clone https://github.com/user/repo.git
cd repo

# 2. 创建功能分支
git checkout -b feature/new-feature

# 3. 开发并提交
git add .
git commit -m "Add new feature"

# 4. 推送到远程
git push origin feature/new-feature

# 5. 合并到主分支
git checkout main
git merge feature/new-feature
git push origin main

# 6. 清理分支
git branch -d feature/new-feature
```

### 解决冲突

```bash
# 合并时出现冲突
git merge feature-branch
# Auto-merging file.txt
# CONFLICT (content): Merge conflict in file.txt

# 查看冲突文件
git status

# 手动编辑解决冲突，然后
git add file.txt
git commit -m "Resolve merge conflict"
```

### 回退操作

```bash
# 撤销未提交的更改
git restore file.txt

# 撤销最近一次提交（保留更改）
git reset --soft HEAD~1

# 完全回退到某个提交
git reset --hard abc123

# 创建撤销提交（安全方式）
git revert abc123
```