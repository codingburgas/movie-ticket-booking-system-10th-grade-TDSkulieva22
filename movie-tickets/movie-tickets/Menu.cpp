#include <iostream>
#include "Menu.h"
#include "Admin.h"
#include "User.h"

using namespace std;

void showMainMenu() {
	// TODO
}

void showAdminLogin() {
	system("cls");

	string username;
	string password;

	cout << "=== Login for Admin ===\n";
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	if (adminLogin(username,password)) {
		cout << "Login successful" << endl;
	}
	else {
		cout << "Wrong username or password!" << endl;
	}
	system("pause");
}

void showUserMenu() {
	int choice;
	do {
		system("cls");
		cout << "==== User Menu ====\n";
		cout << "1. Login\n";
		cout << "2. Register\n";
		cout << "3. Back\n";
		cout << "Select an option: ";

		cin >> choice;

		system("cls");

		switch (choice)
		{
		//case 1: userLogin(json); break;
		//case 2: userRegister(); break;
		case 3: return;
		default: cout << "Invalid selection" << endl; break;
		}



	} while (choice != 3);
}
