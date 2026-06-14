#include "executor/cpp_executor.hpp"
#include <filesystem>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <string>
#include <cstring>
#include <fstream>
#include <jsoncpp/json/json.h>
#include <fcntl.h> 
#include <unistd.h>
#include <chrono>
#include "system/cgroup_manager.hpp"
#include <iostream>
#include "system/sandbox.hpp"

terminationSignal map_signal(int signal){
    if(signal == SIGABRT) return terminationSignal::SIGABRT_;
    if(signal == SIGFPE) return terminationSignal::SIGFPE_;
    if(signal == SIGKILL) return terminationSignal::SIGKILL_;
    if(signal == SIGSEGV) return terminationSignal::SIGSEGV_;
    if(signal == SIGXCPU) return terminationSignal::SIGXCPU_;
    return terminationSignal::SIGOTHER_;
}

ExecutionResult CppExecutor::execute(const CompilationResult& compilation_result, const TestcaseRecord& testcase, int test_number, const ProblemRecord& problem){
    
    SandBox sandbox(compilation_result);

    time_limit = problem.time_limit;
    memory_limit = problem.memory_limit_kb * 1024;
    // if(!sandbox.get_limits()){
    //     return ExecutionResult::internal_error("Cant Read constraints", compilation_result.get_submission_id(), compilation_result.get_problem_id());
    // }
    
    if(!sandbox.set_cgroup_limits()){
        sandbox.destroy_group();
        return ExecutionResult::internal_error("CGroup failed", compilation_result.get_submission_id(), compilation_result.get_problem_id());
    }

    std::filesystem::path path_ = "submissions/" + (std::to_string(compilation_result.get_submission_id()));
    std::filesystem::path input_filepath = testcase.input_path;
    std::filesystem::path error_filepath = "user_error" + std::to_string(test_number) + ".txt";
    std::filesystem::path output_filepath = "output" + std::to_string(test_number) + ".txt";
    int input_fd = open((input_filepath).c_str(), O_RDONLY);
    int output_fd = open((path_/output_filepath).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int error_fd = open((path_/error_filepath).c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if(input_fd < 0 || output_fd < 0 || error_fd < 0){
        /*Error*/
        sandbox.destroy_group();
        if(input_fd >= 0) close(input_fd);
        if(output_fd >= 0) close(output_fd);
        if(error_fd >= 0) close(error_fd);
        return ExecutionResult::internal_error("Invalid File Desciptors",  compilation_result.get_submission_id(), compilation_result.get_problem_id());
    }

    std::chrono::steady_clock::time_point start_time;
    pid_t pid = fork();
    if(pid < 0){
        /*Error*/
        sandbox.destroy_group();
        close(input_fd);
        close(output_fd);
        close(error_fd);
        return ExecutionResult::internal_error("Fork failed in Executor", compilation_result.get_submission_id(), compilation_result.get_problem_id());
    }

    if(pid > 0){

        sandbox.attach(pid);
        start_time = std::chrono::steady_clock::now();
        
        close(input_fd);
        close(output_fd);
        close(error_fd);

        
    }
    else{
        if(!sandbox.set_rlimits()){
            perror("setrlimit failed");
            _exit(1);
        }

        dup2(input_fd, STDIN_FILENO);
        dup2(output_fd, STDOUT_FILENO);
        dup2(error_fd, STDERR_FILENO);

        close(input_fd);
        close(output_fd);
        close(error_fd);

        char* args[] = {
            const_cast<char*>(compilation_result.get_executable_path().c_str()),
            NULL
        };

        execvp(const_cast<char*>(compilation_result.get_executable_path().c_str()), args);

        perror("exec failed");
        _exit(1);
    }
    

    
    int status = 0;
    bool killed_for_timeout = false;

    while(true){
        pid_t res = waitpid(pid, &status, WNOHANG);

        if(res == -1){
            sandbox.destroy_group();
            return ExecutionResult::internal_error("Waitpid error in executor", compilation_result.get_submission_id(), compilation_result.get_problem_id());
        }

        if(res > 0) break;

        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();

        uint64_t elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

        if(elapsed_time > time_limit){
            kill(pid, SIGKILL);
            killed_for_timeout = true;
            waitpid(pid, &status, 0);
            break;
        }

        usleep(1000);

    }

    std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();

    uint64_t submission_id = compilation_result.get_submission_id();
    int problem_id = compilation_result.get_problem_id();
    
    std::filesystem::path path_to_stdout = path_/output_filepath;
    std::filesystem::path path_to_stderr = path_/error_filepath;
    uint64_t execution_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    size_t peak_memory = sandbox.get_memory_peak();
    sandbox.destroy_group();
    if(WIFEXITED(status)){
        int exit_code = WEXITSTATUS(status);
        if(exit_code == 0) 
            return ExecutionResult::successful_execution(
                path_to_stdout,
                path_to_stderr,
                execution_time_ms,
                peak_memory,
                submission_id,
                problem_id
                );

        return ExecutionResult::failed_execution(
            exit_code,
            terminationSignal::NONE,
            path_to_stdout,
            path_to_stderr,
            execution_time_ms,
            peak_memory,
            submission_id,
            problem_id
        );

    }
    else if(WIFSIGNALED(status)){
        int signal = WTERMSIG(status);
        terminationSignal termination_signal = map_signal(signal);
        if(killed_for_timeout) termination_signal = terminationSignal::SIGXCPU_;
        return ExecutionResult::failed_execution(
            -1,
            termination_signal,
            path_to_stdout,
            path_to_stderr,
            execution_time_ms,
            peak_memory,
            submission_id,
            problem_id
        );
    }

    return ExecutionResult::internal_error("Unknown exit in Executor", compilation_result.get_submission_id(), compilation_result.get_problem_id());

}
