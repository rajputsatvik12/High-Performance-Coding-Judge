#ifndef MIDDLEWARE_HPP
#define MIDDLEWARE_HPP

#include <crow.h>
#include <string>
#include "jwt_manager.hpp"

class AuthMiddleware{
    private:
        JWTManager jwt_manager;
    public:
    struct context{
        int user_id;
        bool authenticated = false;
    };

        void before_handle(crow::request& req, crow::response& res, context& ctx);
        void after_handle(crow::request& req, crow::response& res, context& ctx);
};

#endif