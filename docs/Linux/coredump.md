# coredump

## 1. 设定 coredump

### 1.1 开启 coredump

`ulimit -a` 查看 coredump 是否开启。

```shell
➜  crash ulimit -a
-t: cpu time (seconds)              unlimited
-f: file size (blocks)              unlimited
-d: data seg size (kbytes)          unlimited
-s: stack size (kbytes)             8192
-c: core file size (blocks)         0
-m: resident set size (kbytes)      unlimited
-u: processes                       63418
-n: file descriptors                1024
-l: locked-in-memory size (kbytes)  65536
-v: address space (kbytes)          unlimited
-x: file locks                      unlimited
-i: pending signals                 63418
-q: bytes in POSIX msg queues       819200
-e: max nice                        0
-r: max rt priority                 0
-N 15:                              unlimited
```

可以看出，系统默认不开启 coredump。开启 coredump 方式有两种，

- 临时开始

```shell
ulimit -c unlimited
```

- 永久开启

修改 /etc/profile 文件，添加 `ulimit -S -c unlimited > /dev/null 2>&1`，或者修改 /etc/security/limits.conf，添加 `* soft core 0`。

### 1.2 修改 coredump 路径

使用 sysctl 命令更改 core 文件位置

```shell
sudo sysctl -w kernel.core_pattern=/var/crash/%e.%s.%t.core
```

此命令将指定路径更新至 /proc/sys/kernel/core_pattern 文件。

也可以通过在 /etc/sysctl.conf 中添加以下行来永久更改core文件路径

```shell
kernel.core_pattern="/var/crash/%e.%s.%t.core"
```

core_pattern中的格式化符号说明

| Format specifiers | Description         |
| ----------------- | ------------------- |
| %e                | 文件名              |
| %g                | 进程运行时使用的gid |
| %p                | 进程的pid           |
| %s                | 导致转储的信号      |
| %t                | 转储发生的时间      |
| %u                | 进程运行时的uid     |

## 2. coredump 调试

apport-unpack安装

```shell
apt install apport
```

crash 文件解压

```shell
apport-unpack *.crash <target directory>
```

调试

```shell
gdb <program> <CoreDump file>
```

## 3. gdb 常用命令

启动gdb调试会话

```shell
gdb <executable>
```

设置断点

```shell
b <function>     # break，在函数处设置断点
b <line_number>  # break，在代码行号处设置断点
```

运行并停在第一个断点

```shell
r  # run，运行程序
```

单步调试

```shell
s  # step，单步跟踪，进入函数
n  # next，单步跟踪，不进入函数
```

继续运行

```shell
c  # continue，继续运行
```

打印变量

```shell
p <variable>  # print，打印变量值
```

打印堆栈信息

```shell
bt       # backtrace，打印简短堆栈信息
bt full  # 打印完整堆栈信息，包括参数值
```

切换和退出线程

```shell
thread <thread_id>   # 切换到指定线程
info threads         # 显示所有线程
thread apply all bt  # 显示所有线程堆栈信息
```

其他

```shell
display <exp>             # 每次停止时，自动打印表达式 exp 的值
undisplay <exp>           # 取消显示表达式的值

set var <name> = <value>  # 设置变量的值

list                      # 显示源代码
list <function>           # 显示函数的源代码

jump <location>           # 跳转到指定的代码位置

shell <cmd>               # 在gdb中执行shell命令
```



## 4. C++异常无法在堆栈中展开

GCC8以下的编译器无法记录C++抛出的异常，该问题在GCC8修复。

[我们是怎么发现C++异常从堆栈追踪中消失的原因的](https://abcdabcd987.com/libstdc++-bug/)
