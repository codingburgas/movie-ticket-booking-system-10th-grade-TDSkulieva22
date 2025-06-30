#include "pch.h"
#include "Offers.h"
#include "DatabaseManager.h"
#include "Booking.h"
#include "Menu.h"
#include "Colors.h"

//Global handles for environment and database connection
SQLHENV hEnv = nullptr;
SQLHDBC hDbc = nullptr;

bool DatabaseManager::connect() { //Establish database connection
	//Connection for SQL Server
	SQLWCHAR connStr[] = L"Driver={ODBC Driver 17 for SQL Server};Server=.;Database=CinemaDB;Trusted_Connection=yes;";
	SQLWCHAR outConnSTR[1024];
	SQLSMALLINT outConnSTRLen;

	SQLRETURN ret;

	//Allocate and set environment
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	//Attempt connection
	ret = SQLDriverConnectW(hDbc, NULL, connStr, SQL_NTS, outConnSTR, 1024, &outConnSTRLen, SQL_DRIVER_COMPLETE);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}


void DatabaseManager::disconnect() { //Free database connection
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool DatabaseManager::executeNonQuery(const wstring& query) { //INSERT/UPDATE/DELETE
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

//Execute query and display single column result
bool DatabaseManager::executeQuery(const wstring& query, int col) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLWCHAR buffer[100];

	//Fetch and print result rows
	while (SQLFetch(hStmt) == SQL_SUCCESS)
	{
		SQLGetData(hStmt, col, SQL_C_WCHAR, buffer, sizeof(buffer), NULL);
		wcout << L"Result: " << buffer << endl;
	}
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DatabaseManager::getAllActiveOffers(vector<OfferDetails>& offers) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

	//Select only active offers
	wstring query = L"SELECT OfferId, Title, Description, IsActive FROM Offers WHERE IsActive = 1 ORDER BY OfferId";
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLINTEGER offerId;
	SQLWCHAR titleBuffer[256];
	SQLWCHAR descriptionBuffer[256];
	SQLUSMALLINT isActive; 

	SQLLEN titleLen, descriptionLen, isActiveLen;

	//Bind columns
	SQLBindCol(hStmt, 1, SQL_C_SLONG, &offerId, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_C_WCHAR, titleBuffer, sizeof(titleBuffer), &titleLen);
	SQLBindCol(hStmt, 3, SQL_C_WCHAR, descriptionBuffer, sizeof(descriptionBuffer), &descriptionLen);
	SQLBindCol(hStmt, 4, SQL_C_BIT, &isActive, 0, &isActiveLen); 

	//Store result in vector
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		OfferDetails offer;
		offer.offerId = offerId;
		offer.title = titleBuffer;
		offer.description = descriptionBuffer;
		offer.isActive = (isActive == 1); //Convert SQL BIT to C++ bool
		offers.push_back(offer);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DatabaseManager::updateOffer(int offerId, const wstring& newTitle, const wstring& newDescription, bool newIsActive) {
	wstring query = L"UPDATE Offers SET Title = N'" + newTitle + L"', Description = N'" + newDescription + L"', IsActive = " + (newIsActive ? L"1" : L"0") + L", LastModifiedDate = GETDATE() WHERE OfferId = " + std::to_wstring(offerId);
	return executeNonQuery(query);
}

//Get list of movie titles
vector<wstring> DatabaseManager::getMovieTitles(const wstring& sqlQuery) {
	vector<wstring> movieTitles;
	SQLHSTMT hStmt = SQL_NULL_HANDLE;

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		return movieTitles;
	}

	ret = SQLExecDirectW(hStmt, (SQLWCHAR*)sqlQuery.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return movieTitles;
	}

	SQLWCHAR titleBuffer[256];
	SQLLEN titleLen;

	SQLBindCol(hStmt, 1, SQL_C_WCHAR, titleBuffer, sizeof(titleBuffer), &titleLen);

	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		movieTitles.push_back(titleBuffer);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return movieTitles;
}

bool DatabaseManager::getMovieDetails(const wstring& movieTitle, MovieDetails& details) {
	SQLHSTMT hStmtDetails = SQL_NULL_HANDLE;

	//SQL to get all movie info
	wstring movieDetailsQuery = L"SELECT Title, Genre, Duration, Story, Category, Languages, Actor, Director "
		L"FROM Movies WHERE Title = N'" + movieTitle + L"';";

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmtDetails);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		return false;
	}

	ret = SQLExecDirectW(hStmtDetails, (SQLWCHAR*)movieDetailsQuery.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmtDetails);
		return false;
	}

	if (SQLFetch(hStmtDetails) == SQL_SUCCESS) {
		SQLWCHAR buffer[512];
		SQLLEN len;
		SQLINTEGER durationVal;

		//Read all fields into structure
		SQLGetData(hStmtDetails, 1, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.title = buffer;

		SQLGetData(hStmtDetails, 2, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.genre = buffer;

		SQLGetData(hStmtDetails, 3, SQL_C_SLONG, &durationVal, 0, NULL);
		details.duration = static_cast<int>(durationVal);

		SQLGetData(hStmtDetails, 4, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.story = buffer;

		SQLGetData(hStmtDetails, 5, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.category = buffer;

		SQLGetData(hStmtDetails, 6, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.languages = buffer;

		SQLGetData(hStmtDetails, 7, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.actor = buffer;

		SQLGetData(hStmtDetails, 8, SQL_C_WCHAR, buffer, sizeof(buffer), &len);
		details.director = buffer;

		SQLFreeHandle(SQL_HANDLE_STMT, hStmtDetails);
		return true;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmtDetails);
	return false;
}

vector<ShowtimeDetails> DatabaseManager::getShowtimes(const wstring& programTableName, const wstring& movieTitle, const wstring& cinema, const wstring& date) {
	vector<ShowtimeDetails> showtimes;
	SQLHSTMT hStmtProgram = SQL_NULL_HANDLE;

	//Fetch date, time, and hall for the movie
	wstring programDetailsQuery = L"SELECT Date, Time, Hall "
		L"FROM [CinemaDB].[dbo].[" + programTableName + L"] "
		L"WHERE MovieId = (SELECT MovieId FROM Movies WHERE Title = N'" + movieTitle + L"') "
		L"AND Location = N'" + cinema + L"' "
		L"AND Date = N'" + date + L"' "
		L"ORDER BY Time;";

	//Allocate statement handle
	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmtProgram);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		return showtimes;
	}

	//Execute the query
	ret = SQLExecDirectW(hStmtProgram, (SQLWCHAR*)programDetailsQuery.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmtProgram);
		return showtimes;
	}

	SQLWCHAR dateBuffer[50], timeBuffer[50], hallBuffer[50];
	SQLLEN dateLen, timeLen, hallLen;

	//Bind the result columns
	SQLBindCol(hStmtProgram, 1, SQL_C_WCHAR, dateBuffer, sizeof(dateBuffer), &dateLen);
	SQLBindCol(hStmtProgram, 2, SQL_C_WCHAR, timeBuffer, sizeof(timeBuffer), &timeLen);
	SQLBindCol(hStmtProgram, 3, SQL_C_WCHAR, hallBuffer, sizeof(hallBuffer), &hallLen);

	//Fetch the rows and populate the result vector
	while (SQLFetch(hStmtProgram) == SQL_SUCCESS) {
		ShowtimeDetails showtime;
		showtime.date = dateBuffer;
		showtime.time = timeBuffer;
		showtime.hall = hallBuffer;
		showtimes.push_back(showtime);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmtProgram);
	return showtimes;
}

vector<Seat> DatabaseManager::getSeatMap(int programId) {
	vector<Seat> seats;
	SQLHSTMT hStmt = SQL_NULL_HANDLE;

	//Query to get seat data
	wstring query = L"SELECT seat_row, seat_number, status FROM SeatReservations WHERE program_id = " + to_wstring(programId);

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return seats;
	}

	//Fetch and store seat info
	SQLINTEGER row, number, status;
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		SQLGetData(hStmt, 1, SQL_C_SLONG, &row, 0, nullptr);
		SQLGetData(hStmt, 2, SQL_C_SLONG, &number, 0, nullptr);
		SQLGetData(hStmt, 3, SQL_C_SLONG, &status, 0, nullptr);
		seats.push_back({ row, number, status == 1 });
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return seats;
}

bool DatabaseManager::reserveSeats(int programId, const vector<Seat>& selectedSeats, int userId) {
	for (const auto& seat : selectedSeats) {
		//INSERT query for each seat
		wstring query = L"INSERT INTO SeatReservations (program_id, seat_row, seat_number, user_id, reservation_time, status) VALUES (" +
			to_wstring(programId) + L", " + to_wstring(seat.row) + L", " + to_wstring(seat.number) + L", " +
			to_wstring(userId) + L", '2025-12-12',1)";
		if (!executeNonQuery(query)) return false;
	}
	return true;
}

int DatabaseManager::getProgramId(const wstring& tableName, const wstring& movieTitle, const wstring& location, const wstring& date) {
	SQLHSTMT hStmt = SQL_NULL_HANDLE;
	int programId = -1;

	//Join ProgramMap and program table to find program_id
	wstring query =
		L"SELECT TOP 1 pm.id "
		L"FROM ProgramMap pm "
		L"JOIN [" + tableName + L"] p ON pm.program_table_id = p.Id AND pm.table_name = N'" + tableName + L"' "
		L"JOIN Movies m ON p.MovieId = m.MovieId "
		L"WHERE m.Title = N'" + movieTitle + L"' AND p.Location = N'" + location + L"' AND p.Date = N'" + date + L"'";

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
		if (SQLFetch(hStmt) == SQL_SUCCESS) {
			SQLGetData(hStmt, 1, SQL_C_SLONG, &programId, 0, nullptr);
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return programId;
}

vector<wstring> DatabaseManager::getMovieHall(const wstring& programTableName, const wstring& movieTitle, const wstring& date) {
	vector<wstring> halls;  //Result vector to hold hall names
	SQLHSTMT hStmt = SQL_NULL_HANDLE;


	wstring query = L"SELECT DISTINCT Hall "
		L"FROM [CinemaDB].[dbo].[" + programTableName + L"] "
		L"WHERE MovieId = (SELECT MovieId FROM Movies WHERE Title = N'" + movieTitle + L"') "
		L"AND [Date] = '" + date + L"';";

	//Allocate a statement handle
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
		return halls;
	}

	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return halls;
	}

	//Buffer to store hall name
	SQLWCHAR hallBuffer[100];
	SQLLEN hallLen;

	SQLBindCol(hStmt, 1, SQL_C_WCHAR, hallBuffer, sizeof(hallBuffer), &hallLen);

	while (SQLFetch(hStmt) == SQL_SUCCESS) { //Fetch all distinct halls and add to the vector
		halls.push_back(hallBuffer);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);  //Clean up
	return halls;
}

int DatabaseManager::getLoggedInUserId(wstring username) {
	 
	SQLHSTMT hStmt = SQL_NULL_HANDLE;
	int userId = -1;  //Default value if user is not found

	//Get the user ID by matching the username
	wstring query =
		L"SELECT TOP 1 Id "
		L"FROM Users "
		L"WHERE Username = N'" + username + L"'";

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) == SQL_SUCCESS) {
		if (SQLFetch(hStmt) == SQL_SUCCESS) {
			SQLGetData(hStmt, 1, SQL_C_SLONG, &userId, 0, nullptr);
		}
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return userId;  //Return the retrieved user ID
}

bool DatabaseManager::getReservationsByCity() {
	SQLHSTMT hStmt = SQL_NULL_HANDLE;

	wstring city;
	setColor(GREEN);
	wcout << L"    --> Enter city: ";
	resetColor();
	getline(wcin, city);
	newLine(2);

	//SQL query to retrieve reservations
	wstring query =
		L"SELECT sr.id, sr.program_id, sr.seat_row, sr.seat_number, u.Username "
		L"FROM SeatReservations AS sr "
		L"JOIN ProgramMap AS pm "
		L"ON sr.program_id = pm.id "
		L"JOIN Users AS u "
		L"ON sr.user_id = u.id "
		L"WHERE pm.table_name LIKE N'" + city + L"Program'";


	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	//Define variables to hold the results
	SQLINTEGER id, program_id, seat_row, seat_number, user_id;

	SQLWCHAR username[100];
	SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_C_SLONG, &program_id, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_C_SLONG, &seat_row, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_C_SLONG, &seat_number, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_C_WCHAR, username, sizeof(username), NULL);

	//Loop through each fetched row
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		setColor(LIGHT_BLUE);
		wcout << L" | Reservation ID: | ";
		resetColor();
		wcout << id;

		setColor(LIGHT_BLUE);
		wcout << L" | City: | ";
		resetColor();
		wcout << city;

		setColor(LIGHT_BLUE);
		wcout << L" | Row: | ";
		resetColor();
		wcout << seat_row;

		setColor(LIGHT_BLUE);
		wcout << L" | Number: | ";
		resetColor();
		wcout << seat_number;

		setColor(LIGHT_BLUE);
		wcout << L" | User: | ";
		resetColor();
		wcout << username << endl;

		setColor(LIGHT_RED);
		wcout << L"______________________________" << endl;
		resetColor();
		newLine(1);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true; //To indicate success
}

bool DatabaseManager::getReservationsByUserId(int userId) {
	SQLHSTMT hStmt = SQL_NULL_HANDLE;

	//Get reservations for the given userId
	wstring query =
		L"SELECT sr.id, sr.program_id, sr.seat_row, sr.seat_number, pm.table_name, u.Username "
		L"FROM SeatReservations AS sr "
		L"JOIN ProgramMap AS pm "
		L"ON sr.program_id = pm.id "
		L"JOIN Users AS u "
		L"ON sr.user_id = u.id "
		L"WHERE sr.user_id = " + to_wstring(userId);

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	//Variables to hold fetched data
	SQLINTEGER id, program_id, seat_row, seat_number;
	SQLWCHAR username[100], table_name[100];

	//Bind columns to variables
	SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_C_SLONG, &program_id, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_C_SLONG, &seat_row, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_C_SLONG, &seat_number, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_C_WCHAR, table_name, sizeof(table_name), NULL);
	SQLBindCol(hStmt, 6, SQL_C_WCHAR, username, sizeof(username), NULL);

	//Output reservation details
	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		setColor(LIGHT_GREEN);
		wcout << L" | Reservation ID: | ";
		resetColor();
		wcout << id;

		setColor(LIGHT_GREEN);
		wcout << L" | Program ID: | ";
		resetColor();
		wcout << program_id;

		setColor(LIGHT_GREEN);
		wcout << L" | Program: | ";
		resetColor();
		wcout << table_name;

		setColor(LIGHT_GREEN);
		wcout << L" | Row: | ";
		resetColor();
		wcout << seat_row;

		setColor(LIGHT_GREEN);
		wcout << L" | Number: | ";
		resetColor();
		wcout << seat_number;

		setColor(LIGHT_GREEN);
		wcout << L" | User: | ";
		resetColor();
		wcout << username << endl;

		wcout << L"______________________________" << endl;
		newLine(2);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DatabaseManager::deleteReservation(int userId) {
	setColor(LIGHT_RED);
	wcout << L"    --> Enter the reservation you want to delete (ID): ";
	resetColor();

	int reservationId;
	wcin >> reservationId;

	SQLHSTMT hStmt = SQL_NULL_HANDLE;
	wstring query = L"DELETE FROM SeatReservations WHERE id = " + to_wstring(reservationId) + L" AND user_id = " + to_wstring(userId);

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		setColor(LIGHT_BLUE);
		wcout << L"Reservation with ID " << reservationId << L" successfully deleted.\n";
		resetColor();
	}
	else {
		setColor(RED);
		wcout << L" !!! Error: Could not delete reservation with ID " << reservationId << L".\n";
		resetColor();

		//Free the statement handle and return false due to failure
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	//Free statement handle and return true on success
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}