#include "scripts/http/http_server.h"
#include "scripts/log.h"

static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void run(){
    qiu::http::HttpServer::ptr server(new qiu::http::HttpServer);
    qiu::Address::ptr addr = qiu::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)){
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    
    sd->addServlet("/qiu/xx", [](qiu::http::HttpRequest::ptr req, 
    qiu::http::HttpResponse::ptr rsp, qiu::http::HttpSession::ptr session){
        rsp->setBody(req->toString());
        return 0;
    });

    sd->addGlobServlet("/qiu/*", [](qiu::http::HttpRequest::ptr req, 
    qiu::http::HttpResponse::ptr rsp, qiu::http::HttpSession::ptr session){
        rsp->setBody("Glob:\r\n" + req->toString());
        return 0;
    });
    server->start();
}


int main(int argc, char** argv){
    qiu::IOManager iom(2);
    iom.schedule(run);
    return 0;
}