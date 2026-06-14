#include "core/submission_manager.hpp"
#include "jsoncpp/json/json.h"
#include <fstream>
#include <filesystem>

SubmissionManager::SubmissionManager(SubmissionRepo& submission_repo, ProblemRepo& problem_repo, ResultRepo& result_repo, TestcaseRepo& testcase_repo) :
    submission_repo(submission_repo),
    problem_repo(problem_repo),
    result_repo(result_repo),
    testcase_repo(testcase_repo){}

std::string to_string(Verdict verdict){
    const std::map<Verdict, std::string> mpp = {
        {Verdict::Accepted, "Accepted"},
        {Verdict::CompilationError, "Compilation Error"},
        {Verdict::InternalError, "Internal Error"},
        {Verdict::MemoryLimitExceeded, "Memory Limit Exceeded"},
        {Verdict::RuntimeError, "Runtime Error"},
        {Verdict::TimeLimitExceeded, "Time Limit Exceeded"},
        {Verdict::WrongAnswer, "Wrong Answer"}
    }; 

    return mpp.at(verdict);
}

// Json::Value jsonify(const JudgeResult& result){
//     Json::Value result_json;
//     result_json["problem_id"] = result.problem_id;
//     result_json["execution_time"] = result.execution_time_ms;
//     result_json["memory"] = result.peak_memory;
//     result_json["verdict"] = to_string(result.verdict);

//     return result_json;
// }

uint64_t SubmissionManager::accept_submission(const std::string& source_code, int user_id, const int problem_id, Language language){
    
    // uint64_t submission_id = next_submission_id.fetch_add(1);
    // if(!std::filesystem::create_directory(path_to_dir)){
    //     /*Internal error*/
    //     return UINT64_MAX;
    // }

    // try{
    //     std::filesystem::rename(source_file, path_to_source);
    // }
    // catch(const std::filesystem::filesystem_error& e){
    //     /*Internal error*/
    //     return UINT64_MAX;
    // }

    // Submission submission{submission_id, path_to_source, problem_id, language};

    // {
    //     std::lock_guard<std::mutex> lock(queue_mutex);
    //     submission_queue.push(submission);
    // }

    // return submission_id;

    uint64_t submission_id = submission_repo.create(problem_id, user_id, language, source_code);

    std::filesystem::path path_to_dir = "submissions/" + std::to_string(submission_id);
    std::filesystem::create_directories(path_to_dir);
    std::filesystem::path path_to_source = path_to_dir / "source.cpp";

    std::ofstream out(path_to_source);
    out << source_code;

    return submission_id;
}

void SubmissionManager::process_submissions(Judge& judge){
    
    SubmissionRecord next_pending = submission_repo.get_next_pending();
    if(next_pending.submission_id == 0) return;

    ProblemRecord problem = problem_repo.get_by_id(next_pending.problem_id);
    std::vector<TestcaseRecord> testcases = testcase_repo.get_by_problem(next_pending.problem_id);

    std::vector<JudgeResult> results = judge.process_submission(next_pending, problem, testcases);


    for(const auto& result : results){
        int result_id = result_repo.create(result.submission_id, result.testcase_id, result.verdict, result.execution_time_ms, result.peak_memory);
    }

    submission_repo.update_status(next_pending.submission_id, "Finished");

}

ProblemRecord SubmissionManager::get_problem(int problem_id){
    return problem_repo.get_by_id(problem_id);
}

SubmissionRecord SubmissionManager::get_submission(uint64_t submission_id){
    return submission_repo.get_by_id(submission_id);
}

std::vector<ResultRecord> SubmissionManager::get_results(uint64_t submission_id){
    return result_repo.get_by_submission(submission_id);
}

std::vector<ProblemRecord> SubmissionManager::get_problems(){
    return problem_repo.get_all();
}