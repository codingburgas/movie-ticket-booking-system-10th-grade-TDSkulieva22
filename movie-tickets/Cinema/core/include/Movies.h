#pragma once
#include "pch.h"

//Structure that represents a movie
struct Movie {
	string TITLE;
	vector<string> GENRE;
	int DURATION;
	vector<string> STORY;
	string CATEGORY;
	string LANGUAGE;
	vector<string> ACTORS;
	string DIRECTOR;
	string WHERE;
	//vector<string> DATE;
	string ROOM;
	//ector<string> TIME;

	// Pointer to the next riddle
	Movie* next;
};
//Movie* loadMoviesFromFile(string& filename);
//void saveMoviesToFile(Movie* head, string& filename);


