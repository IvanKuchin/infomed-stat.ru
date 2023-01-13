#include "utilities_infomed1.h"

auto GetUserAAARole(CUser *user, CMysql *db) -> string
{
	return GetUserAAARole(user->GetID(), db);
}

auto GetUserAAARole(string user_id, CMysql *db) -> string
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(db->Query("SELECT `role` FROM `users_aaa` WHERE `user_id`=" + quoted(user_id) + ";"))
	{
		result = db->Get(0, 0);
	}
	else
	{
		MESSAGE_DEBUG("", "", "user role is default");
	}

	MESSAGE_DEBUG("", "", "result (" + result + ")");

	return result;
}

auto GetUserVisibilityScope(CUser *user, CMysql *db) -> string
{
	return GetUserVisibilityScope(user->GetID(), db);
}

auto GetUserVisibilityScope(string user_id, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	result = GetValueFromDB("SELECT `scope` FROM `users_visibility_scope` WHERE `user_id`=" + quoted(user_id) + ";", db);

	if(result.empty())
	{
		MESSAGE_ERROR("", "", "this field can't be empty, the value must be set during entry creation");
		result = "local";
	}

	MESSAGE_DEBUG("", "", "result (" + result + ")");

	return result;
}

auto UpdateUserVisibilityScope(const string &user_id, const string &scope, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	error_message = ""s;

	db->Query("UPDATE `users_visibility_scope` SET `scope`=" + quoted(scope) + " WHERE `user_id`=" + quoted(user_id) + ";");
	if(db->isError())
	{
		error_message = gettext("SQL syntax error");
		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "result (" + error_message + ")");

	return error_message;
}

auto GetMedicalRecordsSQLFilter_BasedOnVisibility(const string &user_id, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	filter = ""s;
	auto	visibility_scope = GetUserVisibilityScope(user_id, db);

	if(visibility_scope == "global")
	{
		filter = "1";
	}
	else if(visibility_scope == "local")
	{
		filter = " `company_id` IN (" + Get_CompanyIDByUserID_sqlquery(user_id) + ") ";
	}
	else
	{
		MESSAGE_ERROR("", "", "unknown visibility scope (" + visibility_scope + ")");
	}

	MESSAGE_DEBUG("", "", "finish (" + filter + ")");

	return filter;
}

// --- Returns user list in JSON format grabbed from DB
// --- Input: dbQuery - SQL format returns users
//			db	  - DB connection
//			user	- current user object
string GetUserListInJSONFormat(string dbQuery, CMysql *db, CUser *user)
{
	MESSAGE_DEBUG("", "", "start");

	ostringstream					 ost;
	string							 sessid, lookForKey;
	unordered_set<unsigned long>	 setOfUserID;

	struct	ItemClass
	{
		string	userID;
		string	login;
		string	userName;
		string	userNameLast;
		string	userNameMiddle;
		string	first_name_en;
		string	last_name_en;
		string	middle_name_en;
		string	country_code;
		string	phone;
		string	email;
		string	email_changeable;
		string	isblocked;
		string	userType;
		string	userSex;
		string	userBirthday;
		string	userBirthdayAccess;
		string	userAppliedVacanciesRender;
		string	userCurrentCityID;
		string	passport_series;
		string	passport_number;
		string	passport_issue_date;
		string	passport_issue_authority;
		string	foreign_passport_number;
		string	foreign_passport_expiration_date;
		string	citizenship_code;
		string	site_theme_id;
		string	userLastOnline;
		string	userLastOnlineSecondSinceY2k;
		string	helpdesk_subscription_S1_sms;
		string	helpdesk_subscription_S2_sms;
		string	helpdesk_subscription_S3_sms;
		string	helpdesk_subscription_S4_sms;
		string	helpdesk_subscription_S1_email;
		string	helpdesk_subscription_S2_email;
		string	helpdesk_subscription_S3_email;
		string	helpdesk_subscription_S4_email;
	};
	vector<ItemClass>		itemsList;
	auto					itemsCount = 0;
	auto					user_role = (user ? GetUserAAARole(user, db) : ""s);

	if((itemsCount = db->Query(dbQuery)) > 0)
	{
		for(auto i = 0; i < itemsCount; ++i)
		{
			ItemClass	item;
			item.userID								= db->Get(i, "id");
			item.login								= db->Get(i, "login");
			item.userName							= db->Get(i, "name");
			item.userNameLast						= db->Get(i, "nameLast");
			item.userNameMiddle						= db->Get(i, "nameMiddle");
			item.country_code						= db->Get(i, "country_code");
			item.phone								= db->Get(i, "phone");
			item.email								= db->Get(i, "email");
			item.email_changeable					= db->Get(i, "email_changeable");
			item.isblocked							= db->Get(i, "isblocked");
			item.userSex							= db->Get(i, "sex");
			item.userType							= db->Get(i, "type");
			item.userBirthday						= db->Get(i, "birthday");
			item.userBirthdayAccess					= db->Get(i, "birthdayAccess");
			item.userAppliedVacanciesRender			= db->Get(i, "appliedVacanciesRender");
			item.userCurrentCityID					= db->Get(i, "geo_locality_id");
			item.site_theme_id						= db->Get(i, "site_theme_id");
			item.passport_series					= db->Get(i, "passport_series");
			item.passport_number					= db->Get(i, "passport_number");
			item.passport_issue_date				= db->Get(i, "passport_issue_date");
			item.passport_issue_authority			= db->Get(i, "passport_issue_authority");
			item.citizenship_code					= db->Get(i, "citizenship_code");
			item.first_name_en						= db->Get(i, "first_name_en");
			item.last_name_en						= db->Get(i, "last_name_en");
			item.middle_name_en						= db->Get(i, "middle_name_en");
			item.foreign_passport_number			= db->Get(i, "foreign_passport_number");
			item.foreign_passport_expiration_date	= db->Get(i, "foreign_passport_expiration_date");
			item.userLastOnline						= db->Get(i, "last_online");
			item.userLastOnlineSecondSinceY2k		= db->Get(i, "last_onlineSecondsSinceY2k");
			item.site_theme_id						= db->Get(i, "site_theme_id");
			item.helpdesk_subscription_S1_email		= db->Get(i, "helpdesk_subscription_S1_email");
			item.helpdesk_subscription_S2_email		= db->Get(i, "helpdesk_subscription_S2_email");
			item.helpdesk_subscription_S3_email		= db->Get(i, "helpdesk_subscription_S3_email");
			item.helpdesk_subscription_S4_email		= db->Get(i, "helpdesk_subscription_S4_email");
			item.helpdesk_subscription_S1_sms		= db->Get(i, "helpdesk_subscription_S1_sms");
			item.helpdesk_subscription_S2_sms		= db->Get(i, "helpdesk_subscription_S2_sms");
			item.helpdesk_subscription_S3_sms		= db->Get(i, "helpdesk_subscription_S3_sms");
			item.helpdesk_subscription_S4_sms		= db->Get(i, "helpdesk_subscription_S4_sms");

			itemsList.push_back(item);
		}


		ost.str("");
		for(int i = 0; i < itemsCount; i++)
		{
			// --- if user_list have duplicates(1, 2, 1), avoid duplications
			if(setOfUserID.find(stol(itemsList[i].userID)) == setOfUserID.end())
			{
				auto				userID					= itemsList[i].userID;
				auto				userBirthday			= itemsList[i].userBirthday;
				auto				userCurrentCityID		= itemsList[i].userCurrentCityID;
				auto				userCurrentCity			= ""s;
				auto				avatarPath				= ""s;
				auto				userLastOnline			= itemsList[i].userLastOnline;
				auto				userLastOnlineSecondSinceY2k = itemsList[i].userLastOnlineSecondSinceY2k;
				auto				isMe					= user && (userID == user->GetID());

				ostringstream		ost1;

				setOfUserID.insert(stol(userID));

				// --- Get user avatars
				avatarPath = "empty";
				if(db->Query("select * from `users_avatars` where `userid`=" + quoted(userID) + " and `isActive`=\"1\";"))
				{
					avatarPath = "/images/avatars/avatars" + db->Get(0, "folder") + "/" + db->Get(0, "filename");
				}

				if(userCurrentCityID.length() && db->Query("SELECT `title` FROM `geo_locality` WHERE `id`=\"" + userCurrentCityID + "\";"))
					userCurrentCity = db->Get(0, "title");

				if(ost.str().length()) ost << ", ";

				if(isMe)
				{
					// --- user have to be able to see his own bday
				}
				else
				{
					if(itemsList[i].userBirthdayAccess == "private") userBirthday = "";
				}

				ost << "{"
						"\"id\": \""								<< itemsList[i].userID << "\", "
						"\"name_first\": \""						<< itemsList[i].userName << "\", "
						"\"name_last\": \""							<< itemsList[i].userNameLast << "\","
						"\"name_middle\": \""						<< itemsList[i].userNameMiddle << "\","
						"\"userSex\": \""							<< itemsList[i].userSex << "\","
						"\"userType\": \""							<< itemsList[i].userType << "\","
						"\"birthday\": \""							<< userBirthday << "\","
						"\"birthdayAccess\": \""					<< itemsList[i].userBirthdayAccess << "\","
						"\"last_online\": \""						<< itemsList[i].userLastOnline << "\","
						"\"last_online_diff\": \""					<< to_string(GetTimeDifferenceFromNow(userLastOnline)) << "\","
						"\"last_onlineSecondsSinceY2k\": \""		<< userLastOnlineSecondSinceY2k << "\","
						"\"avatar\": \""							<< avatarPath << "\","
						"\"site_theme_id\": \""						<< itemsList[i].site_theme_id << "\","
						"\"themes\": ["								<< GetSiteThemesInJSONFormat("SELECT * FROM `site_themes`", db, user) << "],"
						// "\"numberUnreadMessages\": \""			<< numberUreadMessages << "\", "
						// "\"appliedVacanciesRender\": \""			<< userAppliedVacanciesRender << "\","
						// "\"userFriendship\": \""					<< userFriendship << "\","
						// "\"currentCity\": \""					<< userCurrentCity << "\", "
						// "\"currentEmployment\": "				<< userCurrentEmployment << ", "
						// "\"languages\": ["		 				<< GetLanguageListInJSONFormat("SELECT * FROM `language` WHERE `id` in (SELECT `language_id` FROM `users_language` WHERE `user_id`=\"" + userID + "\");", db) << "], "
						// "\"skills\": ["		 					<< GetSkillListInJSONFormat("SELECT * FROM `skill` WHERE `id` in (SELECT `skill_id` FROM `users_skill` WHERE `user_id`=\"" + userID + "\");", db) << "], "
						// "\"subscriptions\":[" 					<< (user && (user->GetID() == userID) ? GetUserSubscriptionsInJSONFormat("SELECT * FROM `users_subscriptions` WHERE `user_id`=\"" + userID + "\";", db) : "") << "],"
						"\"isblocked\": \""							<< ((isMe || (user_role == "admin")) ? itemsList[i].isblocked : "") << "\","
						"\"login\": \""								<< ((isMe || (user_role == "admin")) ? itemsList[i].login : "") << "\","
						"\"country_code\": \""						<< ((isMe || (user_role == "admin")) ? itemsList[i].country_code : "") << "\","
						"\"phone\": \""								<< ((isMe || (user_role == "admin")) ? itemsList[i].phone : "") << "\","
						"\"email\": \""								<< ((isMe || (user_role == "admin")) ? itemsList[i].email : "") << "\","
						"\"email_changeable\": \""					<< ((isMe || (user_role == "admin")) ? itemsList[i].email_changeable : "") << "\","
						"\"helpdesk_subscriptions_sms\": ["			<< (isMe ? quoted(itemsList[i].helpdesk_subscription_S1_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S2_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S3_sms) + "," + quoted(itemsList[i].helpdesk_subscription_S4_sms)  : "") << "],"
						"\"helpdesk_subscriptions_email\": ["		<< (isMe ? quoted(itemsList[i].helpdesk_subscription_S1_email) + "," + quoted(itemsList[i].helpdesk_subscription_S2_email) + "," + quoted(itemsList[i].helpdesk_subscription_S3_email) + "," + quoted(itemsList[i].helpdesk_subscription_S4_email)  : "") << "],"
						"\"aaa\":\""								<< ((isMe || (user_role == "admin")) ? GetUserAAARole(itemsList[i].userID, db) : "") << "\","
						"\"visibility_scope\":\""					<< ((isMe || (user_role == "admin")) ? GetUserVisibilityScope(itemsList[i].userID, db) : "") << "\","
						"\"isMe\": \""								<< ((isMe) ? "yes" : "no") << "\""
						"}";
			} // --- if user is not dupicated
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are users returned by request [" + dbQuery + "]");
	}

	MESSAGE_DEBUG("", "", "finish [length" + to_string(ost.str().length()) + "]");

	return ost.str();
}

// --- Function returns list of users "liked" messageID in JSON-format
// --- input: messageID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetMessageLikesUsersList(string messageID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='like' and `messageID`='" + messageID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from users where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" usersBookID in JSON-format
// --- input: usersBookID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetBookLikesUsersList(string usersBookID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeBook' and `messageID`='" + usersBookID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" usersLanguageID in JSON-format
// --- input: usersLanguageID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetLanguageLikesUsersList(string usersLanguageID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeLanguage' and `messageID`='" + usersLanguageID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" usersCompanyID in JSON-format
// --- input: usersCompanyID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetCompanyLikesUsersList(string usersCompanyID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeCompany' and `messageID`='" + usersCompanyID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" usersCertificationID in JSON-format
// --- input: usersCertificationID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetCertificationLikesUsersList(string usersCertificationID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeCertification' and `messageID`='" + usersCertificationID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" usersCourseID in JSON-format
// --- input: usersCourseID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetCourseLikesUsersList(string usersCourseID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeCourse' and `messageID`='" + usersCourseID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" universityDegreeID in JSON-format
// --- input: universityDegreeID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetUniversityDegreeLikesUsersList(string universityDegreeID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `feed_message_params` where `parameter`='likeUniversityDegree' and `messageID`='" + universityDegreeID + "';");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from `users` where id in (";
		for(int i = 0; i < affected; i++)
		{
			if(i > 0) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns list of users "liked" bookID in JSON-format
// --- input: bookID, user, db
//			user - used for friendship status definition
// --- output: was this message "liked" by particular user or not
string GetBookRatingUsersList(string bookID, CUser *user, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `users_books` where `bookID`=\"" + bookID + "\";");
	if(affected > 0)
	{
		ost.str("");
		ost << "select * from users where id in (";
		for(int i = 0; i < affected; ++i)
		{
			if(i) ost << ",";
			ost << db->Get(i, "userID");
		}
		ost << ") and `isactivated`='Y' and `isblocked`='N';";
		result = GetUserListInJSONFormat(ost.str(), db, user);
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

string GetCompanyPositionIDByTitle(string positionTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			positionID = "";

	MESSAGE_DEBUG("", "", "start");

	if(positionTitle.length())
	{
		if(db->Query("SELECT `id` FROM `company_position` WHERE `title`=\"" + positionTitle + "\";"))
		{
			positionID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "company position not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `company_position` SET `area`=\"\", `title`=\"" + positionTitle + "\";");
			if(tmp)
				positionID = to_string(tmp);
			else
			{
				MESSAGE_ERROR("", "", "fail to insert to company_position table");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "positionTitle is empty");
	}

	result = positionID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

string GetLanguageIDByTitle(string languageTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			languageID = "0";

	MESSAGE_DEBUG("", "", "start");

	if(languageTitle.length())
	{
		if(db->Query("SELECT `id` FROM `language` WHERE `title`=\"" + languageTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "languageTitle [" + languageTitle + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `language` SET `title`=\"" + languageTitle + "\";");
			if(tmp)
				languageID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into language");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "languageTitle is empty");
	}

	result = languageID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");


	return result;
}

string GetSkillIDByTitle(string skillTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			languageID = "0";

	MESSAGE_DEBUG("", "", "start");

	if(skillTitle.length())
	{
		if(db->Query("SELECT `id` FROM `skill` WHERE `title`=\"" + skillTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;

			MESSAGE_DEBUG("", "", "skillTitle [" + skillTitle + "] not found. Creating new one.");

			tmp = db->InsertQuery("INSERT INTO `skill` SET `title`=\"" + skillTitle + "\";");
			if(tmp)
				languageID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into skill");
			}
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "skillTitle is empty");
	}

	result = languageID;

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

// --- Function returns array of book rating
// --- input: bookID, db
// --- output: book rating array
string GetBookRatingList(string bookID, CMysql *db)
{
	int			 affected;
	string		  result = "";

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("select * from `users_books` where `bookID`=\"" + bookID + "\";");
	if(affected > 0)
	{
		for(int i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

// --- Function returns array of course rating
// --- input: courseID, db
// --- output: course rating array
string GetCourseRatingList(string courseID, CMysql *db)
{
	int				affected;
	string			result = "";

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("select * from `users_courses` where `track_id`=\"" + courseID + "\";");
	if(affected > 0)
	{
		for(int i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");

	return result;
}

string GetMessageCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"message\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetCompanyCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"company\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetLanguageCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"language\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetBookCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"book\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetCertificateCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type` in (\"certification\", \"course\") and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetUniversityDegreeCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"university\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

string GetMessageSpam(string messageID, CMysql *db)
{
	ostringstream	ost;
	int				affected;
	string			result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and messageID='" << messageID << "';";
	affected = db->Query(ost.str());


	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	MESSAGE_DEBUG("", "", "finish");


	return result;
}

// --- Function returns true or false depends on userID "spamed" it or not
// --- input: messageID, userID
// --- output: was this message "spamed" by particular user or not
string GetMessageSpamUser(string messageID, string userID, CMysql *db)
{
	ostringstream	ost;
	int				affected;
	string			result = "0";

	MESSAGE_DEBUG("", "", "start");

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and `messageID`='" << messageID << "' and `userID`='" << userID << "' ;";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}


	MESSAGE_DEBUG("", "", "finish");


	return result;
}

