#ifndef CPP_EXECUTOR_H
#define CPP_EXECUTOR_H

#include "executor/executor.hpp"

class CppExecutor : public Executor{
    public:
        ExecutionResult execute(const CompilationResult& compilation_result) override;
};

#endif