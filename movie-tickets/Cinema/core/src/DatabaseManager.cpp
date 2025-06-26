#include "pch.h"
#include "Offers.h"
#include "DatabaseManager.h"
#include "Booking.h"
#include "Menu.h"

SQLHENV hEnv = nullptr;
SQLHDBC hDbc = nullptr;

bool DatabaseManager::connect() {
	SQLWCHAR connStr[] = L"Driver={ODBC Driver 17 for SQL Server};Server=.;Database=CinemaDB;Trusted_Connection=yes;";
	SQLWCHAR outConnSTR[1024];
	SQLSMALLINT outConnSTRLen;

	SQLRETURN ret;

	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

	ret = SQLDriverConnectW(hDbc, NULL, connStr, SQL_NTS, outConnSTR, 1024, &outConnSTRLen, SQL_DRIVER_COMPLETE);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}


void DatabaseManager::disconnect() {
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}

bool DatabaseManager::executeNonQuery(const wstring& query) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
}

bool DatabaseManager::executeQuery(const wstring& query, int col) {
	SQLHSTMT hStmt;
	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLWCHAR buffer[100];

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

	// Select only active offers
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

	SQLBindCol(hStmt, 1, SQL_C_SLONG, &offerId, 0, NULL); // OfferId
	SQLBindCol(hStmt, 2, SQL_C_WCHAR, titleBuffer, sizeof(titleBuffer), &titleLen); // Title
	SQLBindCol(hStmt, 3, SQL_C_WCHAR, descriptionBuffer, sizeof(descriptionBuffer), &descriptionLen); // Description
	SQLBindCol(hStmt, 4, SQL_C_BIT, &isActive, 0, &isActiveLen); // IsActive (BIT type)

	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		OfferDetails offer;
		offer.offerId = offerId;
		offer.title = titleBuffer;
		offer.description = descriptionBuffer;
		offer.isActive = (isActive == 1); // Convert SQL BIT to C++ bool
		offers.push_back(offer);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DatabaseManager::updateOffer(int offerId, const wstring& newTitle, const wstring& newDescription, bool newIsActive) {
	wstring query = L"UPDATE Offers SET Title = N'" + newTitle + L"', Description = N'" + newDescription + L"', IsActive = " + (newIsActive ? L"1" : L"0") + L", LastModifiedDate = GETDATE() WHERE OfferId = " + std::to_wstring(offerId);
	return executeNonQuery(query);
}

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

	wstring programDetailsQuery = L"SELECT Date, Time, Hall "
		L"FROM [CinemaDB].[dbo].[" + programTableName + L"] "
		L"WHERE MovieId = (SELECT MovieId FROM Movies WHERE Title = N'" + movieTitle + L"') "
		L"AND Location = N'" + cinema + L"' "
		L"AND Date = N'" + date + L"' "
		L"ORDER BY Time;";

	SQLRETURN ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmtProgram);
	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		return showtimes;
	}

	ret = SQLExecDirectW(hStmtProgram, (SQLWCHAR*)programDetailsQuery.c_str(), SQL_NTS);

	if (!(ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmtProgram);
		return showtimes;
	}

	SQLWCHAR dateBuffer[50], timeBuffer[50], hallBuffer[50];
	SQLLEN dateLen, timeLen, hallLen;

	SQLBindCol(hStmtProgram, 1, SQL_C_WCHAR, dateBuffer, sizeof(dateBuffer), &dateLen);
	SQLBindCol(hStmtProgram, 2, SQL_C_WCHAR, timeBuffer, sizeof(timeBuffer), &timeLen);
	SQLBindCol(hStmtProgram, 3, SQL_C_WCHAR, hallBuffer, sizeof(hallBuffer), &hallLen);

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

	wstring query = L"SELECT seat_row, seat_number, status FROM SeatReservations WHERE program_id = " + to_wstring(programId);

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return seats;
	}

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
	vector<wstring> halls;
	SQLHSTMT hStmt = SQL_NULL_HANDLE;


	wstring query = L"SELECT DISTINCT Hall "
		L"FROM [CinemaDB].[dbo].[" + programTableName + L"] "
		L"WHERE MovieId = (SELECT MovieId FROM Movies WHERE Title = N'" + movieTitle + L"') "
		L"AND [Date] = '" + date + L"';";

	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) {
		return halls;
	}

	if (SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS) {
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return halls;
	}

	SQLWCHAR hallBuffer[100];
	SQLLEN hallLen;

	SQLBindCol(hStmt, 1, SQL_C_WCHAR, hallBuffer, sizeof(hallBuffer), &hallLen);

	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		halls.push_back(hallBuffer);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return halls;
}

int DatabaseManager::getLoggedInUserId(wstring username) {
	 
	SQLHSTMT hStmt = SQL_NULL_HANDLE;
	int userId = -1;

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
	return userId;
}

bool DatabaseManager::getReservationsByCity() {
	SQLHSTMT hStmt = SQL_NULL_HANDLE;

	wstring city;
	wcout << L"Enter city: ";
	cin.ignore();
	cin.clear();
	getline(wcin, city);


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

	SQLINTEGER id, program_id, seat_row, seat_number, user_id;

	SQLWCHAR username[100];
	SQLBindCol(hStmt, 1, SQL_C_SLONG, &id, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_C_SLONG, &program_id, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_C_SLONG, &seat_row, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_C_SLONG, &seat_number, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_C_WCHAR, username, sizeof(username), NULL);


	while (SQLFetch(hStmt) == SQL_SUCCESS) {
		wcout << L"------------------------" << endl;
		wcout << L" Reservation ID: " << id << endl;
		wcout << L" City: " << city << endl;
		wcout << L" Row: " << seat_row << endl;
		wcout << L" Number: " << seat_number << endl;
		wcout << L" Username: " << username << endl;
		wcout << L"------------------------" << endl;
		newLine(2);
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}

bool DatabaseManager::deleteReservation(int userId) {
	wcout << L"Enter the reservation you want to delete (ID): ";
	int reservationId;
	wcin >> reservationId;

	SQLHSTMT hStmt = SQL_NULL_HANDLE;
	wstring query = L"DELETE FROM SeatReservations WHERE id = " + to_wstring(reservationId) + L" AND user_id = " + to_wstring(userId);

	SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	SQLRETURN ret = SQLExecDirectW(hStmt, (SQLWCHAR*)query.c_str(), SQL_NTS);

	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		wcout << L"Reservation with ID " << reservationId << L" successfully deleted.\n";
	}
	else {
		wcout << L"Error: Could not delete reservation with ID " << reservationId << L".\n";
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
		return false;
	}

	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	return true;
}
