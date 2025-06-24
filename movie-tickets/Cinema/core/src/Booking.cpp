#include "pch.h"
#include "Booking.h"
#include "Colors.h"
#include "DrawText.h"
#include "DatabaseManager.h"
#include "Menu.h"
#include <Payment.h>

map<wstring, vector<wstring>> validLocations = {
    {L"Sofia", {L"The Mall", L"Mall Paradise"}},
    {L"Plovdiv", {L"Plovdiv Plaza", L"Mall Plovdiv"}},
    {L"Burgas", {L"Mall Galleria"}},
    {L"Varna", {L"Mall Varna"}}
};

vector<wstring> validDates = {
    L"2025-07-14",
    L"2025-07-15",
    L"2025-07-16",
    L"2025-07-17",
    L"2025-07-18"
};

void printSeatMap(const vector<Seat>& seats) {
    wstring map[10][10];

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {          
            map[i][j] = L"(   )";
        }
    }
    for (const auto& seat : seats) {
        if (seat.isReserved) {
            map[seat.row - 1][seat.number - 1] = L"( X )";
        }
    }

    for (int i = 0; i < 10; i++) {
        resetColor();
        wcout << "         " << L"   Row " << i + 1 << L": ";
        for (int j = 0; j < 10; j++){
            if (map[i][j] == L"( X )") {
                setColor(RED);
            }
            else {
                setColor(GREEN);
            }
            wcout << map[i][j] << " ";
        }
        newLine(2);
    }
    resetColor();
}

void reserveTicket(int userId) {
    DatabaseManager db;
    ReservationInput input;

    cinemaCity();
    newLine(4);

    while (true) {
        setColor(BLUE);
        wcout << L"    --> Enter city: ";
        resetColor();
        getline(wcin, input.city);

        if (validLocations.count(input.city) == 0) {
            setColor(RED);
            newLine(1);
            wcout << L"   !!! Invalid city. Available options: Sofia, Plovdiv, Burgas, Varna\n";
            resetColor();
            newLine(1);
        }
        else {
            break;
        }
    }

    while (true) {
        setColor(BLUE);
        wcout << L"    --> Enter location: ";
        resetColor();

        getline(wcin, input.location);

        const auto& locations = validLocations[input.city];
        if (find(locations.begin(), locations.end(), input.location) == locations.end()) {
            setColor(RED);
            newLine(1);
            wcout << L"   !!! Invalid location for " << input.city << L". Available options:\n";
            resetColor();
            newLine(1);
            for (const auto& loc : locations) {
                setColor(YELLOW);
                wcout << L"   - " << loc << endl;
                resetColor();
            }
            newLine(1);
        }
        else {
            break;
        }
    }

    while (true) {
        setColor(BLUE);
        wcout << L"    --> Enter date (YYYY-MM-DD): ";
        resetColor();

        getline(wcin, input.date);

        if (find(validDates.begin(), validDates.end(), input.date) == validDates.end()) {
            setColor(RED);
            newLine(1);
            wcout << L"   !!! Invalid date. Available dates are:\n";
            resetColor();
            newLine(1);
            for (const auto& d : validDates) {
                setColor(YELLOW);
                wcout << L"   - " << d << endl;
                resetColor();
            }
            newLine(1);
        }
        else {
            break;
        }
    }

    wstring programTable = input.city + L"Program";

    wstring query = L"SELECT DISTINCT m.Title "
        L"FROM Movies m "
        L"JOIN [" + programTable + L"] p ON m.MovieId = p.MovieId "
        L"WHERE p.Location = N'" + input.location + L"' AND p.Date = N'" + input.date + L"'";

    vector<wstring> movies = db.getMovieTitles(query);

    system("cls");
    cinemaCity();

    setColor(GREEN);
    printCentered("These are all the available movies that meet your requirements : ", 10);
    resetColor();

    newLine(3);

    for (size_t i = 0; i < movies.size(); i++) {
        wcout <<"     " << i + 1 << L". " << movies[i] << endl;
    }
    int movieIndex;
    newLine(2);

    setColor(YELLOW);
    wcout << L"    --> Choose a movie: ";
    resetColor();

    wcin >> movieIndex; 
    wcin.ignore();
    input.movieTitle = movies[movieIndex - 1];

    int programId = db.getProgramId(programTable, input.movieTitle, input.location, input.date);

    
    auto result = db.getMovieHall(programTable, input.movieTitle,input.date);


    vector<Seat> currentSeats = db.getSeatMap(programId);

    system("cls");
    cinemaCity();
    newLine(3);

    printSeatMap(currentSeats);

    int numTickets;

    newLine(1);
    setColor(YELLOW);
    wcout << L"    --> Number of tickets: "; 
    resetColor();

    wcin >> numTickets;
    newLine(1);
    int currentRow;
    vector<Seat> selectedSeats;
    for (int i = 0; i < numTickets; i++) {
        int row, num;
        setColor(YELLOW);
        wcout << "    " << L"--> Seat #" << i + 1 << L" - Row: ";
        resetColor();
        wcin >> row;
        currentRow = row;

        setColor(YELLOW);
        wcout << "    " << L"--> Seat #" << i + 1 << L" - Number: ";
        resetColor();
        wcin >> num;

        bool isFree = true;
        for (const auto& s : currentSeats)
            if (s.row == row && s.number == num && s.isReserved)
                isFree = false;

        if (isFree) {
            selectedSeats.push_back({ row, num, false });
        }
        else {
            setColor(RED);
            wcout << L"This seat has already been booked. Choose another." << endl;
            resetColor();
            newLine(1);
            i--;
        }
    }

    if (db.reserveSeats(programId, selectedSeats, userId)) {
        system("cls");
        cinemaCity();
        setColor(YELLOW);
        printCentered("Successful reservation!", 10);
        newLine(2);
        resetColor();
        printSeatMap(db.getSeatMap(programId));

        system("cls");
        cinemaCity();
        newLine(3);
        Payment pm;
        pm.startPayment(input.movieTitle, input.city, input.location, input.date, result, selectedSeats,currentRow, programTable);
    }
    else {
        setColor(RED);
        wcout << L" Error.\n";
        resetColor();
    }
}
