#pragma once

#include "pch.h"
#include "Admin.h"
#include "Offers.h"
#include "Booking.h"
#include "Seat.h"

extern SQLHENV hEnv;
extern SQLHDBC hDbc;

struct MovieDetails {
	wstring title;
	wstring genre;
	int duration = 0;
	wstring story;
	wstring category;
	wstring languages;
	wstring actor;
	wstring director;
};

struct ShowtimeDetails {
	wstring date;
	wstring time;
	wstring hall;
};

class DatabaseManager
{


public:
	bool connect();
	void disconnect();
	bool executeNonQuery(const wstring& query);
	bool executeQuery(const wstring& query, int col);
	bool getAllActiveOffers(vector<OfferDetails>& offers);
	bool updateOffer(int offerId, const wstring& newTitle, const wstring& newDescription, bool newIsActive);
	vector<wstring> getMovieTitles(const wstring& sqlQuery);
	bool getMovieDetails(const wstring& movieTitle, MovieDetails& details);
	vector<ShowtimeDetails> getShowtimes(const wstring& programTableName, const wstring& movieTitle, const wstring& cinema, const wstring& date);
	vector<Seat> getSeatMap(int programId);
	bool reserveSeats(int programId, const vector<Seat>& seats, int userId);
	int getProgramId(const wstring& tableName, const wstring& movieTitle, const wstring& location, const wstring& date);
	vector<wstring> getMovieHall(const wstring& programTableName, const wstring& movieTitle,const wstring& date);
	int getLoggedInUserId(wstring username);
};