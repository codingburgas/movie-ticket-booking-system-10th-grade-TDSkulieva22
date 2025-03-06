#include "User.h"
#include <iostream>
#include <map>

using namespace std;

map<string, string> users;

void userRegister() {
	string username;
	string password;


	cout<< "=== Регистрация ===\n";
	cout << "Въведете потребителско име: ";
	cin >> username;

	if (users.find(username) != users.end()) {
		cout << "Грешка: потребителското име вече съществува!\n";
		system("pause");
		return;
	}

	cout << "Въведете парола: ";
	cin >> password;
	users[username] = password;

	cout << "Успешна регистрация!\n";
	system("pause");
}

void userLogin() {
	string username;
	string password;

	cout << "=== Вход ===\n";
	cout << "Потребителско име: ";
	cin >> username;
	cout << "Парола: ";
	cin >> password;

	if (users.find(username) != users.end() && users[username] == password) {
		cout << "Успешен вход" << endl;
	}
	else{
		cout << "Грешно име и парола" << endl;
	}

	system("pause");
}