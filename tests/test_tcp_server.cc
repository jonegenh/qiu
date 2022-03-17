#include "scripts/tcp_server.h"
#include "scripts/iomanager.h"
#include "scripts/log.h"

qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void run(){
    auto addr = qiu::Address::LookupAny("0.0.0.0:8033");
    // auto addr2 = qiu::UnixAddress::ptr(new qiu::UnixAddress("/tmp/unix_addr"));
    QIU_LOG_INFO(g_logger) << *addr;
    std::vector<qiu::Address::ptr> addrs;
    addrs.push_back(addr);
    // addrs.push_back(addr2);

    qiu::TcpServer::ptr tcp_server(new qiu::TcpServer);
    std::vector<qiu::Address::ptr> fails;
    while(!tcp_server->bind(addrs, fails)){
        sleep(2);
    }
    tcp_server->start();
    QIU_LOG_INFO(g_logger) << "begin start";
}

int main(int argc, char** argv){
    qiu::IOManager iom(2);
    iom.schedule(run);
    return 0;
}