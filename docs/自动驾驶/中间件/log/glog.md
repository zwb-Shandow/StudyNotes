# glog

## 记录 core dump 信息

### 使用

通过 `google::InstallFailureSignalHandler` 即可注册，将 core dump 信息输出到 stderr，如:

```c++
#include <glog/logging.h>
#include <string>
#include <fstream>

void SignalHandle(const char* data, int size)
{
    std::ofstream fs("glog_dump.log",std::ios::app);
    std::string str = std::string(data,size);
    fs<<str;
    fs.close();
    LOG(ERROR)<<str;
}

class GLogHelper
{
public:
    GLogHelper(char* program)
    {
        google::InitGoogleLogging(program);
        FLAGS_colorlogtostderr=true;
        // 默认捕捉 SIGSEGV 信号信息并输出至stderr
        google::InstallFailureSignalHandler();
        // 可通过以下方法自定义输出方式
        google::InstallFailureWriter(&SignalHandle);
    }
    ~GLogHelper()
    {
        google::ShutdownGoogleLogging();
    }
};
 
void fun()
{
    int* pi = new int;
    delete pi;
    pi = 0;
    int j = *pi;
}
 
int main(int argc,char* argv[]) {
    GLogHelper gh(argv[0]);
    fun();
}
```

### 源码实现

函数声明位于 `glog/logging.h.in`，函数实现位于 `signalhandler.cc`中。