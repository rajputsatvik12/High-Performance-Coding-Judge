cmake_minimum_required(VERSION 3.15)
project(judge_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =========================
# Dependencies
# =========================
find_package(Threads REQUIRED)
find_package(libpqxx CONFIG REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)

# =========================
# Include directories
# =========================
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    /home/satvik_rajput/vcpkg/installed/x64-linux/include
)

# =========================
# CORE LIB
# =========================
add_library(core_lib
    src/core/judge.cpp
    src/core/submission_manager.cpp
    src/core/validator.cpp
)

target_link_libraries(core_lib
    Threads::Threads
)

# =========================
# DB LIB
# =========================
add_library(db_lib
    src/db/database.cpp
    src/db/problem_repo.cpp
    src/db/result_repo.cpp
    src/db/submission_repo.cpp
    src/db/testcase_repo.cpp
    src/db/user_repo.cpp
)

target_link_libraries(db_lib
    libpqxx::pqxx
)

# =========================
# EXECUTOR LIB
# =========================
add_library(executor_lib
    src/executor/c_executor.cpp
    src/executor/cpp_executor.cpp
    src/executor/python_executor.cpp
    src/executor/execution_result.cpp
)

target_link_libraries(executor_lib
    Threads::Threads
)

# =========================
# COMPILER LIB
# =========================
add_library(compiler_lib
    src/compiler/c_compiler.cpp
    src/compiler/cpp_compiler.cpp
    src/compiler/python_compiler.cpp
    src/compiler/compilation_result.cpp
)

# =========================
# SYSTEM LIB
# =========================
add_library(system_lib
    src/system/cgroup_manager.cpp
    src/system/rlimit_manager.cpp
    src/system/sandbox.cpp
)

# =========================
# API LIB
# =========================
add_library(api_lib
    src/api/server.cpp
)

target_link_libraries(api_lib
    Threads::Threads
)

# =========================
# MAIN EXECUTABLE
# =========================
add_executable(judge_engine
    src/main.cpp
)

target_link_libraries(judge_engine
    api_lib
    core_lib
    db_lib
    executor_lib
    compiler_lib
    system_lib
    libpqxx::pqxx
    Threads::Threads
)