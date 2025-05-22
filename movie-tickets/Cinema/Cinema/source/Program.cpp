#include "User.h"
#include "Admin.h"
#include "Menu.h"
#include "Validation.h"
#include "pch.h"
#include "DatabaseManager.h"


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

	// wstring name = L"testlogin";
	// wstring pass = L"testparola";
	// wstring insertQuery = L"  INSERT INTO Users(Username,Pass) VALUES (N'" + name + L"',N'" + pass + L"') ";
	// wcout<<db.executeNonQuery(insertQuery);

	/*db.executeQuery(L"SELECT * FROM Users",2);
	db.executeQuery(L"SELECT * FROM Users", 3);
	*/



	  hideCursor();
	  menu();
	  db.disconnect();
}
