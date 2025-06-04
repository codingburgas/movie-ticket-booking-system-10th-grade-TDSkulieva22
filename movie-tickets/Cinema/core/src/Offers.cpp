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

        cout << "\n    Enter Offer ID to edit (0 to return to Admin Menu): ";
        cin >> offerId;

        if (offerId == 0) {
            offerEditRunning = false;
            break;
        }

        cin.ignore();

        cout << "    Enter New Title (leave blank to keep current): ";
        getline(cin, newTitle_str);

        cout << "    Enter New Description (leave blank to keep current): ";
        getline(cin, newDescription_str);

        cout << "    Is Offer Active? (y/n, leave blank to keep current): ";
        string isActiveInput;
        getline(cin, isActiveInput);

        bool newIsActive;

        if (isActiveInput.empty() || (tolower(isActiveInput[0]) != 'y' && tolower(isActiveInput[0]) != 'n')) {
            cout << "    This field cannot be left blank. Please enter 'y' or 'n'.\n";
            _getch();
            continue; // Re-run loop
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
