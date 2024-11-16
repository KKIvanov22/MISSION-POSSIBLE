#include "validation.h"
#include "sqlite3.h"
#include <algorithm>
#include <cctype>

// Check if an account with the given username exists in the accounts database.
const bool Validate::doesAccountExist(const std::string& targetUsername) {
    sqlite3* db;
    sqlite3_open("../data/accounts.db", &db);

    std::string sql = "SELECT username FROM accounts WHERE username = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, targetUsername.c_str(), -1, SQLITE_STATIC);

    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return exists;
}

// Check if the given password matches the password associated with the given username.
const bool Validate::isPasswordCorrect(const std::string& targetUsername, const std::string& targetPassword) {
    sqlite3* db;
    sqlite3_open("../data/accounts.db", &db);

    std::string sql = "SELECT password FROM accounts WHERE username = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, targetUsername.c_str(), -1, SQLITE_STATIC);

    bool correct = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string storedPassword = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (storedPassword == targetPassword) {
            correct = true;
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return correct;
}

// Check if the string contains any special characters.
bool Validate::containsSpecial(const std::string& str) {
    return std::any_of(str.begin(), str.end(), [](unsigned char c) {
        return !std::isalnum(c);
        });
}

// Check if the string contains any digits.
bool Validate::containsDigit(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isdigit);
}

// Check if the string contains any lowercase letters.
bool Validate::containsLowercase(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::islower);
}

// Check if the string contains any uppercase letters.
bool Validate::containsUppercase(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isupper);
}
