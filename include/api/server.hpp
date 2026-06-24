#ifndef SERVER_HPP
#define SERVER_HPP

#include <crow.h>
#include "core/submission_manager.hpp"
#include "jwt_manager.hpp"

class ApiServer{
    private:
        SubmissionManager& manager;
        JWTManager& jwt_manager;

    public:
        ApiServer(SubmissionManager& manager, JWTManager& jwt_manager);

        void run(int port);
};

#endif