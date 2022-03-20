#include<iostream>
#include<thread>
#include"scripts/log.h"
#include"scripts/util.h"
#include <string.h>

int main(int argc, char** argv){
    qiu::Logger::ptr logger(new qiu::Logger);
    logger->addAppender(qiu::LogAppender::ptr(new qiu::StdoutLogAppender));
    qiu::FileLogAppender::ptr file_appender(new qiu::FileLogAppender("./log.txt"));
    qiu::LogFormatter::ptr fmt(new qiu::LogFormatter("%d%T%p%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(qiu::LogLevel::ERROR);
    logger->addAppender(file_appender);
    
    //qiu::LogEvent::ptr event(new qiu::LogEvent(__FILE__, __LINE__ , 0, qiu::GetThreadId(), qiu::GetFiberId(), time(0)));
    //event->getSS() << "hello qiu log";
    //logger->log(qiu::LogLevel::DEBUG,event);
    std::cout << "hello qiu log" << std::endl;
    QIU_LOG_INFO(logger) << "test macro";
    QIU_LOG_ERROR(logger) << "test macro error";

    QIU_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aasdasa");
    
    auto l = qiu::LoggerMgr::GetInstance()->getLogger("xx");
    QIU_LOG_INFO(l) << "xxx";

    return 0;
}
