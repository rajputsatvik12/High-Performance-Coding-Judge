#include "system/sandbox.hpp"
#include <fstream>
#include <jsoncpp/json/json.h>

bool SandBox::get_limits(){
    std::filesystem::path path_to_config = "problems/" + compilation_result.get_problem_id() + "/config.json";

    std::ifstream problem_config(path_to_config);
    Json::Value root;
    Json::Reader reader;
    if(!reader.parse(problem_config, root)){
        cgroup_manager -> destroy();
        return false;
    }

    time_limit = root["time_limit"].asInt();
    memory_limit = (root["memory_limit"].asInt() + runtime_overhead) * 1024 * 1024;

    problem_config.close();
    return true;
}

bool SandBox::destroy_group(){
    return cgroup_manager -> destroy();
}

bool SandBox::set_cgroup_limits(){

    if(!cgroup_manager -> create()){
        /*Error*/
        return false;
    }

    if(!cgroup_manager -> set_memory_limit(memory_limit) || 
    !cgroup_manager -> set_cpu_limit(100000, 100000) || 
    !cgroup_manager -> disable_swap() || 
    !cgroup_manager -> set_pids_limit(64)
){
        /*Error*/
        cgroup_manager -> destroy();
        return false;
    }

    return true;
}

bool SandBox::set_rlimits(){
    return rlimit_manager -> set_memory_limit(memory_limit) && 
        rlimit_manager -> set_cpu_limit(time_limit) &&
        rlimit_manager -> set_fsize_limit();
}

bool SandBox::attach(int pid){
    return cgroup_manager -> attach(pid);
}

size_t SandBox::get_memory_peak(){
    return cgroup_manager -> get_memory_peak();
}