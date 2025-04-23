#include "User.h"
#include "Admin.h"
#include "Menu.h"
#include "Validation.h"

void gotoxy(int x, int y) {
	COORD coord = { static_cast<SHORT>(x),static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{
	hideCursor();
	initUsers();
	menu();
}
