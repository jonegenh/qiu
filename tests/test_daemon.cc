#include "scripts/daemon.h"
#include "scripts/iomanager.h"
#include "scripts/log.h"

static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

qiu::Timer::ptr timer;
int server_main(int argc, char** argv){
    QIU_LOG_INFO(g_logger) << qiu::ProcessInfoMgr::GetInstance()->toString();
    qiu::IOManager iom(1);
    timer = iom.addTimer(1000, [](){
        QIU_LOG_INFO(g_logger) << "onTimer";
        static int count = 0;
        if(++count > 10){
            timer->cancel();
        }
    }, true);
    return 0;
}

int main(int argc, char** argv){
    return qiu::start_daemon(argc, argv, server_main, argc != 1);
}