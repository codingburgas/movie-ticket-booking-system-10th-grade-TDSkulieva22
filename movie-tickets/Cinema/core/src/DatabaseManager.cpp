#include "pch.h"
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
