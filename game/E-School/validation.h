#pragma once

#include "pch.h"

class Validate {
public:
    const bool doesAccountExist(const std::string& targetUsername);
    const bool isPasswordCorrect(const std::string& targetUsername, const std::string& targetPassword);
    bool containsSpecial(const std::string& password);
    bool containsDigit(const std::string& password);
    bool containsLowercase(const std::string& password);
    bool containsUppercase(const std::string& password);
};
