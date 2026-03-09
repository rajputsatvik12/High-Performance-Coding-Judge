#ifndef CGROUP_MANAGER_H
#define CGROUP_MANAGER_H

#include <string>
#include <filesystem> 

inline int runtime_overhead = 30; //MB

class CGroupManager{

    public:
        CGroupManager(const std::string& name);
        ~CGroupManager();

        bool create();
        bool destroy();

        bool set_memory_limit(size_t memory_limit);
        bool set_cpu_limit(int quota_us, int period_us);
        bool set_pids_limit(int max_pids);
        bool disable_swap();

        bool attach(pid_t pid);

        size_t get_memory_current() const; // hat peeche hat aaya memory current
        size_t get_memory_peak() const;

        const std::string& get_name() const;
        const std::filesystem::path get_path() const;

        private:
            std::string name;
            std::filesystem::path path_;
            bool created;
            bool write_file(const std::string& file, const std::string& value) const;
            std::string read_file(const std::string& file) const;

};

#endif