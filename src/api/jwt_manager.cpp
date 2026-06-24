#include "api/jwt_manager.hpp"
// #include <dotenv.h>

std::string JWTManager::generate_token(const int user_id){
    auto now = std::chrono::system_clock::now();
    auto expires = now + std::chrono::hours(1);

    return jwt::create()
    .set_subject(std::to_string(user_id))
    .set_issued_at(now)
    .set_expires_at(expires)
    .sign(jwt::algorithm::hs256{std::getenv("JWT_MANAGER_KEY")});
}

bool JWTManager::verify_token(const std::string& token){
    try {
        auto decoded = jwt::decode(token);
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{std::getenv("JWT_MANAGER_KEY")});
        verifier.verify(decoded);
        return true;
    } catch(...) {
        return false;
    }
}
