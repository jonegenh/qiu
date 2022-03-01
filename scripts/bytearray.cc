#include "bytearray.h"
#include "endian.h"
#include <string.h>
namespace qiu{

ByteArray::Node::Node(size_t s)
    :ptr(new char[s])
    ,size(s)
    ,next(nullptr){
}

ByteArray::Node::Node()
    :ptr(nullptr)
    ,size(0)
    ,next(nullptr){
}

ByteArray::Node::~Node(){
    if(ptr){
        delete[] ptr;
    }
}


ByteArray::ByteArray(size_t base_size)
    :m_baseSize(base_size)
    ,m_position(0)
    ,m_capacity(base_size)
    ,m_size(0)
    ,m_endian(QIU_BIG_ENDIAN)
    ,m_root(new Node(base_size))
    ,m_cur(m_root){

}

ByteArray::~ByteArray(){
    Node* tmp = m_root;
    while(tmp){
        m_cur = tmp;
        tmp = tmp->next;
        delete m_cur;
    }
}

bool ByteArray::isLittleEndian() const{
    return m_endian == QIU_LITTLE_ENDIAN;
}

void ByteArray::setIsLittleEndian(bool val){
    if(val){
        m_endian = QIU_LITTLE_ENDIAN;
    }else{
        m_endian = QIU_BIG_ENDIAN;
    }
}

void ByteArray::writeFint8(int8_t value){
    write(&value, sizeof(value));
}

void ByteArray::writeFuint8(uint8_t value){
    write(&value, sizeof(value));
}

void ByteArray::writeFint16(int16_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

void ByteArray::writeFuint16(uint16_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

void ByteArray::writeFint32(int32_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

void ByteArray::writeFuint32(uint32_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

void ByteArray::writeFint64(int64_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

void ByteArray::writeFuint64(uint64_t value){
    if(m_endian != QIU_BYTE_ORDER){
        value = byteswap(value);
    }
    write(&value, sizeof(value));
}

static uint32_t EncodeZigzag32(const int32_t& v){
    if(v < 0){
        return ((uint32_t)(-v)) * 2 - 1;
    } else{
        return v * 2;
    }
}

static uint64_t EncodeZigzag64(const int64_t& v){
    if(v < 0){
        return ((uint64_t)(-v)) * 2 - 1;
    } else{
        return v * 2;
    }
}

static int32_t DecodeZigzag32(const uint32_t& v){
    return (v >> 1) ^ -(v & 1);
}

static int64_t DecodeZigzag64(const uint64_t& v){
    return (v >> 1) ^ -(v & 1);
}

void ByteArray::writeInt32(int32_t value){
    writeUint32(EncodeZigzag32(value));
}

void ByteArray::writeUint32(uint32_t value){
    uint8_t tmp[5];
    uint8_t i = 0;
    while(value >= 0x80){
        tmp[i++] = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    tmp[i++] = value;
    write(tmp, i);
}

void ByteArray::writeInt64(int64_t value){
    writeUint64(EncodeZigzag64(value));
}

void ByteArray::writeUint64(uint64_t value){
    uint8_t tmp[10];
    uint8_t i = 0;
    while(value >= 0x80){
        tmp[i++] = (value & 0x7F) | 0x80;
        value >>= 7;
    }
    tmp[i++] = value;
    write(tmp, i);
}

void ByteArray::writeFloat(float value){
    uint32_t v;
    memcpy(&v, &value, sizeof(value));
    writeFuint32(v);
}

void ByteArray::writeDouble(double value){
    uint64_t v;
    memcpy(&v, &value, sizeof(value));
    writeFuint64(v);
}

void ByteArray::writeStringF16(const std::string& value){
    writeFuint16(value.size());
    write(value.c_str(), value.size());
}

void ByteArray::writeStringF32(const std::string& value){
    writeFuint32(value.size());
    write(value.c_str(), value.size());
}

void ByteArray::writeStringF64(const std::string& value){
    writeFuint64(value.size());
    write(value.c_str(), value.size());
}

void ByteArray::writeStringVint(const std::string& value){
    writeUint64(value.size());
    write(value.c_str(), value.size());
}

void ByteArray::writeStringWithoutLength(const std::string& value){
    write(value.c_str(), value.size());
}

int8_t ByteArray::readFint8(){
    int8_t v;
    read(&v, sizeof(v));
    return v;
}

uint8_t ByteArray::readFuint8(){
    uint8_t v;
    read(&v, sizeof(v));
    return v;
}

#define XX(type) \
    type v; \
    read(&v, sizeof(v)); \
    if(m_endian == QIU_BYTE_ORDER){ \
        return v; \
    }else{ \
        return byteswap(v); \
    }

int16_t ByteArray::readFint16(){
    XX(int16_t);
}

uint16_t ByteArray::readFuint16(){
    XX(uint16_t);
}

int32_t ByteArray::readFint32(){
    XX(int32_t);
}

uint32_t ByteArray::readFuint32(){
    XX(uint32_t);
}

int64_t ByteArray::readFint64(){
    XX(int64_t);
}

uint64_t ByteArray::readFuint64(){
    XX(uint64_t);
}

#undef XX

int32_t ByteArray::readInt32(){
    return DecodeZigzag32(readUint32());
}

uint32_t ByteArray::readUint32(){
    uint32_t result = 0;
    for(int i = 0 ; i < 32 ; i += 7){
        uint8_t b = readFuint8();
        if(b < 0x80){
            result |= ((uint32_t)b) << i;
            break;
        }else{
            result |= ((uint32_t)(b & 0x7F)) << i;
        }
    }
    return result;
}

int64_t ByteArray::readInt64(){
    return DecodeZigzag64(readUint64());
}

uint64_t ByteArray::readUint64(){
    uint64_t result = 0;
    for(int i = 0 ; i < 64 ; i += 7){
        uint8_t b = readFuint8();
        if(b < 0x80){
            result |= ((uint64_t)b) << i;
            break;
        }else{
            result |= ((uint64_t)(b & 0x7F)) << i;
        }
    }
    return result;
}

float ByteArray::readFloat(){
    uint32_t v = readFuint32();
    float value;
    memcpy(&value, &v, sizeof(v));
    return value;
}

double ByteArray::readDouble(){
    uint64_t v = readFuint64();
    double value;
    memcpy(&value, &v, sizeof(v));
    return value;
}

std::string ByteArray::readStringF16(){
    uint16_t len = readFuint16();
    std::string buff(len);
    read(&buff[0], len);
    return buff;
}

std::string ByteArray::readStringF32(){
    uint32_t len = readFuint32();
    std::string buff(len);
    read(&buff[0], len);
    return buff;
}

std::string ByteArray::readStringF64(){
    uint64_t len = readFuint64();
    std::string buff(len);
    read(&buff[0], len);
    return buff;
}

std::string ByteArray::readStringFVint(){
    uint64_t len = readFuint64();
    std::string buff(len);
    read(&buff[0], len);
    return buff;
}

void ByteArray::clear(){
    m_position = m_size = 0;
    m_capacity = m_baseSize;
    Node* tmp = m_root->next;
    while(tmp){
        m_cur = tmp;
        tmp = tmp->next;
        delete m_cur;
    }
    m_cur = m_root;
    m_root->next = nullptr;
}

void ByteArray::write(const void* buf, size_t size){
    if(size == 0){
        return;
    }
    addCapacity(size);

    size_t npos = m_position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;

    while(size > 0){
        if(ncap >= size){
            memcpy(m_cur->ptr + npos, buf + bpos, size);
            m_position += size;
            bpos += size;
            size = 0;
        }else {
            memcpy(m_cur->ptr + npos, buf + bpos, ncap);
            m_position += ncap;
            bpos += ncap;
            size -= ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }

    if(m_position > m_size){
        m_size = m_position;
    }
}

void ByteArray::read(void* buf, size_t size){
    if(size > getReadSize()){
        throw std::out_of_range("not enough len");
    }

    size_t npos = m_position % m_baseSize;
    size_t ncap = m_cur->size - npos;
    size_t bpos = 0;
    while(size > 0){
        if(ncap >= size){
            memcpy(buf + bpos, m_cur->ptr + npos, size);
            if(m_cur->size == npos + size){
                m_cur = m_cur->next;
            }
            m_position += size;
            bpos += size;
            size = 0;
        }else{
            memcpy(buf + bpos, m_cur->ptr + npos, ncap);
            m_position += ncap;
            bpos += ncap;
            size -= ncap;
            m_cur = m_cur->next;
            ncap = m_cur->size;
            npos = 0;
        }
    }
}

void ByteArray::setPosition(size_t v){

}

bool ByteArray::writeToFile(const std::string& name) const{

}

void ByteArray::readFromFile(const std::string& name){

}

void ByteArray::addCapacity(size_t size){

}

}