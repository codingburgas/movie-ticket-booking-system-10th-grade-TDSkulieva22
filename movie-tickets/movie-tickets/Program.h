#pragma once
#include "pch.h"
// Declare functions
bool autehnticate(const json& users, const string& username, const string& password);
void displayMenu(const vector<string>& options, int selected, const vector<string>& fileContent);
void gotoxy(int x, int y);
void hideCursor();
vector<string> readEntireFile(const string& filename);

