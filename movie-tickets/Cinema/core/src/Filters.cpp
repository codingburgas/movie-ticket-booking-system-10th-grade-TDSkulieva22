#include "pch.h"
#include "Filters.h"
#include "Colors.h"
#include "DrawText.h"
#include "DatabaseManager.h"
#include "Menu.h"

void showMovieFilters() {
    system("cls");
    newLine(1);

    string town;
    setColor(LIGHT_BLUE);
    cout << "    --> Enter your preferred town (Sofia, Burgas, Plovdiv, Varna): ";
    resetColor();
    getline(cin, town);

    string cinema;

    if (town == "Sofia" || town == "sofia") {
        setColor(LIGHT_BLUE);
        cout << "    --> Choose a cinema: Mall Paradise or The Mall: ";
        resetColor();
        getline(cin, cinema);
        if (cinema != "Mall Paradise" && cinema != "The Mall") {
            setColor(RED);
            cout << "    Invalid cinema for Sofia.\n";
            resetColor();
            return;
        }
    }
    else if (town == "Burgas" || town == "burgas") {
        setColor(LIGHT_BLUE);
        cout << "    --> Choose a cinema: Mall Galleria: ";
        resetColor();
        getline(cin, cinema);
        if (cinema != "Mall Galleria") {
            setColor(RED);
            cout << "    Invalid cinema for Burgas.\n";
            resetColor();
            return;
        }
    }
    else if (town == "Plovdiv" || town == "plovdiv") {
        setColor(LIGHT_BLUE);
        cout << "    --> Choose a cinema: Plovdiv Plaza or Mall Plovdiv: ";
        resetColor();
        getline(cin, cinema);
        if (cinema != "Plovdiv Plaza" && cinema != "Mall Plovdiv") {
            setColor(RED);
            cout << "    Invalid cinema for Plovdiv.\n";
            resetColor();
            return;
        }
    }
    else if (town == "Varna" || town == "varna") {
        setColor(LIGHT_BLUE);
        cout << "    --> Choose a cinema: Mall Varna: ";
        resetColor();
        getline(cin, cinema);
        if (cinema != "Mall Varna") {
            setColor(RED);
            cout << "    Invalid cinema for Varna.\n";
            resetColor();
            return;
        }
    }
    else {
        setColor(RED);
        cout << "    Invalid town selected.\n";
        resetColor();
        return;
    }

    string date;
    setColor(LIGHT_BLUE);
    cout << "    --> Choose a date (2025-07-14, 2025-07-15, 2025-07-16, 2025-07-17, 2025-07-18): ";
    resetColor();
    getline(cin, date);
    if (date != "2025-07-14" && date != "2025-07-15" && date != "2025-07-16" && date != "2025-07-17" && date != "2025-07-18") {
        setColor(RED);
        cout << "    Invalid date selected.\n";
        resetColor();
        return;
    }

    string genre;
    setColor(LIGHT_BLUE);
    cout << "    --> Do you want a specific genre? (Enter genre or 'all' for all movies): ";
    resetColor();
    getline(cin, genre);

    
    cout << "\nSelected Filters:\n";
    cout << "Town: " << town << "\n";
    cout << "Cinema: " << cinema << "\n";
    cout << "Date: " << date << "\n";
    cout << "Genre: " << genre << "\n";

    _getch();
}


