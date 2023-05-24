# 操作系统

## Linux 内核
 - 了解内核的架构、模块、工作机制
 - 掌握内核的内存管理、进程调度、文件系统等

## Linux 系统调用
 - 进程、线程、内存、文件IO

### 进程间通信

#### 管道(PIPE)
1. 命令管道
在 shell 中使用管道将一个进程的输出连接到另一个进程的输入，实现过滤和数据流重定向

```
ps aux | grep sshd
```
2. 父子进程通信
通过匿名管道实现父子进程之间的数据交换
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  int pipefd[2]; 
  pid_t pid;

  if (pipe(pipefd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();

  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) { // 子进程
    close(pipefd[1]); // 关闭写端
    int child_data;
    read(pipefd[0], &child_data, sizeof(child_data));
    printf("Child process received data: %d\n", child_data); 
    close(pipefd[0]);
  } else { // 父进程
    close(pipefd[0]); // 关闭读端  
    int parent_data = 42;
    write(pipefd[1], &parent_data, sizeof(parent_data));
    close(pipefd[1]);
  }

  return 0;
}
```
3. 信号处理
将信号写入管道，可中断或通知另一个进程
4. 线程通信
线程之间可以利用管道进行数据传递和同步
5. 缓冲区
管道可用作高速缓冲区，收集处理过的数据并输出
6. 并发控制
可以利用管道实现进程或线程的并发控制
7. 屏幕输出
将屏幕输出重定向到管道中捕获

#### 信号量(Semaphore)
同步机制，用于解决对共享资源的互斥访问问题

```c
#include <semaphore.h>
#include <pthread.h>

sem_t empty, full; // 两个信号量
int buffer[BUFFER_SIZE];

void *producer(void *arg) {
  while (1) {
    // 生产
    sem_wait(&empty);
    put_to_buffer();
    sem_post(&full);
  }
}

void *consumer(void *arg) {
  while (1) {    
    // 消费    
    sem_wait(&full);
    get_from_buffer();
    sem_post(&empty);
  }
}

int main() {

  // 初始化信号量
  sem_init(&empty, 0, BUFFER_SIZE); 
  sem_init(&full, 0, 0);

  pthread_create(&producer_thread, NULL, producer, NULL);
  pthread_create(&consumer_thread, NULL, consumer, NULL);

  // 等待线程结束......

  sem_destroy(&empty);
  sem_destroy(&full);

  return 0;
}
```
#### 信号(Signal)
SIGINT, SIGKILL

#### 消息队列
#### 共享内存
共享内存提供了最快的进程间通信方式,但需要妥善管理同步与互斥
```c
#include <sys/mman.h>
#include <sys/stat.h>       
#include <fcntl.h>          
#include <semaphore.h>

const char *name = "OS";
sem_t *mutex; 

int main() {
  // 创建共享内存
  int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

  // 配置大小
  ftruncate(shm_fd, 4096);

  // 映射共享内存
  char *ptr = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

  // 写入数据
  sprintf(ptr, "%s", "Hello!"); 

  // 使用信号量同步
  sem_wait(mutex);
  // 访问共享内存
  printf("%s\n", ptr); 
  sem_post(mutex);

  // 删除共享内存
  shm_unlink(name);
}
```
#### 套接字
1. 可以用于不同机器间的进程通信。
套接字可基于TCP/IP在网络中的不同主机进行通信。其他IPC方式如管道、信号仅限于在同一主机的进程间通信。
2. 支持双向通信。
套接字通信是全双工的,两端可以同时读写。其他IPC方式大多是单向的。
3. 数据格式灵活。
套接字传输的是字节流,开发人员可以定义自定义的应用层协议,添加头信息等。
4. 易于编程。
套接字提供面向流和数据报两种接口,可以通过文件读写类接口实现进程间通信。
5. 通信高效。
套接字可以利用内核缓冲提高通信效率。
6. 更丰富的同步原语。
套接字提供了丰富的函数用于进程同步和通信管理,如select、poll等。
但套接字也有一些缺点,如需要序列化/反序列化数据,通信延迟取决于网络情况等。
总体来说,套接字提供了一个功能强大的进程间通信机制,可以跨主机通信,但需要处理网络连接的复杂性。

## Linux 命令行和shell

## Linux 进程管理
 - 如何查看和控制Linux 进程
 - 了解进程间通信方式

## Linux 网络管理

## Linux 软件包管理

## Linux 编程接口

## Linux 内存和性能管理
