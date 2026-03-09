#ifndef EXECUTION_RESULT_H
#define EXECUTION_RESULT_H

#include <filesystem>
#include "executor/termination_signal.hpp"

struct ExecutionResult{
    private:
        bool success;
        int exit_code;
        terminationSignal termination_signal;
        std::filesystem::path path_to_stdout;
        std::filesystem::path path_to_stderr;
        uint64_t execution_time_ms;
        size_t peak_memory;
        std::string error_message;
        uint64_t submission_id;
        std::string problem_id;
        ExecutionResult(
                bool success,
                int exit_code, 
                terminationSignal termination_signal,
                std::filesystem::path path_to_stdout,
                std::filesystem::path path_to_stderr,
                uint64_t execution_time_ms,
                size_t peak_memory,
                std::string error_message,
                uint64_t submission_id,
                std::string problem_id
            );

    public:
        static ExecutionResult successful_execution(std::filesystem::path path_to_stdout, std::filesystem::path path_to_stderr, uint64_t execution_time_ms, size_t peak_memory, uint64_t submission_id, std::string problem_id);
        static ExecutionResult failed_execution(int exit_code, terminationSignal termination_signal, std::filesystem::path path_to_stdout, std::filesystem::path path_to_stderr, uint64_t execution_time_ms, size_t peak_memory, uint64_t submission_id, std::string problem_id);
        static ExecutionResult internal_error(std::string error_message, uint64_t submission_id, std::string problem_id);

        bool is_successful() const;
        int get_exit_code() const;
        terminationSignal get_termination_signal() const;
        std::filesystem::path get_stdout_path() const;
        std::filesystem::path get_stderr_path() const;
        uint64_t get_execution_time_ms() const;
        size_t get_peak_memory() const;
        std::string get_internal_error() const;
        uint64_t get_submission_id() const;
        std::string get_problem_id() const;
};

#endif