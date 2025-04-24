#include "pch.h"
#include "Filters.h"

string selectedTown() {
	// string towns[] = { "Burgas", "Plovdiv", "Sofia", "Varna" };
	string towns[] = {
		"Burgas-program.json",
		"Plovdiv-program.json",
		"Sofia-program.json",
		"Varna-program.json"
	};

	const int townCount = sizeof(towns) / sizeof(towns[0]);

	cout << "Select city" << endl;

	for (int i = 0; i < townCount; i++) {
		string displayTowns = towns[i].substr(0,towns[i].find('-'));
		cout << (i + 1) << ". " << displayTowns << endl;
	}

	int choice;
	cin >> choice;


	if (choice >= 1 && choice <= townCount) {
		return towns[choice - 1];
	}
	else {
		cout << "Invalid town selected. Please try again." << endl;
	}
}