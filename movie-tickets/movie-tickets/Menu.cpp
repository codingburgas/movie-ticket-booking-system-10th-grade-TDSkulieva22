#include "Menu.h"
#include "Admin.h"
#include "User.h"

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);

}

void printCentered(const string& text, int y) {
	int padding = (80 - text.length() + 35) / 2;  //Calculate the padding needed
	COORD pos = { (SHORT)padding, (SHORT)y };  //Structure to specify the position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	cout << text;
}
void newLine(int newLines)
{
	for (int i = 0; i < newLines; i++)
	{
		cout << endl;
	}
}


