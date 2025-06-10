#pragma once
#ifndef BOOKING_H
#define BOOKING_H
#include "pch.h"
#include <vector>

class Booking {
public:
	Booking(int rows, int cols);
	void displaySeats() const;
	void reserveSeat(int row, int col);
	void freeSeat(int row, int col);

private:
	vector<vector<char>> seats;
	int rows;
	int cols;
};

#endif
