#ifndef USER_H
#define USER_H

#include "json.hpp"
using json = nlohmann::json;

bool userLogin(const json& users);
int userRegister(json& users);


#endif // !USER_H


