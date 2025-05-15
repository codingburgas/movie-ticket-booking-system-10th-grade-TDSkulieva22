#include "pch.h"

class DatabaseManager
{
private:
	SQLHENV hEnv;
	SQLHENV hDbc;

public:
	bool connect();
	void disconnect();
	bool executeNonQuery(const wstring& query);
	bool executeQuery(const wstring& query, int col);
};