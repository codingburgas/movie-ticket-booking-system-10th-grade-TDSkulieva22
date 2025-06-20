#include "pch.h"
#include "Payment.h"

void Payment::startPayment() {
	string paymentMethod;
	cout << "cash/online";
	cin >> paymentMethod;

	if (paymentMethod == "cash") {
		proccessCashPayment();
	}
	else if (paymentMethod == "online") {
		processOnlinePayment();
	}
	else {
		cout << "Invalid method" << endl;
	}
}
bool Payment::isValidCardNumber(const string& number) {
	regex card_regex("^\\d{4} \\d{4} \\d{4} \\d{4}$");
	return regex_match(number, card_regex);
}
bool Payment::isValidCVV(const string& cvv) {
	regex cvv_regex("^\\d{3}$");
	return regex_match(cvv, cvv_regex);
}
bool Payment::isValidExpiry(const string& expiry) {
	regex expiry_regex("^(0[1-9]|1[0-2])/\\d{2}$");
	return regex_match(expiry, expiry_regex);
}
void Payment::proccessCashPayment() {
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
}
void Payment::processOnlinePayment() {
	string cardNumber;
	string expiry;
	string cvv;

	double amount = 15;

	cout << "Online transaction" << endl;
	cout << "Enter card number (16 digits): ";
	cin.ignore();
	getline(cin, cardNumber);

	cout << "Enter expiry date: ";
	getline(cin, expiry);

	cout << "Enter CVV: ";
	getline(cin, cvv);

	if (!isValidCardNumber(cardNumber)) {
		cout << "Invalid card number" << endl;
	}
	if (!isValidExpiry(expiry)) {
		cout << "Invalid expiry date" << endl;
	}

	if (!isValidCVV(cvv)) {
		cout << "Invalid dvv" << endl;
	}

	cout << "Payment of " << amount << " is successfull" << endl;
}