#ifndef COMPILER_H
#define COMPILER_H

#include "compiler/compilation_result.hpp"
#include "db/submission_repo.hpp"

class Compiler{
    public:
        virtual ~Compiler() = default;
        virtual CompilationResult compile(const SubmissionRecord&) = 0;
        
};


#endif