#include"../scripts/scripts.h"
#include<unistd.h>
#include<thread>
qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

int count = 0;
// qiu::RWMutex s_mutex;
qiu::Mutex s_mutex;

void fun1(){
    QIU_LOG_INFO(g_logger) << "name: " << qiu::Thread::GetName() << " this:name: " << qiu::Thread::GetThis()->getName()
    << " id: " << qiu::GetThreadId()
    << " this.id:" << qiu::Thread::GetThis()->getId();

    for(int i = 0 ; i < 100000 ; ++i){
        // qiu::RWMutex::WriteLock lock(s_mutex);
        qiu::Mutex::Lock lock(s_mutex);
        count++;
    }
}
void fun2(){
    while(true){
        QIU_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3(){
    while(true){
        QIU_LOG_INFO(g_logger) << "==============================================";
    }
}

int main(int argc, char** argv){
    QIU_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/qiu/cppws/Myproject/bin/conf/log2.yml");
    qiu::Config::LoadFromYaml(root);

    std::vector<qiu::Thread::ptr> thrs;
    for(int i = 0 ; i < 2 ; ++i){
        qiu::Thread::ptr thr(new qiu::Thread(&fun2, "name_" + std::to_string(i * 2)));
        qiu::Thread::ptr thr2(new qiu::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        thrs.push_back(thr2);
    }
    
    for(size_t i = 0 ; i < thrs.size() ; ++i){
        thrs[i]->join();
    }
    
    QIU_LOG_INFO(g_logger) << "count=" << count;
    QIU_LOG_INFO(g_logger) << "thread test end";
    return 0;
}