#include "core/submission_manager.hpp"
#include "compiler/cpp_compiler.hpp"
#include "executor/cpp_executor.hpp"
#include "core/judge.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if(argc != 3){
        std::cout << "Usage: ./judge_cli <source_file> <problem_id>\n";
        return 1;
    }

    std::string source_file = argv[1];
    std::string problem_id = argv[2];

    SubmissionManager manager;
    auto compiler = std::make_unique<CppCompiler>();
    auto executor = std::make_unique<CppExecutor>();
    auto validator = std::make_unique<Validator>();

    Judge judge(
        std::move(compiler),
        std::move(executor),
        std::move(validator)
    );

    uint64_t submission_id = manager.accept_submission(source_file, problem_id, Language::Cpp);
    
    if(submission_id == UINT64_MAX){
        std::cout << "Internal Error" << '\n';
    }
    else std::cout << "Submission accepted with ID: " << submission_id << "\n";

    manager.process_submissions(std::move(compiler), std::move(executor), std::move(validator), judge);

    return 0;
}
