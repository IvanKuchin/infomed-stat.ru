#ifndef __UTILITIES_TIMECARD__H__
#define __UTILITIES_TIMECARD__H__

#include "utilities.h"
#include "c_float.h"
#include "c_cache_obj.h"
// --- IMPORTANT !!!
// --- do NOT add any header file with potentially recursive includes


auto      		GetChatMessagesInJSONFormat(string dbQuery, CMysql *) -> string;
auto      		GetUnreadChatMessagesInJSONFormat(CUser *, CMysql *) -> string;
auto      		GetMessageImageList(string imageSetID, CMysql *) -> string;
auto      		GetMessageLikesUsersList(string messageID, CUser *, CMysql *) -> string;
auto			GetUserAAARole(string user_id, CMysql *db) -> string;
auto			GetUserAAARole(CUser *user, CMysql *db) -> string;
auto      		GetUserListInJSONFormat(string dbQuery, CMysql *, CUser *) -> string;
auto 			GetBookLikesUsersList(string usersBookID, CUser *, CMysql *) -> string;
auto 			GetLanguageLikesUsersList(string usersLanguageID, CUser *, CMysql *) -> string;
auto 			GetCompanyLikesUsersList(string usersCompanyID, CUser *, CMysql *) -> string;
auto 			GetCertificationLikesUsersList(string usersCertificationID, CUser *, CMysql *) -> string;
auto 			GetCourseLikesUsersList(string usersCourseID, CUser *, CMysql *) -> string;
auto 			GetUniversityDegreeLikesUsersList(string messageID, CUser *, CMysql *) -> string;
auto 			GetBookRatingUsersList(string bookID, CUser *, CMysql *) -> string;

auto			GetCompanyPositionIDByTitle(string title, CMysql *) -> string;
auto			GetLanguageIDByTitle(string title, CMysql *) -> string;
auto			GetSkillIDByTitle(string title, CMysql *) -> string;
auto 			GetBookRatingList(string bookID, CMysql *) -> string;
auto 			GetCourseRatingList(string courseID, CMysql *) -> string;
auto 	     	GetMessageCommentsCount(string messageID, CMysql *) -> string;
auto 			GetCompanyCommentsCount(string messageID, CMysql *) -> string;
auto 			GetLanguageCommentsCount(string messageID, CMysql *) -> string;
auto 			GetBookCommentsCount(string messageID, CMysql *) -> string;
auto 			GetCertificateCommentsCount(string messageID, CMysql *) -> string;
auto 			GetUniversityDegreeCommentsCount(string messageID, CMysql *) -> string;
auto    	  	GetMessageSpam(string messageID, CMysql *) -> string;
auto    	  	GetMessageSpamUser(string messageID, string userID, CMysql *) -> string;


// --- timecard functions
auto	 		CheckHTTPParam_Timeentry(const string &srcText) -> string;
vector<string> 	SplitTimeentry(const string& s, const char c = ',');

auto 			GetNewsFeedInJSONFormat(string whereStatement, int currPage, int newsOnSinglePage, CUser *, CMysql *) -> string;
auto      		GetCompanyListInJSONFormat(string dbQuery, CMysql *, CUser *, bool quickSearch = true, bool includeEmployedUsersList = false) -> string;
auto      		GetUserNotificationSpecificDataByType(unsigned long typeID, unsigned long actionID, CMysql *, CUser *) -> string;
auto      		GetUserNotificationInJSONFormat(string sqlRequest, CMysql *, CUser *) -> string;


auto			CouldCompanyBeDeleted(string id, CMysql *, CUser *user) -> string;
auto			CouldCompanyTitleBeChanged(string company_id, string new_title, CMysql *, CUser *user) -> string;

auto			AddAccessDataRight(string company_id, string company_id_data_available, CMysql *) -> string;
auto			RemoveAccessDataRight(string company_id, CMysql *) -> string;

auto			GetCompanyID(CUser *, CMysql *) -> string;
auto			IsPatientExists(string first_name, string middle_name, string last_name, string birthdate, string patient_id, CMysql *) -> string;

auto			GetMedicalRecordsInJSONFormat(string sql_query, CMysql *, CUser *) -> string;

auto			IsUserAllowedToChangePatientRecord(string patient_id, CUser *user, CMysql *db) -> bool;

#endif
