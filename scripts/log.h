#ifndef __QIU_LOG_H__
#define __QIU_LOG_H__
#include<string>
#include<stdint.h>
#include<memory>
#include<list>
#include<sstream>
#include<fstream>
#include<iostream>
#include<vector>
#include<stdarg.h>
#include<map>
#include "util.h"
#include "singleton.h"
#include"thread.h"

#define QIU_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        qiu::LogEventWrap(qiu::LogEvent::ptr(new qiu::LogEvent(logger,level,__FILE__,__LINE__,0, qiu::GetThreadId(), \
        qiu::GetFiberId(), time(0), qiu::Thread::GetName()))).getSS()

#define QIU_LOG_DEBUG(logger) QIU_LOG_LEVEL(logger, qiu::LogLevel::DEBUG)
#define QIU_LOG_INFO(logger) QIU_LOG_LEVEL(logger, qiu::LogLevel::INFO)
#define QIU_LOG_WARN(logger) QIU_LOG_LEVEL(logger, qiu::LogLevel::WARN)
#define QIU_LOG_ERROR(logger) QIU_LOG_LEVEL(logger, qiu::LogLevel::ERROR)
#define QIU_LOG_FATAL(logger) QIU_LOG_LEVEL(logger, qiu::LogLevel::FATAL)

#define QIU_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        qiu::LogEventWrap(qiu::LogEvent::ptr(new qiu::LogEvent(logger, level, \
                        __FILE__,__LINE__,0,qiu::GetThreadId(),\
                        qiu::GetFiberId(), time(0), qiu::Thread::GetName()))).getEvent()->format(fmt, __VA_ARGS__)

#define QIU_LOG_FMT_DEBUG(logger, fmt, ...) QIU_LOG_FMT_LEVEL(logger, qiu::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define QIU_LOG_FMT_INFO(logger, fmt, ...) QIU_LOG_FMT_LEVEL(logger, qiu::LogLevel::INFO, fmt, __VA_ARGS__)
#define QIU_LOG_FMT_WARN(logger, fmt, ...) QIU_LOG_FMT_LEVEL(logger, qiu::LogLevel::WARN, fmt, __VA_ARGS__)
#define QIU_LOG_FMT_ERROR(logger, fmt, ...) QIU_LOG_FMT_LEVEL(logger, qiu::LogLevel::ERROR, fmt, __VA_ARGS__)
#define QIU_LOG_FMT_FATAL(logger, fmt, ...) QIU_LOG_FMT_LEVEL(logger, qiu::LogLevel::FATAL, fmt, __VA_ARGS__)

#define QIU_LOG_ROOT() qiu::LoggerMgr::GetInstance()->getRoot()
#define QIU_LOG_NAME(name) qiu::LoggerMgr::GetInstance()->getLogger(name)

namespace qiu{

class Logger;
class LoggerManager;

//log level
class LogLevel{
public:
    enum Level{
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    
    static const char* ToString(LogLevel::Level level);
    static LogLevel::Level FromString(const std::string& str);
};

//log event
class LogEvent{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,const char* file, int32_t line, uint32_t elapse,
    uint32_t thread_id, uint32_t fiber_id, uint64_t time, const std::string& thread_name);

    const char* getFile() const {return m_file;}
    int32_t getLine() const {return m_line;}
    uint32_t getElapse() const {return m_elapse;}
    uint32_t getThreadId() const {return m_threadId;}
    uint32_t getFiberId() const {return m_fiberId;}
    uint64_t getTime() const {return m_time;}
    const std::string& getThreadName() const {return m_threadName;}
    std::string getContent() const {return m_ss.str();}
    std::shared_ptr<Logger> getLogger() const {return m_logger;}
    LogLevel::Level getLevel() const {return m_level;}

    std::stringstream& getSS() { return m_ss;}
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    const char* m_file = nullptr; //file name
    int32_t m_line = 0;           // row
    uint32_t m_elapse = 0;        // from start to now - time(ms)
    uint32_t m_threadId = 0;      // thread id
    uint32_t m_fiberId = 0;       // fiber id
    uint64_t m_time;              //time
    std::string m_threadName;
    std::stringstream m_ss;

    std::shared_ptr<Logger> m_logger;
    LogLevel::Level m_level;
};

class LogEventWrap{
public:
    LogEventWrap(LogEvent::ptr event);
    ~LogEventWrap();
    LogEvent::ptr getEvent() const { return m_event;}
    std::stringstream& getSS();
private:
    LogEvent::ptr m_event;
};


//log formatter
class LogFormatter{
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);
public:
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        FormatItem(const std::string& fmt =""){}
        virtual ~FormatItem(){}
        virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    };
    
    void init();

    bool isError() const { return m_error;}
    const std::string getPattern() const {return m_pattern;}
private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    bool m_error = false;
};


//log out
class LogAppender{
friend class Logger;
public:
    typedef std::shared_ptr<LogAppender> ptr;
    typedef Spinlock MutexType;
    virtual ~LogAppender(){}

    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;
    virtual std::string toYamlString() = 0;

    void setFormatter(LogFormatter::ptr val);
    LogFormatter::ptr getFormatter();
    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level level) {m_level = level;}
protected:
    LogLevel::Level m_level = LogLevel::DEBUG;
    bool m_hashFormatter = false;
    MutexType m_mutex;
    LogFormatter::ptr m_formatter;
};

class Logger : public std::enable_shared_from_this<Logger>{
friend class LoggerManager;
public:
    typedef std::shared_ptr<Logger> ptr;
    typedef Spinlock MutexType;

    Logger(const std::string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    void clearAppenders();
    LogLevel::Level getLevel() const {return m_level;}
    void setLevel(LogLevel::Level val) {m_level = val;}
    
    const std::string& getName() const {return m_name;}

    void setFormatter(LogFormatter::ptr val);
    void setFormatter(const std::string& val);

    LogFormatter::ptr getFormatter();

    std::string toYamlString();
private:
    std::string m_name;
    LogLevel::Level m_level;
    MutexType m_mutex;
    std::list<LogAppender::ptr> m_appenders;
    LogFormatter::ptr m_formatter;
    Logger::ptr m_root;
};

class StdoutLogAppender:public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;

    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;
private:

};

class FileLogAppender : public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    bool reopen();
    void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) override;
    std::string toYamlString() override;
private:
    std::string m_filename;
    std::ofstream m_filestream;
    uint64_t m_lastTime = 0;
};

class LoggerManager{
public:
    typedef Spinlock MutexType;
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);
    
    void init();
    Logger::ptr getRoot() const {return m_root;}

    std::string toYamlString();
private:
    MutexType m_mutex;
    std::map<std::string, Logger::ptr> m_loggers;
    Logger::ptr m_root;
};

typedef qiu::Singleton<LoggerManager> LoggerMgr;

}

#endif