#include "core/judge.hpp"
#include "jsoncpp/json/json.h"
#include <fstream>
#include <iostream>
#include "system/sandbox.hpp"

Judge::Judge(
    std::unique_ptr<Compiler> compiler,
    std::unique_ptr<Executor> executor,
    std::unique_ptr<Validator> validator
)
    :compiler(std::move(compiler)),
    executor(std::move(executor)),
    validator(std::move(validator))
{}

Verdict Judge::classify_signal(terminationSignal termination_signal, size_t peak_memory, size_t memory_limit){
    if(termination_signal == terminationSignal::SIGXCPU_) return Verdict::TimeLimitExceeded;

    if(termination_signal == terminationSignal::SIGKILL_){
        if(peak_memory > memory_limit) return Verdict::MemoryLimitExceeded;
        return Verdict::TimeLimitExceeded;
    }

    return Verdict::RuntimeError;

}

JudgeResult Judge::process_submission(const Submission& submission){
    
    CompilationResult compilation_result = compiler -> compile(submission);
    
    if(!compilation_result.is_successful()){
        /*Compilation Error*/
        return JudgeResult(
            Verdict::CompilationError,
            0,
            0,
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }

    ExecutionResult execution_result = executor -> execute(compilation_result);

    if(!execution_result.get_internal_error().empty()){
        /*Internal error*/
        std::cout << execution_result.get_internal_error() << '\n';
        return JudgeResult(
            Verdict::InternalError,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }

    if(execution_result.get_execution_time_ms() > time_limit){
        return JudgeResult(
            Verdict::TimeLimitExceeded,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }

    if(execution_result.get_termination_signal() != terminationSignal::NONE){
        /*Classify signal*/
        Verdict verdict = classify_signal(execution_result.get_termination_signal(), execution_result.get_peak_memory(), memory_limit);

        return JudgeResult(
            verdict,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            execution_result.get_submission_id(),
            execution_result.get_problem_id()
        );
    }

    if(execution_result.get_exit_code() != 0){
        /*Runtime error*/
        return JudgeResult(
            Verdict::RuntimeError,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }
    
    std::filesystem::path user_output = execution_result.get_stdout_path();
    std::filesystem::path expected_output = "problems/" + execution_result.get_problem_id() + "/tests/output1.txt";
    ValidatorResult validation_result = validator -> validate(user_output, expected_output);
    if(!validation_result.get_error_message().empty()){
        /*Internal Error*/
        return JudgeResult(
            Verdict::InternalError,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }
    
    if(validation_result.get_match()){
        /*Accepted*/
        return JudgeResult(
            Verdict::Accepted,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
    }

    /*Wrong Answer*/
    return JudgeResult(
            Verdict::WrongAnswer,
            execution_result.get_execution_time_ms(),
            execution_result.get_peak_memory(),
            compilation_result.get_submission_id(),
            compilation_result.get_problem_id()
        );
}