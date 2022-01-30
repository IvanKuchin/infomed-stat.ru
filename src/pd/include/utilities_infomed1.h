#ifndef __UTILITIES_TIMECARD__H__
#define __UTILITIES_TIMECARD__H__

#include "utilities.h"
#include "c_float.h"
#include "c_cache_obj.h"
#include "utilities_sql_queries.h"
// --- IMPORTANT !!!
// --- do NOT add any header file with potentially recursive includes


// --- notification types
#define NOTIFICATION_GENERAL_FROM_USER                          98
#define NOTIFICATION_GENERAL_FROM_COMPANY                       99
#define NOTIFICATION_ADD_TASK_TO_SOW                            100
#define NOTIFICATION_CHANGE_CUSTOMER_NAME                       101
#define NOTIFICATION_CHANGE_PROJECT_NAME                        102
#define NOTIFICATION_CHANGE_TASK_NAME                           103
#define NOTIFICATION_CHANGE_TASK_DURATION                       104
#define NOTIFICATION_DELETE_TASK                                105
#define NOTIFICATION_AGENCY_INITIATED_SOW                       106
#define NOTIFICATION_SUBCONTRACTOR_SIGNED_SOW                   107
#define NOTIFICATION_AGENCY_ABOUT_SUBC_REGISTRATION             108
#define NOTIFICATION_AGENCY_GENERATED_AGREEMENTS                109
#define NOTIFICATION_AGENCY_ABOUT_APPROVER_REGISTRATION 		110
#define NOTIFICATION_AGENCY_ABOUT_EMPLOYEE_REGISTRATION 		111
#define NOTIFICATION_SUBC_BUILT_ABSENCE_REQUEST                 112
#define NOTIFICATION_SUBC_CHANGED_ABSENCE_REQUEST               113


auto      		GetChatMessagesInJSONFormat(string dbQuery, CMysql *) -> string;
auto      		GetUnreadChatMessagesInJSONFormat(CUser *, CMysql *) -> string;
auto      		GetMessageImageList(string imageSetID, CMysql *) -> string;
auto      		GetMessageLikesUsersList(string messageID, CUser *, CMysql *) -> string;
auto			GetUserAAARole(string user_id, CMysql *) -> string;
auto			GetUserAAARole(CUser *, CMysql *) -> string;

auto			GetUserVisibilityScope(string user_id, CMysql *) -> string;
auto			GetUserVisibilityScope(CUser *, CMysql *) -> string;
auto 			UpdateUserVisibilityScope(const string &user_id, const string &scope, CMysql *) -> string;
auto 			GetMedicalRecordsSQLFilter_BasedOnVisibility(const string &user_id, CMysql *) -> string;

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


auto			CouldCompanyBeDeleted(string id, CMysql *, CUser *) -> string;
auto			CouldCompanyTitleBeChanged(string company_id, string new_title, CMysql *, CUser *) -> string;

auto			AddAccessDataRight(string company_id, string company_id_data_available, CMysql *) -> string;
auto			RemoveAccessDataRight(string company_id, CMysql *) -> string;

auto			GetCompanyID(CUser *, CMysql *) -> string;
auto			IsPatientExists(string first_name, string middle_name, string last_name, string birthdate, string patient_id, CMysql *) -> string;

auto			GetMedicalRecordsInJSONFormat(string sql_query, CMysql *, CUser *) -> string;

auto			IsUserAllowedToChangePatientRecord(string patient_id, CUser *, CMysql *) -> bool;

#endif
