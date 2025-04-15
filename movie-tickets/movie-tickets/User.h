#pragma once
#include "pch.h"

using json = nlohmann::json;

bool userLogin(const json& users);
int userRegister(json& users);
json readUsersFromJson(const string& filename);



