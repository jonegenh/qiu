#include "../scripts/socket.h"
#include "../scripts/scripts.h"
#include "../scripts/iomanager.h"
static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void test_socket(){
   qiu::IPAddress::ptr addr = qiu::Address::LookupAnyIPAddress("www.baidu.com");
    if(addr){
        QIU_LOG_INFO(g_logger) << "get address: " << addr->toString();
    }else {
        QIU_LOG_ERROR(g_logger) << "get address fail";
        return;
    }
    qiu::Socket::ptr sock = qiu::Socket::CreateTCP(addr);
    addr->setPort(80);
    QIU_LOG_INFO(g_logger) << "addr=" << addr->toString();
    if(!sock->connect(addr)){
        QIU_LOG_ERROR(g_logger) << "connect " << addr->toString() << " fail";
    }else{
        QIU_LOG_INFO(g_logger) << "connect " << addr->toString() << " connected";
    }

    const char buff[] = "GET / HTTP/1.0\r\n\r\n";
    int rt = sock->send(buff, sizeof(buff));
    if(rt <= 0){
        QIU_LOG_INFO(g_logger) << "send fail rt=" << rt;
        return; 
    }

    std::string buffs;
    buffs.resize(4096);
    rt = sock->recv(&buffs[0], buffs.size());

    if(rt <= 0){
        QIU_LOG_INFO(g_logger) << "recv fail rt=" << rt;
        return; 
    }

    buffs.resize(rt);
    QIU_LOG_INFO(g_logger) << buffs;
}

int main(int argc, char** argv){
    qiu::IOManager iom;
    iom.schedule(&test_socket);
    return 0;
}