#pragma once
#include "pch.h"
#include "Seat.h"




//wcout << L"\n\n=== Payment Receipt ===\n";
//wcout << L"Movie: " << movieTitle << endl;
//wcout << L"City: " << city << endl;
//wcout << L"Location: " << location << endl;
//wcout << L"Date: " << date << endl;
//wcout << L"Hall: " << hallNumber << endl;
//wcout << L"Seats: ";
//
//for (auto& seat : selectedSeats) wcout << seat.row << "-" << seat.number << L" ";
//wcout << L"\nReservation Number: " << reservationNumber << endl;
//wcout << L"Please pay with this reservation number at the counter.\n";
//wcout << L"=========================\n\n";

class Payment {
public:
	void startPayment(wstring title, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats, int row, wstring programTableName);
private:
	bool isValidCardNumber(const string& number);
	bool isValidCVV(const string& cvv);
	bool isValidExpiry(const string& expiry);
	void proccessCashPayment(wstring movieTitle, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats);
	void processOnlinePayment(wstring programTableName, wstring movieTitle, wstring date, int row, vector<Seat> selectSeats);
	//wstring movieTitle;
	//wstring city;
	//wstring location;
	//wstring date;
	//int hallNumber;
	//vector<Seat> selectedSeats;
	//int reservationNumber;

};