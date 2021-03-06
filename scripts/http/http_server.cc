#include "http_server.h"
#include "scripts/log.h"

namespace qiu{
namespace http{

static qiu::Logger::ptr g_logger = QIU_LOG_NAME("system");

HttpServer::HttpServer(bool keepalive,qiu::IOManager* worker
    ,qiu::IOManager* accept_worker)
    :TcpServer(worker,accept_worker)
    ,m_isKeepalive(keepalive){
    m_dispatch.reset(new ServletDispatch);
}

void HttpServer::handleClient(Socket::ptr client) {
    HttpSession::ptr session(new HttpSession(client));
    do{
        auto req = session->recvRequest();
        if(!req){
            QIU_LOG_WARN(g_logger) << "recv http request fail, errno="
            << errno << " errstr=" << strerror(errno)
            << " client:" << *client;
            break;
        }
        HttpResponse::ptr rsp(new HttpResponse(req->getVersion(), req->isClose() || !m_isKeepalive));
        rsp->setHeader("Server", getName());
        m_dispatch->handle(req, rsp, session);
        // rsp->setBody("hello qiu");

        // QIU_LOG_INFO(g_logger) << "request:" << std::endl
        // << *req;
        // QIU_LOG_INFO(g_logger) << "request:" << std::endl
        // << *rsp;
        session->sendResponse(rsp);
        if(!m_isKeepalive || req->isClose()){
            break;
        }
    } while(true);
    session->close();
}

}
}