#pragma once
#include "pch.h"

using json = nlohmann::json;
extern json users;
void initUsers();

bool userLogin(const json& users);
int userRegister(json& users);
json readUsersFromJson(const string& filename);



