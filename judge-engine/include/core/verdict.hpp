#ifndef VERDICT_H
#define VERDICT_H

enum class Verdict{
    Accepted,
    WrongAnswer,
    TimeLimitExceeded,
    CompilationError,
    RuntimeError,
    MemoryLimitExceeded,
    InternalError
};

#endif