#ifndef USER_REPO_HPP
#define USER_REPO_HPP

#include "database.hpp"

struct UserRecord{
    int user_id = 0;
    std::string username;
    std::string password_hash;
    std::string created_at;
};

class UserRepo{

    private:
        Database& db;

    public:
        UserRepo(Database& db);
        int create(std::string& username, std::string& password_hash);
        bool exists(std::string& username);
        UserRecord get_by_id(int user_id);
        UserRecord get_by_username(std::string& username);
};

#endif