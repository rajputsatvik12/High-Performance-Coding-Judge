#include "core/submission_manager.hpp"
#include "compiler/cpp_compiler.hpp"
#include "executor/cpp_executor.hpp"
#include "core/judge.hpp"
#include "api/server.hpp"
#include <iostream>

void menu(){
    std::cout << "----- Main Menu -----" << '\n';

    std::cout << "1. Submit" << '\n';
    std::cout << "2. Quit" << '\n';
}

int main(int argc, char* argv[]) {
    
    std::cout << "----- Judge Running -----" << '\n';

    std::string conn = "host=localhost "
    "port=5432 "
    "dbname=judge_db "
    "user=judge_user "
    "password=judge123 ";

    try {
        Database db(conn);
        std::cout << "Database created" << '\n';
        SubmissionRepo submission_repo(db);
        ProblemRepo problem_repo(db);
        ResultRepo result_repo(db);
        TestcaseRepo testcase_repo(db);
        SubmissionManager manager(submission_repo, problem_repo, result_repo, testcase_repo);

        ApiServer server(manager);
        server.run(18080);

    // while(true){
    //     menu();

    //     int cmd;
    //     std::cin >> cmd;

    //     if(cmd == 2) break;

    //     std::cout << "Paste source code. Type END on a new line when finished.\n";

    //     std::string source_code;
    //     std::string line;

    //     // std::getline(std::cin >> std::ws, line); 
        
    //     while(std::getline(std::cin, line)){
    //         if(line == "END") break;
    //         source_code += line + '\n';
    //     }

    //     std::cout << "Enter Problem ID: " << '\n';
    //     int problem_id;
    //     std::cin >> problem_id;

    //     std::cout << "Source Code:" << '\n';
    //     std::cout << source_code << '\n';

    //     auto compiler = std::make_unique<CppCompiler>();
    //     auto executor = std::make_unique<CppExecutor>();
    //     auto validator = std::make_unique<Validator>();

    //     Judge judge(
    //         std::move(compiler),
    //         std::move(executor),
    //         std::move(validator)
    //     );

    //     uint64_t submission_id = manager.accept_submission(source_code, 1, problem_id, Language::Cpp);
        
    //     if(submission_id == UINT64_MAX){
    //         std::cout << "Internal Error" << '\n';
    //     }
    //     else std::cout << "Submission accepted with ID: " << submission_id << "\n";

    //     manager.process_submissions(judge);
    // }

    // std::cout << "----- Judge Stopped -----" << '\n';
    return 0;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}