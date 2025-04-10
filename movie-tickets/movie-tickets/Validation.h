#pragma once
#include "pch.h"

bool isValidPassword(const string& password);
bool autehnticate(const json& users, const string& username, const string& password);
string getHiddenPassword();
