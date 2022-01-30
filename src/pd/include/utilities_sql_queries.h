#ifndef __UTILITIES_SQL_QUERIES__H__
#define __UTILITIES_SQL_QUERIES__H__

#include <string>

inline auto Get_CompanyIDByUserID_sqlquery(const string &user_id) -> string
{
	return ("SELECT `company_id` FROM `company_employment` WHERE `user_id` IN (" + user_id + ") ");
}

#endif
