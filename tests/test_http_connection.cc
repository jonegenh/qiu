#include <iostream>
#include "scripts/http/http_connection.h"
#include "scripts/log.h"
#include "scripts/iomanager.h"
#include <fstream>

static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void test_pool(){
    qiu::http::HttpConnectionPool::ptr pool(new qiu::http::HttpConnectionPool(
        "www.sylar.top", "", 80, 10, 1000 * 30, 5));
    qiu::IOManager::GetThis()->addTimer(1000, [pool](){
        auto r = pool->doGet("/", 300);
        QIU_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void run(){
    qiu::Address::ptr addr = qiu::Address::LookupAnyIPAddress("www.sylar.top:80");
    if(!addr){
        QIU_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    qiu::Socket::ptr sock = qiu::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt){
        QIU_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    qiu::http::HttpConnection::ptr conn(new qiu::http::HttpConnection(sock));
    qiu::http::HttpRequest::ptr req(new qiu::http::HttpRequest);
    req->setPath("/blog/");
    req->setHeader("host", "www.sylar.top");
    QIU_LOG_INFO(g_logger) << "req:" << std::endl
        << *req;
    
    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if(!rsp){
        QIU_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    QIU_LOG_INFO(g_logger) << "rsp:" << std::endl
        << *rsp;

    std::ofstream ofs("rsp.dat");
    ofs << *rsp;

    QIU_LOG_INFO(g_logger) << "========================";

    auto rt1 = qiu::http::HttpConnection::DoGet("http://www.sylar.top/blog/", 3000);
    QIU_LOG_INFO(g_logger) << "result=" << rt1->result
                           << " error=" << rt1->error
                           << " rsp=" << (rt1->response ? rt1->response->toString() : "");

    QIU_LOG_INFO(g_logger) << "========================";
    test_pool();
}

int main(int argc, char** argv){
    qiu::IOManager iom(2);
    iom.schedule(run);    
    return 0;
}