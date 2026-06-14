#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <string>

enum class Language{
    C,
    Cpp,
    Python
};

inline std::string language_to_string(Language language){
    if(language == Language::C) return "C";
    if(language == Language::Cpp) return "Cpp";
    return "Python";
}

inline Language to_language(std::string language){
    if(language == "C") return Language::C;
    if(language == "Cpp") return Language::Cpp;
    return Language::Python;
}

#endif