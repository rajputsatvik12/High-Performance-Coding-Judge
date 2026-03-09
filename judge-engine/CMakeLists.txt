cmake_minimum_required(VERSION 3.10)
project(judge-engine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================
# Include Directories
# ============================

include_directories(include)

# ============================
# Source Files
# ============================

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# ============================
# Executable
# ============================

add_executable(judge_engine ${SOURCES})

# ============================
# Libraries
# ============================

find_package(Threads REQUIRED)

# jsoncpp (system installed)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

target_include_directories(judge_engine PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(judge_engine PRIVATE ${JSONCPP_LIBRARIES} Threads::Threads)