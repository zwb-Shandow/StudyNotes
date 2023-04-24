# supervisor

Supervisor是用Python开发的一套通用的进程管理程序，能将一个普通的命令行进程变为后台daemon，并监控进程状态，异常退出时能自动重启。它是通过fork/exec的方式把这些被管理的进程当作supervisor的子进程来启动，这样只要在supervisor的配置文件中，把要管理的进程的可执行文件的路径写进去即可。也实现当子进程挂掉的时候，父进程可以准确获取子进程挂掉的信息的，可以选择是否自己启动和报警。supervisor还提供了一个功能，可以为supervisord或者每个子进程，设置一个非root的user，这个user就可以管理它对应的进程。

> 以 Ubuntu18.04 为例

## 1. 安装

- apt 安装

```shell
apt-get install supervisor
```

- pip 安装

```shell
pip install supervisor
```

## 2. 使用

supervisor 配置文件: `/etc/supervisor/supervisord.conf`

子进程配置文件路径: `/etc/supervisor/conf.d/`

## 3. 配置文件

supervisord 配置文件：

```
; supervisor config file

[unix_http_server]
file=/var/run/supervisor.sock   ; (the path to the socket file)
chmod=0700                       ; sockef file mode (default 0700)

[supervisord]
logfile=/var/log/supervisor/supervisord.log ; (main log file;default $CWD/supervisord.log)
pidfile=/var/run/supervisord.pid ; (supervisord pidfile;default supervisord.pid)
childlogdir=/var/log/supervisor            ; ('AUTO' child log dir, default $TEMP)

; the below section must remain in the config file for RPC
; (supervisorctl/web interface) to work, additional interfaces may be
; added by defining them in separate rpcinterface: sections
[rpcinterface:supervisor]
supervisor.rpcinterface_factory = supervisor.rpcinterface:make_main_rpcinterface

[supervisorctl]
serverurl=unix:///var/run/supervisor.sock ; use a unix:// URL  for a unix socket

; The [include] section can just contain the "files" setting.  This
; setting can list multiple files (separated by whitespace or
; newlines).  It can also contain wildcards.  The filenames are
; interpreted as relative to this file.  Included files *cannot*
; include files themselves.

; 若需要指定模块启动顺序，则可将启动模块序列按顺序在group中启动
[group:test]
programs=roscore,bag,bridge

[include]
files = /etc/supervisor/conf.d/*.conf
```



子进程配置文件示例:

```
[program:bridge]
directory=/catkin_ws
command=bash -c "source /opt/ros/melodic/setup.sh && source /catkin_ws/install/setup.sh && roslaunch bridge bridge.launch"

autorestart=True
autostart=True
user=root
stdout_logfile_maxbytes=50MB
stdout_logfile=/catkin_ws/bridge.log
```

注意：

[1] supervisor 的 command 并不是使用 bash 或 sh 来执行的，所以用 bash -c "xxx && xx"来实现 bash 命令

[2] 配置完成后，执行 `supervisord -c /etc/supervisor/supervisord.conf`，此时默认开启了所有服务

## 4. 常用命令

```shell
supervisorctl update  # 更该完配置文件后，必须执行更新
supervisorctl start <program>
supervisorctl restart <program>
supervisorctl stop <program>
supervisorctl reload  # 重启 supervisord 服务
supervisorctl  # 进入到交互模式下，使用 help 查看所有命令
```

## 5. 异常处理

若出现如下错误：

```shell
error: <class 'socket.error'>, [Errno 111] Connection refused: file: /usr/lib64/python2.7/socket.py line: 224
```

检查 supervisord 进程

```shell
ps aux | grep supervisord
```

若未发现进程号，则手动启动进程

```shell
supervisord -c /etc/supervisor/supervisord.conf
```

## 6. 参考链接

[1] [Supervisor使用详解](https://www.jianshu.com/p/0b9054b33db3)

[2] [关于supervisor 的使用以及配置](https://www.cnblogs.com/piperck/p/5380938.html)