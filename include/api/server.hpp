#ifndef SERVER_HPP
#define SERVER_HPP

#include <crow.h>
#include "core/submission_manager.hpp"

class ApiServer{
    private:
        SubmissionManager& manager;

    public:
        ApiServer(SubmissionManager& manager);

        void run(int port);
};

#endif