#ifndef JWT_MANAGER_HPP
#define JWT_MANAGER_HPP

#include <jwt-cpp/jwt.h>
#include <chrono>

class JWTManager{
    public:
        std::string generate_token(const int user_id);
        bool verify_token(const std::string& token);
};

#endif