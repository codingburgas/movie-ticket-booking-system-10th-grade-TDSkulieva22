#pragma once
#include "pch.h"
#include "Offers.h"

//struct Movie;
//
//bool adminLogin(string username,string password);
//void adminMenu(Movie*& moviesHead);

class Admin {
public:
    bool adminLogin();
    bool checkAdminCredentials(const string& username, const string& password);
    void adminMenu();
    void manageOffers();
};