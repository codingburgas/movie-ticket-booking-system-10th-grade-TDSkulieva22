#include "pch.h"
#include "Admin.h"
#include "Offers.h"

extern SQLHENV hEnv;
extern SQLHDBC hDbc;

class DatabaseManager
{


public:
	bool connect();
	void disconnect();
	bool executeNonQuery(const wstring& query);
	bool executeQuery(const wstring& query, int col);
	bool getAllActiveOffers(std::vector<OfferDetails>& offers);
	bool updateOffer(int offerId, const wstring& newTitle, const wstring& newDescription, bool newIsActive);
};