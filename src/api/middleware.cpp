#include "api/middleware.hpp"

void AuthMiddleware::before_handle(crow::request& req, crow::response& res, context& ctx) {
        if (req.method == crow::HTTPMethod::OPTIONS){
            res.add_header(
                "Access-Control-Allow-Origin",
                "http://localhost:5173");

            res.add_header(
                "Access-Control-Allow-Headers",
                "Content-Type, Authorization");

            res.add_header(
                "Access-Control-Allow-Methods",
                "GET, POST, PUT, DELETE, OPTIONS");

            res.code = 200;
            res.end();
            return;
        }

        if (req.url == "/login" || req.url == "/signup") {
            return;
        }

        std::string auth_header = req.get_header_value("Authorization");
        if (auth_header.empty() || auth_header.find("Bearer ") != 0) {
            res.code = 401;
            res.body = "{\"error\":\"Missing or invalid token\"}";
            res.end();
            return;
        }

        std::string token = auth_header.substr(7);
        
        if (!jwt_manager.verify_token(token)) {
            res.code = 401;
            res.body = "{\"error\":\"Invalid or expired token\"}";
            res.end();
            return;
        }
        
        auto decoded = jwt::decode(token);
        ctx.user_id = std::stoi(decoded.get_subject());
        ctx.authenticated = true;
    }

void AuthMiddleware::after_handle(crow::request& req, crow::response& res, context& ctx){
    res.add_header(
        "Access-Control-Allow-Origin",
        "http://localhost:5173");

    res.add_header(
        "Access-Control-Allow-Headers",
        "Content-Type, Authorization");

    res.add_header(
        "Access-Control-Allow-Methods",
        "GET, POST, PUT, DELETE, OPTIONS");
}