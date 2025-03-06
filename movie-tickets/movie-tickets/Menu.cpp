#include <iostream>
#include "Menu.h"
#include "Admin.h"
#include "User.h"

using namespace std;

void showMainMenu() {
	int choice;
	do {

		system("cls");
		cout << "==== ������ ���� ====\n";
		cout << "1. ���� ���� �����\n";
		cout << "2. ���� ���� ����������\n";
		cout << "3. �����\n";
		cout << "�������� �����:";

		cin >> choice;

		switch (choice)
		{
		case 1: showAdminLogin(); break;
		case 2: showUserMenu(); break;
		case 3: exit(0);
		default: cout << "��������� �����" << endl; break;
		}


	} while (choice != 3);
}

void showAdminLogin() {
	system("cls");

	string username;
	string password;

	cout << "=== ���� �� �����===\n";
	cout << "Username: ";
	cin >> username;
	cout << "Password: ";
	cin >> password;

	if (adminLogin(username,password)) {
		cout << "������� ����" << endl;
	}
	else {
		cout << "������ ��� ��� ������!" << endl;
	}
	system("pause");
}

void showUserMenu() {
	int choice;
	do {
		system("cls");
		cout << "==== ������������� ���� ====\n";
		cout << "1. ����\n";
		cout << "2. �����������\n";
		cout << "3. �����\n";
		cout << "�������� �����:";

		cin >> choice;

		switch (choice)
		{
		case 1: userLogin(); break;
		case 2: userRegister(); break;
		case 3: return;
		default: cout << "��������� �����" << endl; break;
		}



	} while (choice != 3);
}
