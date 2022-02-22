#include "../scripts/scripts.h"

qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void run_in_fiber(){
    QIU_LOG_INFO(g_logger) << "run_in_fiber begin";
    qiu::Fiber::YieldToHold();
    QIU_LOG_INFO(g_logger) << "run_in_fiber end";
    qiu::Fiber::YieldToHold();
}

void test_fiber(){
    QIU_LOG_INFO(g_logger) << "main begin -1";
    {
        qiu::Fiber::GetThis();
        QIU_LOG_INFO(g_logger) << "main begin";
        qiu::Fiber::ptr fiber(new qiu::Fiber(run_in_fiber));
        fiber->swapIn();
        QIU_LOG_INFO(g_logger) << "main after swapIn";
        fiber->swapIn();
        QIU_LOG_INFO(g_logger) << "main after end";
        fiber->swapIn();
    }
    QIU_LOG_INFO(g_logger) << "main after end2";
}

int main(int argc, char** argv){
    qiu::Thread::SetName("main");

    std::vector<qiu::Thread::ptr> thrs;
    for(int i = 0 ; i < 3 ; ++i){
        thrs.push_back(qiu::Thread::ptr(new qiu::Thread(&test_fiber,"name_" + std::to_string(i))));
    }
    for(auto i : thrs){
        i->join();
    }
    return 0;
}