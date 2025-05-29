#include "pch.h"
#include "Admin.h"
#include "Menu.h"
#include "DrawText.h"
#include "Movies.h"
#include "Filters.h"
#include "DatabaseManager.h" 


//string toLower(const string& input) {
//	string result = input;
//	transform(result.begin(), result.end(), result.begin(), ::tolower);
//	return result;
//}
//
//string trim(const string& str) {
//	size_t first = str.find_first_not_of(" \t\n\r\f\v");
//	if (first == string::npos) return "";
//	size_t last = str.find_last_not_of(" \t\n\r\f\v");
//	return str.substr(first, last - first + 1);
//}

void addMovie() {
    system("cls");
    cinemaCity();
    newLine(1);

    cout << "     --- Add New Movie ---\n\n";

    // Gather movie details from the admin
    string title, genre, story, category, languages, actor, director;
    int duration;

    cin.ignore();

    cout << "     Enter Title: ";
    getline(cin, title);

    cout << "     Enter Genre: ";
    getline(cin, genre);

    cout << "     Enter Duration (minutes): ";
    cin >> duration;
    cin.ignore();

    cout << "     Enter Story: ";
    getline(cin, story);

    cout << "     Enter Category (e.g., PG-13, R): ";
    getline(cin, category);

    cout << "     Enter Languages: ";
    getline(cin, languages);

    cout << "     Enter Main Actor(s): ";
    getline(cin, actor);

    cout << "     Enter Director: ";
    getline(cin, director);

    // Convert string to wstring for SQL Unicode compatibility
    wstring Title = wstring(title.begin(), title.end());
    wstring Genre = wstring(genre.begin(), genre.end());
    wstring Story = wstring(story.begin(), story.end());
    wstring Category = wstring(category.begin(), category.end());
    wstring Languages = wstring(languages.begin(), languages.end());
    wstring Actor = wstring(actor.begin(), actor.end());
    wstring Director = wstring(director.begin(), director.end());

    // Concatenate into a single wstring SQL query
    wstring sqlQuery = L"INSERT INTO Movies (Title, Genre, Duration, Story, Category, Languages, Actor, Director) VALUES (";
    sqlQuery += L"N'" + Title + L"', ";
    sqlQuery += L"N'" + Genre + L"', ";
    sqlQuery += to_wstring(duration) + L", ";
    sqlQuery += L"N'" + Story + L"', ";
    sqlQuery += L"N'" + Category + L"', ";
    sqlQuery += L"N'" + Languages + L"', ";
    sqlQuery += L"N'" + Actor + L"', ";
    sqlQuery += L"N'" + Director + L"')";

    // Execute the query
    DatabaseManager dbManager;
    if (dbManager.connect()) {
        if (dbManager.executeNonQuery(sqlQuery)) {
            cout << "\n     Movie added successfully!\n";
        }
        else {
            cout << "\n     Failed to add movie.\n";
        }
        dbManager.disconnect();
    }
    else {
        cout << "\n     Failed to connect to the database.\n";
    }

    cout << "\n     Press any key to continue...";
    _getch(); // Wait for user input
}

void deleteMovie() {
    system("cls");
    cinemaCity();
    newLine(1);

    cout << "     --- Delete Movie ---\n\n";

    string title;
    cin.ignore();
    cout << "     Enter the Title of the Movie to Delete: ";
    getline(cin, title);

    wstring Title = wstring(title.begin(), title.end());
   
    wstring sqlQuery = L"DELETE FROM Movies WHERE Title = N'" + Title + L"'";

 
    DatabaseManager dbManager;
    if (dbManager.connect()) {
        if (dbManager.executeNonQuery(sqlQuery)) {
            cout << "\n     Movie deleted successfully (if it existed).\n";
        }
        else {
            cout << "\n     Failed to delete movie.\n";
        }
        dbManager.disconnect();
    }
    else {
        cout << "\n     Failed to connect to the database.\n";
    }

    cout << "\n     Press any key to continue...";
    _getch(); // Wait for user input
}
