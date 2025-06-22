#include "pch.h"
#include "Payment.h"
#include "Menu.h"
#include "Colors.h"
#include "DrawText.h"

void Payment::startPayment() {
	cinemaCity();
	newLine(2); 

	string paymentMethod;
	cout << "    --> How would you like to pay? (cash or online)  ";
	cin >> paymentMethod;

	if (paymentMethod == "cash") {
		system("cls");
		proccessCashPayment();
	}
	else if (paymentMethod == "online") {
		system("cls");
		processOnlinePayment();
	}
	else {
		setColor(RED);
		cout << "Invalid method" << endl;
		resetColor();
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
	cinemaCity();
	newLine(2);

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
	cinemaCity();
	newLine(2);

	string cardNumber;
	string expiry;
	string cvv;

	double amount = 15;

	cout << "Online transaction" << endl;

	setColor(BLUE);
	cout << "    --> Enter card number (#### #### #### ####): ";
	resetColor();

	cin.ignore();
	getline(cin, cardNumber);

	setColor(BLUE);
	cout << "    --> Enter expiry date (MM/YY): ";
	resetColor();

	getline(cin, expiry);

	setColor(BLUE);
	cout << "    --> Enter CVV (3 digits): ";
	resetColor();

	getline(cin, cvv);

	if (!isValidCardNumber(cardNumber) || !isValidExpiry(expiry) || !isValidCVV(cvv)) {
		setColor(RED);
		cout << "Payment failed due to invalid details.\n";
		resetColor();
		return;
	}

	cout << "    --> Payment of " << amount << " is successfull!" << endl;
}