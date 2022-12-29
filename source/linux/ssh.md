# ssh

## 1. ssh 文件目录

### 1.1 id_rsa（私钥）

服务器上经过rsa算法生成的私钥。与公钥是一对的密钥对，用于连接其他服务器用。

### 1.2 id_rsa.pub（公钥）

服务器上经过rsa算法生成的公钥。与私钥是一对的密钥对，用于连接其他服务器用。
将主机A的id_rsa.pub内容copy到B主机的authorized_keys文件中，这样A主机就能不通过密码连接到B主机。

### 1.3 authorized_keys（授权文件）

实现真正无密码连接，即为授权文件，当把master的公钥添加到authorized_keys文件中后，下次连接直接输入ssh master即可，不需要再次输入密码。（我们本地可以自动生成公钥，将公钥输入到该文件中，本地就可无密码连接到该服务器，分布式集群时也会用到。）

#### 单向
1. 登录A机器，输入`ssh-keygen -t [rsa|dsa]`，将会生成密钥文件和私钥文件 id_rsa,id_rsa.pub或id_dsa,id_dsa.pub
2. 将 .pub 文件复制到B机器的 .ssh 目录， 并 `cat id_dsa.pub >> ~/.ssh/authorized_keys`，这样A就能不用密码就连接到B了。

#### 双向

1. 登录A、B主机，输入`ssh-keygen -t rsa`，全部回车，采用默认值。
2.  将~/.ssh目录下生成的公钥分别写入到对方主机的authorized_keys中。这样A、B之间就可以不通过密码连接对方主机。

### 1.4 known_hosts

ssh会把你每个你访问过计算机的公钥(public key)都记录在~/.ssh/known_hosts。当下次访问相同计算机时，OpenSSH会核对公钥。如果公钥不同，OpenSSH会发出警告， 避免你受到DNS Hijack之类的攻击。

## 2. roslaunch 启动远程机器节点

```shell
ssh -oHostKeyAlgorithms='ssh-rsa' 192.168.1.89
```

[A is not in your SSH known_hosts file.](https://answers.ros.org/question/41446/a-is-not-in-your-ssh-known_hosts-file/)

## 参考链接

[1] [SSH基本知识](https://wangdoc.com/ssh/basic)

[2] [.ssh目录下各文件的作用](https://blog.csdn.net/qq_16268979/article/details/108899178)