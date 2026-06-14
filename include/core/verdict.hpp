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

inline std::string verdict_to_string(Verdict verdict){
    if(verdict == Verdict::Accepted) return "Accepted";
    if(verdict == Verdict::WrongAnswer) return "WrongAnswer";
    if(verdict == Verdict::TimeLimitExceeded) return "TimeLimitExceeded";
    if(verdict == Verdict::CompilationError) return "CompilationError";
    if(verdict == Verdict::RuntimeError) return "RuntimeError";
    if(verdict == Verdict::MemoryLimitExceeded) return "MemoryLimitExceeded";
    return "InternalError";
}

inline Verdict to_verdict(std::string verdict){
    if(verdict == "Accepted") return Verdict::Accepted;
    if(verdict == "WrongAnswer") return Verdict::WrongAnswer;
    if(verdict == "TimeLimitExceeded") return Verdict::TimeLimitExceeded;
    if(verdict == "CompilationError") return Verdict::CompilationError;
    if(verdict == "RuntimeError") return Verdict::RuntimeError;
    if(verdict == "MemoryLimitExceeded") return Verdict::MemoryLimitExceeded;
    return Verdict::InternalError;
}

#endif