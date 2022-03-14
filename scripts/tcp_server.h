#ifndef __QIU_TCP_SERVER_H__
#define __QIU_TCP_SERVER_H__

#include <memory>
#include <functional>
#include "iomanager.h"
#include "socket.h"
#include "address.h"
#include "noncopyable.h"

namespace qiu{

class TcpServer: public std::enable_shared_from_this<TcpServer> 
                , Noncopyable {
public:
    typedef std::shared_ptr<TcpServer> ptr;
    TcpServer(qiu::IOManager* worker = qiu::IOManager::GetThis(),
              qiu::IOManager* accept_worker = qiu::IOManager::GetThis());
    virtual ~TcpServer();

    virtual bool bind(qiu::Address::ptr addr);
    virtual bool bind(const std::vector<Address::ptr>& addrs, 
                    std::vector<Address::ptr>& fails);
    virtual bool start();
    virtual void stop();

    uint64_t getRecvTimeout() const { return m_recvTimeout;}
    std::string getName() const { return m_name;}
    void setRecvTimeout(uint64_t v) { m_recvTimeout = v;}
    void setName(const std::string& v) { m_name = v;}

    bool isStop() const { return m_isStop;}
protected:
    virtual void handleClient(Socket::ptr client);
    virtual void startAccept(Socket::ptr sock);
private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    IOManager* m_acceptworker;
    uint64_t m_recvTimeout;
    std::string m_name;
    bool m_isStop;
};

}

#endif