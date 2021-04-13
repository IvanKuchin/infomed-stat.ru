#ifndef __UTILITIES__H__
#define __UTILITIES__H__

#include <curl/curl.h>
#include <fstream>  	//--- used for CopyFile function
#include <map>
#include <vector>
#include <list>
#include <unordered_set>
#include <algorithm>
#include <regex>
#include <cctype> 		//--- used for tolower
#include <sys/stat.h>
#include <sys/types.h>	// --- DIR
#include <dirent.h>		// --- opendir, rmdir
#include <execinfo.h>   // --- backtrace defined here
#include <signal.h>     // --- signal interception
#include <openssl/sha.h>
#include <Magick++.h>
#include <codecvt>		// --- codecvt_utf8

#include "c_date_spelling.h"
#include "c_smsc.h"
#include "cmysql.h"
#include "cfiles.h"
#include "cuser.h"
#include "cexception.h"
#include "clog.h"
#include "localy.h"

using namespace std;

auto			crash_handler(int sig)														-> void;
auto			GetSHA512(const string &src)												-> string;
auto			wide_to_multibyte(std::wstring const& s)									-> string;
auto			multibyte_to_wide(std::string const& s)										-> wstring;
auto      		rtrim(string& str)															-> string;
auto      		ltrim(string& str)															-> string;
auto      		trim(string& str)															-> string;
auto      		quoted(string src)															-> string;
auto			quoted(const vector<string> &)												-> vector<string>;
auto  			toUpper(const string &)														-> string;
auto  			toLower(const string &)														-> string;
auto      		GetRandom(int len)															-> string;
auto      		DeleteHTML(string src, bool removeBR = true)								-> string;
auto      		RemoveQuotas(string src)													-> string;
auto      		RemoveSpecialSymbols(wstring src)											-> wstring;
auto      		RemoveSpecialSymbols(string src)											-> string;
auto 			RemoveSpecialHTMLSymbols(const wstring &src)								-> wstring;
auto      		RemoveSpecialHTMLSymbols(const string &src)									-> string;
auto      		ReplaceDoubleQuoteToQuote(string src)										-> string;
auto      		ReplaceCRtoHTML(string src)													-> string;
auto			ReplaceWstringAccordingToMap(const wstring &src, const map<wstring, wstring> &replacements)	-> wstring;
auto      		CleanUPText(const string messageBody, bool removeBR = true)					-> string;
auto      		RemoveAllNonAlphabetSymbols(const wstring &src)								-> wstring;
auto      		RemoveAllNonAlphabetSymbols(const string &src)								-> string;
auto			ConvertTextToHTML(const string &messageBody)								-> string;
auto			ConvertHTMLToText(const string &src)										-> string;
auto 			ConvertHTMLToText(const wstring &src)										-> wstring;
auto 			CheckHTTPParam_Text(const string &srcText)									-> string;
auto			CheckHTTPParam_Number(const string &srcText)								-> string;
auto	 		CheckHTTPParam_Date(string srcText)											-> string;
auto	 		CheckHTTPParam_Float(const string &srcText)									-> string;
auto			CheckHTTPParam_Email(const string &srcText)									-> string;
auto      		GetSecondsSinceY2k()														-> double;
auto      		GetLocalFormattedTimestamp()												-> string;
auto      		GetTimeDifferenceFromNow(const string timeAgo)								-> double;
auto      		GetMinutesDeclension(const int value)										-> string;
auto      		GetHoursDeclension(const int value)											-> string;
auto      		GetDaysDeclension(const int value)											-> string;
auto      		GetMonthsDeclension(const int value)										-> string;
auto      		GetYearsDeclension(const int value)											-> string;
auto      		GetHumanReadableTimeDifferenceFromNow (const string timeAgo)				-> string;
auto      		SymbolReplace(const string where, const string src, const string dst)		-> string;
auto      		SymbolReplace_KeepDigitsOnly(const string &where)							-> string;
auto         	qw(const string src, vector<string> &dst)									-> int;
auto			join(const vector<string>& vec, string separator = ",")						-> string;
auto			split(const string& s, const char& c)										-> vector<string>;
auto      		UniqueUserIDInUserIDLine(string userIDLine)									-> string; //-> decltype(static_cast<string>("123")
auto      		AutodetectSexByName(string name, CMysql *)									-> string;
auto			GetPasswordNounsList(CMysql *)												-> string;
auto			GetPasswordAdjectivesList(CMysql *)											-> string;
auto			GetPasswordCharacteristicsList(CMysql *)									-> string;
auto			isAllowed_Guest_Action(string action, c_config *)							-> bool;


auto			GetZipInJSONFormat(string zip_id, CMysql *, CUser *)						-> string;
auto 			GetBankInJSONFormat(string sqlQuery, CMysql *, CUser *)						-> string;
auto      		GetGeoCountryListInJSONFormat(string dbQuery, CMysql *, CUser *)			-> string;
auto			GetGeoLocalityIDByCityAndRegion(string regionName, string cityName, CMysql *) -> string;
auto			GetCompanyPositionsInJSONFormat(string dbQuery, CMysql *, CUser *)			-> string;
auto			AllowMessageInNewsFeed(CUser *me, const string messageOwnerID, const string messageAccessRights, vector<string> *messageFriendList) -> bool;
auto			isPersistenceRateLimited(string REMOTE_ADDR, CMysql *)						-> bool;
auto			isFilenameImage(const string &filename)										-> bool;
auto			isFilenameVideo(const string &filename)										-> bool;
auto			CopyFile(const string src, const string dst)								-> void;
auto			GetCompanyDuplicates(CMysql *)												-> string;
auto			GetPicturesWithEmptySet(CMysql *)											-> string;
auto			GetPicturesWithUnknownMessage(CMysql *)										-> string;
auto			GetPicturesWithUnknownUser(CMysql *)										-> string;
auto			GetRecommendationAdverse(CMysql *)											-> string;
auto			GetUserAvatarByUserID(string userID, CMysql *)								-> string;
auto			RemoveMessageImages(string sqlWhereStatement, CMysql *)						-> void;
auto			RemoveBookCover(string sqlWhereStatement, c_config *, CMysql *)				-> void;
auto			GetUserTagsFromText(string srcMessage)										-> vector<string>;
auto    		RemoveSpecifiedCover(string itemID, string itemType, c_config *, CMysql *)	-> bool;
auto        	CheckUserEmailExisting(string userNameToCheck, CMysql *)					-> bool;
auto			RedirStdoutToFile(string fname)												-> bool;
auto			RedirStderrToFile(string fname)												-> bool;
auto 			isBotIP(string ip)															-> bool;
auto			isAdverseWordsHere(string text, CMysql *)									-> bool;
auto			stod_noexcept(const string &) noexcept 										-> double;
auto			MaskSymbols(string src, int first_pos, int last_pos)						-> string;
auto 			CutTrailingZeroes(string number)											-> string;
auto			GetSiteThemesInJSONFormat(string sqlQuery, CMysql *, CUser *)				-> string;

auto			GetMessageOwner(string messageID, CUser *, CMysql *)						-> pair<string, string>;
auto 			AmIMessageOwner(string messageID, CUser *, CMysql *) 						-> bool;
auto			GetUserSubscriptionsInJSONFormat(string sqlQuery, CMysql *) 				-> string;
auto			SubscribeToCompany(string companyID, CUser *, CMysql *) 					-> string;
auto			UnsubscribeFromCompany(string companyID, CUser *, CMysql *) 				-> string;
auto			SubscribeToGroup(string groupID, CUser *, CMysql *) 						-> string;
auto			UnsubscribeFromGroup(string groupID, CUser *, CMysql *) 					-> string;

// --- check file/image from cfile-handler
auto			CheckFileFromHandler(string f_name, string f_type, CFiles *files, string file_extension, c_config *)	-> string;
auto			SaveFileFromHandler(string f_name, string f_type, CFiles *files, string file_extension, c_config *)	-> string;

// --- file system functions
auto			CleanupFilename(string filename) -> string;

// --- string counters
auto			GetNumberOfCntrls (const wstring &src) -> unsigned int;
auto			GetNumberOfCntrls (const  string &src) -> unsigned int;
auto			GetNumberOfSpaces (const wstring &src) -> unsigned int;
auto			GetNumberOfSpaces (const  string &src) -> unsigned int;
auto			GetNumberOfDigits (const wstring &src) -> unsigned int;
auto			GetNumberOfDigits (const  string &src) -> unsigned int;
auto			GetNumberOfLetters(const wstring &src) -> unsigned int;
auto			GetNumberOfLetters(const  string &src) -> unsigned int;

// --- SMS functions
auto			SendPhoneConfirmationCode(const string &country_code, const string &phone_number, const string &session, c_config * const config_dir, CMysql * const db, CUser * const user) -> string;
auto			CheckPhoneConfirmationCode(const string &confirmation_code, const string &session, CMysql *, CUser *) -> vector<pair<string, string>>;
auto			RemovePhoneConfirmationCodes(string sessid, CMysql *) -> string;

// --- DB functions
auto			GetValueFromDB(string sql, CMysql *) -> string;
auto			GetValuesFromDB(string sql, CMysql *) -> vector<string>;

// --- login functions
auto			GetCountryCodeAndPhoneNumberBySMSCode(const string &confirmation_code, const string &session, CMysql *) -> tuple<string, string, string>;

// --- file system functions
auto			CreateDir(const string &dir) -> bool;
auto 			RmDirRecursive(const char *dirname) -> bool;
auto        	isDirExists(const std::string& name) -> bool;
auto        	isFileExists(const std::string& name) -> bool;
auto			isFilenameImage(const string &filename) -> bool;
auto			isFilenameVideo(const string &filename) -> bool;
auto			GetFileExtension(const string &filename) -> string;
auto 			getFileSize(const std::string& name) -> off_t;
auto			CleanupFilename(string	filename) -> string;
auto        	CopyFile(const string src, const string dst) -> void;

// --- helpdesk
auto      		GetHelpdeskBaseUserInfoInJSONFormat(string dbQuery, CMysql *, CUser *) -> string;
auto			GetHelpDeskTicketsInJSONFormat(string sqlQuery, CMysql *, CUser *) -> string;
auto			GetHelpDeskTicketHistoryInJSONFormat(string sqlQuery, CMysql *, CUser *) -> string;
auto			GetHelpDeskTicketAttachInJSONFormat(string sqlQuery, CMysql *, CUser *) -> string;
auto			isHelpdeskTicketOwner(string ticket_id, string user_id, CMysql *, CUser *) -> bool;

// --- FAQ
auto			GetFAQInJSONFormat(string sqlQuery, CMysql *, CUser *) -> string;

// --- date functions
auto			GetTMObject(string date)										-> struct tm;
auto 			GetFirstAndLastDateOfThisMonth()								-> pair<struct tm, struct tm>;
auto 			GetFirstAndLastDateOfLastMonth()								-> pair<struct tm, struct tm>;
auto 			GetFirstAndLastDateOfThisWeek()									-> pair<struct tm, struct tm>;
auto 			GetFirstAndLastDateOfLastWeek()									-> pair<struct tm, struct tm>;
auto			GetFirstAndLastMonthDaysByDate(const struct tm &_date)			-> pair<struct tm, struct tm>;
auto			GetSpellingDate(long int seconds_since_epoch)					-> string;
auto			GetSpellingFormattedDate(string date, string format)			-> string;
auto			GetSpellingFormattedDate(struct tm, string format)				-> string;
auto			operator <(const struct tm &tm_1, const struct tm &tm_2)		-> bool;
auto			operator <=(const struct tm &tm_1, const struct tm &tm_2)		-> bool;
auto			operator >(const struct tm &tm_1, const struct tm &tm_2)		-> bool;
auto			operator >=(const struct tm &tm_1, const struct tm &tm_2)		-> bool;
auto			operator ==(const struct tm &tm_1, const struct tm &tm_2)		-> bool;
auto			PrintDate(const struct tm &_tm)									-> string;
auto			PrintSQLDate(const struct tm &_tm)								-> string;
auto			PrintDateTime(const struct tm &_tm)								-> string;
auto			PrintTime(const struct tm &_tm, string format)					-> string;
auto			DateInPast(string date_to_check)								-> bool;

// --- function set for image upload/removal
auto			isItemAllowedToChange(string itemID, string itemType, c_config *, CMysql *, CUser *) -> string;
// auto 		GetSpecificData_GetNumberOfFolders(string itemType) -> int;
// auto 		GetSpecificData_GetMaxFileSize(string itemType) -> int;
// auto		 	GetSpecificData_GetMaxWidth(string itemType) -> unsigned int;
// auto		 	GetSpecificData_GetMaxHeight(string itemType) -> unsigned int;
// auto 		GetSpecificData_GetBaseDirectory(string itemType) -> string;
// auto			GetSpecificData_GetFinalFileExtension(string itemType) -> string;
// auto 		GetSpecificData_SelectQueryItemByID(string itemID, string itemType) -> string;
// auto 		GetSpecificData_UpdateQueryItemByID(string itemID, string itemType, string folderID, string fileName) -> string;
// auto 		GetSpecificData_GetDBCoverPhotoFolderString(string itemType) -> string;
// auto 		GetSpecificData_GetDBCoverPhotoFilenameString(string itemType) -> string;
// auto 		GetSpecificData_GetDataTypeByItemType(const string &itemType) -> string;
// auto 		GetSpecificData_AllowedToChange(string itemID, string itemType, CMysql *, CUser *) -> string;



// --- UTF8 encoding/decoding
auto         	convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)	-> int;
auto			utf8_to_cp1251(const string &)												-> string;
auto 			convert_cp1251_to_utf8(const char *in, char *out, int size)					-> bool;

// --- base64 encoding/decoding
static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
typedef unsigned char BYTE;

auto 			is_base64(BYTE c)										-> bool;
auto 			base64_encode(BYTE const* buf, unsigned int bufLen) 	-> string;
auto 			base64_decode(std::string const& encoded_string) 		-> string;

struct ExifInfo 
{
	string  DateTime;
	string  GPSAltitude;
	string  GPSLatitude;
	string  GPSLongitude;
	string  GPSSpeed;
	string  Model;
	string  Authors;
	string  ApertureValue;
	string  BrightnessValue;
	string  ColorSpace;
	string  ComponentsConfiguration;
	string  Compression;
	string  DateTimeDigitized;
	string  DateTimeOriginal;
	string  ExifImageLength;
	string  ExifImageWidth;
	string  ExifOffset;
	string  ExifVersion;
	string  ExposureBiasValue;
	string  ExposureMode;
	string  ExposureProgram;
	string  ExposureTime;
	string  Flash;
	string  FlashPixVersion;
	string  FNumber;
	string  FocalLength;
	string  FocalLengthIn35mmFilm;
	string  GPSDateStamp;
	string  GPSDestBearing;
	string  GPSDestBearingRef;
	string  GPSImgDirection;
	string  GPSImgDirectionRef;
	string  GPSInfo;
	string  GPSTimeStamp;
	string  ISOSpeedRatings;
	string  JPEGInterchangeFormat;
	string  JPEGInterchangeFormatLength;
	string  Make;
	string  MeteringMode;
	string  Orientation;
	string  ResolutionUnit;
	string  SceneCaptureType;
	string  SceneType;
	string  SensingMethod;
	string  ShutterSpeedValue;
	string  Software;
	string  SubjectArea;
	string  SubSecTimeDigitized;
	string  SubSecTimeOriginal;
	string  WhiteBalance;
	string  XResolution;
	string  YCbCrPositioning;
	string  YResolution;
};

auto		ParseGPSLongitude(const string longitudeStr)	-> string;
auto		ParseGPSLatitude(const string latitudeStr)		-> string;
auto		ParseGPSAltitude(const string altitudeStr)		-> string;
auto		ParseGPSSpeed(const string speedStr)			-> string;

auto		GetDomain()										-> string;
auto		isDemoDomain()									-> bool;

#endif
