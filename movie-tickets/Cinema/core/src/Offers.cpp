#include "pch.h"
#include "Offers.h"
#include "DatabaseManager.h"
#include "Colors.h"
#include "Menu.h"
#include "DrawText.h"

void Admin::manageOffers() {
    DatabaseManager dbManager;
    bool offerEditRunning = true;

    if (!dbManager.connect()) {
        setColor(RED);
        printCentered("Failed to connect to database for offer management.", 6);
        resetColor();
        _getch();
        return;
    }

    while (offerEditRunning) {
        system("cls");
        cinemaCity();
        newLine(2);

        int offerId;
        string newTitle_str, newDescription_str;
        char newIsActive_char;

        setColor(LIGHT_BLUE);
        cout << "\n    --> Enter Offer ID to edit (0 to return to Admin Menu): ";
        resetColor();

        cin >> offerId;

        if (offerId == 0) {
            offerEditRunning = false;
            break;
        }

        cin.ignore();

        setColor(LIGHT_BLUE);
        cout << "    Enter New Title: ";
        resetColor();
        getline(cin, newTitle_str);
        newLine(1);

        setColor(LIGHT_BLUE);
        cout << "    Enter New Description : ";
        resetColor();
        getline(cin, newDescription_str);

        newLine(1);
        setColor(LIGHT_BLUE);
        cout << "    Is Offer Active? (yes / no): ";
        resetColor();
        string isActiveInput;
        getline(cin, isActiveInput);
        newLine(2);

        bool newIsActive;

        if (isActiveInput.empty() || (tolower(isActiveInput[0]) != 'yes' && tolower(isActiveInput[0]) != 'no')) {
            setColor(RED);
            cout << "    This field cannot be left blank. Please enter 'yes' or 'no'.\n";
            resetColor();
            _getch();
            continue; // Rerun loop
        }
        else {
            newIsActive = (tolower(isActiveInput[0]) == 'y');
        }

        wstring wNewTitle(newTitle_str.begin(), newTitle_str.end());
        wstring wNewDescription(newDescription_str.begin(), newDescription_str.end());

        if (dbManager.updateOffer(offerId, wNewTitle, wNewDescription, newIsActive)) {
            setColor(LIGHT_GREEN);
            printCentered("Offer updated successfully!", 8);
            resetColor();
        }
        else {
            setColor(RED);
            printCentered("Failed to update offer. Check ID or database connection.", 8);
            resetColor();
        }
        _getch();
    }
    dbManager.disconnect();
}
