#pragma once
#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include <vector>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;
// Declare functions
bool autehnticate(const json& users, const string& username, const string& password);
void displayMenu(const vector<string>& options, int selected, const vector<string>& fileContent);
void gotoxy(int x, int y);
void hideCursor();
vector<string> readEntireFile(const string& filename);
json readUsersFromJson(const string& filename);

#endif
