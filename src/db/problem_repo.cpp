#include "db/problem_repo.hpp"

ProblemRepo::ProblemRepo(Database& db) : db(db){};

ProblemRecord ProblemRepo::get_by_id(int problem_id){

    ProblemRecord problem_record;

    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT * FROM problems 
        WHERE problem_id = $1;
        )", problem_id);

    if(result.empty()) return problem_record;

    problem_record.problem_id = result[0]["problem_id"].as<int>();
    problem_record.title = result[0]["title"].as<std::string>();
    problem_record.statement = result[0]["statement"].as<std::string>();
    problem_record.time_limit = result[0]["time_limit"].as<int>();
    problem_record.memory_limit_kb = result[0]["memory_limit_kb"].as<int>();
    problem_record.created_at = result[0]["created_at"].as<std::string>();

    return problem_record;
}

std::vector<ProblemRecord> ProblemRepo::get_all(){

    pqxx::work txn(db.get_connection());

    pqxx::result results = txn.exec(R"(
        SELECT * FROM problems;
        )");

    std::vector<ProblemRecord> problems;

    for(const auto& result : results){
        ProblemRecord problem_record;

        problem_record.problem_id = result["problem_id"].as<int>();
        problem_record.title = result["title"].as<std::string>();
        problem_record.statement = result["statement"].as<std::string>();
        problem_record.time_limit = result["time_limit"].as<int>();
        problem_record.memory_limit_kb = result["memory_limit_kb"].as<int>();
        problem_record.created_at = result["created_at"].as<std::string>();

        problems.push_back(problem_record);
    }

    return problems;
}

bool ProblemRepo::exists(int problem_id){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT problem_id FROM problems 
        WHERE problem_id = $1;
        )", problem_id);

    return !result.empty();
}

int ProblemRepo::create(std::string& title, std::string& statement, int time_limit, int memory_limit_kb){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        INSERT INTO problems
        (title, statement, time_limit, memory_limit_kb)
        VALUES
        ($1, $2, $3, $4)
        RETURNING problem_id;
        )", title, statement, time_limit, memory_limit_kb);

    txn.commit();

    return result[0]["problem_id"].as<int>();
}