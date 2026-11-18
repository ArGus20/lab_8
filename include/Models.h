#pragma once

#include <string>

struct User {
    int id = 0;
    std::string name;
    std::string email;
    std::string department;
};

struct PrintDocument {
    int id = 0;
    int userId = 0;
    std::string fileName;
    int pageCount = 0;
    std::string status;
    int priority = 1;
};
