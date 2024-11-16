#include "accessData.h"
#include "sqlite3.h"

char currentUser[25] = "";

void DataAccess::addAccount(const string& username, const string& password) const {
    sqlite3* db;
    sqlite3_open("../data/accounts.db", &db);

    string sql = "INSERT INTO accounts (username, password) VALUES (?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void DataAccess::addGrade(const string& subject, const string& grade) const {
    sqlite3* db;
    sqlite3_open("../data/gradeBook.db", &db);

    string sql = "INSERT INTO grades (username, subject, grade) VALUES (?, ?, ?)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, currentUser, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, subject.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, grade.c_str(), -1, SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
