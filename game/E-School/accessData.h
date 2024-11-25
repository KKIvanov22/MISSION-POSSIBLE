#pragma once

#include "pch.h"

extern char currentUser[25];

// This class contains the methods which are used for accessing the database.
class DataAccess {
public:
    // Adds an account to the database "accounts".
    void addAccount(const string& username, const string& password) const;
    void addGrade(const string& subject, const string& grade)const;
};