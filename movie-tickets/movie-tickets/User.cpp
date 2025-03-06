#include "User.h"
#include <iostream>
#include <map>

using namespace std;

map<string, string> users;

void userRegister() {
	string username;
	string password;


	cout<< "=== ����������� ===\n";
	cout << "�������� ������������� ���: ";
	cin >> username;

	if (users.find(username) != users.end()) {
		cout << "������: ��������������� ��� ���� ����������!\n";
		system("pause");
		return;
	}

	cout << "�������� ������: ";
	cin >> password;
	users[username] = password;

	cout << "������� �����������!\n";
	system("pause");
}

void userLogin() {
	string username;
	string password;

	cout << "=== ���� ===\n";
	cout << "������������� ���: ";
	cin >> username;
	cout << "������: ";
	cin >> password;

	if (users.find(username) != users.end() && users[username] == password) {
		cout << "������� ����" << endl;
	}
	else{
		cout << "������ ��� � ������" << endl;
	}

	system("pause");
}