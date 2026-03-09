# Judge Engine

A **sandboxed coding judge written in C++** that securely compiles and executes user submissions using Linux kernel resource control mechanisms. The system is designed to evaluate competitive programming solutions by compiling submitted code, running it inside an isolated environment, and generating verdicts based on runtime behavior and output.

The project focuses on **systems-level execution control**, using process isolation and kernel resource limits to safely execute untrusted programs.

## Features

### Secure Execution

User programs are executed inside an isolated environment using:

- **Linux cgroups** to enforce memory, CPU, and process limits
- **rlimits** to restrict runtime resource usage
- **fork/exec** process isolation
- controlled **stdin/stdout/stderr redirection**


### Compilation Pipeline

The current system supports:

C++ submissions

Each submission is compiled into an executable that is stored in the submission directory and later executed by the sandbox.

### Multi-Test Evaluation

Submissions are evaluated against multiple test cases.

For each test case:

1. The test input is provided to the user program.
2. The program is executed inside the sandbox.
3. Output and errors are captured.
4. Runtime statistics are recorded.

Execution stops early if a failure verdict is detected.

### Runtime Monitoring

During execution, the judge records:

- execution time
- peak memory usage
- process exit status
- termination signals

This information is used to determine the final verdict.

### Verdict Detection

The judge currently detects the following outcomes:

AC – Accepted  
WA – Wrong Answer  
TLE – Time Limit Exceeded  
MLE – Memory Limit Exceeded  
RE – Runtime Error  

Verdicts are determined using execution results, termination signals, and resource usage.

---

## Execution Pipeline

The evaluation pipeline follows this structure:
```
Submission
↓
Compile source code
↓
For each test case
↓
Create sandbox environment
↓
Execute program
↓
Capture output and resource usage
↓
Generate verdict
```

## Sandbox Design

Execution isolation is handled through a sandbox layer that abstracts system resource controls.

The sandbox is responsible for:

- creating and managing **cgroups**
- applying **resource limits**
- attaching execution processes to the sandbox
- collecting **memory usage statistics**
- cleaning up the environment after execution

Two major components power the sandbox:

**CGroupManager**
- creates and manages cgroups
- sets memory, CPU, and process limits
- tracks runtime memory usage

**rlimits**
- enforce CPU time limits
- restrict memory space
- restrict output file size

## Project Structure

```
judge-engine
 ┣ .vscode
 ┃ ┗ c_cpp_properties.json
 ┣ include
 ┃ ┣ compiler
 ┃ ┃ ┣ c_compiler.hpp
 ┃ ┃ ┣ compilation_result.hpp
 ┃ ┃ ┣ compiler.hpp
 ┃ ┃ ┣ cpp_compiler.hpp
 ┃ ┃ ┗ python_compiler.hpp
 ┃ ┣ core
 ┃ ┃ ┣ config.hpp
 ┃ ┃ ┣ judge.hpp
 ┃ ┃ ┣ language.hpp
 ┃ ┃ ┣ submission.hpp
 ┃ ┃ ┣ submission_manager.hpp
 ┃ ┃ ┣ validator.hpp
 ┃ ┃ ┗ verdict.hpp
 ┃ ┣ executor
 ┃ ┃ ┣ c_executor.hpp
 ┃ ┃ ┣ cpp_executor.hpp
 ┃ ┃ ┣ execution_result.hpp
 ┃ ┃ ┣ executor.hpp
 ┃ ┃ ┣ python_executor.hpp
 ┃ ┃ ┗ termination_signal.hpp
 ┃ ┗ system
 ┃ ┃ ┣ cgroup_manager.hpp
 ┃ ┃ ┣ rlimit_manager.hpp
 ┃ ┃ ┗ sandbox.hpp
 ┣ problems
 ┃ ┣ A
 ┃ ┃ ┣ tests
 ┃ ┃ ┃ ┣ input1.txt
 ┃ ┃ ┃ ┗ output1.txt
 ┃ ┃ ┗ config.json
 ┃ ┗ B
 ┃ ┃ ┣ tests
 ┃ ┃ ┃ ┣ input1.txt
 ┃ ┃ ┃ ┗ output1.txt
 ┃ ┃ ┗ config.json
 ┣ src
 ┃ ┣ compiler
 ┃ ┃ ┣ c_compiler.cpp
 ┃ ┃ ┣ compilation_result.cpp
 ┃ ┃ ┣ cpp_compiler.cpp
 ┃ ┃ ┗ python_compiler.cpp
 ┃ ┣ core
 ┃ ┃ ┣ judge.cpp
 ┃ ┃ ┣ submission_manager.cpp
 ┃ ┃ ┗ validator.cpp
 ┃ ┣ executor
 ┃ ┃ ┣ c_executor.cpp
 ┃ ┃ ┣ cpp_executor.cpp
 ┃ ┃ ┣ execution_result.cpp
 ┃ ┃ ┗ python_executor.cpp
 ┃ ┣ system
 ┃ ┃ ┣ cgroup_manager.cpp
 ┃ ┃ ┣ rlimit_manager.cpp
 ┃ ┃ ┗ sandbox.cpp
 ┃ ┗ main.cpp
 ┣ submissions
 ┣ .gitignore
 ┣ CMakeLists.txt
 ┗ README.md
 ```

## Resource Isolation

Each execution is performed inside a dedicated sandbox with limits applied through cgroups:

- memory.max
- cpu.max
- pids.max

This ensures user programs cannot consume excessive system resources or affect the host system.

## Requirements

The project requires a Linux environment with cgroups v2 enabled.

Dependencies include:

- Linux
- g++
- cmake
- jsoncpp

The system relies on Linux system calls such as:

- fork
- exec
- waitpid
- setrlimit

## Current Limitations

The current version supports:

- C++ submissions only
- local execution on a single machine
- sequential submission evaluation

---

## Planned Improvements

Future improvements include:

- multi-language support
- concurrent submission processing
- syscall filtering using seccomp
- filesystem isolation using namespaces
- distributed worker architecture

## Architecture Diagram
```mermaid
graph TB
%% === STYLES ===
classDef core fill:#1E90FF,stroke:#000,color:#000,stroke-width:2px,rx:10px,ry:10px;
classDef resource fill:#FFD700,stroke:#000,color:#000,stroke-width:2px,rx:10px,ry:10px;
classDef module fill:#9ACD32,stroke:#000,color:#000,stroke-width:2px,rx:10px,ry:10px;
classDef external fill:#FF69B4,stroke:#000,color:#000,stroke-width:2px,rx:10px,ry:10px;

%% === USERS ===
User(("User<br/>Submits Code"))

%% === MAIN APPLICATION ===
MainApp["Main Application<br/>src/main.cpp"]:::core

%% === SUBMISSION MANAGEMENT ===
SubmissionManager["Submission Manager"]:::core

%% === JUDGING CORE ===
Judge["Judge"]:::core

%% === COMPILER ===
Compiler["Compiler"]:::core
CppCompiler["CppCompiler<br/>g++ Implementation"]:::module

%% === EXECUTOR ===
Executor["Executor"]:::core
CppExecutor["CppExecutor<br/>Runs Binaries"]:::module

%% === VALIDATOR ===
Validator["Validator"]:::core

%% === RESOURCE MANAGEMENT ===
ResourceManagement["Resource Management"]:::core
CgroupManager["cgroup_manager<br/>Linux Cgroups"]:::resource
RlimitManager["rlimit_manager<br/>Resource Limits"]:::resource
Sandbox["sandbox<br/>Sandbox Environment"]:::resource

%% === SUPPORTING MODULES ===
CoreModule["core<br/>Data Structures"]:::module
UtilsModule["utils<br/>Configuration"]:::module
ProblemsModule["problems<br/>Test Data"]:::module

%% === EXTERNAL DATA ===
ProblemConfig["Problem Configurations<br/>JSON Files"]:::external
Submissions["Submissions<br/>Source Code Files"]:::external

%% === DATA FLOW ===
User -->|"submits code"| SubmissionManager
SubmissionManager -->|"creates directory and enqueues"| Judge
Judge -->|"calls compile"| Compiler
Compiler -->|"returns compilation result"| Judge
Judge -->|"calls execute"| Executor
Executor -->|"runs executable in sandbox"| Judge
Judge -->|"validates output"| Validator
Validator -->|"returns validation result"| Judge
Judge -->|"stores result in JSON"| Submissions

%% === RESOURCE MANAGEMENT FLOW ===
Executor -->|"enforces limits"| ResourceManagement
ResourceManagement --> CgroupManager
ResourceManagement --> RlimitManager
ResourceManagement --> Sandbox

%% === PROBLEM CONFIGURATION FLOW ===
Judge -->|"reads constraints"| ProblemConfig
SubmissionManager -->|"moves source code"| Submissions

%% === SUPPORTING MODULES FLOW ===
Judge --> CoreModule
Judge --> UtilsModule
Judge --> ProblemsModule

%% === COMPONENT CONNECTIONS ===
MainApp --> SubmissionManager
MainApp --> Judge
MainApp --> Compiler
MainApp --> Executor
MainApp --> Validator
MainApp --> ResourceManagement

%% === EXTERNAL DEPENDENCIES ===
ProblemConfig -->|"specifies constraints"| Judge
Submissions -->|"contains source code"| SubmissionManager
```