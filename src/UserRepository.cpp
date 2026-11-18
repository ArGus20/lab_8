#include "UserRepository.h"
#include "Database.h"

int UserRepository::add(const User& user) {
    return getStorage().insert(user);
}

std::optional<User> UserRepository::findById(int id) {
    return getStorage().get_optional<User>(id);
}

std::vector<User> UserRepository::findAll() {
    return getStorage().get_all<User>();
}

void UserRepository::update(const User& user) {
    getStorage().update(user);
}

void UserRepository::remove(int id) {
    getStorage().remove<User>(id);
}
