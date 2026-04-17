#include "User.h"

User::User(std::string u, std::string p, std::string r) : username(u), password(p), role(r) {}

std::string User::getUsername() const { return username; }
std::string User::getPassword() const { return password; }
std::string User::getRole() const { return role; }

void User::setPassword(std::string newPassword) { password = newPassword; }
