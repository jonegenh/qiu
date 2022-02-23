#ifndef __QIU_ADDRESS_H__
#define __QIU_ADDRESS_H__

#include <memory>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>

namespace qiu{

class Address{
public:
    typedef std::shared_ptr<Address> ptr;
    virtual ~Address() {}

    int getFamily() const;
};

}

#endif