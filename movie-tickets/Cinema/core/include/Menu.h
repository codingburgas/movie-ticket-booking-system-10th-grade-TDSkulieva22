#pragma once
#include "pch.h"

void menu();
void displayMenu(int selected);
void printCentered(const string& text, int y);
wstring to_wstring(const string& str);
void newLine(int newLines);
void hideCursor();
void options(const vector<string>& optionsList, int selected, int startY);

