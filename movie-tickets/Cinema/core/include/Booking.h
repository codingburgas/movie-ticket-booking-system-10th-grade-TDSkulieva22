#pragma once


#include "pch.h"
#include "DatabaseManager.h"
#include "Seat.h"


struct ReservationInput {
    wstring city;
    wstring location;
    wstring date;
    wstring movieTitle;
};


void printSeatMap(const vector<Seat>& seats);
void reserveTicket(int userId);

