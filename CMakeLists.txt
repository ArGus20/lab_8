cmake_minimum_required(VERSION 3.20)

project(PrintQueue)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(PrintQueue
    src/main.cpp
    src/Database.cpp
    src/UserRepository.cpp
    src/DocumentRepository.cpp
    src/SQLiteComparison.cpp
)

target_include_directories(PrintQueue PRIVATE include)

target_link_libraries(PrintQueue PRIVATE
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)
