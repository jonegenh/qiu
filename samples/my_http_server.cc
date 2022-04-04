#include "scripts/http/http_server.h"
#include "scripts/log.h"

qiu::Logger::ptr g_logger = QIU_LOG_NAME("system");

void run(){
    qiu::Address::ptr addr = qiu::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr){
        QIU_LOG_ERROR(g_logger) << "get address error";
        return; 
    }

    qiu::http::HttpServer::ptr http_server(new qiu::http::HttpServer(true));
    while(!http_server->bind(addr)){
        QIU_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }

    http_server->start();
}

int main(int argc, char** argv){
    g_logger->setLevel(qiu::LogLevel::ERROR);
    qiu::IOManager iom(1);
    iom.schedule(run);
    return 0;
}