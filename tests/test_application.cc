#include "scripts/log.h"
#include "scripts/application.h"

static qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

int main(int argc, char** argv){
    qiu::Application app;
    if(app.init(argc, argv)){
        return app.run();
    }
    return 0;
}