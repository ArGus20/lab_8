#include "Database.h"
#include <sqlite3.h>

Storage& getStorage() {
    static Storage storage = [] {
        auto db = createStorage();

        db.on_open = [](sqlite3* connection) {
            sqlite3_exec(
                connection,
                "PRAGMA foreign_keys = ON;",
                nullptr,
                nullptr,
                nullptr
            );
        };

        return db;
    }();

    return storage;
}