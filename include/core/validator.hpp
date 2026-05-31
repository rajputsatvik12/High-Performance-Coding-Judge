#ifndef VALIDATOR_H
#define VALIDATOR_H
#include <filesystem>

class ValidatorResult{
    
    private:
        bool match;
        std::string error_message;

        ValidatorResult(bool match,
            std::string error_message
        ):
        match(match),
        error_message(error_message){};

    public:
        static ValidatorResult successful_validation(bool match);
        static ValidatorResult internal_error(std::string error_message);

        std::string get_error_message();
        bool get_match();

};

class Validator{
    public:
        ValidatorResult validate(std::filesystem::path user_output, std::filesystem::path expected_output);
};

#endif