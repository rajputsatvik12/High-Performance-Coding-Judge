#include "db/testcase_repo.hpp"

TestcaseRepo::TestcaseRepo(Database& db) : db(db){}

int TestcaseRepo::create(int problem_id, std::filesystem::path& input_path, std::filesystem::path& expected_output_path, bool is_hidden){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        INSERT INTO testcases 
        (problem_id, input_path, expected_output_path, is_hidden)
        VALUES
        ($1, $2, $3, $4)
        RETURNING testcase_id;
        )", problem_id, input_path, expected_output_path, is_hidden);

    txn.commit();

    return result[0]["testcase_id"].as<int>();
}

TestcaseRecord TestcaseRepo::get_by_id(std::optional<int> testcase_id){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT * FROM testcases
        WHERE testcase_id = $1;
        )", testcase_id);

    TestcaseRecord testcase_record;

    if(result.empty()) return testcase_record;

    testcase_record.testcase_id = result[0]["testcase_id"].as<int>();
    testcase_record.problem_id = result[0]["problem_id"].as<int>();
    testcase_record.input_path = result[0]["input_path"].as<std::string>();
    testcase_record.expected_output_path = result[0]["expected_output_path"].as<std::string>();
    testcase_record.is_hidden = result[0]["is_hidden"].as<bool>();

    return testcase_record;
}

std::vector<TestcaseRecord> TestcaseRepo::get_by_problem(int problem_id){
    pqxx::work txn(db.get_connection());

    pqxx::result results = txn.exec_params(R"(
        SELECT * FROM testcases
        WHERE problem_id = $1
        ORDER BY testcase_id;
        )", problem_id);
    
    std::vector<TestcaseRecord> testcases;

    for(const auto& result : results){
        TestcaseRecord testcase_record;

        testcase_record.testcase_id = result["testcase_id"].as<int>();
        testcase_record.problem_id = result["problem_id"].as<int>();
        testcase_record.input_path = result["input_path"].as<std::string>();
        testcase_record.expected_output_path = result["expected_output_path"].as<std::string>();
        testcase_record.is_hidden = result["is_hidden"].as<bool>();

        testcases.push_back(testcase_record);
    }

    return testcases;
}

std::pair<int, int> TestcaseRepo::count_by_problem(int problem_id){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT COUNT(*) AS num_hidden_tests FROM testcases
        WHERE problem_id = $1
        AND is_hidden = true;
        )", problem_id);

    std::pair<int,int> num_testcases;

    num_testcases.first = result[0]["num_hidden_tests"].as<int>();

    result = txn.exec_params(R"(
        SELECT COUNT(*) AS num_visible_tests FROM testcases
        WHERE problem_id = $1
        AND is_hidden = false;
        )", problem_id);

    num_testcases.second = result[0]["num_visible_tests"].as<int>();
    
    return num_testcases;
}

bool TestcaseRepo::exists(std::optional<int> testcase_id){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT testcase_id FROM testcases
        WHERE testcase_id = $1;
        )", testcase_id);

    return !result.empty();
}