#include "pch.h"
#include "Payment.h"
#include "Menu.h"
#include "Colors.h"
#include "DrawText.h"
#include <DatabaseManager.h>

void Payment::startPayment(wstring title, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats,int row, wstring programTableName) {
	cinemaCity();
	newLine(3); 

	string paymentMethod;

	setColor(BLUE);
	cout << "    --> How would you like to pay? (cash or online):  ";
	resetColor();

	cin >> paymentMethod;

	if (paymentMethod == "cash") {
		system("cls");
		proccessCashPayment(title,city, location, date, hall, selectSeats);
	}
	else if (paymentMethod == "online") {
		system("cls");
		processOnlinePayment(programTableName,  title,  date,  row,selectSeats);
	}
	else {
		setColor(RED);
		cout << "Invalid method" << endl;
		resetColor();
	}
}

//If a card number matches the format
bool Payment::isValidCardNumber(const string& number) {
	regex card_regex("^\\d{4} \\d{4} \\d{4} \\d{4}$");
	return regex_match(number, card_regex);
}

//CVV to be exactly 3 digits
bool Payment::isValidCVV(const string& cvv) {
	regex cvv_regex("^\\d{3}$");
	return regex_match(cvv, cvv_regex);
}

//MM/YY with correct month range 01-12
bool Payment::isValidExpiry(const string& expiry) {
	regex expiry_regex("^(0[1-9]|1[0-2])/\\d{2}$");
	return regex_match(expiry, expiry_regex);
}
void Payment::proccessCashPayment(wstring movieTitle, wstring city, wstring location, wstring date, vector<wstring> hall, vector<Seat> selectSeats) {
	cinemaCity();
	newLine(2);

	srand(time(nullptr));
	int reservationNumber = rand() % 1000000 + 1;  //Generate random reservation number

	wcout << L"_______________________________________" << endl;

	setColor(GREEN);
	wcout << "            " << L"   --- Payment Receipt ---\n";
	resetColor();

	setColor(LIGHT_GREEN);
	wcout << L"    Movie: ";
	resetColor();
	wcout << movieTitle << endl;

	setColor(LIGHT_GREEN);
	wcout << L"    City: ";
	resetColor();
	wcout << city << endl;

	setColor(LIGHT_GREEN);
	wcout << L"    Location: ";
	resetColor();
	wcout << location << endl;

	setColor(LIGHT_GREEN);
	wcout << L"    Date: ";
	resetColor();
	wcout << date << endl;

	setColor(LIGHT_GREEN);
	wcout << L"    Hall: ";
	resetColor();
	wcout << hall[0] << endl;

	setColor(LIGHT_GREEN);
	wcout << L"    Seats: ";
	resetColor();

	 for (auto& seat : selectSeats) wcout << seat.row << "-" << seat.number << L" ";
	 newLine(1);

	 setColor(LIGHT_GREEN);
	 wcout << L"    Reservation Number: ";
	 resetColor();
	 wcout << reservationNumber << endl;

	 wcout << L"_______________________________________";
	 newLine(2);
	 setColor(LIGHT_RED);
	 wcout << L"!!! Please pay with this reservation number at the counter.\n";
	 resetColor();
	 newLine(2);
}
void Payment::processOnlinePayment(wstring programTableName,wstring movieTitle,wstring date, int row, vector<Seat> selectSeats) {
	cinemaCity();
	newLine(2);

	DatabaseManager db;
	string cardNumber;
	string expiry;
	string cvv;
	
	//Get hall type for the movie
	const vector<wstring> hall = db.getMovieHall(programTableName, movieTitle,date);

	setColor(LIGHT_GREEN);
	printCentered("Online transaction", 8);
	resetColor();

	newLine(2);

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

	setColor(BLUE);

	double hallPr;
	string currentHall;

	//Calculate base price
	if (hall[0] == L"2D") {
		hallPr = 5;
		currentHall = "2D";
	}
	else if (hall[0] == L"3D") {
		hallPr = 6;
		currentHall = "3D";
	}
	else if (hall[0] == L"4DX") {
		hallPr = 7;
		currentHall = "4DX";
	}
	else if (hall[0] == L"IMAX") {
		hallPr = 8;
		currentHall = "IMAX";
	}
	else {
		hallPr = 9;
		currentHall = "VIP";
	}

	//Adjust price by seat row category
	if (row >= 1 && row <= 4) {
		hallPr += 1;
	}
	else if (row >= 5 && row <= 7) {
		hallPr += 2;
	}
	else {
		hallPr += 3;
	}

	if (date == L"2025-07-14") {
		hallPr = hallPr - (hallPr * 0.10);
	}

	cout << "    --> Enter a valid coupon code:  ";
	resetColor();

	string couponCode;
	getline(cin, couponCode);

	if (couponCode == "TICKET2%") {
		hallPr = hallPr - (hallPr * 0.02);
	}

	
	
	//Validate card details
	if (!isValidCardNumber(cardNumber) || !isValidExpiry(expiry) || !isValidCVV(cvv)) {
		setColor(RED);
		cout << "Payment failed due to invalid details.\n";
		resetColor();
		return;
	}

	//Round amount to 2 decimals
	double amount = hallPr;
	amount = round(amount * 100) / 100.0;

	cout << fixed << setprecision(2);
	newLine(2);

	setColor(YELLOW);
	cout << "    --> Payment of " << amount << " euro is successfull!" << endl;
	resetColor();
}