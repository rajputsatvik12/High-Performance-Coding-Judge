#ifndef COMPILER_H
#define COMPILER_H

#include "core/submission.hpp"
#include "compiler/compilation_result.hpp"

class Compiler{
    public:
        virtual ~Compiler() = default;
        virtual CompilationResult compile(const Submission&) = 0;
        
};


#endif