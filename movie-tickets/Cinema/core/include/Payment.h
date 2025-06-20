#pragma once
#include "pch.h"
#include "Seat.h"

class Payment {
public:
	void startPayment();
private:
	bool isValidCardNumber(const string& number);
	bool isValidCVV(const string& cvv);
	bool isValidExpiry(const string& expiry);
	void proccessCashPayment();
	void processOnlinePayment();
	//wstring movieTitle;
	//wstring city;
	//wstring location;
	//wstring date;
	//int hallNumber;
	//vector<Seat> selectedSeats;
	//int reservationNumber;

};