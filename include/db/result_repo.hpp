#ifndef RESULT_REPO_HPP
#define RESULT_REPO_HPP

#include "database.hpp"
#include "core/verdict.hpp"

struct ResultRecord{
    uint64_t result_id = 0;
    uint64_t submission_id;
    std::optional<int> testcase_id;

    Verdict verdict;

    int execution_time_ms;
    int peak_memory_bytes;

    std::string judged_at;
};

class ResultRepo{

    private:
        Database& db;

    public:
        ResultRepo(Database& db);
        uint64_t create(uint64_t submission_id, std::optional<int> testcase_id, Verdict verdict, int execution_time_ms, int peak_memory_bytes);
        std::vector<ResultRecord> get_by_submission(uint64_t submission_id);
};

#endif