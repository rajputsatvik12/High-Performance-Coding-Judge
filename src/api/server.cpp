#include "api/server.hpp"
#include "hash.cpp"
#include "db/user_repo.hpp"
#include "api/middleware.hpp"
#include <optional>

ApiServer::ApiServer(SubmissionManager& manager, JWTManager& jwt_manager) : manager(manager), jwt_manager(jwt_manager){}

void ApiServer::run(int port){

    crow::App<AuthMiddleware> app;

    CROW_ROUTE(app, "/ping")([](){
        return "Judge Alive";
    });

    CROW_ROUTE(app, "/submit").methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
        auto body = crow::json::load(req.body);

        if(!body){
            return crow::response(400, "Invalid JSON");
        }

        auto& auth_ctx = app.get_context<AuthMiddleware>(req);
        int user_id = auth_ctx.user_id;
        int problem_id = body["problem_id"].i();
        std::string source_code = body["source_code"].s();
        Language language = to_language(body["language"].s());

        uint64_t submission_id = manager.accept_submission(source_code, user_id, problem_id, language);
        manager.process_submissions();
        manager.process_submissions();

        crow::json::wvalue response;
        response["submission_id"] = submission_id;

        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/submissions/<uint>").methods(crow::HTTPMethod::GET)([this](uint submission_id){


        crow::json::wvalue response;
        SubmissionRecord record = this -> manager.get_submission(submission_id);
        response["submission_id"] = submission_id;
        response["problem_id"] = record.problem_id;
        response["language"] = language_to_string(record.language);
        response["status"] = record.status;
        response["submitted_at"] = record.submitted_at;
        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/results/<uint>").methods(crow::HTTPMethod::GET)([this](uint submission_id){

        crow::json::wvalue response;
        std::vector<ResultRecord> records = this -> manager.get_results(submission_id);

        response["submission_id"] = records[0].submission_id;
        response["verdict"] = verdict_to_string(records.back().verdict);
        int execution_time_ms = 0, peak_memory = 0;
        crow::json::wvalue::list results;
        for(const ResultRecord& record : records){
            crow::json::wvalue rec;
            if(record.testcase_id.has_value()) rec["testcase_id"] = record.testcase_id.value();
            else rec["testcase_id"] = nullptr;
            rec["verdict"] = verdict_to_string(record.verdict);
            rec["execution_time_ms"] = record.execution_time_ms;
            rec["peak_memory"] = record.peak_memory_bytes;
            execution_time_ms = std::max(execution_time_ms, record.execution_time_ms);
            peak_memory = std::max(peak_memory, record.peak_memory_bytes);

            results.push_back(rec);
        }

        response["execution_time_ms"] = execution_time_ms;
        response["peak_memory"] = peak_memory;

        response["results"] = std::move(results);

        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/problems").methods(crow::HTTPMethod::GET)([this](){

        crow::json::wvalue::list response;
        crow::json::wvalue respo;
        std::vector<ProblemRecord> records = this -> manager.get_problems();
        for(const ProblemRecord& record : records){
            crow::json::wvalue rec;
            rec["problem_id"] = record.problem_id;
            rec["title"] = record.title;

            response.push_back(rec);
        }
        respo["problems"] = std::move(response);
        return crow::response(200, respo);
    });

    CROW_ROUTE(app, "/problems/<int>").methods(crow::HTTPMethod::GET)([this](int problem_id){
        
        crow::json::wvalue response;
        ProblemRecord record = this -> manager.get_problem(problem_id);

        response["problem_id"] = record.problem_id;
        response["title"] = record.title;
        response["time_limit"] = record.time_limit;
        response["memory_limit"] = record.memory_limit_kb;
        response["statement"] = record.statement;

        return crow::response(200, response);
    });

    CROW_ROUTE(app, "/login").methods(crow::HTTPMethod::POST)([this](const crow::request& req){
        auto body = crow::json::load(req.body);
        std::string username = body["username"].s();
        std::string password = body["password"].s();

        std::string hashed_password = hash_password(password);

        UserRecord user_record = manager.get_user(username);

        if(user_record.user_id != 0 && user_record.password_hash == hashed_password){
            std::string token = jwt_manager.generate_token(user_record.user_id);

            crow::json::wvalue response;

            response["token"] = token;
            response["user"]["user_id"] = user_record.user_id;
            response["user"]["username"] = user_record.username;

            return crow::response(200, response);
        }

        crow::json::wvalue error;
        error["error"] = "Invalid credentials";
        return crow::response(400, error);
    });

    CROW_ROUTE(app, "/signup").methods(crow::HTTPMethod::POST)([this](const crow::request& req){
        
        auto body = crow::json::load(req.body);
        std::string username = body["username"].s();
        std::string password = body["password"].s();
        std::string email = body["email"].s();

        std::string hashed_password = hash_password(password);

        uint64_t user_id = manager.create_user(username, hashed_password, email);

        std::string token = jwt_manager.generate_token(user_id);

        crow::json::wvalue response;
        response["token"] = token;
        response["user"]["username"] = username;
        return crow::response(200, response);
    });

    // CROW_ROUTE(app, "/me").methods(crow::HTTPMethod::GET)([this](const crow::request& req){
        
    //     auto body = crow::json::load(req.body);
    // });

    app.port(port).multithreaded().run();

}
