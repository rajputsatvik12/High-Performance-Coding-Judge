#ifndef SUBMISSION_H
#define SUBMISSION_H

#include "core/language.hpp"
#include <filesystem>
#include <string> 

struct Submission{
    uint64_t submission_id;
    std::filesystem::path source_file;
    std::string problem_id;
    Language language;
};

#endif