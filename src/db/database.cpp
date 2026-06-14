#include "db/database.hpp"
#include <iostream>

void Database::create_tables(){

    std::cout << "Creating tables" << '\n';
    pqxx::work txn(*conn);

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS users(
        user_id SERIAL PRIMARY KEY,
        username VARCHAR(25) UNIQUE NOT NULL,
        email VARCHAR(40) UNIQUE NOT NULL,
        password_hash VARCHAR(50) NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )");

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS problems(
        problem_id SERIAL PRIMARY KEY,
        statement TEXT NOT NULL,
        title VARCHAR(50) NOT NULL,
        time_limit INTEGER NOT NULL,
        memory_limit_kb INTEGER NOT NULL,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )");

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS testcases(
        testcase_id SERIAL PRIMARY KEY,
        problem_id INTEGER NOT NULL REFERENCES problems(problem_id),
        input_path TEXT NOT NULL,
        expected_output_path TEXT NOT NULL,
        is_hidden BOOLEAN DEFAULT FALSE,
        created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )");

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS submissions(
        submission_id SERIAL PRIMARY KEY,
        problem_id INTEGER NOT NULL REFERENCES problems(problem_id),
        user_id INTEGER NOT NULL REFERENCES users(user_id),
        language VARCHAR(10) NOT NULL,
        source_code TEXT NOT NULL UNIQUE,
        status VARCHAR (10) DEFAULT 'Pending',
        submitted_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
    )");

    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS results(
        result_id SERIAL PRIMARY KEY,
        submission_id INTEGER NOT NULL REFERENCES submissions(submission_id),
        testcase_id INTEGER NULL REFERENCES testcases(testcase_id),
        verdict VARCHAR(30) NOT NULL,
        execution_time_ms INTEGER NOT NULL,
        peak_memory_bytes INTEGER NOT NULL,
        judged_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        );
        )");

    txn.commit();
    std::cout << "Created tables" << '\n';
}

Database::Database(const std::string& conn_str)
    : conn(std::make_unique<pqxx::connection>(conn_str)){
        create_tables();
}

pqxx::connection& Database::get_connection(){
    return *conn;
}
