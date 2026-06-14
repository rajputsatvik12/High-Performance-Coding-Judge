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

std::vector<JudgeResult> Judge::process_submission(const SubmissionRecord& submission, const ProblemRecord& problem, const std::vector<TestcaseRecord>& testcases){
    
    CompilationResult compilation_result = compiler -> compile(submission);
    std::vector<JudgeResult> results;

    if(!compilation_result.is_successful()){
        /*Compilation Error*/
        results.push_back(JudgeResult(
            Verdict::CompilationError,
            0,
            0,
            compilation_result.get_submission_id(), 
            compilation_result.get_problem_id(),
            std::nullopt
        ));
        return results;
    }
    
    int cnt = 0;
    uint64_t execution_time_ms = 0;
    size_t peak_memory = 0;
    for(const auto& testcase : testcases){

        ExecutionResult execution_result = executor -> execute(compilation_result, testcase, ++cnt, problem);

        if(!execution_result.get_internal_error().empty()){
            /*Internal error*/
            std::cout << execution_result.get_internal_error() << '\n';
            results.push_back(JudgeResult(
                Verdict::InternalError,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }

        if(execution_result.get_execution_time_ms() > time_limit){
            results.push_back(JudgeResult(
                Verdict::TimeLimitExceeded,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }

        if(execution_result.get_termination_signal() != terminationSignal::NONE){
            /*Classify signal*/
            Verdict verdict = classify_signal(execution_result.get_termination_signal(), execution_result.get_peak_memory(), memory_limit);

            results.push_back(JudgeResult(
                verdict,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                execution_result.get_submission_id(),
                execution_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }

        if(execution_result.get_exit_code() != 0){
            /*Runtime error*/
            results.push_back(JudgeResult(
                Verdict::RuntimeError,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }
        
        std::filesystem::path user_output = execution_result.get_stdout_path();
        std::filesystem::path expected_output = testcase.expected_output_path;
        ValidatorResult validation_result = validator -> validate(user_output, expected_output);

        if(!validation_result.get_error_message().empty()){
            /*Internal Error*/
            results.push_back(JudgeResult(
                Verdict::InternalError,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }
        
        if(!validation_result.get_match()){
            /*Wrong Answer*/
            results.push_back(JudgeResult(
                Verdict::WrongAnswer,
                execution_result.get_execution_time_ms(),
                execution_result.get_peak_memory(),
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
            return results;
        }
        execution_time_ms = std::max(execution_time_ms, execution_result.get_execution_time_ms());
        peak_memory = std::max(peak_memory, execution_result.get_peak_memory());

        /*Accepted*/
        results.push_back(JudgeResult(
                Verdict::Accepted,
                execution_time_ms,
                peak_memory,
                compilation_result.get_submission_id(),
                compilation_result.get_problem_id(),
                testcase.testcase_id
            ));
        }

    return results;
}