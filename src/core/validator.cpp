#include "core/validator.hpp"
#include <fstream>
#include <iterator>

ValidatorResult ValidatorResult::internal_error(std::string error_message){
    return ValidatorResult(false, error_message);
}

ValidatorResult ValidatorResult::successful_validation(bool match){
    return ValidatorResult(match, "");
}

std::string ValidatorResult::get_error_message(){
    return this -> error_message;
}

bool ValidatorResult::get_match(){
    return this -> match;
}

ValidatorResult Validator::validate(std::filesystem::path user_output, std::filesystem::path expected_output){
    
    std::ifstream user(user_output), expected(expected_output);

    if(!user || !expected) return ValidatorResult::internal_error("Could not open files");

    bool match = std::equal(std::istreambuf_iterator<char>(user), std::istreambuf_iterator<char>(),
                        std::istreambuf_iterator<char>(expected), std::istreambuf_iterator<char>());

    user.close();
    expected.close();

    return ValidatorResult::successful_validation(match);
}