#define UNICODE
#include "pch.h"
#include "Booking.h"


Booking::Booking(int rows, int cols) : rows(rows), cols(cols) {
	seats = vector<vector<char>>(rows, vector<char>(cols, 'G'));
}

void Booking::displaySeats() const {
	for (const auto& row : seats) {
		for (char seat : row) {
			cout << (seat == 'G' ? "\033[32m💺\033[0m " : "\033[31m💺\033[0m ");
		}
		cout << endl;
	}
}



void Booking::reserveSeat(int row, int col) {
	if (row >= 0 && row < rows && col >= 0 && col < cols && seats[row][col] == 'G') {
		seats[row][col] = 'R';
	}
}

void Booking::freeSeat(int row, int col) {
	if (row >= 0 && row < rows && col >= 0 && col < cols && seats[row][col] == 'R') {
		seats[row][col] = 'G';
	}
}
