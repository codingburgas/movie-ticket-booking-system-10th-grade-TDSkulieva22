#include "pch.h"
#include "Movies.h"
#include "Validation.h"
#include "Menu.h"
#include "Colors.h"
#include "DrawText.h"
#include "Filters.h"

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



void saveMoviesToFile(Movie* head, const string& filename) {
	ofstream file(filename);
	if (!file) {
		cout << "Failed to open file: " << filename << endl;
		return;
	}

	json moviesJson = json::array();
	Movie* current = head;

	while (current) {
		// Add each riddle as a JSON object to the array
		moviesJson.push_back({
			{"TITLE", current->TITLE},
			{"GENRE", current->GENRE},
			{"DURATION", current->DURATION},
			{"STORY", current->STORY},
			{"CATEGORY", current->CATEGORY},
			{"LANGUAGE", current->LANGUAGE},
			{"ACTORS", current->ACTORS},
			{"DIRECTOR", current->DIRECTOR},
			{"WHERE", current->WHERE},
			//{"DATE", current->DATE},
			{"ROOM", current->ROOM},
			//{"TIME", current->TIME}
			});
		current = current->next;
	}

	// Write the JSON data to the file, indented by 4 spaces for better formatting
	file << moviesJson.dump(4);
	file.close();
	cout << "The new movie was added";
}


void addMovie(Movie*& head, const string& filename) {
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
		getline(cin, category);

		if (category == "B" || category == "D" || category == "C+" || category == "D+") {
			newMovie->CATEGORY = category;
			break;
		}
		else {
			cout << "Try again (valid options: B, D, C+, D+): ";
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

	newMovie->next = head;
	head = newMovie;
	
	saveMoviesToFile(head, filename);
}

void editMovie(Movie* head, string& filename) {
	cinemaCity();

	if (head == nullptr) {
		cout << "Empty list!" << endl;
	}

	cout << "Pick film to edit:" << endl;
	int count = 1;
	Movie* current = head;
	vector<Movie*> moviesList;
	// Traverse the list to display available films
	while (current != nullptr) {
		cout << count << ". " << current->TITLE << endl;
		moviesList.push_back(current);
		current = current->next;
		count++;
	}

	int choice;
	cout << "Enter the number of the film: ";
	cin >> choice;

	if (choice < 1 || choice > moviesList.size()) {
		cout << "Incorrect choice!" << endl;
	}
	// Get the film to edit
	Movie* toEdit = moviesList[choice - 1];

	cout << "Edit the movie details" << endl;

	//Edit the film title
	cout << "Current title: " << toEdit->TITLE << endl;
	cout << "Enter new title: ";
	string newTitle;
	cin.ignore();
	getline(cin, newTitle);
	if (!newTitle.empty()) {
		toEdit->TITLE = newTitle;
	}

	//Genre
	//Genre
	//Genre

	//Edit the film duration
	cout << "Current duration: " << toEdit->DURATION << endl;
	cout << "Enter new duration: ";
	int newDuration;
	cin.ignore();
	cin >> newDuration;
	//if (!newDuration.empty()) {
		toEdit->DURATION = newDuration;
	//}

	//Story
    //Story
	//Story

	//Edit the film category
    cout << "Current category: " << toEdit->CATEGORY << endl;
	cout << "Enter new category: ";
	string newCategory;
	cin.ignore();
	getline(cin, newCategory);
	if (!newCategory.empty()) {
		toEdit->CATEGORY = newCategory;
	}

	//Edit the film language
	cout << "Current language: " << toEdit->LANGUAGE << endl;
	cout << "Enter new language: ";
	string newLanguage;
	cin.ignore();
	getline(cin, newLanguage);
	if (!newLanguage.empty()) {
		toEdit->LANGUAGE = newLanguage;
	}

	// Edit the actors
	cout << "Current actor: " << endl;
	for (size_t i = 0; i < toEdit->ACTORS.size(); ++i) {
		cout << i + 1 << ". " << toEdit->ACTORS[i] << endl;
	}

	cout << "Enter the number of the actor you want to edit: ";
	int actorChoice;
	cin >> actorChoice;
	cin.ignore();

	// Edit the selected actor
	if (actorChoice > 0 && actorChoice <= toEdit->ACTORS.size()) {
		cout << "Enter new actor: ";
		string newActor;
		getline(cin, newActor);
		// Replace the old actor name with the new one
		toEdit->ACTORS[actorChoice - 1] = newActor;
	}

	//Edit the director name
	cout << "Current director: " << toEdit->DIRECTOR << endl;
	cout << "Enter new director: ";
	string newDirector;
	cin.ignore();
	getline(cin, newDirector);
	if (!newDirector.empty()) {
		toEdit->DIRECTOR = newDirector;
	}

	//WHERE here
	//WHERE here
	//WHERE here

	//DATE here
	//Date here
	//Date here

	cout << "Current room: " << toEdit->ROOM << endl;
	cout << "Enter new room: ";
	string newRoom;
	cin.ignore();
	getline(cin, newRoom);
	if (!newRoom.empty()) {
		toEdit->ROOM = newRoom;
	}

	//TIME here
	//TIME here
	//TIME here

}