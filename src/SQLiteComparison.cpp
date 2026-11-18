#include "SQLiteComparison.h"
#include <sqlite3.h>
#include <iostream>

void demonstrateSQLiteCAPI() {
    sqlite3* db = nullptr;

    if (sqlite3_open("print_queue.db", &db) != SQLITE_OK) {
        std::cout << "Database error\n";
        if (db) sqlite3_close(db);
        return;
    }

    sqlite3_stmt* stmt = nullptr;

    const char* sql =
        "INSERT INTO Users(name, email, department) "
        "VALUES(?, ?, ?)";

    sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, "C API User", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, "capi@gmail.com", -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, "Testing", -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        int id = static_cast<int>(sqlite3_last_insert_rowid(db));

        std::cout << "C API: user added, ID = " << id << '\n';

        sqlite3_finalize(stmt);
        stmt = nullptr;

        sql = "SELECT id, name, email, department FROM Users WHERE id = ?";

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            std::cout << "Found: "
                      << sqlite3_column_int(stmt, 0) << " | "
                      << sqlite3_column_text(stmt, 1) << " | "
                      << sqlite3_column_text(stmt, 2) << " | "
                      << sqlite3_column_text(stmt, 3) << '\n';
        }

        sqlite3_finalize(stmt);
        stmt = nullptr;

        sql = "DELETE FROM Users WHERE id = ?";

        sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_step(stmt);
    } else {
        std::cout << "Insert error: " << sqlite3_errmsg(db) << '\n';
    }

    if (stmt) sqlite3_finalize(stmt);

    sqlite3_close(db);
}
