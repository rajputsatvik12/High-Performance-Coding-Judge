#include "core/submission_manager.hpp"
#include "jsoncpp/json/json.h"
#include <fstream>
#include <filesystem>

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

Json::Value jsonify(const JudgeResult& result){
    Json::Value result_json;
    result_json["problem_id"] = result.problem_id;
    result_json["execution_time"] = result.execution_time_ms;
    result_json["memory"] = result.peak_memory;
    result_json["verdict"] = to_string(result.verdict);

    return result_json;
}

uint64_t SubmissionManager::accept_submission(const std::filesystem::path& source_file, const std::string& problem_id, Language language){
    
    uint64_t submission_id = next_submission_id.fetch_add(2);
    std::filesystem::path path_to_dir = "submissions/" + std::to_string(submission_id);
    std::filesystem::path path_to_source = path_to_dir / "source.cpp";
    if(!std::filesystem::create_directory(path_to_dir)){
        /*Internal error*/
        return UINT64_MAX;
    }

    try{
        std::filesystem::rename(source_file, path_to_source);
    }
    catch(const std::filesystem::filesystem_error& e){
        /*Internal error*/
        return UINT64_MAX;
    }

    Submission submission{submission_id, path_to_source, problem_id, language};

    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        submission_queue.push(submission);
    }

    return submission_id;
}

void SubmissionManager::process_submissions(std::unique_ptr<Compiler> compiler,
            std::unique_ptr<Executor> executor,
            std::unique_ptr<Validator> validator, Judge& judge){
        while(!submission_queue.empty()){
            Submission submission;

            std::lock_guard<std::mutex> lock(queue_mutex);
            submission = submission_queue.front();
            submission_queue.pop();

            JudgeResult result = judge.process_submission(submission);
            Json::Value result_json = jsonify(result);

            std::filesystem::path path_to_result = "submissions/" + std::to_string(result.submission_id) + "/result.json";
            std::ofstream result_file(path_to_result);
            result_file << result_json;
            result_file.close();

            std::cout << "Submission ID: " << result.submission_id << '\n';
            std::cout << "Problem ID: " << result.problem_id << '\n';
            std::cout << "Verdict: " << to_string(result.verdict) << '\n';
            std::cout << "Execution Time: " << result.execution_time_ms << '\n';
            std::cout << "Memory Used: " << result.peak_memory << '\n';

        }
    }