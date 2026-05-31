#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "compiler/compilation_result.hpp"
#include "executor/execution_result.hpp"

class Executor{
    public:
        virtual ~Executor() = default;
        virtual ExecutionResult execute(const CompilationResult& compilation_result, int test_number) = 0;

};

#endif