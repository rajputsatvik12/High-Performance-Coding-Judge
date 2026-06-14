#include "db/user_repo.hpp"

UserRepo::UserRepo(Database& db) : db(db){}

int UserRepo::create(std::string& username, std::string& password_hash){
    
    if(exists(username)) return -1;

    pqxx::work txn(db.get_connection());
    pqxx::result result = txn.exec_params(R"(
        INSERT INTO users 
        (username, password_hash)
        VALUES
        ($1, $2)
        RETURNING user_id;
        )", username, password_hash);

    txn.commit();

    return result[0]["user_id"].as<int>();
}

bool UserRepo::exists(std::string& username){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT user_id FROM users
        WHERE username = $1;
        )", username);

    return !result.empty();
}

UserRecord UserRepo::get_by_id(int user_id){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT * FROM users 
        WHERE user_id = $1;
        )", user_id);

    UserRecord user_record;

    if(result.empty()) return user_record;
    
    user_record.user_id = result[0]["user_id"].as<int>();
    user_record.username = result[0]["username"].as<std::string>();
    user_record.password_hash = result[0]["password_hash"].as<std::string>();
    user_record.created_at = result[0]["created_at"].as<std::string>();
    return user_record;
}

UserRecord UserRepo::get_by_username(std::string& username){
    pqxx::work txn(db.get_connection());

    pqxx::result result = txn.exec_params(R"(
        SELECT * FROM users 
        WHERE username = $1;
        )", username);

    UserRecord user_record;

    if(result.empty()) return user_record;
    
    user_record.user_id = result[0]["user_id"].as<int>();
    user_record.username = result[0]["username"].as<std::string>();
    user_record.password_hash = result[0]["password_hash"].as<std::string>();
    user_record.created_at = result[0]["created_at"].as<std::string>();
    return user_record;
}