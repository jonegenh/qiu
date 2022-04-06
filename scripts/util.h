#ifndef __QIU_UTIL_H__
#define __QIU_UTIL_H__

#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdint.h>
#include<vector>
#include<string>
namespace qiu {
pid_t GetThreadId();
uint32_t GetFiberId();

void Bacatrace(std::vector<std::string>& bt, int size = 64, int skip = 1);
std::string BacktraceToString(int size = 64, int skip = 2, const std::string& prefix = "");

// 时间ms
uint64_t GetCurrentMs();
uint64_t GetCurrentUs();

std::string Time2Str(time_t ts = time(0), const std::string& format = "%Y-%m-%d %H:%M:%S");

} // namespace qiu

#endif