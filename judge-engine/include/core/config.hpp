#ifndef CONFIG_H
#define CONFIG_H

#include <chrono>
#include <filesystem>

struct ProblemConfig{
    std::chrono::milliseconds time_limit;
    size_t memory_limit; // bytes
    std::filesystem::path path_to_tests;
};

#endif