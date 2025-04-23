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

Movie* loadMoviesFromFile(string& filename) {
	Movie* head = nullptr;
	ifstream file;
	if (!openFile(file, filename)) {
		return nullptr;
	}

	// JSON object that will contain the data from the file
	json j;
	file >> j;
	srand(time(nullptr));
	int randomIndex = rand() % j.size();
	json selectedMovie = j[randomIndex];
	file.close();

	// (range-based loop) to iterate over all elements in the JSON object
	for (const auto& item : j) {
		Movie* newMovie = new Movie;
		newMovie->TITLE = item["TITLE"];
		newMovie->GENRE = item["GENRE"];
		newMovie->DURATION = item["DURATION"];
		newMovie->STORY = item["STORY"];
		newMovie->CATEGORY = item["CATEGORY"];
		newMovie->LANGUAGE = item["LANGUAGE"];
		newMovie->ACTORS = item["ACTORS"];
		newMovie->DIRECTOR = item["DIRECTOR"];
		newMovie->WHERE = item["WHERE"];
		//newMovie->DATE = item["DATE"];
		newMovie->ROOM = item["ROOM"];
		//newMovie->TIME = item["TIME"];

		newMovie->next = head;
		head = newMovie;
	}
	return head;
}

void addMovie(Movie* head, string& filename) {
	cinemaCity();
	Movie* newMovie = new Movie;
	cin.ignore();
	
	cout << "Enter the movie name: ";
	while (true) {
		getline(cin, newMovie->TITLE);
		if (!newMovie->TITLE.empty()) {
			break;
		}
		else {
			cout << "Movie name cannot be empty ";
		}
	}
	//Add movie genre
	cout << "How many genres there are: ";
	int genres;
	while (true) {
		cin >> genres;
		if (cin.fail() || genres < 1) {
			// Remove the newline, leaving the buffer clean for subsequent 'getline()' operations
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid try again ";
			cin >> genres;
		}
		else {
			break;
		}
	}
	cin.ignore();
	for (int i = 0; i < genres; i++) {
		string genre;
		cout << "Genre " << i + 1 << ": " << endl;
		while (true) {
			getline(cin, genre);
			if (!genre.empty()) {
				break;
			}
			else {
				cout << "Genre cannot be empty" << endl;
			}
		}
		newMovie->GENRE.push_back(genre);
	}

	//Add duration
	cout << "Enter duration (minutes): " << endl;
	while (true) {
		cin >> newMovie->DURATION;
		if (cin.fail() || newMovie->DURATION > 240 || newMovie->DURATION < 10) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input!";
			cin >> newMovie->DURATION;
		}
		else {
			break;
		}
	}

	//Story here
	//Story here
	//Story here
	//Story here

	//Add movie category
	string category;
	cout << "Enter the movie category: ";
	while (true) {
		getline(cin, newMovie->CATEGORY);
		if (category == "B" || category == "D" || category == "C+" || category == "D+") {
			newMovie->CATEGORY = category;
			break;
		}
		else {
			cout << "Try again: ";
		}
	}

	//Add language
	string language;
	cout << "Enter language(English or Bulgarian): ";
	while (true) {
		getline(cin, language);
		if (language == "English" || language == "Bulgarian") {
			newMovie->LANGUAGE = language;
			break;
		}
		else {
			cout << "Try again: ";
		}
	}

	// Add 3 actors
	cout << "How many actors there are: ";
	int actors;
	while (true) {
		cin >> actors;
		if (cin.fail() || actors < 1) {
			// Remove the newline, leaving the buffer clean for subsequent 'getline()' operations
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid try again ";
			cin >> actors;
		}
		else {
			break;
		}
	}
	cin.ignore();
	for (int i = 0; i < actors; i++) {
		string actor;
		cout << "Actor " << i + 1 << ": " << endl;
		while (true) {
			getline(cin, actor);
			if (!actor.empty()) {
				break;
			}
			else {
				cout << "Actors cannot be empty" << endl;
			}
		}
		newMovie->ACTORS.push_back(actor);
	}

	// Add director
	cout << "Enter director: ";
	while (true) {
		getline(cin, newMovie->DIRECTOR);
		if (!newMovie->DIRECTOR.empty()) {
			break;
		}
		else {
			cout << "Director cannot be ampty" << endl;
		}
	}

	//Where here 
	//Where here
	//Where here 
	

	//Date here
	//Date here
	//Date here
	
	
	string room;
	cout << "Enter the movie room: ";
	while (true) {
		getline(cin, room);
		if (room == "2D" || room == "3D" || room == "4DX 2D") {
			newMovie->ROOM = room;
			break;
		}
		else {
			cout << "Try again: ";
		}
	}

	cout << "Successful added!" << endl;
	// Change the pointer of the new riddle to point to the current first element
	newMovie->next = head;
	head = newMovie;
	// Save the riddle into the json file
	//saveRiddlesToFile(head, filename);
}