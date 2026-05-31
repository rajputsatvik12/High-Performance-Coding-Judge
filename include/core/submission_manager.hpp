#ifndef SUBMISSION_MANAGER_H
#define SUBMISSION_MANAGER_H

#include "core/judge.hpp"
#include <queue>
#include <mutex>
#include <atomic>
#include <iostream>

class SubmissionManager{
    private:
        std::queue<Submission> submission_queue;
        std::atomic<uint64_t> next_submission_id{1};
        std::mutex queue_mutex;

    public:
        SubmissionManager() = default;

        uint64_t accept_submission(const std::filesystem::path& source_file, const std::string& problem_id, Language language);

        void process_submissions(std::unique_ptr<Compiler> compiler,
            std::unique_ptr<Executor> executor,
            std::unique_ptr<Validator> validator, Judge& judge);
};

#endif