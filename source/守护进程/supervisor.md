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

注意：supervisor 的 command 并不是使用 bash 或 sh 来执行的，所以用 bash -c "xxx && xx"来实现 bash 命令

## 4. 参考链接

[1] [Supervisor使用详解](https://www.jianshu.com/p/0b9054b33db3)

[2] [关于supervisor 的使用以及配置](https://www.cnblogs.com/piperck/p/5380938.html)