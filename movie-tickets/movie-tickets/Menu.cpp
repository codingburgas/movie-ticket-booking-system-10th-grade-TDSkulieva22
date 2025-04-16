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

void options(string option1, string option2, int selected, int y1, int y2)
{
    string contentSelectedFirst = ":  > " + option1 + " <  :";
    string contentSelectedSecond = ":  > " + option2 + " <  :";
    string contentFirst = ":    " + option1 + "    :";
    string contentSecond = ":    " + option2 + "    :";

    if (selected == 0) {
        //setColor(LIGHT_PURPLE, BLACK);
        printCentered("...............", y1);
        printCentered(contentSelectedFirst, y1 + 1);
        printCentered("...............", y1 + 2);
        //resetColor();
    }
    else {
        //setColor(GRAY, BLACK);
        printCentered("...............", y1);
        printCentered(contentFirst, y1 + 1);
        printCentered("...............", y1 + 2);
        //resetColor();
    }

    newLine(1);
    if (selected == 1) {
        //setColor(LIGHT_PURPLE, BLACK);
        printCentered("...............", y2);
        printCentered(contentSelectedSecond, y2 + 1);
        printCentered("...............", y2 + 2);
        //resetColor();
    }
    else {
        //setColor(GRAY, BLACK);
        printCentered("...............", y2);
        printCentered(contentSecond, y2 + 1);
        printCentered("...............", y2 + 2);
        //resetColor();
    }
}

void displayMenu(int selected) {
    system("cls");

    //titleCinema();

    newLine(2);

    options("START", "LEAVE", selected, 10, 14);
}

void displayRoleMenu(int selected) {
    system("cls");

    newLine(1);

    options("ADMIN", "USER ", selected, 9, 13);
}

void displayUserOptions(int selected) {
    system("cls");

    newLine(1);

    options("LOGIN", "SIGNUP", selected, 11, 15);
}

void adminMenu() {
    int selected = 0;
    bool adminRunning = true;

    while (adminRunning) {
        system("cls");

        for (int i = 0; i < 6; i++) {
            if (i == selected)
                cout << "> ";
            else
                cout << "  ";

            if (i == 0) cout << "Add movie\n";
            if (i == 1) cout << "Update a show\n";
            if (i == 2) cout << "Delete movie\n";
            if (i == 3) cout << "View reservations\n";
            if (i == 4) cout << "Change offers\n";
            if (i == 5) cout << "Exit Admin Menu\n";
        }

        char key = _getch(); //Read a single character

        if (key == 72) {
            selected = (selected > 0) ? selected - 1 : 3;
        }
        else if (key == 80) {
            selected = (selected < 3) ? selected + 1 : 0;
        }
        else if (key == 13) { //Enter
            switch (selected) {
            case 0:
                system("cls");
                break;
            case 1:
                system("cls");
                break;
            case 2:
                system("cls");
                break;
            case 3:
                system("cls");
                break;
            case 4:
                system("cls");
                break;
            case 5:
                cout << "Exiting Admin Menu...\n";
                adminRunning = false;
                break;
            }
        }
    }
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
                                            userRunning = false;
                                            system("cls");
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



