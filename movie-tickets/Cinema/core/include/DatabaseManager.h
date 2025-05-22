#include "pch.h"

extern SQLHENV hEnv;
extern SQLHDBC hDbc;

class DatabaseManager
{


public:
	bool connect();
	void disconnect();
	bool executeNonQuery(const wstring& query);
	bool executeQuery(const wstring& query, int col);
};