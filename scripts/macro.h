#ifndef __QIU_MACRO_H__
#define __QIU_MACRO_H__

#include<string.h>
#include<assert.h>
#include "util.h"

#if defined __GNUC__ || defined __llvm__
#   define QIU_LICKLY(x)       __builtin_expect(!!(x), 1)
#   define QIU_UNLICKLY(x)     __builtin_expect(!!(x), 0)
#else
#   define QIU_LICKLY(x)       (x)
#   define QIU_UNLICKLY(x)     (x)
#endif

#define QIU_ASSERT(x) \
    if(QIU_UNLICKLY(!(x))){ \
        QIU_LOG_ERROR(QIU_LOG_ROOT()) << "ASSERTION: " #x \
        << "\nbacktrace:\n" \
        << qiu::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define QIU_ASSERT2(x, w) \
    if(QIU_UNLICKLY(!(x))){ \
        QIU_LOG_ERROR(QIU_LOG_ROOT()) << "ASSERTION: " #x \
        << "\n" << w \
        << "\nbacktrace:\n" \
        << qiu::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#endif