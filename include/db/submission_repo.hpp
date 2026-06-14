#ifndef SUBMISSION_REPO
#define SUBMISSION_REPO

#include <cstdint>
#include <string>
#include <vector>
#include "core/language.hpp"
#include "database.hpp"

struct SubmissionRecord{
    uint64_t submission_id = 0;
    int problem_id;
    int user_id;
    Language language;
    std::string source_code;
    std::string status;
    std::string submitted_at;
};

class SubmissionRepo{
    
    private:
        Database& db;

    public:
        SubmissionRepo(Database& db);
        uint64_t create(int problem_id, int user_id, Language language, const std::string& source_code);
        void update_status(uint64_t submission_id, std::string status);
        SubmissionRecord get_by_id(uint64_t submission_id);
        SubmissionRecord get_next_pending();
        std::vector<SubmissionRecord> get_by_user(int user_id);
};

#endif