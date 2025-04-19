#include "Menu.h"
#include "Admin.h"
#include "User.h"
#include "Validation.h"
#include "Colors.h"

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

        if (i == selected)
            content = " > " + optionsList[i] + " <  ";
        else
            content = "   " + optionsList[i] + "    ";

        int y = startY + i * 4;

        printCentered(border, y);
        printCentered(content, y + 1);
        printCentered(border, y + 2);
        newLine(2);
    }
}

void displayMenu(int selected) {
    system("cls");

    //titleCinema();

    newLine(2);

    vector<string> userOptions = {
        "START",
        "LEAVE"
    };

    options(userOptions, selected, 6);
}

void displayRoleMenu(int selected) {
    system("cls");

    newLine(1);

    vector<string> userOptions = {
        "ADMIN",
        "USER"
    };

    options(userOptions, selected, 6);
}

void displayUserOptions(int selected) {
    system("cls");

    newLine(1);

    vector<string> userOptions = {
        "LOGIN",
        "SIGNUP" 
    };

    options(userOptions, selected, 6);
}

void menu() {
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
                printCentered("Game Starting...", 10);
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
                                adminMenu();
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
                                        if (userLogin(users)) {
                                            cout << "Login successful!" << endl;
                                            system("pause");
                                            userRunning = false;
                                            system("cls");
                                            userMainMenu();
                                        }
                                        else {
                                            cout << "Failed to login!" << endl;
                                            system("pause");
                                        }
                                    }
                                    else if (userSelected == 1) {  //"SIGNUP" is selected
                                        int result = userRegister(users);
                                        if (result == 1) {
                                            cout << "Registration successful!" << endl;
                                            system("pause");
                                            userRunning = false;
                                            system("cls");
                                            userMainMenu();
                                        }
                                        else if (result == -1) {
                                            cout << "Error saving user data." << endl;
                                        }
                                        userRunning = false;
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



