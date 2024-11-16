#include "gradeBook.h"
#include "map.h"
#include "menu.h"
#include "sqlite3.h"

// Function to draw subject and corresponding grade on the screen
void drawSubjectAndGrade(const string& subject, const string& grade, int posY) {
    DrawText(subject.c_str(), 850, posY, 20, BLACK);
    DrawText(grade.c_str(), 1000, posY, 20, BLACK);
}

// Function to display the grade book
void gradeBook() {
    Texture2D background = LoadTexture("../images/gradeBookBg.png");
    SetTargetFPS(60);

    sqlite3* db;
    sqlite3_open("../data/gradeBook.db", &db);

    // Main loop to display grade book
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(WHITE);
        DrawTexture(background, 0, 0, WHITE);

        string sql = "SELECT subject, grade FROM grades WHERE username = ?";
        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        sqlite3_bind_text(stmt, 1, currentUser, -1, SQLITE_STATIC);

        int posY = 225;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            string subject = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            string grade = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            drawSubjectAndGrade(subject, grade, posY);
            posY += 30;
        }
        sqlite3_finalize(stmt);

        EndDrawing();
    }

    sqlite3_close(db);
}
