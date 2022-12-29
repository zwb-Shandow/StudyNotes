# linux 常用命令

## 1. readelf

查看 elf 格式的目标文件(可重定位文件、可执行文件、共享目标文件)的信息。

**ELF 文件总体组成**:

elf 文件头描述 elf 文件的总体信息。包括：系统相关，类型相关，加载相关，链接相关。

- 系统相关：elf文件标识的魔术数，以及硬件和平台等相关信息，增加了elf文件的移植性,使交叉编译成为可能。
- 类型相关
- 加载相关：包括程序头表相关信息
- 链接相关：节头表相关信息

## 2. ldd

显示共享库的依赖情况。

## 3. 进程前后台切换

### 3.1 &

命令在控制台后台运行，控制台关掉后，会导致命令终止。

```shell
# 命令后台运行，输出重定向到 out.file
command > out.file 2>&1 &
```

### 3.2 nohup

nohup命令可以在退出帐户之后继续运行相应的进程。nohup就是不挂起的意思( no hang up)。

```shell
nohup command &
```

如果使用nohup命令提交作业，那么在缺省情况下该作业的所有输出都被重定向到一个名为nohup.out的文件中，除非另外指定了输出文件：

```
nohup command > out.file 2>&1 &
```

[注意]:

在使用nohup命令后台运行命令之后，需要**使用exit正常退出当前账户**，这样才能保证命令一直在后台运行。

### 3.3 Ctrl + z

暂停前台命令的执行，将该进程放入后台，然后返回 shell

### 3.4 jobs

查看当前在后台执行的命令，可查看命令进程号

```
$ [1]  - suspended  lvim
$ [2]  + suspended  lvim
```

### 3.5 fg / bg

将后台中的命令调至前台继续运行，如果后台中有多个命令，可以用 fg %jobnumber将选中的命令调出，%jobnumber是通过jobs命令查到的后台正在执行的命令的序号(不是pid)

```shell
fg     # 默认将最近加入的job恢复至前台
fg %N  # 恢复指定job至前台
```

## 4. 网络端口查看

### lsof

```shell
lsof -i:端口号
```

### netstat

## 5. apt

### 5.1 更新或升级

```shell
apt-get update                    # 更新源
apt-get upgrade                   # 更新所有已安装的包
apt-get dist-upgrade              # 发行版升级（慎用）
apt --only-upgrade install <pkg>  # 更新已安装的软件包<pkg>
```

### 5.2 安装或重新安装

```shell
apt-get install <pkg>              # 安装软件包<pkg>，多个软件包用空格隔开
apt-get install --reinstall <pkg>  # 重新安装软件包<pkg>
apt-get install -f <pkg>           # 修复安装（破损的依赖关系）软件包
```

### 5.3 卸载

```shell
apt-get remove <pkg>  # 删除软件包<pkg>（不包括配置文件）
apt-get purge <pkg>   # 删除软件包<pkg>（包括配置文件）
```

### 5.4 下载清除

```shell
apt-get source <pkg>     # 下载pkg包的源代码到当前目录
apt-get download <pkg>   # 下载pkg包的二进制包到当前目录
apt-get source -d <pkg>  # 下载完源码包后，编译
apt-get build-dep <pkg>  # 构建pkg源码包的依赖环境
apt-get clean            # 清除 /var/cache/apt/archives/ 下所有已下载的包
apt-get autoclean        # 类似于clean，但清除的是缓存中过期的包（即已不能下载或者是无用的包）
apt-get autoremove       # 删除因安装软件自动安装的依赖，而现在不需要的依赖包  
```

### 5.5 查询

```shell
apt-cache stats           # 显示系统软件包的统计信息
apt-cache search <pkg>    # 使用关键字pkg搜索软件包（在缓存中搜索）
apt-cache depends <pkg>   # 查看pkg所依赖的软件包
apt-cache rdepends <pkg>  # 查看pkg被那些软件包所依赖
apt-cache policy <pkg>    # 查看pkg的来自于哪个源及已缓存的版本
```

## 6. dpkg

### 6.1 查询

```shell
dpkg -c package.deb      # 显示 deb 中的文件
dpkg --info package.deb  # 显示 deb 详细信息
dpkg -L package.deb      # 显示一个包安装到系统里的文件目录信息
dpkg -S package_name      # 显示以安装包文件的安装目录
```

## 7. 网卡带宽查看

### 7.1 nload

```shell
nload <网卡号>
```

## 8. ls 系列

```shell
ls -a   # 显示全部的文件及文件夹，包括隐藏的文件或文件夹
ls -lh  # 以M为单位查看文件大小
ll -h   # 以G为单位查看文件大小
lsusb   # 用于列出计算机里的usb设备
lsmode  # linux下的驱动有两种形式。一种是加载到内核中的，另一种是以模块化的形式出现的。lsmod就是用于列出计算机里面的驱动模块
```

## 9. pkg-config

https://man.openbsd.org/pkg-config.1