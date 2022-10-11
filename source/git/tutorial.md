# git 教程

![git](../Pic/git.jpg)

## 1. 常用命令

### 1.1 git add

```shell
# 将文件添加到暂存区
git add <file>
# 撤销上次所有 add
git reset HEAD
# 撤销单个文件 add
git reset HEAD <file>
```

**git push**

```shell
# push 到对应分支并覆盖 
git push origin <branch> -f
```

### 1.2 git commit

- 覆盖 commit

```shell
# 使用上一次 git commit log，push 时需加 -f
git commit --ament
```

- 撤销

```shell
# 撤销 commit，删除未提交代码
git reset --hard HEAD^
# 撤销 commit，保留未提交代码
git reset --soft HEAD^
```

### 1.3 git rebase

```shell
git rebase <branch> # rebase 到指定分支
```

### 1.4 git tag

- tag 拉取及查看

```shell
git fetch --tags
git tag --list
git show <tag name>
```

- tag 及推送

```shell
# 为 HEAD 打 tag
git tag <tag name> -m "information"
# 为指定 commit 打 tag
git tag <tag name> <commit id> -m "information"
# 推送单个 tag
git push origin <tag name>
# 推送所有 tag
git push origin --tags
# 删除 tag
git tag -d <tag name>
```

## 2. git subtree

> 使用场景:
>
> 将项目 A 不丢失提交记录的情况下，合并到项目 B

```shell
git subtree add --prefix=rails git://github.com/rails/rails.git master
```

参考链接:[如何合并两个Git库](https://stackoverflow.com/questions/1425892/how-do-you-merge-two-git-repositories)

## 3. git cherry-pick

> 使用场景:
>
> 将指定的 commit 应用与其他分支

参考链接:[git cherry-pick 教程](https://www.ruanyifeng.com/blog/2020/04/git-cherry-pick.html)
