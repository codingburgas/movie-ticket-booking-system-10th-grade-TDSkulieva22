#include "pch.h"
#include "Admin.h"
#include "Menu.h"
#include "DrawText.h"
#include "Movies.h"
#include "Filters.h"
#include "DatabaseManager.h" 
#include "Colors.h"

void addMovie() {
    cin.ignore();
    system("cls");
    cinemaCity();
    newLine(3);

    //Gather movie details
    string title, genre, story, category, languages, actor, director;
    int duration;

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Title: ";
    resetColor();

    getline(cin, title);

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Genre: ";
    resetColor();

    getline(cin, genre);

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Duration (minutes): ";
    resetColor();

    cin >> duration;

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Story: ";
    resetColor();

    getline(cin, story);
    cin.ignore();

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Category (B, D, C+, D+): ";
    resetColor();
    getline(cin, category);
    cin.ignore();

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Languages: ";
    resetColor();
    getline(cin, languages);
    cin.ignore();

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Main Actor: ";
    resetColor();

    getline(cin, actor);
    cin.ignore();

    setColor(LIGHT_BLUE);
    cout << "    --> Enter Director: ";
    resetColor();
    cin.ignore();
    getline(cin, director);

    //Convert string to wstring
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

    DatabaseManager dbManager;
    if (dbManager.connect()) {
        if (dbManager.executeNonQuery(sqlQuery)) {
            setColor(LIGHT_GREEN);
            cout << "\n     Movie added successfully!\n";
            resetColor();
        }
        else {
            setColor(RED);
            cout << "\n     Failed to add movie.\n";
            resetColor();
        }
        dbManager.disconnect();
    }
    else {
        setColor(RED);
        cout << "\n     Failed to connect to the database.\n";
        resetColor();
    }

    _getch(); // Wait for user input
}

void deleteMovie() {
    system("cls");

    cinemaCity();
    newLine(3);

    string title;

    setColor(LIGHT_BLUE);
    cout << "    --> Enter the title of the movie to delete: ";
    resetColor();
    getline(cin, title);

    //Convert from narrow to wide string
    wstring Title = wstring(title.begin(), title.end());
   
    //N prefix indicates Unicode
    wstring sqlQuery = L"DELETE FROM Movies WHERE Title = N'" + Title + L"'";
 
    DatabaseManager dbManager; //Create a DatabaseManager object
    if (dbManager.connect()) {
        if (dbManager.executeNonQuery(sqlQuery)) {
            setColor(LIGHT_GREEN);
            cout << "\n     Movie deleted successfully.\n";
            resetColor();
        }
        else {
            setColor(RED);
            cout << "\n     Failed to delete movie.\n";
            resetColor();
        }
        dbManager.disconnect();
    }
    else {
        setColor(RED);
        cout << "\n     Failed to connect to the database.\n";
        resetColor();
    }
    _getch(); // Wait for user input
}

void editMovie()
{
    system("cls");
    cinemaCity();
    newLine(1);

    string origTitle;

    newLine(4);
    setColor(YELLOW);
    cout << "    --> Enter the Title of the Movie to Edit: ";
    resetColor();
    cin.ignore();
    getline(cin, origTitle);

    //Convert from narrow to wide string
    wstring OrigTitle = wstring(origTitle.begin(), origTitle.end());

    DatabaseManager dbManager;  //DatabaseManager object
    if (!dbManager.connect()) {
        cout << "\n     Failed to connect to the database.\n";
        _getch();
        return;
    }

    bool edit = true; //To control the editing loop

    while (edit) {
        system("cls");
        cinemaCity();
        newLine(4);

        setColor(LIGHT_BLUE);
        cout << "               --- Editing Movie: " << origTitle << " ---\n\n";
        resetColor();

        setColor(YELLOW);
        cout << "    --> What would you like to edit?\n";
        resetColor();

        cout << "     1. Title\n";
        cout << "     2. Genre\n";
        cout << "     3. Duration\n";
        cout << "     4. Story\n";
        cout << "     5. Category\n";
        cout << "     6. Languages\n";
        cout << "     7. Actor\n";
        cout << "     8. Director\n";
        cout << "     0. Cancel / Back to Menu\n";

        newLine(1);
        setColor(YELLOW);
        cout << "    --> Choose an option: ";
        resetColor();

        int choice;
        cin >> choice;
        cin.ignore();   //Clear input buffer

        if (choice == 0) {
            edit = false;
            break;
        }

        string newVal;
        int newDuration = 0;
        wstring sqlQuery;
        bool queryConstructed = false;

        switch (choice) { //Construct SQL query based on user's choice
        case 1:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Title: ";
            resetColor();

            getline(cin, newVal);

            wstring NewTitle = wstring(newVal.begin(), newVal.end());
            sqlQuery = L"UPDATE Movies SET Title = N'" + NewTitle + L"' WHERE Title = N'" + OrigTitle + L"'";

            //Update to reflect the change
            origTitle = newVal;
            OrigTitle = NewTitle;
            queryConstructed = true;
            break;
        }

        case 2:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Genre: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Genre = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }

        case 3: {
            string input;
            int newDuration = 0;
            bool valid = false;

            while (!valid) {
                setColor(YELLOW);
                cout << "    --> Enter new Duration (minutes): ";
                resetColor();
                getline(cin, input);

                valid = !input.empty();

                newDuration = 0;
                for (char c : input) {
                    if (c < '0' || c > '9') { //Check if input is numeric only
                        valid = false;
                        break;
                    }
                    newDuration = newDuration * 10 + (c - '0');
                }

                if (!valid || newDuration == 0) { //Validate positive integer
                    setColor(RED);
                    cout << "     Invalid input! Please enter a positive number.\n";
                    resetColor();
                }
            }

            sqlQuery = L"UPDATE Movies SET Duration = " + to_wstring(newDuration) +
                L" WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }

        case 4:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Story: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Story = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }

        case 5:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Category: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Category = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }

        case 6:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Languages: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Languages = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }
        case 7:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Actor: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Actor = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }

        case 8:
        {
            setColor(YELLOW);
            cout << "    --> Enter new Director: ";
            resetColor();

            getline(cin, newVal);
            sqlQuery = L"UPDATE Movies SET Director = N'" + wstring(newVal.begin(), newVal.end()) + L"' WHERE Title = N'" + OrigTitle + L"'";
            queryConstructed = true;
            break;
        }
        default:
            setColor(RED);
            cout << "\n     Invalid choice. Try again.\n";
            resetColor();

            _getch();
            continue;
        }

        if (dbManager.executeNonQuery(sqlQuery)) {
            setColor(LIGHT_GREEN);
            cout << "\n     Field updated successfully!\n";
            resetColor();
        }
        else {
            setColor(RED);
            cout << "\n     Failed to update. Movie may not exist.\n";
            resetColor();
        }

        setColor(YELLOW);
        cout << "\n     Do you want to edit another field? (yes/no): ";
        resetColor();

        string again;
        cin >> again;
        cin.ignore();

        if (again != "Yes" && again != "yes") { //Case insensitive
                edit = false;
           }
        }

        dbManager.disconnect();
        _getch();
}
