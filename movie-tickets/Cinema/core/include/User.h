#pragma once
#include "pch.h"


class User {
public:
	static bool userExists(const wstring& username);
	static bool userRegister(const wstring& username, const wstring& password);
	static bool userLogin(const wstring& username, const wstring& password);
	static void userMainMenu(wstring username);
	static void userRegisterPr(wstring& username, wstring& password);
	static void userLoginPr(wstring& username, wstring& password);
	void userOffers();
private:
	static int currentUserId;
};

