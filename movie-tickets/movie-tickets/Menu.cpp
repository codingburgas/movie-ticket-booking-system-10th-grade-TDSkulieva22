#include <iostream>
#include "Menu.h"
#include "Admin.h"
#include "User.h"

using namespace std;

void showMainMenu() {
	int choice;
	do {

		system("cls");
		cout << "==== Главно Меню ====\n";
		cout << "1. Вход като Админ\n";
		cout << "2. Вход като Потребител\n";
		cout << "3. Изход\n";
		cout << "Изберете опция:";

		cin >> choice;

		switch (choice)
		{
		case 1: showAdminLogin(); break;
		case 2: showUserMenu(); break;
		case 3: exit(0);
		default: cout << "Невалиден избор" << endl; break;
		}


	} while (choice != 3);
}

void showAdminLogin() {
	system("cls");

	string username;
	string password;

	cout << "=== Вход за Админ===\n";
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;

	if (adminLogin(username,password)) {
		cout << "Успешен вход" << endl;
	}
	else {
		cout << "Грешно име или парола!" << endl;
	}
	system("pause");
}

void showUserMenu() {
	int choice;
	do {
		system("cls");
		cout << "==== Потребителско Меню ====\n";
		cout << "1. Вход\n";
		cout << "2. Регистрация\n";
		cout << "3. Назан\n";
		cout << "Изберете опция:";

		cin >> choice;

		switch (choice)
		{
		case 1: userLogin(); break;
		case 2: userRegister(); break;
		case 3: return;
		default: cout << "Невалиден избор" << endl; break;
		}



	} while (choice != 3);
}
