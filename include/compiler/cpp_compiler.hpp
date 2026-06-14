#ifndef CPP_COMPILER_H
#define CPP_COMPILER_H

#include "compiler/compiler.hpp"

class CppCompiler : public Compiler{
    public:
        CompilationResult compile(const SubmissionRecord& submission) override;
};

#endif