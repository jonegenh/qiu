#ifndef __QIU_MACRO_H__
#define __QIU_MACRO_H__

#include<string.h>
#include<assert.h>
#include "util.h"

#define QIU_ASSERT(x) \
    if(!(x)){ \
        QIU_LOG_ERROR(QIU_LOG_ROOT()) << "ASSERTION: " #x \
        << "\nbacktrace:\n" \
        << qiu::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define QIU_ASSERT2(x, w) \
    if(!(x)){ \
        QIU_LOG_ERROR(QIU_LOG_ROOT()) << "ASSERTION: " #x \
        << "\n" << w \
        << "\nbacktrace:\n" \
        << qiu::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif