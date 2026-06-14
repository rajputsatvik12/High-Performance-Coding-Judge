#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <pqxx/pqxx>
#include <string>
#include <memory>

class Database{

    private:
        std::unique_ptr<pqxx::connection> conn;
        void create_tables();

    public:
        Database(const std::string& conn_str);
        pqxx::connection& get_connection();
};

#endif