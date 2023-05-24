# ros1

## 1. CPack & Catkin

**问题描述**

同时允许cmake 及 catkin 编译时，CPack 打包引入额外文件

```cmake
# CMakeLists.txt
find_package(catkin)
```

**解决方案**

[Remove Catkin artifacts from CPack build](https://stackoverflow.com/questions/50486421/remove-catkin-artifacts-from-cpack-build)

## 2. diagnostic

[diagnostic_analysis](http://wiki.ros.org/diagnostic_analysis)

## 3. boost占位符未定义

**问题描述**

- ros16.04 依赖 boost1.58，占位符为 `_1`
- ros18.04 依赖 boost1.60，占位符为 `boost::placeholders::_1`

**解决方案**

```c++
#include <boost/version.hpp>
#if BOOST_VERSION >= 106000
	using boost::placeholders::_1;
	using boost::placeholders::_2;
	using boost::placeholders::_3;
#endif
```

参考链接: [https://github.com/freeorion/freeorion/issues/3172](https://github.com/freeorion/freeorion/issues/3172)

## 4. tf2 相关

> URDF(United Robotics Description Format)全称统一机器人描述格式，是一个XML语法框架下用来描述机器人的语言格式。
>
> KDL(Kinematics and Dynamics Library)运动学和动力学库定义了一个树结构来表示机器人机构的运动学和动力学参数。kdl_parser提供了从URDF中的XML robot表示构建kdl树的工具。
>
> joint_state_publisher是ros内置package，用于发布非机器人非固定关节的运动
>
> robot_state_publisher是ros内置tf广播器

### 4.1 robot_state_publisher 与 joint_state_publisher 区别

#### joint_state_publisher

为机器人发布 `sensor_msgs/JointState` 消息到话题 `joint_states` 上。该软件包读取 `robot_description` 参数，找到所有非固定关节，并发布与所有这些关节定义的 `JointState` 消息。可以与 `robot_state_publisher` 节点一起使用，也可以发布所有关节状态的变换。

基本上，`robot_state_publisher` 基于其URDF文件发布机器人的tf变换。也可以使用 `joint_state_publisher` 或自己发布关节状态。无论哪种方式，`robot_state_publisher` 都会使用这些信息来计算机器人的正向运动学。

#### robot_state_publisher

使用参数服务器 `/robot_description` 指定的URDF和订阅 `joint_states` 话题获得的关节位置去计算机器人的运动学，并通过tf广播器发布结果

### 4.2 ros命令

Note：首先需要执行 `sudo apt-get install liburdfdom-tools`

```shell
check_urdf *.urdf  # 展示 urdf link 结构

urdf_to_graphiz *.urdf  # tf树输出至pdf

roslaunch urdf_tutorial display.launch model:=*.urdf  # rviz 中展示 urdf 机器人模型
```

## 5. roslaunch

### 5.1 使用eval语句

[How to use import statement inside of an eval in a ROS roslaunch file](https://alspitz.github.io/blog/roslaunch_import.html)

### 5.2 roslaunch启动多机进程

#### 创建ssh连接

roslaunch 实现多机进程的技术基础是ssh。

roslaunch使用Paramiko创建SSH连接，但是Paramiko不支持ECDSA算法(ssh算法的默认值)，因此它无法从ECDSA生成的已知HOSTS文件中读取，无法创建远程连接。

为了解决这个问题，我们删除了known_hosts文件（特别是该行），并重新生成了它（当被询问时存储新主机），使用以下命令执行第一次ssh连接：

```shell
ssh -oHostKeyAlgorithms='ssh-rsa' host-ip
```

#### 定义 machine

test.machine

```xml
<launch>
    <!-- 若pc-master 与 pc-slave 配置了ssh免密登录，则无需指定password
	<machine name="pc-master"
             address="192.168.1.10"
             user="root"
             env-loader="master_env.sh"
             ssh-port="22"
             default="true" />
    <machine name="pc-slave"
             address="192.168.1.20"
             user="root"
             password="123"
             env-loader="slave_env.sh"
             ssh-port="22" />
</launch>
```

#### 定义 env.sh

master_env.sh

```shell
#!/bin/bash

UBUNTU_CODENAME=$(lsb_release -sc)
if [ $UBUNTU_CODENAME == 'xenial' ]; then
  ROS_CODENAME='kinetic'
elif [ $UBUNTU_CODENAME == 'bionic' ]; then
  ROS_CODENAME='melodic'
else
  echo "Unsuport OS ${UBUNTU_CODENAME}"
  exit 1
fi

source /opt/ros/$ROS_CODENAME/setup.bash

export ROS_MASTER_URI=http://192.168.1.10:11311
export ROS_IP=192.168.1.10
exec "$@"
```

slave_env.sh

```shell
#!/bin/bash

UBUNTU_CODENAME=$(lsb_release -sc)
if [ $UBUNTU_CODENAME == 'xenial' ]; then
  ROS_CODENAME='kinetic'
elif [ $UBUNTU_CODENAME == 'bionic' ]; then
  ROS_CODENAME='melodic'
else
  echo "Unsuport OS ${UBUNTU_CODENAME}"
  exit 1
fi

source /opt/ros/$ROS_CODENAME/setup.bash

export ROS_MASTER_URI=http://192.168.1.10:11311
export ROS_IP=192.168.1.20
exec "$@"
```

#### 定义 launch

test.launch

```xml
<launch>
	<include file="test.machine" />
    
    <node machine="pc-master"
          pkg="node1"
          type="node1"
          name="node1" />
    <node machine="pc-slave"
          pkg="node2"
          type="node2"
          name="node2" />
</launch>
```

如果存在 ROS_IP 不生效的情况，则需要排查是否设置了 ROS_HOSTNAME，详细参考[ROS_IP/ROS_HOSTNAME](http://wiki.ros.org/ROS/EnvironmentVariables)。通常 ROS_HOSTNAME 设定后，需要同步更新DNS文件。

## 6 ros TCP/UDP临时端口

[Ephemeral port](https://www.wikiwand.com/zh/%E4%B8%B4%E6%97%B6%E7%AB%AF%E5%8F%A3)

