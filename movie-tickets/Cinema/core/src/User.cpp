#define UNICODE
#include "pch.h"
#include "User.h"
#include "Validation.h"
#include "Menu.h"
#include <sqlext.h>
#include "DatabaseManager.h"
#include "Colors.h"

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
	setColor(WHITE);
	cout << "========= USER REGISTRATION =========\n";
	resetColor();

	string inputUsername;
	string inputPassword;

	cout << "Enter username: ";
	cin >> inputUsername;

	bool valid = false;
	while (!valid) {
		cout << "Enter password (Min 6 chars, 1 uppercase, 1 digit, 1 special): ";
		inputPassword = getHiddenPassword();
		valid = isValidPassword(inputPassword);
	}

	username = wstring(inputUsername.begin(), inputUsername.end());
	password = wstring(inputPassword.begin(), inputPassword.end());
}

bool User::userRegister(const wstring& username, const wstring& password) {

	if (userExists(username)) {
		wcout << L"User already registered!" << endl;
		return false;
	}


	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	wstring query = L"INSERT INTO Users(Username,Pass) VALUES (N'" + username + L"',N'" + password + L"')";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);

	/*
		
	string username;
	string password;


	cout<< "=== Registration ===\n";
	cout << "Enter username: ";
	cin >> username;

	for (const auto& user : users["users"]) {
		if (user["username"] == username) {
			cout << "Error: username already exists!\n";
			return 0;
		}
	}

	bool validPassword = false;
	while (!validPassword) {
		cout << "Enter password: ";
		password = getHiddenPassword();

		validPassword = isValidPassword(password);
	}

	json newUser = { {"username", username}, {"password", password} };
	users["users"].push_back(newUser);

	ofstream outFile("users.json");
	if (outFile.is_open()) {
		outFile << users.dump(4);  
		outFile.close();
		cout << "Successful registration!\n";
		return 1;  
	}
	else {
		cout << "Error saving user data.\n";
		return -1;  
	}

	//wstring wUsername(username.begin(), username.end());
	//wstring wPassword(password.begin(), password.end()); // Consider hashing here
	//wstring insertQuery = L"INSERT INTO Users(Username, Pass) VALUES (N'" + wUsername + L"', N'" + wPassword + L"')";

	//if (db.executeNonQuery(insertQuery)) {
	//	cout << "Successful registration!\n";
	//	return 1;
	//}
	//else {
	//	cout << "Error saving user data to database.\n";
	//	return -1;
	//}

	system("pause")*/
}
void User::userLoginPr(wstring& username, wstring& password) {
	system("cls");
	setColor(WHITE);
	cout << "============= USER LOGIN =============\n";
	resetColor();

	string inputUsername;
	string inputPassword;

	cout << "Enter username: ";
	cin >> inputUsername;

	cout << "Enter password: ";
	inputPassword = getHiddenPassword();

	username = wstring(inputUsername.begin(), inputUsername.end());
	password = wstring(inputPassword.begin(), inputPassword.end());

}
bool User::userLogin(const wstring& username, const wstring& password) {
	// string username;
	// string password;
	// 
	// cout << "=== Login ===\n";
	// cout << "Enter username: ";
	// cin >> username;
	// cout << "Enter password: ";
	// password = getHiddenPassword();




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


	/*wstring wUsername(username.begin(), username.end());
	wstring wPassword(password.begin(), password.end());

	wstring query = L"SELECT COUNT(*) FROM Users WHERE Username = N'" + wUsername + L"' AND Pass = N'" + wPassword + L"'";

	int count = db.executeScalar(query);
	if (count == 1) {
		cout << "Login successful!\n";
		return true;
	}
	else {
		cout << "Invalid username or password.\n";
		return false;
	}*/


	//return autehnticate(users, username, password);

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
		newLine(1);
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
				cout << "Showing current offers...\n";
				menuRunning = false;
				break;
			}

			system("pause");
		}
	}
}