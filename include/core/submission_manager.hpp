#ifndef SUBMISSION_MANAGER_H
#define SUBMISSION_MANAGER_H

#include "core/judge.hpp"
#include "db/database.hpp"
#include "db/submission_repo.hpp"
#include "db/problem_repo.hpp"
#include "db/result_repo.hpp"
#include "db/testcase_repo.hpp"
#include "db/user_repo.hpp"
#include <queue>
#include <mutex>
#include <atomic>
#include <iostream>

class SubmissionManager{
    private:
        SubmissionRepo& submission_repo;
        ProblemRepo& problem_repo;
        ResultRepo& result_repo;
        TestcaseRepo& testcase_repo;
        UserRepo& user_repo;
        Judge& judge;

    public:
        SubmissionManager(SubmissionRepo& submission_repo, ProblemRepo& problem_repo, ResultRepo& result_repo, TestcaseRepo& testcase_repo, UserRepo& user_repo, Judge& judge);

        uint64_t accept_submission(const std::string& source_file, int user_id, const int problem_id, Language language);

        void process_submissions();

        SubmissionRecord get_submission(uint64_t submission_id);

        std::vector<ResultRecord> get_results(uint64_t submission_id);

        ProblemRecord get_problem(int problem_id);

        std::vector<ProblemRecord> get_problems();

        UserRecord get_user(std::string& username);

        int create_user(std::string& username, std::string& password_hash, std::string& email);
};

#endif