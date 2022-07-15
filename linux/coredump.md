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

通过修改 /proc/sys/kernel/core_pattern 文件实现。

```shell
sudo sysctl -w kernel.core_pattern=/var/crash/core.%u.%e.%p
```

## 2. coredump 调试

crash 文件解压

```shell
apport-unpack *.crash <target directory>
```

调试

```shell
gdb <program> <CoreDump file>
```

