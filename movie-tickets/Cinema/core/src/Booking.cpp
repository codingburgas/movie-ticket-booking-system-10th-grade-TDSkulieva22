#include "pch.h"
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
