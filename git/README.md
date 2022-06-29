# git

![git](../images/git.jpg)

## 1. 常用命令

### 1.1 分区转换指令

#### git add

撤销 add

```shell
# 撤销上次所有 add
git reset HEAD
# 撤销单个文件 add
git reset HEAD <file>
```

#### git commit

撤销 commit

```shell
# 撤销 commit，删除未提交代码
git reset --hard HEAD^
# 撤销 commit，保留未提交代码
git reset --soft HEAD^
```

#### git push

### 1.2 分区对比指令
