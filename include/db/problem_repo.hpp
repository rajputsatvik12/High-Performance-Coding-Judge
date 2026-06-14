#ifndef PROBLEM_REPO_HPP
#define PROBLEM_REPO_HPP

#include "database.hpp"

struct ProblemRecord{
    int problem_id = 0;

    std::string title;
    std::string statement;

    int time_limit;
    int memory_limit_kb;

    std::string created_at;
}; 

class ProblemRepo{

    private:
        Database& db;

    public:
        ProblemRepo(Database& db);
        ProblemRecord get_by_id(int problem_id);
        std::vector<ProblemRecord> get_all();
        bool exists(int problem_id);
        int create(std::string& title, std::string& statement, int time_limit, int memory_limit_kb);
};

#endif