#include "scripts/env.h"
#include <iostream>
#include <unistd.h>
#include <fstream>
struct A{
    A() {
        std::ifstream ifs("/proc/" + std::to_string(getpid()) + "/cmdline", std::ios::binary);
        std::string content;
        content.resize(4096);

        ifs.read(&content[0], content.size());
        content.resize(ifs.gcount());
        
        for(size_t i = 0 ; i < content.size() ; ++i){
            std::cout << i << " - " << content[i] << " - " << (int)content[i] << std::endl;
        }
        std::cout << content << std::endl;
    }
};

A a;

int main(int argc, char** argv){
    qiu::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    qiu::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    qiu::EnvMgr::GetInstance()->addHelp("p", "print help");

    if(!qiu::EnvMgr::GetInstance()->init(argc, argv)){
        qiu::EnvMgr::GetInstance()->printHelp();
        return 0;
    }

    std::cout << "exe=" << qiu::EnvMgr::GetInstance()->getExe() << std::endl;
    std::cout << "cwd=" << qiu::EnvMgr::GetInstance()->getCwd() << std::endl;
    
    std::cout << "path=" << qiu::EnvMgr::GetInstance()->getEnv("TEST", "xxx") << std::endl;

    std::cout << "set=" << qiu::EnvMgr::GetInstance()->setEnv("TEST", "yyy") << std::endl;
    std::cout << "path=" << qiu::EnvMgr::GetInstance()->getEnv("TEST", "xxx") << std::endl;

    if(qiu::EnvMgr::GetInstance()->has("p")){
        qiu::EnvMgr::GetInstance()->printHelp();
    }

    return 0;
}