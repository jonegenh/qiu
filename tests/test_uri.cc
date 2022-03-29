#include "scripts/uri.h"
#include "scripts/log.h"
#include <iostream>


int main(int argc, char** argv){
    qiu::Uri::ptr uri = qiu::Uri::Create("http://admin@www.sylar.top/test/中文/中文/uri?id=100&name=sylar#frg");
    std::cout << uri->toString() << std::endl;
    auto addr = uri->createAddress();
    std::cout << *addr << std::endl;
    
    return 0;
}