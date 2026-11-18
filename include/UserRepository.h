#pragma once

#include "Models.h"
#include <optional>
#include <vector>

class IUserRepository {
public:
    virtual ~IUserRepository() = default;

    virtual int add(const User& user) = 0;
    virtual std::optional<User> findById(int id) = 0;
    virtual std::vector<User> findAll() = 0;
    virtual void update(const User& user) = 0;
    virtual void remove(int id) = 0;
};

class UserRepository : public IUserRepository {
public:
    int add(const User& user) override;
    std::optional<User> findById(int id) override;
    std::vector<User> findAll() override;
    void update(const User& user) override;
    void remove(int id) override;
};
