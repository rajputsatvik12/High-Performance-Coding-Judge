#include "compiler/compilation_result.hpp"

bool CompilationResult::is_successful() const{
    return this -> success;
}

std::filesystem::path CompilationResult::get_executable_path() const{
    return this -> path_to_executable;
}

std::string CompilationResult::get_error_message() const{
    return this -> error_message;
}

uint64_t CompilationResult::get_submission_id() const{
    return this -> submission_id;
}

int CompilationResult::get_problem_id() const{
    return this -> problem_id;
}

CompilationResult::CompilationResult(
    uint64_t submission_id,
    int problem_id,
    bool success,
    std::filesystem::path path,
    std::string error
)
    :submission_id(submission_id),
    problem_id(problem_id),
    success(success),
    path_to_executable(path),
    error_message(error)
    {}

CompilationResult CompilationResult::successful_compilation(std::filesystem::path path, uint64_t submission_id, int problem_id){
    return CompilationResult(
        submission_id,
        problem_id,
        true,
        path,
        ""
    );
}

CompilationResult CompilationResult::failed_compilation(std::string error, uint64_t submission_id, int problem_id){
    return CompilationResult(
        submission_id,
        problem_id,
        false,
        "",
        error
    );
}
