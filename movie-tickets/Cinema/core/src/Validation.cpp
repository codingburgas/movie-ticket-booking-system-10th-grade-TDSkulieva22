#include "pch.h"
#include "Validation.h"
#include "Admin.h"
#include "Menu.h"
#include "DrawText.h"
#include "Colors.h"

bool isValidPassword(const string& password) {
	if (password.length() < 6) {
		cout << "Password must be at least 6 characters long." << endl;
		return false;
	}

	bool hasUpper = false;
	bool hasDigit = false;
	bool hasSpecial = false;

	for (char c : password) {  //Iterate through each character in the password string
		if (isupper(c)) {
			hasUpper = true;
		}
		if (isdigit(c)) {
			hasDigit = true;
		}
		if (ispunct(c)) {
			hasSpecial = true;
		}
	}

	if (!hasUpper || !hasDigit || !hasSpecial) {
		cout << "Password must contain at least: \n";
		cout << "* one uppercase letter \n";
		cout << "* one digit \n";
		cout << "one special character(!, @, #, $, %) \n";
		return false;
	}
	return true;
}

string getHiddenPassword() {
	string password;
	char ch;
	while (true) {
		ch = _getch();
		if (ch == 13) break;
		else if (ch == 8) {
			if (!password.empty()) {
				cout << "\b \b";
				password.pop_back();
			}
		}
		else {
			password += ch;
			cout << '*';
		}
	}
	cout << endl;
	return password;
}

bool isAdmin() {
	adminTitle();
	newLine(2);

	string username;
	string password;
	int attempts = 0;

	while (attempts < 3) {
		system("cls");
		adminTitle();

		newLine(2);

		setColor(YELLOW);
		cout << "     -> Enter admin username: ";
		resetColor();

		cin >> username;

		setColor(YELLOW);
		cout << "     -> Enter admin password: ";
		resetColor();

		password = getHiddenPassword();

		if (adminLogin(username, password)) {
			return true;
		}
		else {
			attempts++;
			newLine(2);
			setColor(RED);
			cout << "     !!! Incorrect username or password!" << endl;
			resetColor();
			Sleep(1300);
		}
	}
	return false;
}
