#pragma once
#include "pch.h"
#include "Offers.h"

class Admin {
public:
    bool adminLogin();
    bool checkAdminCredentials(const string& username, const string& password);
    void adminMenu();
    void manageOffers();
};