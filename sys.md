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

# 配置加载
配置的文件夹路径， log.yml, http.yml, tcp.yml, thread.yml
