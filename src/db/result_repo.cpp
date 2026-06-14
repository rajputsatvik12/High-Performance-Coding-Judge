#include "db/result_repo.hpp"
#include <optional>

ResultRepo::ResultRepo(Database& db) : db(db){}

uint64_t ResultRepo::create(uint64_t submission_id, std::optional<int> testcase_id, Verdict verdict, int execution_time_ms, int peak_memory_bytes){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        INSERT INTO results
        (submission_id, testcase_id, verdict, execution_time_ms, peak_memory_bytes)
        VALUES
        ($1, $2, $3, $4, $5)
        RETURNING result_id;
        )", submission_id, testcase_id, verdict_to_string(verdict), execution_time_ms, peak_memory_bytes);

    txn.commit();

    return result[0]["result_id"].as<uint64_t>();
}

std::vector<ResultRecord> ResultRepo::get_by_submission(uint64_t submission_id){
    pqxx::work txn(db.get_connection());

    std::vector<ResultRecord> results_by_submission;

    pqxx::result results = txn.exec_params(R"(
        SELECT * FROM results
        WHERE submission_id = $1;
        )", submission_id);

    for(const auto& result : results){
        ResultRecord result_record;

        result_record.result_id = result["result_id"].as<uint64_t>();
        result_record.submission_id = result["submission_id"].as<uint64_t>();
        if(!result["testcase_id"].is_null()) result_record.testcase_id = result["testcase_id"].as<int>();
        else result_record.testcase_id = std::nullopt;
        result_record.verdict = to_verdict(result["verdict"].as<std::string>());
        result_record.execution_time_ms = result["execution_time_ms"].as<int>();
        result_record.peak_memory_bytes = result["peak_memory_bytes"].as<int>();
        result_record.judged_at = result["judged_at"].as<std::string>();

        results_by_submission.push_back(result_record);
    }

    return results_by_submission;
}