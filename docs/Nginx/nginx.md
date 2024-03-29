# nginx
## nginx配置文件结构

nginx.conf

```nginx
...                               # 全局块

events {                          # events块
	...
}

http {                            # http块
	...                           # http全局块
	server {                      # server块
		...                       # server全局块
		location [PATTERN] {      # location块
			...
		}
	}
}
```

- 全局块：配置影响nginx全局的指令。一般有运行nginx服务器的用户组，nginx进程pid存放路径，日志存放路径，配置文件引入，允许生成worker process数等。
- events块：配置影响nginx服务器或与用户的网络连接。有每个进程的最大连接数，选取哪种事件驱动模型处理连接请求，是否允许同时接受多个网路连接，开启多个网络连接序列化等。
- http块：可以嵌套多个server，配置代理，缓存，日志定义等绝大多数功能和第三方模块的配置。如文件引入，mime-type定义，日志自定义，是否使用sendfile传输文件，连接超时时间，单连接请求数等。
- server块：配置虚拟主机的相关参数，一个http中可以有多个server。
- location块：配置请求的路由，以及各种页面的处理情况。

不同模块指令关系：server继承main；location继承server；upstream既不会继承指令也不会被继承，它有自己的特殊指令，不需要在其他地方的应用

配置文件详解:

```nginx
########### 每个指令必须有分号结束。#################
user administrator administrators;  # 配置用户或者组，默认为nobody nobody。
worker_processes auto;              # 允许生成的进程数，默认为1
pid /run/nginx.pid;                 # 指定nginx进程运行文件存放地址
error_log log/error.log debug;      # 制定日志路径，级别。这个设置可以放入全局块，http块，server块，级别以此为：                 												# debug|info|notice|warn|error|crit|alert|emerg
events {
    accept_mutex on;                 #设置网路连接序列化，防止惊群现象发生，默认为on
    multi_accept on;                 #设置一个进程是否同时接受多个网络连接，默认为off
    # use epoll;                      #事件驱动模型，select|poll|kqueue|epoll|resig|/dev/poll|eventport
    worker_connections  1024;        #最大连接数，默认为512
}
http {
    ##
    # Basic Settings
    ##
    
    sendfile on;              # 允许sendfile方式传输文件，默认为off，可以在http块，server块，location块
    keepalive_timeout 65;     # 连接超时时间，默认为75s，可以在http，server，location块
    sendfile_max_chunk 100k;  # 每个进程每次调用传输数量不能大于设定的值，默认为0，即不设上限。
    
    include       mime.types;                #文件扩展名与文件类型映射表
    default_type  application/octet-stream;  #默认文件类型，默认为text/plain
    
    ##
    # Logging Settings
    ##
    
    # access_log off; #取消服务日志    
    log_format myFormat '$remote_addr–$remote_user [$time_local] $request $status $body_bytes_sent $http_referer $http_user_agent $http_x_forwarded_for'; #自定义格式
    access_log log/access.log myFormat;  #combined为日志格式的默认值
    # access_log /var/log/nginx/access.log;
    # error_log /var/log/nginx/error.log;

    ##
    # Gzip Settings
    ##
    
    gzip on;

    upstream mysvr {   
      server 127.0.0.1:7878;
      server 192.168.10.121:3333 backup;  #热备
    }
    error_page 404 https://www.baidu.com; #错误页
    
    
    server {
        listen       80;              #监听端口
        server_name  127.0.0.1;       #监听地址 
        keepalive_requests 120;       #单连接请求上限次数
        # index index.html;           # 定义首页索引文件的名称
        location  ~*^.+$ {            #请求的url过滤，正则匹配，~为区分大小写，~*为不区分大小写。
           root path;                 #静态资源根目录
           # index vv.txt;            #设置默认页
           proxy_pass  http://mysvr;  #请求转向mysvr 定义的服务器列表
           deny 127.0.0.1;            #拒绝的ip
           allow 172.18.5.54;         #允许的ip           
        } 
    }
}
```

