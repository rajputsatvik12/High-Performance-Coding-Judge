#ifndef COMPILATION_RESULT_H
#define COMPILATION_RESULT_H

#include <filesystem>
#include <string>

struct CompilationResult{
    private:
        uint64_t submission_id;
        std::string problem_id;
        bool success;
        std::filesystem::path path_to_executable;
        std::string error_message;
        CompilationResult(uint64_t submission_id, std::string problem_id, bool success, std::filesystem::path path, std::string error);

    public:
        static CompilationResult successful_compilation(std::filesystem::path path, uint64_t submission_id, std::string problem_id);
        static CompilationResult failed_compilation(std::string error, uint64_t submission_id, std::string problem_id);
        bool is_successful() const;
        std::filesystem::path get_executable_path() const;
        std::string get_error_message() const;
        uint64_t get_submission_id() const;
        std::string get_problem_id() const;
};

#endif