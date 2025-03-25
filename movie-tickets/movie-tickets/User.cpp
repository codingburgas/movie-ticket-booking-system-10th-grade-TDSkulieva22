#include "User.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Program.h"
using json = nlohmann::json;
using namespace std;



int  userRegister(json& users) {
	string username;
	string password;


	cout<< "=== Registration ===\n";
	cout << "Enter username: ";
	cin >> username;

	//if (users.find(username) != users.end()) {
	//	cout << "Error: username already exists!\n";
	//	system("pause");
	//	return;
	//}

	//cout << "Enter password: ";
	//cin >> password;
	//users[username] = password;
	//
	//cout << "Successful registration!\n";
	//system("pause");

	for (const auto& user : users["users"]) {
		if (user["username"] == username) {
			cout << "Error: username already exists!\n";
			return 0;
		}
	}

	cout << "Enter password: ";
	cin >> password;

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


	system("pause");
}

bool userLogin(const json& users) {
	string username;
	string password;

	cout << "=== Login ===\n";
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	cin >> password;

	return autehnticate(users, username, password);

	system("pause");
}