#include "scripts/http/http.h"
#include "scripts/log.h"

void test_request(){
    qiu::http::HttpRequest::ptr req(new qiu::http::HttpRequest);
    req->setHeader("host", "www.baidu.com");
    req->setBody("hello qiu");

    req->dump(std::cout) << std::endl;
}

void test_response(){
    qiu::http::HttpResponse::ptr rsp(new qiu::http::HttpResponse);
    rsp->setHeader("X-X", "qiu");
    rsp->setBody("hello qiu");
    rsp->setStatus(qiu::http::HttpStatus(400));
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv){
    test_request();
    test_response();
    return 0;
}