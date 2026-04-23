#ifndef STAFF_H
#define STAFF_H

#include "User.h"

class Staff : public User {
public:
    Staff(std::string u, std::string p);
};

#endif
