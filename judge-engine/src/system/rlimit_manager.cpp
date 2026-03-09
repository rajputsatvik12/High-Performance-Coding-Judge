#include "system/rlimit_manager.hpp"
#include <sys/resource.h>


bool RLimitManager::set_memory_limit(const size_t& memory_limit){
    rlimit rl_memory;
    rl_memory.rlim_cur = memory_limit;
    rl_memory.rlim_max = memory_limit;

    return setrlimit(RLIMIT_AS, &rl_memory) == 0;
}

bool RLimitManager::set_cpu_limit(const int& time_limit){
    rlimit rl_cpu;
    rl_cpu.rlim_cur = time_limit + 1;
    rl_cpu.rlim_max = time_limit + 1;

    return setrlimit(RLIMIT_CPU, &rl_cpu) == 0;
}

bool RLimitManager::set_fsize_limit(){
    rlimit rl_fsize;
    rl_fsize.rlim_cur = max_file_size;
    rl_fsize.rlim_max = max_file_size;

    return setrlimit(RLIMIT_FSIZE, &rl_fsize) == 0;
}