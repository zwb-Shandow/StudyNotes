# docker 教程

## 1. docker 优势

1. 环境不一致
2. 多用户操作系统下，会相互影响
3. 运维成本过高
4. 安装软件成本过高

## 2. docker 使用

### 2.1 仓库源

> 1. Docker官方中央仓库：
>
>    https://hub.docker.com/
>
> 2. 国内的镜像网站：网易蜂巢，daocloud
>
>    https://c.163.com/hub
>
>    http://hub.daocloud.io/
>
> 3. 公司内部私服
>
>    例如：基于Jfrog托管docker镜像源

### 2.2 登录/登出指定镜像仓库

#### 添加镜像仓库的注册信息

```shell
vim /etc/docker/daemon.json

# 新增内容
{
    "insecure-registries": ["host:port"]
}

```

#### 重启docker

```shell
systemctl restart docker
```

#### 登录镜像仓库

```shell
docker login host:port
user: admin
password: password
```

#### 登出镜像仓库

```shell
docker logout
```

### 2.3 镜像的操作

- 更改 docker 用户组

```shell
# 非 root 用户运行 docker, 重启生效
sudo groupadd docker
sudo usermod -aG docker <username>
```

- 查找镜像

```shell
docker search <镜像名称>
```

- 拉取镜像

```shell
docker pull <镜像名称:tag>   #不加tag会拉取默认版本
```

- 查看本地全部镜像

```shell
docker images
```

- 删除本地镜像

```shell
docker rmi <镜像id>
```

### 2.4 容器的操作

- 运行容器

```shell
docker run <镜像id|镜像名称:tag>
# 制定参数运行
docker run -d -p <宿主机端口:容器端口> --name <容器名称> <镜像id|镜像名称:tag>
# -d: 代表后台运行容器
# -p: 宿主机端口：容器端口：为了映射当前linux的端口和容器的端口
# --name 容器名称：指定容器的名称
# -it: 交互式运行容器，为容器重新分配一个伪输入终端
# --expose=[] 开发一个端口或一组端口
```

- 查看正在运行的容器

```shell
docker ps -a
```

- 查看容器 ip 地址

```shell
docker inspect <容器id>
```

- 查看容器日志

```shell
docker logs -f <容器id>  # -f: 滚动查看日志的最后几行
```

- 进入容器内

```shell
docker exec -it <容器id> /bin/bash
```

- 删除容器

```shell
docker stop <容器id>
docker stop $(docker ps -qa)  # 停止全部容器
docker rm <容器id>
docker rm $(docker ps -qa)  # 删除全部容器
```

- 启动/重启容器

```shell
docker start/restart <容器id>
```

- 从宿主机拷贝文件到容器

```shell
docker cp <宿主机文件> <容器>:<容器内路径>
```

- 从 `Dockerfile` 构建镜像

```shell
docker build -t <name:tag> <Dockerfile path>
```

- 将宿主机文件挂载至docker

采用 -v 参数，若docker路径不存在，则会自动创建

```shell
docker run -d -p <宿主机端口:容器端口> --name <容器名称> -v /opt/source:/opt/traget <镜像id|镜像名称:tag>
```

采用 --mount 参数

```shell
docker run -d -p <宿主机端口:容器端口> --name <容器名称> --mount type=bind,source=/opt/source,target=/opt/target <镜像id|镜像名称:tag>
```

### 2.5 数据卷

```shell
# 1. 将宿主机的内容复制到容器内部
docker cp 文件名称 容器id:容器内部路径

# 2. 创建数据卷
docker volume create 数据卷名称
# 创建数据卷之后，默认会存放在一个目录下 /var/lib/docker/volumes/数据卷名称/_data

# 3. 查看数据卷的详细信息
docker volume inspect 数据卷名称

# 4. 查看全部的数据卷
docker volume ls

# 5. 删除数据卷
docker volume rm 数据卷名称

# 6. 应用数据卷
# 当映射数据卷时，如果不存在，docker会自动创建，会将容器内部自带的文件，存储在默认的存放路径中
docker run -v 数据卷名称：容器内部的路径 镜像id
# 直接指定一个路径作为数据卷的存放位置，这个路径下是空的
docker run -v 路径：容器内部路径 镜像id
```

> 数据卷：将宿主机的一个目录映射到容器的一个目录中。
>
> 可以在宿主机中操作目录中的内容，那么容器内部映射的文件也会一起改变

## 3. Dockerfile

通过 `Dockerfile` 文件，构建自定义的镜像

```dockerfile
FROM <base image>  # 指定当前自定义镜像依赖的环境

COPY <source> <dest>  # 从上下文目录中复制文件或者目录到容器里指定路径.

# 执行命令行, docker build 阶段执行
#【注意】Dockerfile 的指令每执行一次都会在 docker 上新建一层。避免过多无意义的层.
RUN <shell cmd>

# 需要执行的命令（在workdir下执行的，cmd可以写多个，只以最后一个为准） docker run 阶段执行
CMD ["<exexuteable>", "<param1>", "<param2>", ...] 

# 类似于 CMD 指令，但其不会被 docker run 的命令行参数指定的指令所覆盖，而且这些命令行参数会被当作参数送给 ENTRYPOINT 指令指定的程序
ENTRYPOINT ["<executeable>", "<param1>", "<param2>", ...]

# 设置环境变量
ENV <key> <value>

# 构建参数，与 ENV 作用一致. ARG 设置的环境变量只在 docker build 阶段有效，构建好的镜像内不存在此环境变量
ARG <param>[=<default>]

# 声明端口
EXPOSE <port>

# 指定工作目录, 构建镜像的每一层都存在.
WORKDIR <path>

# 给镜像添加一些元数据, 例如镜像作者、邮箱等
LABEL <key>=<value>
```

---

```shell
# 2. 将准备好的Dockerfile和相应的文件拖拽到Linux中，通过Docker的命令制作镜像
docker build -t 镜像名称:[tag]
```

---

#### 实例

> 准备文档
>
> 1. python项目 ：（源码，requirements.txt / pipfile）
> 2. Dockerfile  ： 构建docker镜像使用
> 3. build.sh  ： 构建镜像的指令
> 4. save.sh ： 将构建好的docker镜像保存到本地
> 5. load.sh ： 加载本地docker镜像
> 6. run.sh ：  运行docker镜像

## 4. Docker-Compose

> 之前运行一个镜像，需要添加大量的参数。
>
> 可以通过 Docker-Compose 编写这些参数。
>
> Docker-Compose 可以批量管理容器
>
> 只需要通过一个 docker-compose.yml 文件区维护即可

#### docker-compose.yml 文件编写

```yml
version: '3.1'
services:
	mysql:               # 服务的名称
		restart: always  # 只要docker启动，该容器就跟着启动
		image:           # 指定镜像的路径
		container_name: mysql  # 指定容器名称
		ports:
			- 3306:3306  # 指定端口号的映射
		environment:
			MYSQL_ROOT_PASSWORD: root  # 指定mysql的root用户登录密码
			TZ: Asia/Shanghai  # 指定时区
		volumes:
			- /opt/docker_mysql_tomcat/mysql_data:/var/lib/mysql  # 映射数据卷
	tomcat:
		restart: always
		image:
		container_name: tomcat
		ports:
			- 8080:8080
		environment:
			TZ: Asia/Shanghai
		volumes:
			- /opt/docker_mysql_tomcat/tomcat_webapps:/usr/local/tomcat/webapps
			- /opt/docker_mysql_tomcat/tomcat_logs:/usr/local/tomcat/logs
```

#### 使用docker-compose命令管理容器

> 使用docker-compose命令，默认当前目录下查找docker-compose.yml

```sh
# 1. 基于docker-compose.yml启动管理的容器
docker-compose up -d

# 2. 关闭并删除容器
docker-compose down

# 3. 开启|关闭|重启已经存在的由docker-compose维护的容器
docker-compose start|stop|restart

# 4. 查看由docker-compose管理的容器
docker-compose ps

# 5. 查看日志
docker-compose logs -f
```

#### docker-compose 配置 Dockerfile 使用

> 使用 docker-compose.yml 文件以及 Dockerfile 文件在生成自定义镜像的同时启动当前镜像，并且由 docker-compose 去管理容器

```yml
# docker-compose.yml文件
version: '3.1'
services:
	ssm:
		restart: always
		build:            # 构建自定义镜像
			context: ../  # 指定dockerfile文件所在路径
			dockerfile: Dockerfile  # 指定Dockerfile文件名称
		image: ssm:1.0.1
		container_name: ssm
		ports:
			- 8080:8080
		environment:
			TZ: Asia/Shanghai

```
