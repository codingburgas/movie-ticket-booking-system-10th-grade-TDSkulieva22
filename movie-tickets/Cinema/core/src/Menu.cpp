#include "pch.h"
#include "Menu.h"
#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include "Colors.h"
#include "DrawText.h"
#include "Movies.h"

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);

}

void printCentered(const string& text, int y) {
	int padding = (80 - text.length() + 35) / 2;  //Calculate the padding needed
	COORD pos = { (SHORT)padding, (SHORT)y };  //Structure to specify the position
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	cout << text;
}
void newLine(int newLines)
{
	for (int i = 0; i < newLines; i++)
	{
		cout << endl;
	}
}

void options(const vector<string>& optionsList, int selected, int startY)
{
    for (int i = 0; i < optionsList.size(); i++)
    {
        string content;
        string border = "......................";

        int y = startY + 6 + i * 4;

        bool isSelected = (i == selected);
        content = (isSelected ? " > " : "   ") + optionsList[i] + (isSelected ? " < " : "    ");

        if (isSelected) {
            setColor(LIGHT_RED, BLACK); //Highlighted
        }
        else {
            setColor(BRIGHT_WHITE, BLACK);
        }

        printCentered(border, y);
        printCentered(content, y + 1);
        printCentered(border, y + 2);
        newLine(2);
    }
}

void displayMenu(int selected) {
    system("cls");

    cinemaCity();

    newLine(1);

    vector<string> userOptions = {
        "START",
        "LEAVE"
    };

    options(userOptions, selected, 6);
}

void displayRoleMenu(int selected) {
    system("cls");

    cinemaCity();

    newLine(3);

    vector<string> userOptions = {
        "ADMIN",
        "USER"
    };

    options(userOptions, selected, 6);
}

void displayUserOptions(int selected) {
    system("cls");

    cinemaCity();

    newLine(1);

    vector<string> userOptions = {
        "LOGIN",
        "SIGNUP" 
    };

    options(userOptions, selected, 6);
}

void menu() {
    Movie* moviesHead = nullptr;
    int selected = 0;
    bool running = true;
    while (running) {
        displayMenu(selected);

        char key = _getch();
        switch (key) {
        case 72:
            selected = (selected == 0) ? 1 : 0;
            break;
        case 80:
            selected = (selected == 1) ? 0 : 1;
            break;
        case 13:
            if (selected == 0) {
                system("cls");
                running = false;  //Exit the main menu

                system("cls");

                int roleSelected = 0;
                bool roleRunning = true;

                while (roleRunning) {
                    displayRoleMenu(roleSelected);

                    char keyRole = _getch();  //Get user input for role selection

                    switch (keyRole) {
                    case 72:
                        roleSelected = (roleSelected == 0) ? 1 : roleSelected - 1;
                        break;
                    case 80:
                        roleSelected = (roleSelected == 1) ? 0 : roleSelected + 1;
                        break;
                    case 13:
                        if (roleSelected == 0) {
                            system("cls");
                            if (isAdmin()) {
                                cout << "Admin login successful!" << endl;
                                adminMenu(moviesHead);
                                running = true;  //Go back to the main menu
                            }
                            else {
                                cout << "Log in failed" << endl;
                                running = true;
                            }
                        }
                        else if (roleSelected == 1) {  //"USER" is selected

                            int userSelected = 0;
                            bool userRunning = true;

                            while (userRunning) {
                                system("cls");
                                displayUserOptions(userSelected);
                                int userKey = _getch();
                                if (userKey == 0 || userKey == 224) {
                                    userKey = _getch();  //Get user input for user options
                                    switch (userKey) {
                                    case 72:
                                        userSelected = (userSelected > 0) ? userSelected - 1 : 2 - 1;
                                        break;
                                    case 80:
                                        userSelected = (userSelected < 2 - 1) ? userSelected + 1 : 0;
                                        break;
                                    }
                                }
                                else if (userKey == 13) {
                                    system("cls");
                                    if (userSelected == 0) {  //"LOGIN" is selected
                                        wstring username, password;
                                        wcout << "Enter username: ";
                                        wcin >> username;

                                        wcout << endl;

                                        wcout << "Enter password: ";
                                        wcin >> password;


                                        if (User::userLogin(username, password)) {
                                            cout << "Login successful!" << endl;
                                            system("pause");
                                            userRunning = false;
                                            system("cls");
                                            User::userMainMenu();
                                        }
                                        else {
                                            cout << "Failed to login!" << endl;
                                            system("pause");
                                        }
                                    }
                                    else if (userSelected == 1) { //"SIGNUP" is selected
                                   
                                        wstring username, password;
                                        wcout << L"Enter username: ";
                                        wcin >> username;

                                        wcout << endl;

                                        wcout << L"Enter password: ";
                                        wcin >> password;

                                        if (User::userExists(username)) {
                                            wcout << L"Username already registered!" << endl;
                                            system("pause");
                                        }
                                        else
                                        {
                                            if (User::userRegister(username, password)) {
                                                wcout << L"Registration successful!" << endl;
                                                userRunning = false;
                                                system("cls");
                                                User::userMainMenu();
                                            }
                                            else {
                                                wcout << L"Registration failed!" << endl;
                                            }
                                        }


                                    }
                                }
                            }
                        }
                        roleRunning = false;
                        break;
                    }
                }
            }
            else {
                system("cls");
                printCentered("Goodbye! You can close the app by pressing any key button...", 10);
                running = false;
                break;
            }

        }
    }
}




