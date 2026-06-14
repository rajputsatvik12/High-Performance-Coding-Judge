#include "db/submission_repo.hpp"

SubmissionRepo::SubmissionRepo(Database& db) : db(db){}

uint64_t SubmissionRepo::create(int problem_id, int user_id, Language language, const std::string& source_code){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        INSERT INTO submissions 
        (problem_id, user_id, language, source_code)
        VALUES
        ($1, $2, $3, $4)
        RETURNING submission_id;
        )", problem_id, user_id, language_to_string(language), source_code);

    txn.commit();

    return result[0]["submission_id"].as<uint64_t>();
}

void SubmissionRepo::update_status(uint64_t submission_id, std::string status){
    pqxx::work txn(db.get_connection());

    txn.exec_params(R"(
        UPDATE submissions SET
        status = $1
        WHERE submission_id = $2;
        )", status, submission_id);

    txn.commit();
}

SubmissionRecord SubmissionRepo::get_by_id(uint64_t submission_id){
    pqxx::work txn(db.get_connection());

    SubmissionRecord submission_record;

    pqxx::result result = txn.exec_params(R"(
        SELECT * FROM submissions
        WHERE submission_id = $1;
        )", submission_id);

    submission_record.submission_id = result[0]["submission_id"].as<uint64_t>();
    submission_record.problem_id = result[0]["problem_id"].as<int>();
    submission_record.user_id = result[0]["user_id"].as<int>();
    submission_record.language = to_language(result[0]["language"].as<std::string>());
    submission_record.source_code = result[0]["source_code"].as<std::string>();
    submission_record.status = result[0]["status"].as<std::string>();
    submission_record.submitted_at = result[0]["submitted_at"].as<std::string>();

    return submission_record;
}

SubmissionRecord SubmissionRepo::get_next_pending(){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec(R"(
        SELECT * FROM submissions
        WHERE status = 'Pending'
        ORDER BY submitted_at
        FOR UPDATE SKIP LOCKED
        LIMIT 1;
        )");

    SubmissionRecord submission_record;

    if(result.empty()) return submission_record;

    submission_record.submission_id = result[0]["submission_id"].as<uint64_t>();
    submission_record.problem_id = result[0]["problem_id"].as<int>();
    submission_record.user_id = result[0]["user_id"].as<int>();
    submission_record.language = to_language(result[0]["language"].as<std::string>());
    submission_record.source_code = result[0]["source_code"].as<std::string>();
    submission_record.status = result[0]["status"].as<std::string>();
    submission_record.submitted_at = result[0]["submitted_at"].as<std::string>();

    txn.exec_params(R"(
        UPDATE submissions SET 
        status = 'Running'
        WHERE submission_id = $1;
        )", submission_record.submission_id);

    txn.commit();

    return submission_record;

}

std::vector<SubmissionRecord> SubmissionRepo::get_by_user(int user_id){
    pqxx::work txn(db.get_connection());

    std::vector<SubmissionRecord> user_submissions;

    pqxx::result results = txn.exec_params(R"(
        SELECT * FROM submissions
        WHERE user_id = $1
        ORDER BY submitted_at DESC;
        )", user_id);

    for(const auto& result : results){
        SubmissionRecord submission_record;

        submission_record.submission_id = result["submission_id"].as<uint64_t>();
        submission_record.problem_id = result["problem_id"].as<int>();
        submission_record.user_id = result["user_id"].as<int>();
        submission_record.language = to_language(result["language"].as<std::string>());
        submission_record.source_code = result["source_code"].as<std::string>();
        submission_record.status = result["status"].as<std::string>();
        submission_record.submitted_at = result["submitted_at"].as<std::string>();

        user_submissions.push_back(submission_record);
    }

    return user_submissions;
}