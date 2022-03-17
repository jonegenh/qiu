#include "scripts/http/http_parser.h"
#include "scripts/log.h"

static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

const char test_request_data[] = "POST / HTTP/1.1\r\n"
"Host: www.baidu.com\r\n"
"Content-Length: 10\r\n\r\n"
"1234567890";

void test_request(){
    qiu::http::HttpRequestParser parser;
    std::string tmp = test_request_data;
    size_t s = parser.execute(&tmp[0], tmp.size());
    QIU_LOG_INFO(g_logger) << "execute rt=" << s
    << " has_error=" << parser.hasError()
    << " is_finished=" << parser.isFinished()
    << " total=" << tmp.size()
    << " content_length=" << parser.getContentLength();
    tmp.resize(tmp.size() - s);
    QIU_LOG_INFO(g_logger) << parser.getData()->toString();
    QIU_LOG_INFO(g_logger) << tmp;
}

const char test_response_data[] = "HTTP/1.0 200 OK\r\n"
"Accept-Ranges: bytes\r\n"
"Cache-Control: no-cache\r\n"
"Content-Length: 9508\r\n"
"Content-Type: text/html\r\n\r\n"
"<html>\r\n"
"<meta http-equiv=\"refresh\" content=\"0;url=http://www.baidu.com/\">\r\n"
"</html>\r\n";

void test_response(){
    qiu::http::HttpResponseParser parser;
    std::string tmp = test_response_data;
    size_t s = parser.execute(&tmp[0], tmp.size());

    QIU_LOG_ERROR(g_logger) << "execute rt=" << s
    << " has_error=" << parser.hasError()
    << " is_finished=" << parser.isFinished()
    << " total=" << tmp.size()
    << " content_length=" << parser.getContentLength()
    << " tmp[s]=" << tmp[s];

    tmp.resize(tmp.size() - s);
    QIU_LOG_INFO(g_logger) << parser.getData()->toString();
    QIU_LOG_INFO(g_logger) << tmp;
}

int main(int argc, char** argv){
    test_request();
    QIU_LOG_INFO(g_logger) << "-----------------------------";
    test_response();
    
    return 0;
}