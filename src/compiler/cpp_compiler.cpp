#include "compiler/cpp_compiler.hpp"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <vector>

CompilationResult CppCompiler::compile(const SubmissionRecord& submission){
    std::filesystem::path path_to_executable = "submissions/" + std::to_string(submission.submission_id) + "/main_exec";
    std::string source = "submissions/" + std::to_string(submission.submission_id) + "/source.cpp";
    std::string output = path_to_executable.string();

    char* argv[] = {
        const_cast<char*>("g++"),
        const_cast<char*>(source.c_str()),
        const_cast<char*>("-o"),
        const_cast<char*>(output.c_str()),
        const_cast<char*>("-std=c++20"),
        const_cast<char*>("-O2"),
        const_cast<char*>("-Wall"),
        NULL
    };

    int pipefd[2];
    int res = pipe(pipefd);

    if(res == -1){
        /*Failure: Internal Error*/
        return CompilationResult::failed_compilation("Pipe failed", submission.submission_id, submission.problem_id);
    }

    pid_t pid = fork();

    if(pid < 0){
        /*Failure: Internal Error*/
        close(pipefd[0]);
        close(pipefd[1]);
        return CompilationResult::failed_compilation("Fork failed in compiler", submission.submission_id, submission.problem_id);
    }

    char buffer[4096];
    ssize_t bytes_read;
    std::string error;

    if(pid == 0){
        close(pipefd[0]);
        dup2(pipefd[1], STDERR_FILENO);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execvp("g++", argv); 
        _exit(1);
    }
    else{
        close(pipefd[1]);
        while((bytes_read = read(pipefd[0],buffer, sizeof(buffer))) > 0){
            error.append(buffer, bytes_read);
        }
        close(pipefd[0]);
    }

    int status;
    
    if(waitpid(pid, &status, 0) == -1){
        return CompilationResult::failed_compilation("Waitpid error in compiler", submission.submission_id, submission.problem_id);
    }

    if(!WIFEXITED(status) || WEXITSTATUS(status) != 0){
        return CompilationResult::failed_compilation(error, submission.submission_id, submission.problem_id);
    }
    return CompilationResult::successful_compilation(path_to_executable, submission.submission_id, submission.problem_id);
}
