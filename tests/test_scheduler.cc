#include "scripts/scripts.h"
#include<thread>
#include<vector>
qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void test_fiber(){
    static int s_count = 5;
    QIU_LOG_INFO(g_logger) << "test in fiber s_count" << s_count;
    
    sleep(1);
    if(--s_count >= 0){
        qiu::Scheduler::GetThis()->schedule(&test_fiber,qiu::GetThreadId());
    }
}

int main(int argc, char** argv){
    QIU_LOG_INFO(g_logger) << "main";
    qiu::Scheduler sc(3, false, "test");
    sc.start();
    sleep(2);
    QIU_LOG_INFO(g_logger) << "schedule";
    sc.schedule(&test_fiber);
    sc.stop();
    QIU_LOG_INFO(g_logger) << "over";
    return 0;
}