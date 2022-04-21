#ifndef __QIU_APPLICATION_H__
#define __QIU_APPLICATION_H__

namespace qiu{

class Application{
public:
    Application();
    bool init(int argc, char** argv);
    bool run();
private:
    int main(int argc, char** argv);
private:
    int m_argc = 0;
    char** m_argv = nullptr;

    static Application* s_instance;

};

}


#endif