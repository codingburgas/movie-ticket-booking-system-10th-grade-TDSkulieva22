#pragma once
#include "pch.h"

class Payment {
public:
	void startPayment();
private:
	bool isValidCardNumber(const string& number);
	bool isValidCVV(const string& cvv);
	bool isValidExpiry(const string& expiry);
	void proccessCashPayment();
	void processOnlinePayment();
};