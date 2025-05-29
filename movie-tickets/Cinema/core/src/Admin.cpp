#include "pch.h"
#include "Admin.h"
#include "Menu.h"
#include "DrawText.h"
#include "Movies.h"
#include "Filters.h"

bool adminLogin(string username,string password) {
	 string Admin_User = "admin";
	 string Admin_Pass = "admin";

	return (username == Admin_User && password == Admin_Pass);

}

void adminMenu(Movie*& moviesHead) {
    int selected = 0;
    bool adminRunning = true;

    vector<string> userOptions = {
        "Add movie",
        "Update a show",
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
                    //Movie* townMovies = loadMoviesFromFile(filename);

                    //addMovie(townMovies, filename);
                    break;
                }
            case 1: {
                system("cls");
                cinemaCity();
                newLine(1);

                string filename = selectedTown();
                //Movie* townMovies = loadMoviesFromFile(filename);

                //editMovie(moviesHead, filename);
                break;
            }
            case 2:
                deleteMovie();
                break;
            case 3:
                cout << "View reservations...\n";
                break;
            case 4:
                cout << "Change offers...\n";
                break;
            case 5:
                cout << "Exiting Admin Menu...\n";
                adminRunning = false;
                break;
            }
            system("pause");
        }
    }
}
