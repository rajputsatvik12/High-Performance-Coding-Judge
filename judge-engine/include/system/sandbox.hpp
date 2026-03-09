#ifndef SANDBOX_H
#define SANDBOX_H

#include "compiler/compilation_result.hpp"
#include "cgroup_manager.hpp"
#include "rlimit_manager.hpp"
#include <memory>

inline uint64_t time_limit;
inline size_t memory_limit;

class SandBox{
    private:
        std::unique_ptr<CGroupManager> cgroup_manager;
        CompilationResult compilation_result;
        std::unique_ptr<RLimitManager> rlimit_manager;

    public:
        bool get_limits();
        bool set_cgroup_limits();
        bool set_rlimits();
        bool destroy_group();
        bool attach(int pid);
        size_t get_memory_peak();
        SandBox(const CompilationResult& compilation_result):
        cgroup_manager(std::make_unique<CGroupManager>(std::to_string(compilation_result.get_submission_id()))),
    compilation_result(compilation_result),
    rlimit_manager(std::make_unique<RLimitManager>(std::to_string(compilation_result.get_submission_id()))) {}
        ~SandBox(){destroy_group();};
};

#endif