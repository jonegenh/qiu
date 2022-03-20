#ifndef __QIU_HTTP_HTTP_SERVER_H__
#define __QIU_HTTP_HTTP_SERVER_H__

#include "scripts/tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace qiu{
namespace http{

class HttpServer : public TcpServer{
public:
    typedef std::shared_ptr<HttpServer> ptr;
    HttpServer(bool keepalive = false
        ,qiu::IOManager* worker = qiu::IOManager::GetThis()
        ,qiu::IOManager* accept_worker = qiu::IOManager::GetThis());

    ServletDispatch::ptr getServletDispatch() const { return m_dispatch;}
    void setServletDispatch(ServletDispatch::ptr v) { m_dispatch = v;}
protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    bool m_isKeepalive;
    ServletDispatch::ptr m_dispatch;
};

}
}

#endif