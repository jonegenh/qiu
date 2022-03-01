#ifndef __QIU_ENDIAN_H__
#define __QIU_ENDIAN_H__

#define QIU_LITTLE_ENDIAN 1
#define QIU_BIG_ENDIAN 2

#include <byteswap.h>
#include <stdint.h>
#include <type_traits>

namespace qiu{

template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t),T>::type
byteswap(T value){
    return (T)bswap_64((uint64_t)value);
}

template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t),T>::type
byteswap(T value){
    return (T)bswap_32((uint32_t)value);
}

template<class T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t),T>::type
byteswap(T value){
    return (T)bswap_16((uint16_t)value);
}

#if BYTE_ORDER == BIG_ENDIAN
#define QIU_BYTE_ORDER QIU_BIG_ENDIAN
#else
#define QIU_BYTE_ORDER QIU_LITTLE_ENDIAN
#endif

#if QIU_BYTE_ORDER == QIU_BIG_ENDIAN
template<class T>
T byteswapOnLittleEndian(T t){
    return t;
}

template<class T>
T byteswapOnBigEndian(T t){
    return byteswap(t);
}

#else

template<class T>
T byteswapOnLittleEndian(T t){
    return byteswap(t);
}

template<class T>
T byteswapOnBigEndian(T t){
    return t;
}

#endif

}


#endif