#ifndef __QIU_NONCOPYABLE_H__
#define __QIU_NONCOPYABLE_H__

namespace qiu {

class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}

#endif