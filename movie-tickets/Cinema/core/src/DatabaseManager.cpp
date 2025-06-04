#include "pch.h"
#include "Offers.h"
#include "DatabaseManager.h"

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
