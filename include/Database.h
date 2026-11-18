#pragma once

#include "Models.h"
#include <sqlite_orm/sqlite_orm.h>

inline auto createStorage() {
    using namespace sqlite_orm;

    return make_storage(
        "print_queue.db",

        make_table("Users",
            make_column("id", &User::id,
                primary_key().autoincrement()),
            make_column("name", &User::name),
            make_column("email", &User::email),
            make_column("department", &User::department)
        ),

        make_table("PrintDocuments",
            make_column("id", &PrintDocument::id,
                primary_key().autoincrement()),
            make_column("user_id", &PrintDocument::userId),
            make_column("file_name", &PrintDocument::fileName),
            make_column("page_count", &PrintDocument::pageCount),
            make_column("status", &PrintDocument::status),
            make_column("priority", &PrintDocument::priority),
            foreign_key(&PrintDocument::userId)
                .references(&User::id)
        )
    );
}

using Storage = decltype(createStorage());

Storage& getStorage();
