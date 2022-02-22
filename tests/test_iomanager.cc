#include "../scripts/scripts.h"
#include "../scripts/iomanager.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

int sock = 0;

void test_fiber(){
    QIU_LOG_INFO(g_logger) << "test_fiber sock=" << sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);
    
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr)); 
    addr.sin_family = AF_INET;
    addr.sin_port = htons(80);
    inet_pton(AF_INET, "182.61.200.6", &addr.sin_addr.s_addr);

    if(!connect(sock, (const sockaddr*)&addr, sizeof(addr))){
    }else if(errno == EINPROGRESS){
        QIU_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        qiu::IOManager::GetThis()->addEvent(sock, qiu::IOManager::READ, [](){
            QIU_LOG_INFO(g_logger) << "read callback";
        });
        qiu::IOManager::GetThis()->addEvent(sock, qiu::IOManager::WRITE, [](){
            QIU_LOG_INFO(g_logger) << "write callback";
            // close(sock);
            qiu::IOManager::GetThis()->cancelEvent(sock, qiu::IOManager::READ);
            close(sock);
        });
    }else{
        QIU_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }

}

void test1(){
    std::cout << "EPOLLIN=" << EPOLLIN
    << " EPOLLOUT=" << EPOLLOUT << std::endl;
    qiu::IOManager iom(2, false);
    iom.schedule(&test_fiber);
}

qiu::Timer::ptr s_timer;
void test_timer(){
    qiu::IOManager iom(2);
    s_timer = iom.addTimer(1000,[](){
        static int i = 0;
        QIU_LOG_INFO(g_logger) << "hello timer i=" << i;
        if(++i == 3){
            s_timer->reset(2000, true);
            // s_timer->cancel();
        }
    },true);
}

int main(int argc, char** argv){
    //test1();
    test_timer();
    return 0;
}