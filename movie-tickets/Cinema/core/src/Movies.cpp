#include "pch.h"
#include "Movies.h"
#include "Validation.h"
#include "Menu.h"
#include "Colors.h"
#include "DrawText.h"

string toLower(const string& input) {
	string result = input;
	transform(result.begin(), result.end(), result.begin(), ::tolower);
	return result;
}

string trim(const string& str) {
	size_t first = str.find_first_not_of(" \t\n\r\f\v");
	if (first == string::npos) return "";
	size_t last = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(first, last - first + 1);
}

bool openFile(ifstream& file, const string& filename) {
	file.open(filename);
	if (file.fail()) {
		cout << "Unable to open file " << filename << endl;
		return false;
	}
	return true;
}
