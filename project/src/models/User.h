#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string username;
    std::string password;
    std::string role;

public:
    User(std::string u, std::string p, std::string r);
    virtual ~User() = default;

    std::string getUsername() const;
    std::string getPassword() const;
    std::string getRole() const;

    void setPassword(std::string newPassword);
};

#endif
