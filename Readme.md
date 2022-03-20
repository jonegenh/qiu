# QIU - from sylar

## environment
* Ubuntu20.04.2
* gcc 9.3.0
* g++ 9.3.0
* cmake

## list
* bin -- 二进制
* build -- 中间文件路径
* cmake -- cmake函数文件夹
* CMakeLists.txt -- cmake的定义文件
* lib -- 库的输出路径
* Makefile
* qiu -- 源代码路径
* tests --测试代码

## 日志系统
1)
    log4j
```
logger (定义日志类别)
  |
  |-------Formatter(日志格式)
  |
Appender(日志输出地方)
```

## 配置系统

Config --> Yaml

yaml-cpp: github

```
YAML::Node node = YAML::LoadFile(filename);
for(auto it = node.begin(); it != node.end() ; ++it){
  it->first,it->second;
}

node.IsSequence()
for(size_t i = 0 ; i < node.size() ; ++i){

}

node.IsScalar();
```
配置系统的原则是约定优于配置;

```cpp
template<T, fromStr, ToStr>
class ConfigVar;

template<F,T>
LexicalCast;

//容器偏特化, 目前支持vector
//list, set, map, unordered_set, unordered_map
//map/unordered_map 支持key = std::string
// Config::Lookup(key) key 相同
//类型不同的不会有报错
```

自定义类型,需要实现qiu::LexicalCast,偏特化
实现后,就可以支持Config解析自定义类型,自定义类型可以和
常规stl容器一起使用

配置的事件机制
当一个配置项发生修改的时候,可以反向通知对应的代码,回调

# 日志系统整合配置系统
```yaml
logs:
    - name: root
      level: (debug,info,warn,error,fatal)
      formatter: '%d%T%p%T%t%T%m%n'
      appender:
        - type: (StdoutLogAppender, FileLogAppender)
          level: (debug,...)
          file: /logs/xxx.log
```
```cpp
    qiu::Logger g_logger = qiu::LoggerMgr::GetInstance()->getLogger(name);
    QIU_LOG_INFO(g_logger) << "xxx log";
```
```cpp
static Logger::ptr g_log = QIU_LOG_NAME("system");
//m_root, m_system->m_root 当logger的appenders为空,使用root写logger
```

```cpp
//定义LogDefine LogAppenderDefine,偏特化 LexicalCast
//实现日志配置解析
```

## 线程库

Thread, Mutex

pthread pthread_create

互斥量 mutex
信号量 semaphore

整合logear
logger, appender

Spinlock 替换Mutex
写文件，周期性，reopen


## 协程库封装
定义协程接口
ucontext_t。
macro

```
Fiber::GetThis()
Thread->main_fiber <-------> sub_fiber
            ^
            |
            v
          sub_fiber
```

协程调度模块scheduler
```
        1 - N        1 - M
scheduler --> thread ---> fiber
1.线程池，分配一组线程
2.协程调度器，将协程指定到相应的线程上执行

N : M

m_threads
<function<void()>, fiber, threadid>   m_fibers

schedule(func/fiber)

start()
stop()
run()

1.设置当前线程的scheduler
2.设置当前线程的run，fiber
3.协程调度循环while(true)
  1.协程消息队列里面是否有任务
  2.无任务执行，执行idle
```

```
IOManager(epoll) --> Scheduler
      |
      |
      v
    idle(epoll_wait)

    信号量
PutMessage(msg,) +信号量1,single()
message_queue
    |
    |------ Thread
    |------ Thread
        wait()-信号量1， RevbMessage(msg,)


异步IO，等待数据返回。epoll_wait

epoll_create, epoll_ctl, epoll_wait
```

```
Timer -> addTimer() -->cancel()
获取当前的定时器触发离现在的时间差
返回当前需要触发的定时器
```


```
              [Fiber]                [Timer]
                 ^ N                    ^
                 |                      |
                 |1                     |
              [Thread]            [TimerManager]
                 ^ M                    ^
                 |                      |
                 |1                     |
              [Scheduler] <----- [IOManager(epoll)]
```

## HOOK
sleep,
usleep

socket 相关的(socket, connect, accept)
io相关 (read, write, send, recv, ...)
fd相关 (fcntl, ioctl, ...)

## socket函数库


          [UnixAddress]
                |
            ---------                    |--[IPv4Address] 
            |Address|  --- [IPAddress] --|
            ---------                    |--[IPv6Address]
                |
                |
            ---------
            | Socket|
            ---------

connect,
accept
read/write/close

## 序列化bytearray

write(int, float, int64, ...)
read(int, float, int64, ...)

// 压缩算法->类似protobuf
// 无符号 有符号整型映射-> Zigzag算法


## http协议开发
HTPP/1.1 - API


HttpRequest;
HttpResponse;

GET / HTTP/1.1
host: www.baidu.com

HTTP/1.1 200 OK
Accept-Ranges: bytes
Cache-Control: no-cache
Connection: keep-alive
Content-Length: 9508
Content-Type: text/html

uri: http://www.baidu.com:80/page/xxx?id=10&v=20#fr
    http, 协议
    www.baidu.com, host
    80 端口
    /page/xxx, path
    id=10&v=20 param
    fr fragment

ragel mongrel2

## TcpServer封装
基于TcpServer实现了一个EchoServer

## Stream 针对文件/socket封装
read/write/readFixSize/writeFixSize


## HttpSession/HttpConnection
Server.accept, socket -> session
client connect socket -> Connection

HttpServer : TcpServer

          Servlet <---- FunctionServlet
            |
            |
            v
          ServletDispatch
## 分布协议

## 推荐系统
