#ifndef JUDGE_H
#define JUDGE_H

#include "core/submission.hpp"
#include "compiler/compiler.hpp"
#include "executor/executor.hpp"
#include "core/validator.hpp"
#include "core/verdict.hpp"
#include <memory>

struct JudgeResult{
    Verdict verdict;
    uint64_t execution_time_ms;
    size_t peak_memory;
    uint64_t submission_id;
    std::string problem_id;

    JudgeResult(
        Verdict verdict,
        uint64_t execution_time_ms,
        size_t peak_memory,
        uint64_t submission_id,
        std::string problem_id
    )
    :verdict(verdict),
    execution_time_ms(execution_time_ms),
    peak_memory(peak_memory),
    submission_id(submission_id),
    problem_id(problem_id){}
};

class Judge{
    public:
        Judge(
            std::unique_ptr<Compiler> compiler,
            std::unique_ptr<Executor> executor,
            std::unique_ptr<Validator> validator
        );

        JudgeResult process_submission(const Submission& submission);

    private:
        std::unique_ptr<Compiler> compiler;
        std::unique_ptr<Executor> executor;
        std::unique_ptr<Validator> validator;

        Verdict classify_signal(terminationSignal termination_signal, size_t peak_memory, size_t memory_limit);
};

#endif