#include "User.h"
#include "Program.h"
#include "Validation.h"

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