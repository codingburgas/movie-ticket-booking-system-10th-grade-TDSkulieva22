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

	for (char c : password) {  //Iterate through each character in string
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

	//If all required character types are present
	if (!hasUpper || !hasDigit || !hasSpecial) {
		cout << "Password must contain at least: \n";
		cout << "* one uppercase letter \n";
		cout << "* one digit \n";
		cout << "one special character(!, @, #, $, %) \n";
		return false;
	}
	return true;
}

//Get password input while hiding the characters
string getHiddenPassword() {
	string password;
	char ch;
	while (true) {
		ch = _getch();
		if (ch == 13) break; //If Enter is pressed
		else if (ch == 8) { //Handle Backspace
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
