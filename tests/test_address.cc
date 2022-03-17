#include "scripts/address.h"
#include "scripts/log.h"

qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void test(){
    std::vector<qiu::Address::ptr> addrs;
    
    QIU_LOG_INFO(g_logger) << "begin";
    bool v = qiu::Address::Lookup(addrs, "www.sylar.top");
    QIU_LOG_INFO(g_logger) << "end";
    if(!v){
        QIU_LOG_ERROR(g_logger) << "lookup fail";
        return;
    }

    for(size_t i = 0 ; i < addrs.size() ; ++i){
        QIU_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
    }
}

void test_iface(){
    std::multimap<std::string, std::pair<qiu::Address::ptr, uint32_t> >  results;

    bool v = qiu::Address::GetInterfaceAddress(results);
    if(!v){
        QIU_LOG_ERROR(g_logger) << "GetInterfaceAddress fail";
        return;
    }
    
    for(auto& i : results){
        QIU_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString() << " - "
        << i.second.second;
    }
    
}

void test_ipv4(){
    auto addr = qiu::IPAddress::Create("127.0.0.8");
    if(addr){
        QIU_LOG_INFO(g_logger) << addr->toString();
    }
}

int main(int argc, char** argv){
    // test_ipv4();
    // test_iface();
    test();
    return 0;
}
