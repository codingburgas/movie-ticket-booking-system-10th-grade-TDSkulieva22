#pragma once
#include "pch.h"



class User {
public:
	static bool userExists(const wstring& username);
	static bool userRegister(const wstring& username, const wstring& password);
	static bool userLogin(const wstring& username, const wstring& password);
	static void userMainMenu();
};









