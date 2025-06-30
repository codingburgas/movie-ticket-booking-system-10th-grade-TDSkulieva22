#include "pch.h"
#include "User.h"
#include "Validation.h"
#include "Menu.h"
#include <sqlext.h>
#include "DatabaseManager.h"
#include "Colors.h"
#include "DrawText.h"
#include "Filters.h"
#include "Booking.h"

//External handles for environment
extern SQLHENV hEnv;
extern SQLHDBC hDbc;

DatabaseManager db;

bool User::userExists(const wstring& username) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	if (hDbc == NULL) {
		cout<<"Error!"<<endl;
	}

	//To count users with the given username
	wstring query = L"SELECT COUNT(*) FROM Users WHERE Username = N'" + username + L"'";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLINTEGER count = 0;
	//Fetch the result- count
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
	while (!valid) { //Validation criteria
		setColor(YELLOW);
		cout << "    --> Enter password (Min 6 chars, 1 uppercase, 1 digit, 1 special): ";
		resetColor();
		inputPassword = getHiddenPassword(); //Read password without echoing input
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

	//Count users matching usernameand password
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
	return count > 0;  //Return true if count > 0 (valid login)

	system("pause");
}

void User::userMainMenu(wstring username)
{
	int menuSelected = 0;
	bool menuRunning = true;

	//List of options displayed to the user
	vector<string> userOptions = {
		"View our program",
		"Select a seat",
		"Cancel reservation",
		"Your tickets",
		"See offers",
		"Exit"
	};

	while (menuRunning) {
		system("cls");
		cinemaCity();
		options(userOptions, menuSelected, 3);

		char key = _getch();  //Get keyboard input without waiting for enter

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
				showMovieFilters();
				break;
			case 1:
			{
				DatabaseManager db;
				if (!db.connect()) {
					cout << "Failed to connect." << endl;
					break;
				}

				reserveTicket(db.getLoggedInUserId(username));
				db.disconnect();
				break;
			}
			case 2:
			{
				cinemaCity();
				newLine(3);
				db.getReservationsByUserId(db.getLoggedInUserId(username));
				db.deleteReservation(db.getLoggedInUserId(username));
				break;
			}
			case 3:
				cinemaCity();
				newLine(3);

				db.getReservationsByUserId(db.getLoggedInUserId(username));
				break;
			case 4:
				User userObj;
				userObj.userOffers();

				break;

			case 5:
				return;
			}
			system("pause");    //Pause after action before refreshing menu
		}
	}
}

void User::userOffers() {
	system("cls");
	cinemaCity();

	newLine(4);

	setColor(LIGHT_BLUE);
	wcout << "                   ";
	wcout << L"              ------------ CINEMA TICKET OFFERS ------------\n";
	resetColor();

	newLine(1);

	DatabaseManager dbManager; 
	vector<OfferDetails> offers; 

	if (!dbManager.connect()) { 
		setColor(RED);
		printCentered("Failed to connect to database.", 6);
		resetColor();
		_getch();
		return; 
	}

	if (dbManager.getAllActiveOffers(offers)) { 
		if (offers.empty()) {
			setColor(YELLOW);
			printCentered("No active offers available at the moment.", 8);
			resetColor();
		}
		else {
			int lineNum = 0;
			for (const auto& offer : offers) {
				setColor(GRAY);

				wcout << L"       " << offer.offerId << L"   -->   " << offer.title << L"   -->   " << offer.description << endl;
				resetColor();

				lineNum++;
			}
		}
	}
	else {
		setColor(RED);
		printCentered("Error fetching offers from the database.", 8);
		resetColor();
	}

	dbManager.disconnect(); 
	_getch();  //Wait for key press before returning
}
