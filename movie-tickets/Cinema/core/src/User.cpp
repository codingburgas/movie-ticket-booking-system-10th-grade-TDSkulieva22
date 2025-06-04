#define UNICODE
#include "pch.h"
#include "User.h"
#include "Validation.h"
#include "Menu.h"
#include <sqlext.h>
#include "DatabaseManager.h"
#include "Colors.h"
#include "DrawText.h"

extern SQLHENV hEnv;
extern SQLHDBC hDbc;

bool User::userExists(const wstring& username) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	if (hDbc == NULL) {
		cout<<"mn kofti"<<endl;
	}
	wstring query = L"SELECT COUNT(*) FROM Users WHERE Username = N'" + username + L"'";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLINTEGER count = 0;
	if (SQLFetch(hStmt) == SQL_SUCCESS) {
		SQLGetData(hStmt, 1, SQL_C_SLONG, &count, 0, NULL);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return count > 0;
}

void User::userRegisterPr(wstring& username, wstring& password) {
	system("cls");

	cinemaCity();

	signUpTitle();
	newLine(3);

	string inputUsername;
	string inputPassword;

	setColor(YELLOW);
	cout << "    --> Enter username: ";
	resetColor();
	getline(cin, inputUsername);
	

	bool valid = false;
	while (!valid) {
		setColor(YELLOW);
		cout << "    --> Enter password (Min 6 chars, 1 uppercase, 1 digit, 1 special): ";
		resetColor();
		inputPassword = getHiddenPassword();
		valid = isValidPassword(inputPassword);
	}

	username = wstring(inputUsername.begin(), inputUsername.end());
	password = wstring(inputPassword.begin(), inputPassword.end());
}

bool User::userRegister(const wstring& username, const wstring& password) {

	if (userExists(username)) {
		setColor(RED);
		wcout << L"User already registered!" << endl;
		resetColor();

		return false;
	}


	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	wstring query = L"INSERT INTO Users(Username,Pass) VALUES (N'" + username + L"',N'" + password + L"')";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);

}
void User::userLoginPr(wstring& username, wstring& password) {
	system("cls");


	cinemaCity();

	loginTitle();
	newLine(3);

	string inputUsername;
	string inputPassword;

	setColor(YELLOW);
	cout << "    --> Enter username: ";
	resetColor();
	getline(cin, inputUsername);

	setColor(YELLOW);
	cout << "    --> Enter password: ";
	resetColor();

	inputPassword = getHiddenPassword();

	username = wstring(inputUsername.begin(), inputUsername.end());
	password = wstring(inputPassword.begin(), inputPassword.end());

}
bool User::userLogin(const wstring& username, const wstring& password) {

	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	wstring query = L"SELECT COUNT(*) FROM Users WHERE Username = N'" + username + L"' AND Pass = N'" + password + L"'";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);


	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLINTEGER count = 0;
	if (SQLFetch(hStmt) == SQL_SUCCESS) {
		SQLGetData(hStmt, 1, SQL_C_SLONG, &count, 0, NULL);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return count > 0;

	system("pause");
}

void User::userMainMenu()
{
	int menuSelected = 0;
	bool menuRunning = true;

	vector<string> userOptions = {
		"View our program",
		"Select a seat",
		"Cancel reservation",
		"Your tickets",
		"See offers"
	};

	while (menuRunning) {
		system("cls");
		cinemaCity();
		options(userOptions, menuSelected, 6);

		char key = _getch();

		if (key == 72) { //Up
			menuSelected = (menuSelected == 0) ? userOptions.size() - 1 : menuSelected - 1;
		}
		else if (key == 80) { //Down
			menuSelected = (menuSelected == userOptions.size() - 1) ? 0 : menuSelected + 1;
		}
		else if (key == 13) { //Enter
			system("cls");

			switch (menuSelected) {
			case 0:
				cout << "Showing program...\n";
				break;
			case 1:
				cout << "Seat selection...\n";
				break;
			case 2:
				cout << "Cancelling reservation...\n";
				break;
			case 3:
				cout << "Displaying your tickets...\n";
				break;
			case 4:
				//cout << "Showing current offers...\n";
				//menuRunning = false;
				userOffers();
				break;
			}

			system("pause");
		}
	}
}

void User::userOffers() {
	system("cls");
	setColor(WHITE);
	cout << "========= CINEMA TICKET OFFERS =========\n";
	resetColor();

	printCentered("1. Monday Special: Get 10% discount on all reserved tickets for Mondays.\n", 4);
	printCentered("2. Group Discount: If you reserve tickets for a group of up to 10 people, get 5% off the total.\n", 6);
	printCentered("3. Coupon Code Offer: Enter a valid coupon code (TICKET2%) at checkout to get 2% discount.\n", 8);
	printCentered("4. Loyalty Bonus: Buy 5 tickets and get the 6th ticket free!\n", 10);

	setColor(YELLOW);
	printCentered("Note: Discounts cannot be combined.\n", 12);
	resetColor();
}
