#ifndef RLIMIT_MANAGER_H
#define RLIMIT_MANAGER_H

#include <string>

const long long max_file_size = 10 * 1024 * 1024; // 10 MB

class RLimitManager{
    private:
        std::string name;

    public:
        RLimitManager(const std::string& name):
        name(name){};

        bool set_memory_limit(const size_t& memory_limit);
        bool set_cpu_limit(const int& time_limit);
        bool set_fsize_limit();
    
};

#endif