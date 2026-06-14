#include "executor/execution_result.hpp"

bool ExecutionResult::is_successful() const{
    return this -> success;
}

int ExecutionResult::get_exit_code() const{
    return this -> exit_code;
}

terminationSignal ExecutionResult::get_termination_signal() const{
    return this -> termination_signal;
}

std::filesystem::path ExecutionResult::get_stdout_path() const{
    return this -> path_to_stdout;
}

std::filesystem::path ExecutionResult::get_stderr_path() const{
    return this -> path_to_stderr;
}

uint64_t ExecutionResult::get_execution_time_ms() const{
    return this -> execution_time_ms;
}

size_t ExecutionResult::get_peak_memory() const{
    return this -> peak_memory;
}

std::string ExecutionResult::get_internal_error() const{
    return this -> error_message;
}

uint64_t ExecutionResult::get_submission_id() const{
    return this -> submission_id;
}

int ExecutionResult::get_problem_id() const{
    return this -> problem_id;
}

ExecutionResult::ExecutionResult(
    bool success,
    int exit_code,
    terminationSignal termination_signal,
    std::filesystem::path path_to_stdout,
    std::filesystem::path path_to_stderr,
    uint64_t execution_time_ms,
    size_t peak_memory,
    std::string error_message,
    uint64_t submission_id,
    int problem_id
)
    :success(success),
    exit_code(exit_code),
    termination_signal(termination_signal),
    path_to_stdout(path_to_stdout),
    path_to_stderr(path_to_stderr),
    execution_time_ms(execution_time_ms),
    peak_memory(peak_memory),
    error_message(error_message),
    submission_id(submission_id),
    problem_id(problem_id)
{}

ExecutionResult ExecutionResult::successful_execution(std::filesystem::path path_to_stdout, std::filesystem::path path_to_stderr, uint64_t execution_time_ms, size_t peak_memory, uint64_t submission_id, int problem_id){
    return ExecutionResult(
        true,
        0,
        terminationSignal::NONE,
        path_to_stdout,
        path_to_stderr,
        execution_time_ms,
        peak_memory,
        "",
        submission_id,
        problem_id
    );
}

ExecutionResult ExecutionResult::failed_execution(int exit_code, terminationSignal termination_signal, std::filesystem::path path_to_stdout, std::filesystem::path path_to_stderr, uint64_t execution_time_ms, size_t peak_memory, uint64_t submission_id, int problem_id){
    return ExecutionResult(
        false,
        exit_code,
        termination_signal,
        path_to_stdout,
        path_to_stderr,
        execution_time_ms,
        peak_memory,
        "",
        submission_id,
        problem_id
    );
}

ExecutionResult ExecutionResult::internal_error(std::string error_message, uint64_t submission_id, int problem_id){
    return ExecutionResult(
        false,
        0,
        terminationSignal::NONE,
        "",
        "",
        0,
        0,
        error_message,
        submission_id,
        problem_id
    );
}