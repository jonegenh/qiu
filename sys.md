# 守护进程

daemon,

```cpp
fork 
 | ---- 子进程 --> 执行server
 | ---- wait(pid)
```
# 输入参数解析
int argc, char** argv

./scripts -d -c conf

# 环境变量
getenv
setenv

/proc/pid/cmdline | cwd | exe

利用/proc/pid/cmdline，和全局变量构造函数，实现在进入main函数前解析参数

1.读写环境变量
2.获取程序的绝对路径， 基于绝对路径设置cwd
3.可以通过cmdline，在进入main函数之前，解析好参数

# 配置加载
配置的文件夹路径， log.yml, http.yml, tcp.yml, thread.yml
