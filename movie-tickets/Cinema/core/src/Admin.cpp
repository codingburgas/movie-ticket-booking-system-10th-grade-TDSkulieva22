#include "pch.h"
#include "Admin.h"
#include "Menu.h"
#include "DrawText.h"
#include "Movies.h"
#include "Filters.h"
#include "Colors.h"
#include "Validation.h"
#include "DatabaseManager.h"
#include "Offers.h"

//bool adminLogin(string username,string password) {
//	 string Admin_User = "admin";
//	 string Admin_Pass = "admin";
//
//	return (username == Admin_User && password == Admin_Pass);
//
//}
bool Admin::checkAdminCredentials(const string& username, const string& password) {
    string Admin_User = "admin";
    string Admin_Pass = "admin";
    return (username == Admin_User && password == Admin_Pass);
}

bool Admin::adminLogin() {
    string username;
    string password;
    int attempts = 0;

    while (attempts < 3) {
        system("cls");
        adminTitle(); // Use your existing adminTitle function
        newLine(2);

        setColor(YELLOW);
        cout << "      -> Enter admin username: ";
        resetColor();
        cin >> username;

        setColor(YELLOW);
        cout << "      -> Enter admin password: ";
        resetColor();
        password = getHiddenPassword(); 

        if (checkAdminCredentials(username, password)) { // Use the private helper to verify
            setColor(LIGHT_GREEN);
            printCentered("Admin login successful!", 8);
            resetColor();
            _getch();
            return true;
        }
        else {
            attempts++;
            newLine(2);
            setColor(RED);
            cout << "      !!! Incorrect username or password! " << (3 - attempts) << " attempts remaining." << endl;
            resetColor();
            Sleep(1300); // Pause for 1.3 seconds
        }
    }
    setColor(RED);
    printCentered("Too many failed login attempts. Exiting.", 8);
    resetColor();
    _getch();
    return false; // Login failed after 3 attempts
}

void Admin::adminMenu() {
    int selected = 0;
    bool adminRunning = true;

    vector<string> userOptions = {
        "Add movie",
        "Update movie details",
        "Delete movie",
        "View reservations",
        "Change offers",
        "Exit Admin Menu"
    };

    while (adminRunning) {
        system("cls");

        cinemaCity();

        options(userOptions, selected, 3);

        char key = _getch(); //Read a single character

        if (key == 72) {
            selected = (selected == 0) ? userOptions.size() - 1 : selected - 1;
        }
        else if (key == 80) {
            selected = (selected == userOptions.size() - 1) ? 0 : selected + 1;
        }
        else if (key == 13) { //Enter
            system("cls");

            switch (selected) {
            case 0: {
                    system("cls");
                    cinemaCity();
                    newLine(1);

                    addMovie();
                    break;
                }
            case 1: {
                system("cls");
                cinemaCity();
                newLine(1);

                editMovie();
                break;
            }
            case 2:
                system("cls");
                cinemaCity();
                newLine(1);

                deleteMovie();
                break;
            case 3:
            {
                DatabaseManager db;
                if (db.connect()) {
                   
                    db.getReservationsByCity();
                    db.disconnect();
                }
                break;
            }
            case 4:
                cout << "Change offers...\n";
                this->manageOffers();
                break;
            case 5:
                return;
            }
            system("pause");
        }
    }
}

