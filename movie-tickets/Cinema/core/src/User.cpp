#include "pch.h"
#include "User.h"
#include "Validation.h"
#include "Menu.h"

json users;  //Global json object to store user data
void initUsers() {
	users = readUsersFromJson("users.json");
}

json readUsersFromJson(const string& filename) {
	ifstream file(filename);
	json j;
	if (file.is_open()) {
		file >> j;
		file.close();


	}
	else {
		j = { {"users", json::array()} };
	}
	return j;
}

int  userRegister(json& users) {
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


	system("pause");
}

bool userLogin(const json& users) {
	string username;
	string password;

	cout << "=== Login ===\n";
	cout << "Enter username: ";
	cin >> username;
	cout << "Enter password: ";
	password = getHiddenPassword();

	return autehnticate(users, username, password);

	system("pause");
}

void userMainMenu()
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