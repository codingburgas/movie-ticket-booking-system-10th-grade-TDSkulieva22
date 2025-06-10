#include "User.h"
#include "Admin.h"
#include "Menu.h"
#include "Validation.h"
#include "pch.h"
#include "DatabaseManager.h"
#include "Booking.h"

void gotoxy(int x, int y) {
	COORD coord = { static_cast<SHORT>(x),static_cast<SHORT>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int main()
{

	DatabaseManager db;
	if (!db.connect()) {
		wcout << L"Invalid connection" << endl;
		return 1;
	}





	  hideCursor();
	  menu();
	  db.disconnect();
}
