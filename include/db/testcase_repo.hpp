#ifndef TESTCASE_REPO_HPP
#define TESTCASE_REPO_HPP

#include <filesystem>
#include <optional>
#include "database.hpp"

struct TestcaseRecord{
    std::optional<int> testcase_id;
    int problem_id;

    std::filesystem::path input_path;
    std::filesystem::path expected_output_path;

    bool is_hidden;
};

class TestcaseRepo{

    private:
        Database& db;

    public:
        TestcaseRepo(Database& db);
        int create(int problem_id, std::filesystem::path& input_path, std::filesystem::path& expected_output_path, bool is_hidden = false);
        TestcaseRecord get_by_id(std::optional<int> testcase_id);
        std::vector<TestcaseRecord> get_by_problem(int problem_id);
        std::pair<int, int> count_by_problem(int problem_id);
        bool exists(std::optional<int> testcase_id);
};

#endif