﻿#include "pch.h"
#include "Booking.h"
#include "Colors.h"
#include "DrawText.h"
#include "DatabaseManager.h"
#include "Menu.h"

//Booking::Booking(int rows, int cols) : rows(rows), cols(cols) {
//	seats = vector<vector<char>>(rows, vector<char>(cols, 'G'));
//}
//
//void Booking::displaySeats() const {
//	for (const auto& row : seats) {
//		for (char seat : row) {
//			cout << (seat == 'G' ? "\033[32m💺\033[0m " : "\033[31m💺\033[0m ");
//		}
//		cout << endl;
//	}
//}
//
//
//
//void Booking::reserveSeat(int row, int col) {
//	if (row >= 0 && row < rows && col >= 0 && col < cols && seats[row][col] == 'G') {
//		seats[row][col] = 'R';
//	}
//}
//
//void Booking::freeSeat(int row, int col) {
//	if (row >= 0 && row < rows && col >= 0 && col < cols && seats[row][col] == 'R') {
//		seats[row][col] = 'G';
//	}
//}
//
void printSeatMap(const vector<Seat>& seats) {
    wstring map[10][10];

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            map[i][j] = L"1";

    for (const auto& seat : seats) {
        if (seat.isReserved)
            map[seat.row - 1][seat.number - 1] = L"X";
    }

    for (int i = 0; i < 10; i++) {
        wcout << L"Row " << i + 1 << L": ";
        for (int j = 0; j < 10; j++)
            wcout << map[i][j] << " ";
        wcout << endl;
    }
}

void reserveTicket(int userId) {
    DatabaseManager db;
    ReservationInput input;

    wcout << L"Enter town: "; getline(wcin, input.city);
    wcout << L"Enter location: "; getline(wcin, input.location);
    wcout << L"Enter date (YYYY-MM-DD): "; getline(wcin, input.date);

    wstring programTable = input.city + L"Program";

    wstring query = L"SELECT DISTINCT m.Title "
        L"FROM Movies m "
        L"JOIN [" + programTable + L"] p ON m.MovieId = p.MovieId "
        L"WHERE p.Location = N'" + input.location + L"' AND p.Date = N'" + input.date + L"'";

    vector<wstring> movies = db.getMovieTitles(query);

    for (size_t i = 0; i < movies.size(); i++) {
        wcout << i + 1 << L". " << movies[i] << endl;
    }
    int movieIndex;
    wcout << L"Choose a movie: ";
    wcin >> movieIndex; wcin.ignore();
    input.movieTitle = movies[movieIndex - 1];

    int programId = db.getProgramId(programTable, input.movieTitle, input.location, input.date);

    vector<Seat> currentSeats = db.getSeatMap(programId);
    printSeatMap(currentSeats);

    int numTickets;
    wcout << L"Number of tickets: "; wcin >> numTickets;

    vector<Seat> selectedSeats;
    for (int i = 0; i < numTickets; i++) {
        int row, num;
        wcout << L"Seat #" << i + 1 << L" - Row: "; wcin >> row;
        wcout << L"Seat #" << i + 1 << L" - Number: "; wcin >> num;

        bool isFree = true;
        for (const auto& s : currentSeats)
            if (s.row == row && s.number == num && s.isReserved)
                isFree = false;

        if (isFree) {
            selectedSeats.push_back({ row, num, false });
        }
        else {
            wcout << L"This seat has already been booked. Choose another." << endl;
            i--;
        }
    }

    if (db.reserveSeats(programId, selectedSeats, userId)) {
        system("cls");
        wcout << L"\n Successful reservation!\n\n";
        printSeatMap(db.getSeatMap(programId));
    }
    else {
        wcout << L" Error.\n";
    }
}
