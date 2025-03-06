#include "User.h"
#include <iostream>
#include <map>

using namespace std;

map<string, string> users;

void userRegister() {
	string username;
	string password;


	cout<< "=== Registration ===\n";
	cout << "Enter username: ";
	cin >> username;

	if (users.find(username) != users.end()) {
		cout << "Error: username already exists!\n";
		system("pause");
		return;
	}

	cout << "Enter password: ";
	cin >> password;
	users[username] = password;

	cout << "Successful registration!\n";
	system("pause");
}

void userLogin() {
	string username;
	string password;

	cout << "=== Login ===\n";
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	if (users.find(username) != users.end() && users[username] == password) {
		cout << "Login successful" << endl;
	}
	else{
		cout << "Wrong username or password" << endl;
	}

	system("pause");
}