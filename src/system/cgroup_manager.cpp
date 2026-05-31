#include "system/cgroup_manager.hpp"
#include <string>
#include <filesystem>
#include <sys/stat.h>
#include <sys/wait.h>
#include<fstream>
#include <iostream>
#include <cstring>

CGroupManager::CGroupManager(const std::string& name)
    :name(name),
    path_("/sys/fs/cgroup/myjudge/" + name),
    created(false) {}

bool CGroupManager::create(){
    if (mkdir(path_.c_str(), 0755) != 0) {
    std::cerr << "mkdir(" << path_ << ") failed: " << std::strerror(errno) << '\n';
    return false;
}
    std::cerr << "Exists? " << std::filesystem::exists(path_) << '\n';
    created = true;
    return true;
}

bool CGroupManager::destroy(){
    if(!created) return false;

    if(rmdir(path_.c_str()) != 0) return false;

    created = false;
    return true;
}

bool CGroupManager::write_file(const std::string& file, const std::string& value) const{
    std::ofstream out(path_/file);

    if (!out.is_open()) {
    std::cerr << "Failed opening " << (path_ / file) << ": " << strerror(errno) << '\n';
    return false;
}

    out << value;
    
    return out.good();
}

std::string CGroupManager::read_file(const std::string& file) const{
    std::ifstream in(path_/file);
    std::cout << file << '\n';
    if(!in.is_open()) throw std::runtime_error("Could not open file!");

    std::string content;
    std::getline(in, content);

    if(content.empty()) throw std::runtime_error("Empty contents!");

    return content;
}

bool CGroupManager::set_memory_limit(size_t memory_limit){
    return write_file("memory.max",std::to_string(memory_limit));
}

bool CGroupManager::set_cpu_limit(int quota_us, int period_us){
    return write_file("cpu.max", std::to_string(quota_us) + " " + std::to_string(period_us));
}

bool CGroupManager::set_pids_limit(int max_pids){
    return write_file("pids.max",std::to_string(max_pids));
}

bool CGroupManager::disable_swap(){
    return write_file("memory.swap.max", "0");
}

bool CGroupManager::attach(pid_t pid){
    return write_file("cgroup.procs",std::to_string(pid));
}

size_t CGroupManager::get_memory_current() const{
    return std::stoull(read_file("memory.current"));
}

size_t CGroupManager::get_memory_peak() const{
    return std::stoull(read_file("memory.peak"));
}

CGroupManager::~CGroupManager(){
    if(created) rmdir(path_.c_str());
}