#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <vector>
#include <windows.h>

#include "Menu.h"
using namespace std;

void gotoxy(int x, int y) {
	COORD coord = { static_cast<SHORT>(x),static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


vector<string> readEntireFile(const string& filename) {
	vector<string> lines;
	ifstream file(filename);
	string line;
	if (file.is_open()) {
		while (getline(file, line)) {
			lines.push_back(line);
		}
		file.close();
	}
	else {
		lines.push_back("MAIN MENU");
	}
	return lines;

}

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle,&info);

}

void displayMenu(const vector<string>& options, int selected, const vector<string>& fileContent) {
	system("cls");

	for (const string& line : fileContent) {
		cout << line << endl;
	}
	cout << "===========================" << endl;

	for (int i = 0; i < options.size(); i++)
	{
		cout << "| ";
		if (i == selected) {
			cout << ">" << options[i];
			for (int j = options[i].length(); j < 23; j++) cout << " ";
		}
		else {
			cout << " " << options[i];
			for (int j = options[i].length(); j < 23; j++) cout << " ";
		}
		cout << "|" << endl;
	}
	cout << "===========================" << endl;
}

int main()
{
	// showMainMenu();

	vector<string> options = { "Login as ADMIN" , "Login as USER" };
	int selected = 0;
	bool running = true;
	hideCursor();

	while (running) {

		vector<string> fileContent = readEntireFile("title.txt");
		displayMenu(options, selected, fileContent);


		int key = _getch();
		if (key == 0 || key == 224) {
			key = _getch();
			switch (key)
			{
			case 72:
				selected = (selected > 0) ? selected - 1 : options.size() - 1;
				break;
			case 80:
				selected = (selected < options.size() - 1) ? selected + 1 : 0;
				break;
			}
		}
		else if (key == 13) {
			system("cls");
			if (selected == 0) {
				cout << "Selected  0:" << endl;
			}
			else if (selected == 1) {
				cout << "Selected 1:"<<endl;
			}
			cout << "\nPress enter to continue...";
			cin.get();
		}

	}


	return 0;
}

