#ifndef __QIU_HTTP_SESSION_H__
#define __QIU_HTTP_SESSION_H__

#include "scripts/socket_stream.h"
#include "http.h"

namespace qiu {
namespace http{

class HttpSession : public SocketStream{
public:
    typedef std::shared_ptr<HttpSession> ptr;
    HttpSession(Socket::ptr sock, bool owner = true);
    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr rsp);
private:
};

}
}

#endif