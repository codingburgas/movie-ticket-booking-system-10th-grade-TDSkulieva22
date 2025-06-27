#include "pch.h"
#include "Filters.h"
#include "Colors.h"
#include "DrawText.h"
#include "DatabaseManager.h"
#include "Menu.h"

void showMovieFilters() {
    system("cls");
    cinemaCity();
    newLine(5);

    wstring town;
    setColor(LIGHT_BLUE);
    wcout << L"    --> Enter your preferred town (Sofia, Burgas, Plovdiv, Varna): ";
    resetColor();
    getline(wcin, town);

    wstring programTableName;
    wstring cinema;

    if (town == L"Sofia" || town == L"sofia") {
        do {
            setColor(LIGHT_BLUE);
            wcout << L"    --> Choose a cinema: Mall Paradise or The Mall: ";
            resetColor();
            getline(wcin, cinema);
            if (cinema != L"Mall Paradise" && cinema != L"The Mall") {
                setColor(RED);
                wcout << L"    Invalid cinema for Sofia.\n";

                resetColor();
            }
        } while (cinema != L"Mall Paradise" && cinema != L"The Mall");

        programTableName = L"SofiaProgram";
    }
    else if (town == L"Burgas" || town == L"burgas") {
        do {
            setColor(LIGHT_BLUE);
            wcout << L"    --> Choose a cinema- Mall Galleria: ";
            resetColor();
            getline(wcin, cinema);
            if (cinema != L"Mall Galleria") {
                setColor(RED);
                wcout << L"    Invalid cinema for Burgas.\n";

                resetColor();
            }
        } while (cinema != L"Mall Galleria");

        programTableName = L"BurgasProgram";
    }
    else if (town == L"Plovdiv" || town == L"plovdiv") {
        do {
            setColor(LIGHT_BLUE);
            wcout << L"    --> Choose a cinema- Plovdiv Plaza or Mall Plovdiv: ";
            resetColor();
            getline(wcin, cinema);
            if (cinema != L"Plovdiv Plaza" && cinema != L"Mall Plovdiv") {
                setColor(RED);
                wcout << L"    Invalid cinema for Plovdiv.\n";

                resetColor();
            }
        } while (cinema != L"Plovdiv Plaza" && cinema != L"Mall Plovdiv");

        programTableName = L"PlovdivProgram";
    }
    else if (town == L"Varna" || town == L"varna") {
        do {
            setColor(LIGHT_BLUE);
            wcout << L"    --> Choose a cinema- Mall Varna: ";
            resetColor();
            getline(wcin, cinema);
            if (cinema != L"Mall Varna") {
                setColor(RED);
                wcout << L"    Invalid cinema for Varna.\n";

                resetColor();
            }
        } while (cinema != L"Mall Varna");
        
        
        programTableName = L"VarnaProgram";
    }
    else {
        setColor(RED);
        wcout << L"    Invalid town selected.\n";
        resetColor();
        return;
    }

    wstring date;
    setColor(LIGHT_BLUE);
    wcout << L"    --> Choose a date (2025-07-14, 2025-07-15, 2025-07-16, 2025-07-17, 2025-07-18): ";
    resetColor();
    getline(wcin, date);
    if (date != L"2025-07-14" && date != L"2025-07-15" && date != L"2025-07-16" && date != L"2025-07-17" && date != L"2025-07-18") {
        setColor(RED);
        wcout << L"    Invalid date selected.\n";
        resetColor();
        return;
    }

    wstring genre;
    setColor(LIGHT_BLUE);
    wcout << L"    --> Do you want a specific genre? (Enter genre or 'all' for all movies): ";
    resetColor();
    getline(wcin, genre);

    wstring sqlQuery = L"SELECT DISTINCT M.Title "
        L"FROM Movies AS M "
        L"JOIN [CinemaDB].[dbo].[" + programTableName + L"] AS P ON M.MovieId = P.MovieId "
        L"WHERE P.Location = N'" + cinema + L"' "
        L"AND P.Date = N'" + date + L"'";

    if (genre != L"all" && !genre.empty()) {
        sqlQuery += L" AND M.Genre = N'" + genre + L"'";
    }
    sqlQuery += L" ORDER BY M.Title;";

    DatabaseManager dbManager;
    vector<wstring> movieTitles;

    if (dbManager.connect()) {
        movieTitles = dbManager.getMovieTitles(sqlQuery);
        
        if (movieTitles.empty()) {
            setColor(YELLOW);
            wcout << L"    No movies found matching your criteria.\n";
            resetColor();
            dbManager.disconnect();
            _getch();
            return;
        }
        
        system("cls");
        cinemaCity();

        newLine(3);
        setColor(LIGHT_GREEN);
        wcout << L"    --- Available Movies ---\n\n";
        resetColor();

        for (int i = 0; i < movieTitles.size(); ++i) {
            wcout << L"    " << (i + 1) << L". " << movieTitles[i] << L"\n";
        }

        newLine(2);
        setColor(LIGHT_BLUE);
        wcout << L"    --> Enter the number of the movie to see details (0 to go back): ";
        resetColor();
        
        int movieChoice;
        wcin >> movieChoice;
        wcin.ignore();

        if (movieChoice == 0) {
            dbManager.disconnect();
            return;
        }

        if (movieChoice > 0 && movieChoice <= movieTitles.size()) {
            wstring selectedMovieTitle = movieTitles[movieChoice - 1];

            MovieDetails details;

            if (dbManager.getMovieDetails(selectedMovieTitle, details)) {
                system("cls");
                cinemaCity();

                newLine(2);
                setColor(LIGHT_GREEN);
                wcout << L"    --- Movie Details ---\n\n";
                resetColor();

                vector<ShowtimeDetails> showtimes = dbManager.getShowtimes(programTableName, selectedMovieTitle, cinema, date);
                displayMovieInfo(details, showtimes);
            }
            else {
                setColor(RED);
                wcout << L"    Failed to retrieve movie details.\n";
                resetColor();
            }
        }
        else {
            setColor(RED);
            wcout << L"    Invalid movie choice.\n";
            resetColor();
        }
        dbManager.disconnect();
    }
    else {
        setColor(RED);
        wcout << L"\n    Failed to connect to the database.\n";
        resetColor();
    }
    _getch();
}

void displayMovieInfo(const MovieDetails& details, const vector<ShowtimeDetails>& showtimes) {
    system("cls");

    cinemaCity();
    newLine(3);

    setColor(LIGHT_GREEN);
    wcout << L"          --- Movie Details ---" << endl; 
    resetColor();

    newLine(1);
    setColor(BLUE);
    wcout << L"    Title: ";
    resetColor();

    wcout << details.title << L"\n";

    setColor(BLUE);
    wcout << L"    Genre: ";
    resetColor();

    wcout << details.genre << L"\n";

    setColor(BLUE);
    wcout << L"    Duration: ";
    resetColor();

    wcout << details.duration << L" minutes\n";

    newLine(1);
    setColor(BLUE);
    wcout << L"    Story: " << endl;
    resetColor();

    wcout << "    " << details.story << L"\n";
    newLine(1);

    setColor(BLUE);
    wcout << L"    Category: ";
    resetColor();

    wcout << details.category << L"\n";

    setColor(BLUE);
    wcout << L"    Languages: ";
    resetColor();

    wcout << details.languages << L"\n";

    setColor(BLUE);
    wcout << L"    Actor: ";
    resetColor();

    wcout << details.actor << L"\n";

    setColor(BLUE);
    wcout << L"    Director: ";
    resetColor();

    wcout << details.director << L"\n";

    if (!showtimes.empty()) {
        setColor(LIGHT_GREEN);
        newLine(2);
        wcout << L"\n          --- Showtimes ---\n";
        resetColor();
        newLine(1);
        for (const auto& showtime : showtimes) {
            wstring shortTime = showtime.time.substr(0, 5);
            setColor(BLUE);
            wcout << L"    Date: ";
            resetColor();

            wcout << showtime.date;

            setColor(BLUE);
            wcout << L", Time: ";
            resetColor();

            wcout << shortTime;

            setColor(BLUE);
            wcout << L", Hall: ";
            resetColor();

            wcout << showtime.hall << L"\n";
        }
    }
    else {
        setColor(YELLOW);
        wcout << L"    No showtimes found for this movie at the selected cinema and date.\n";
        resetColor();
    }
}


