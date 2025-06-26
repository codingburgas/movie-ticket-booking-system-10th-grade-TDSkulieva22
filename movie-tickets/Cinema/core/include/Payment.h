#pragma once
#include "pch.h"
#include "Seat.h"

class Payment {
public:
	void startPayment(wstring title, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats, int row, wstring programTableName);
private:
	bool isValidCardNumber(const string& number);
	bool isValidCVV(const string& cvv);
	bool isValidExpiry(const string& expiry);
	void proccessCashPayment(wstring movieTitle, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats);
	void processOnlinePayment(wstring programTableName, wstring movieTitle, wstring date, int row, vector<Seat> selectSeats);

};