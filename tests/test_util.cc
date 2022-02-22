#include "../scripts/scripts.h"
#include<assert.h>

qiu::Logger::ptr g_logger = QIU_LOG_ROOT();

void test_assert(){
    QIU_LOG_INFO(g_logger) << qiu::BacktraceToString(10, 2, "   ");
    QIU_ASSERT2(1 == 0,"abcdef xx");
}

int main(int argc, char** argv){
    test_assert();
    return 0;
}