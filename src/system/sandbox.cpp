#include "system/sandbox.hpp"
#include <fstream>
#include <pqxx/pqxx>
#include <iostream>

// bool SandBox::get_limits(){
    
//     time_limit;
//     memory_limit;

//     return true;
// }

bool SandBox::destroy_group(){
    return cgroup_manager -> destroy();
}

bool SandBox::set_cgroup_limits(){

    if(!cgroup_manager -> create()){
        /*Error*/
        return false;
    }

    // system("ls -l /sys/fs/cgroup/myjudge/1");

    if(!cgroup_manager -> set_memory_limit(memory_limit) || 
    !cgroup_manager -> set_cpu_limit(1000000, 1000000) || 
    !cgroup_manager -> disable_swap() || 
    !cgroup_manager -> set_pids_limit(64)
){
        /*Error*/
        cgroup_manager -> destroy();
        return false;
    }

    // if (!cgroup_manager->set_memory_limit(memory_limit)) {
    // std::cerr << "set_memory_limit failed\n";
    // return false;
    // }

    // if (!cgroup_manager->set_cpu_limit(100000, 100000)) {
    //     std::cerr << "set_cpu_limit failed\n";
    //     return false;
    // }

    // if (!cgroup_manager->disable_swap()) {
    //     std::cerr << "disable_swap failed\n";
    //     return false;
    // }

    // if (!cgroup_manager->set_pids_limit(64)) {
    //     std::cerr << "set_pids_limit failed\n";
    //     return false;
    // }

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