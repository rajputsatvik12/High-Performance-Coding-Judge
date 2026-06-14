#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "compiler/compilation_result.hpp"
#include "executor/execution_result.hpp"
#include "db/testcase_repo.hpp"
#include "db/problem_repo.hpp"

class Executor{
    public:
        virtual ~Executor() = default;
        virtual ExecutionResult execute(const CompilationResult& compilation_result, const TestcaseRecord& testcase, int test_number, const ProblemRecord& problem) = 0;

};

#endif