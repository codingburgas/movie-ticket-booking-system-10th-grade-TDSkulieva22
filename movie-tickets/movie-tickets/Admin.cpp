#include "Admin.h"

bool adminLogin(string username,string password) {
	 string Admin_User = "admin";
	 string Admin_Pass = "admin";

	return (username == Admin_User && password == Admin_Pass);

}
