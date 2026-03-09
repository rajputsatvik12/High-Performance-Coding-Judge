#ifndef TERMINATION_SIGNAL_H
#define TERMINATION_SIGNAL_H

enum class terminationSignal{
    SIGSEGV_,
    SIGXCPU_,
    SIGKILL_,
    SIGABRT_,
    SIGFPE_,
    SIGOTHER_,
    NONE
};

#endif