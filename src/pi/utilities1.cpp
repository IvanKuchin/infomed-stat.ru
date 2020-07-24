#include "utilities.h"

void crash_handler(int sig)
{
	void	   *array[10];
	size_t		nptrs;
	char	   **strings;

	// get void*'s for all entries on the stack
	nptrs = backtrace(array, 10);

	// print out all the frames to stderr
	fprintf(stderr, "Error: signal %d:\n", sig);
	backtrace_symbols_fd(array, nptrs, STDERR_FILENO);


	// --- try to print out to the CLog
	strings = backtrace_symbols(array, nptrs);
	if(strings)
	{
		for(unsigned int i = 0; i < nptrs; i++)
		{
			MESSAGE_ERROR("", "", string(strings[i]));
		}

		free(strings);
	}

	exit(1);
}

auto GetLocale()
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start");

	result = setlocale(LC_ALL, "");

	if(result.empty()) MESSAGE_ERROR("", "", "fail to define locale");

	MESSAGE_DEBUG("", "", "finish(result length is " + to_string(result.length()) + ")");

	return result;
}

std::string rtrim(std::string& str)
{
	str.erase(str.find_last_not_of(' ')+1);		 //suffixing spaces
	return str;
}

std::string ltrim(std::string& str)
{
	str.erase(0, str.find_first_not_of(' '));	   //prefixing spaces
	return str;
}

std::string trim(std::string& str)
{
	rtrim(str);
	ltrim(str);
	return str;
}

string	quoted(string src)
{
	return '"' + src + '"';
}

auto toUpper(const string &src) -> string
{
	auto	result(""s);
	auto	locale_str = GetLocale();

	MESSAGE_DEBUG("", "", "start");

	if(locale_str.length())
	{
		auto	wtemp = multibyte_to_wide(src);
		locale	loc(locale_str.c_str());

		for(auto &c: wtemp) c = toupper(c, loc);

		result = wide_to_multibyte(wtemp);
	}
	else
	{
		MESSAGE_ERROR("", "", "fail to define locale. Impossible run through toLower")
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto toLower(const string &src) -> string
{
	auto	result(""s);
	auto	locale_str = GetLocale();

	MESSAGE_DEBUG("", "", "start");

	if(locale_str.length())
	{
		auto	wtemp = multibyte_to_wide(src);
		locale	loc(locale_str.c_str());

		for(auto &c: wtemp) c = tolower(c, loc);

		result = wide_to_multibyte(wtemp);
	}
	else
	{
		MESSAGE_ERROR("", "", "fail to define locale. Impossible run through toLower")
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

string	GetPasswordNounsList(CMysql *db)
{
	string 	result = "";
	int		affected;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_nouns`;");
	if(affected)
	{
		int		total_number_of_words = stoi(db->Get(0, "total")) - 1;

		affected = db->Query("SELECT * FROM `password_dictionary_nouns` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(int i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_nouns is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string	GetPasswordCharacteristicsList(CMysql *db)
{
	string 	result = "";
	int		affected;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_characteristics`;");
	if(affected)
	{
		int		total_number_of_words = stoi(db->Get(0, "total"));

		affected = db->Query("SELECT * FROM `password_dictionary_characteristics` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(int i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_characteristics is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string	GetPasswordAdjectivesList(CMysql *db)
{
	string 	result = "";
	int		affected;

	MESSAGE_DEBUG("", "", "start");

	affected = db->Query("SELECT COUNT(*) as `total` FROM `password_dictionary_adjectives`;");
	if(affected)
	{
		int		total_number_of_words = stoi(db->Get(0, "total"));

		affected = db->Query("SELECT * FROM `password_dictionary_adjectives` WHERE `id` in (round(rand()*" + to_string(total_number_of_words) + ") + 1, round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1,round(rand()*" + to_string(total_number_of_words) + ") + 1)");
		for(int i = 0; i < affected; ++i)
		{
			if(result.length()) result += ",";
			result += string("\"") + db->Get(i, "word") + "\"";
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "password_dictionary_adjectives is empty");
	}

	MESSAGE_DEBUG("", "", "finish (result length is " + to_string(result.length()) + ")");

	return result;
}

string GetRandom(int len)
{
	string	result;
	int	i;

	for(i = 0; i < len; i++)
	{
		result += (char)('0' + (int)(rand()/(RAND_MAX + 1.0) * 10));
	}

	return result;
}

string DeleteHTML(string src, bool removeBR /* = true*/)
{
	auto			  	result = src;
	string::size_type   firstPos, lastPos;

	MESSAGE_DEBUG("", "", "start (src.len = " + to_string(src.length()) + ")");

	firstPos = result.find("<");
	if(firstPos != string::npos)
	{
		lastPos = result.find(">", firstPos);
		if(lastPos == string::npos) lastPos = result.length();

		while(firstPos != string::npos)
		{
			if(removeBR)
				result.erase(firstPos, lastPos - firstPos + 1);
			else
			{
				string  htmlTag;

				// --- this will run in case "keep BR"
				htmlTag = result.substr(firstPos + 1, lastPos-firstPos-1);
				transform(htmlTag.begin(), htmlTag.end(),htmlTag.begin(), ::tolower);

				if(htmlTag != "br")
				{
					result.erase(firstPos, lastPos - firstPos + 1);
				}
				else
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "(" + src + ")" + "[" + to_string(__LINE__) + "]: keep <br> at pos: " + to_string(firstPos));
					}
				}
			}

			firstPos = result.find("<", firstPos + 1);
			if(firstPos == string::npos) break;
			lastPos = result.find(">", firstPos);
			if(lastPos == string::npos) lastPos = result.length();
		}
	} // --- if "<" found in srcStr

	MESSAGE_DEBUG("", "", "finish(result.len = " + to_string(result.length()) + ")");

	return result;
}

/*
	Delete symbol " from string src
*/
string RemoveQuotas(string src)
{
	auto				result = src;
	string::size_type	pos = 0;

	MESSAGE_DEBUG("", "", "start (src = " + src + ")");

	while((pos = result.find("\"", pos)) != string::npos)
	{
		result.replace(pos, 1, "\\\"");
		pos += 2;
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

/*
	Delete special symbols like \t \\ \<
*/
auto RemoveSpecialSymbols(wstring src) -> wstring
{
	auto					result = src;
	map<wstring, wstring>	map_replacement_1 = {
		{L"\\", L""},
		{L"\t", L" "}
	};


	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");

	result = ReplaceWstringAccordingToMap(result, map_replacement_1);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto RemoveSpecialSymbols(string src) -> string
{
	return(wide_to_multibyte(RemoveSpecialSymbols(multibyte_to_wide(src))));
}

/*
	Delete special symbols like \t \\ \<
*/
auto RemoveSpecialHTMLSymbols(const wstring &src) -> wstring
{
	auto					result(src);
	wstring::size_type		pos = 0;
	map<wstring, wstring>	map_replacement = {
		{L"\\", L"&#92;"},
		{L"\t", L" "},
		{L"<", L"&lt;"},
		{L">", L"&gt;"},
		{L"\"", L"&quot;"}
	};

	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");

	result = ReplaceWstringAccordingToMap(result, map_replacement);

	MESSAGE_DEBUG("", "", "finish (result.length = " + to_string(result.length()) + ")");

	return result;
}

auto RemoveSpecialHTMLSymbols(const string &src) -> string
{
	return(wide_to_multibyte(RemoveSpecialHTMLSymbols(multibyte_to_wide(src))));
}

/*
	Change " symbol to " from string src
*/
string ReplaceDoubleQuoteToQuote(string src)
{
	auto				result = src;
	string::size_type	pos = 0;

	while((pos = result.find("\"", pos)) != string::npos)
	{
		result.replace(pos, 1, "'");
		// pos += 2;
	}

	return result;
}


/*
	Change CR/CRLF symbol to <BR> from string src
*/
auto ReplaceCRtoHTML(wstring src) -> wstring
{

	auto					result = src;
	map<wstring, wstring>	map_replacement_1 = {
		{L"\r\n", L"<br>"}
	};
	map<wstring, wstring>	map_replacement_2 = {
		{L"\n", L"<bR>"},
		{L"\r", L"<Br>"}
	};


	MESSAGE_DEBUG("", "", "start");

	result = ReplaceWstringAccordingToMap(result, map_replacement_1);
	result = ReplaceWstringAccordingToMap(result, map_replacement_2);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto ReplaceCRtoHTML(string src) -> string
{
	return(wide_to_multibyte(ReplaceCRtoHTML(multibyte_to_wide(src))));
}

string CleanUPText(const string messageBody, bool removeBR/* = true*/)
{
	string	  result = messageBody;

	MESSAGE_DEBUG("", "", "start");

	result = DeleteHTML(result, removeBR);
	result = ReplaceDoubleQuoteToQuote(result);
	result = ReplaceCRtoHTML(result);
	result = RemoveSpecialSymbols(result);
	trim(result);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

/*
	Delete any special symbols
	ATTENTION !!!
	use it carefully
	symbol(N) replaced to ""
	Used only for matching duplicates
*/
auto RemoveAllNonAlphabetSymbols(const wstring &src) -> wstring
{
	auto					result(src);
	map<wstring, wstring>	map_replacement_1 = {
		{L"&lt;", L""},
		{L"&gt;", L""},
		{L"&quot;", L""},
		{L"&#92;", L""},
	};
	map<wstring, wstring>	map_replacement_2 = {
		{L" ", L""},
		{L"\\", L""},
		{L"/", L""},
		{L"\t", L""},
		{L"<", L""},
		{L">", L""},
		{L"№", L""},
		{L"—", L""},
		{L"\"", L""},
		{L"'", L""},
		{L";", L""},
		{L":", L""},
		{L"`", L""},
		{L".", L""},
		{L",", L""},
		{L"%", L""},
		{L"-", L""},
		{L"N", L""},
	};


	MESSAGE_DEBUG("", "", "start");

	while ((result.length()) && (result.at(result.length() - 1) == L'\\')) result.replace(result.length() - 1, 1, L"");
	result = ReplaceWstringAccordingToMap(result, map_replacement_1);
	result = ReplaceWstringAccordingToMap(result, map_replacement_2);

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

auto RemoveAllNonAlphabetSymbols(const string &src) -> string
{
	return(wide_to_multibyte(RemoveAllNonAlphabetSymbols(multibyte_to_wide(src))));
}

string ConvertTextToHTML(const string &messageBody)
{
	string 		result = messageBody;

	MESSAGE_DEBUG("", "", "start");

	result = RemoveSpecialHTMLSymbols(result);
	result = DeleteHTML(result);
	result = ReplaceCRtoHTML(result);
	trim(result);

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return result;
}

auto CheckHTTPParam_Text(const string &srcText) -> string
{
	return	ConvertTextToHTML(srcText);
}

auto CheckHTTPParam_Number(const string &srcText) -> string
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	if(srcText.length())
	{
		smatch	m;
		regex	r("[^0-9]");

		if(regex_search(srcText, m, r))
		{
			MESSAGE_ERROR("", "", "can't convert(" + srcText + ") to number");
		}
		else
		{
			result = srcText;
		}
	}

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

string CheckHTTPParam_Float(const string &srcText)
{
	auto	result = ""s;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	if(srcText.length())
	{
		regex	r("^[0-9]+(\\.[0-9]+)?$");

		if(regex_match(srcText, r))
		{
			result = srcText;
		}
		else
		{
			MESSAGE_ERROR("", "", "can't convert(" + srcText + ") to float");
		}
	}

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

string CheckHTTPParam_Date(string srcText)
{
	string	result = "";

	{
		MESSAGE_DEBUG("", "", "start param(" + srcText + ")");
	}

	if(srcText.length())
	{
		regex	r("^([[:digit:]]{1,2})\\/([[:digit:]]{1,2})\\/([[:digit:]]{2,4})$");
		smatch	sm;

		trim(srcText);

		if(regex_match(srcText, sm, r))
		{
			auto	date	= stoi(sm[1]);
			auto	month	= stoi(sm[2]);
			auto	year	= stoi(sm[3]);

			if(year < 100) year += 2000;

			if((1 <= date) && (date <= 31))
			{
				if((1 <= month) && (month <= 12))
				{
					if((1900 <= year) && (year <= 2100))
					{
						auto tm_obj = GetTMObject(srcText);

						mktime(&tm_obj);

						if((date == tm_obj.tm_mday) && (month == (tm_obj.tm_mon + 1)) && (year == (tm_obj.tm_year + 1900)))
						{
							result = srcText;
						}
						else
						{
							MESSAGE_ERROR("", "", "wrong date (" + srcText + " -> " + to_string(date) + "/" + to_string(month) + "/" + to_string(year) + " vs " + to_string(tm_obj.tm_mday) + "/" + to_string(tm_obj.tm_mon + 1) + "/" + to_string(tm_obj.tm_year + 1900) + ")");
						}
					}
					else
					{
						MESSAGE_ERROR("", "", "year (" + to_string(year) + ") is out of range");
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "month (" + to_string(month) + ") is out of range");
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "date (" + to_string(date) + ") is out of range");
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "(" + srcText + ") doesn't match date regex");
		}
	}

	{
		MESSAGE_DEBUG("", "", "finish (result length = " + to_string(result.length()) + ")");
	}

	return	result;
}

string CheckHTTPParam_Email(const string &srcText)
{
	string		result = "";

    regex       positionRegex(".*([+-][[:digit:]]+\\.[[:digit:]]+)([+-][[:digit:]]+\\.[[:digit:]]+)([+-][[:digit:]]+\\.[[:digit:]]+).*");
    smatch      matchResult;

	MESSAGE_DEBUG("", "", "start param(" + srcText + ")");

	result = ConvertTextToHTML(srcText);

	if(regex_match(srcText, regex("^[._[:alnum:]]+@[[:alnum:]][\\-.[:alnum:]]*[[:alnum:]]\\.[[:alnum:]]{2,5}$") ))
    {
    	if(result.length() > 128)
    	{
    		result = "";
    		MESSAGE_DEBUG("", "", "e-mail too long >128 symbols");
    	}
    	else
    	{
	    	// --- regex matched
    	}
    }
    else
    {
		MESSAGE_DEBUG("", "", "email doesn't match regex " + result);

    	result = "";
    }

	MESSAGE_DEBUG("", "", "finish ( result length = " + to_string(result.length()) + ")");

	return	result;
}

string	GetDefaultActionFromUserType(CUser *user, CMysql *db)
{
	string	result = GUEST_USER_DEFAULT_ACTION;

	MESSAGE_DEBUG("", "", "start");

	if(user->GetType() == "guest") result = GUEST_USER_DEFAULT_ACTION;
	else if(user->GetType() == "user") result = LOGGEDIN_DOCTOR_DEFAULT_ACTION;
	else
	{
		MESSAGE_ERROR("", "", "unknown user type (" + user->GetType() + ")");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

double GetSecondsSinceY2k()
{
	time_t	  timer;
	// struct tm   y2k = {0};
	double		seconds;
	double		secondsY2kUTC = 946684800;

	// y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	// y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	timer = time(NULL);  /* get current time; same as: timer = time(NULL)  */
	// seconds = difftime(timer,mktime(&y2k));
	seconds = timer - secondsY2kUTC;

	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetSecondsSinceY2k(): end (seconds since Y2k" << std::to_string(seconds) << ")";
		log.Write(DEBUG, ost.str());
	}

	return seconds;
}

string GetLocalFormattedTimestamp()
{
	time_t	  now_t;
	struct tm   *local_tm;
	char		buffer[80];
	string		result = "";

	now_t = time(NULL);
	local_tm = localtime(&now_t);
	if(local_tm == NULL)
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetLocalFormatedTimestamp(): ERROR in running localtime(&t)";
		log.Write(ERROR, ost.str());
	}

	memset(buffer, 0, 80);
	strftime(buffer,80,"%Y-%m-%02d %T", local_tm);
	result = buffer;


	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetLocalFormatedTimestamp(): end (" << result << ")";
		log.Write(DEBUG, ost.str());
	}

	return result;

}

// --- input format: "2015-06-10 00:00:00"
// --- return: number of second difference from now
double GetTimeDifferenceFromNow(const string timeAgo)
{
	time_t	  now_t, checked_t;
	// char		utc_str[100];
	struct tm   *local_tm, check_tm;
	ostringstream	ost;
/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): start";
		log.Write(DEBUG, ost.str());
	}
*/

	now_t = time(NULL);
	local_tm = localtime(&now_t);
	if(local_tm == NULL)
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(now): ERROR in running localtime(&t)";
		log.Write(ERROR, ost.str());
	}
/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(now): now_t = " << now_t;
		log.Write(DEBUG, ost.str());
	}
*/

	// now2_t = time(NULL);
	// check_tm = localtime(&now2_t);
	sscanf(timeAgo.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &check_tm.tm_year, &check_tm.tm_mon, &check_tm.tm_mday, &check_tm.tm_hour, &check_tm.tm_min, &check_tm.tm_sec);
	check_tm.tm_year -= 1900;
	check_tm.tm_mon -= 1;
	check_tm.tm_isdst = 0;	// --- "Summer time" is OFF. Be carefull with it.
							// --- Russia is not using Daylight saving
							// --- USA is

	// --- For testing purposes try to use the same Daylight saving as in local clock
	// --- Test starts on 9/11. If there is no side effect, comment-out previous line.
	if(local_tm) check_tm.tm_isdst = local_tm->tm_isdst;	// --- "Summer time" is the same as locak clock.

/*
	{
		CLog	log;
		ostringstream	ost;

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): checked year = " << check_tm.tm_year << " checked month = " << check_tm.tm_mon << " checked day = " << check_tm.tm_mday << " checked hour = " << check_tm.tm_hour << " checked min = " << check_tm.tm_min << " checked sec = " << check_tm.tm_sec;
		log.Write(DEBUG, ost.str());
	}
*/
	checked_t = mktime(&check_tm);

	{
		CLog	log;
		ostringstream	ost;
		char	buffer[80];

		ost.str("");
		strftime(buffer,80,"check_tm: date regenerated: %02d-%b-%Y %T %Z  %I:%M%p.", &check_tm);
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): " << buffer << "";
		log.Write(DEBUG, ost.str());

		memset(buffer, 0, 80);
		strftime(buffer,80,"local_tm: date regenerated: %02d-%b-%Y %T %Z  %I:%M%p.", local_tm);
		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): " << buffer << "";
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): difftime( now_t=" << now_t << ", checked_t=" << checked_t << ")";
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost << "GetTimeDifferenceFromNow(" << timeAgo << "): end (difference = " << difftime(now_t, checked_t) << ")";
		log.Write(DEBUG, ost.str());
	}

	return difftime(now_t, checked_t);
}

string GetMinutesDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "минуту"},
		{2, "минуты"},
		{3, "минут"}
	};

	string				result;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}


string GetHoursDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "час"},
		{2, "часа"},
		{3, "часов"}
	};
	string				result;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetDaysDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "день"},
		{2, "дня"},
		{3, "дней"}
	};
	string				result;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetMonthsDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "месяц"},
		{2, "месяца"},
		{3, "месяцев"}
	};
	string				result;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

string GetYearsDeclension(const int value)
{
	map<int, string> 	mapDeclension = {
		{1, "год"},
		{2, "года"},
		{3, "лет"}
	};
	string				result;

	if(value % 10 == 0) 						{ result = mapDeclension.at(3); };
	if(value % 10 == 1) 						{ result = mapDeclension.at(1); };
	if((value % 10 >= 2) and (value % 10 <= 4))	{ result = mapDeclension.at(2); };
	if((value % 10 >= 5) and (value % 10 <= 9))	{ result = mapDeclension.at(3); };
	if((value >= 5) and (value <= 20))			{ result = mapDeclension.at(3); };
	return result;
}

// --- input format: "2015-06-10 00:00:00"
// --- return: human readable format
string GetHumanReadableTimeDifferenceFromNow (const string timeAgo)
{
	double			seconds = GetTimeDifferenceFromNow(timeAgo);
	double			minutes = seconds / 60;
	double			hours = minutes / 60;
	double			days = hours / 24;
	double			months = days / 30;
	double			years = months / 12;
	ostringstream	ost;

	ost.str("");
	if(years >= 1)
	{
		ost << (int)years << " " << GetYearsDeclension(years);
	}
	else if(months >= 1)
	{
		ost << (int)months << " " << GetMonthsDeclension(months);
	}
	else if(days >= 1)
	{
		ost << (int)days << " " << GetDaysDeclension(days);
	}
	else if(hours >= 1)
	{
		ost << (int)hours << " " << GetHoursDeclension(hours);
	}
	else
	{
		ost << (int)minutes << " " << GetMinutesDeclension(minutes);
	}

	ost << " назад.";


	// --- commented to reduce logs flooding
	{
		CLog	log;
		ostringstream	ost1;

		ost1.str("");
		ost1 << "string GetHumanReadableTimeDifferenceFromNow (" << timeAgo << "): sec difference (" << seconds << ") human format (" << ost.str() << ")" ;
		log.Write(DEBUG, ost1.str());
	}


	return ost.str();
}

string SymbolReplace(const string where, const string src, const string dst)
{
	auto	  result(where);
	auto	  pos = result.find(src);

	while(pos != string::npos)
	{
		result.replace(pos, src.length(), dst);
		pos = result.find(src, pos + 1);
	}

	return result;
}

auto SymbolReplace_KeepDigitsOnly(const string &where) -> string
{
	auto	  	result = where;
	auto		i = 0u;

	while(i < result.length())
	{
		auto	currSymb = result.at(i);

		if((currSymb >= static_cast<char>('0')) && (currSymb <= static_cast<char>('9')))
			i++;
		else
			result.replace(i, 1, "");
	}

	return result;
}

bool CheckUserEmailExisting(string userNameToCheck, CMysql *db) {
	CUser		user;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	user.SetDB(db);
	user.SetLogin(userNameToCheck);
	user.SetEmail(userNameToCheck);

	if(user.isLoginExist() or user.isEmailDuplicate()) {
		{
			CLog	log;
			MESSAGE_DEBUG("", "", "login or email already registered");
		}
		return true;
	}
	else {
		{
			CLog	log;
			MESSAGE_DEBUG("", "", "login or email not yet exists");
		}
		return false;
	}
}

string UniqueUserIDInUserIDLine(string userIDLine) //-> decltype(static_cast<string>("123"))
{
	list<long int>	listUserID;
	string			result {""};
	std::size_t		prevPointer {0}, nextPointer;

	{
		MESSAGE_DEBUG("", "", "start (" + userIDLine + ")");
	}

	do
	{
		nextPointer = userIDLine.find(",", prevPointer);
		if(nextPointer == string::npos)
		{
			listUserID.push_back(atol(userIDLine.substr(prevPointer).c_str()));
		}
		else
		{
			listUserID.push_back(atol(userIDLine.substr(prevPointer, nextPointer - prevPointer).c_str()));
		}
		prevPointer = nextPointer + 1;
	} while( (nextPointer != string::npos) );

	listUserID.sort();
	listUserID.unique();
	for(auto it: listUserID)
	{
		result += (result.length() ? "," : "") + to_string(it);
	}

	{
		MESSAGE_DEBUG("", "", "finish (result " + result + ")");
	}

	return result;
}

string GetChatMessagesInJSONFormat(string dbQuery, CMysql *db)
{
	ostringstream	result, ost;
	int				affected;

	{
		MESSAGE_DEBUG("", "", "start");
	}
	result.str("");

	affected = db->Query(dbQuery);
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			result << (i ? "," : "") << "{\
				\"id\": \""						<< db->Get(i, "id") << "\", \
				\"message\": \"" 				<< ReplaceDoubleQuoteToQuote(db->Get(i, "message")) << "\", \
				\"fromType\": \"" 				<< db->Get(i, "fromType") << "\",\
				\"fromID\": \""					<< db->Get(i, "fromID") << "\",\
				\"toType\": \""			 		<< db->Get(i, "toType") << "\",\
				\"toID\": \""	 				<< db->Get(i, "toID") << "\",\
				\"messageStatus\": \""		  << db->Get(i, "messageStatus") << "\",\
				\"messageType\": \""			<< db->Get(i, "messageType") << "\",\
				\"eventTimestampDelta\": \""	<< GetHumanReadableTimeDifferenceFromNow(db->Get(i, "eventTimestamp")) << "\",\
				\"secondsSinceY2k\": \""		<< db->Get(i, "secondsSinceY2k") << "\",\
				\"eventTimestamp\": \""			<< db->Get(i, "eventTimestamp") << "\"\
			}";
		}
	}

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]: end"));
	}

	return  result.str();
}

bool	isFilenameImage(string filename)
{
	bool	result = false;
	regex   e1("[.](gif|jpg|jpeg|png)$", regex_constants::icase);
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "(" + filename + ")[" + to_string(__LINE__) + string("]: start" ));
	}

	result = regex_search(filename, e1);

	{
		MESSAGE_DEBUG("", "", "finish (result: " + (result ? "true" : "false") + ")");
	}
	return  result;
}

bool	isFilenameVideo(string filename)
{
	bool	result = false;
	regex   e1("[.](mov|avi|mp4|webm)$", regex_constants::icase);
	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "(" + filename + ")[" + to_string(__LINE__) + string("]: start" ));
	}

	result = regex_search(filename, e1);

	{
		MESSAGE_DEBUG("", "", "finish (result: " + (result ? "true" : "false") + ")");
	}
	return  result;
}

// --- extrasct all @[[:digit:]] patterns form srcMessage
vector<string> GetUserTagsFromText(string srcMessage)
{
	vector<string>  result;
	regex		   exp1("@([[:digit:]]+)");

	{
		MESSAGE_DEBUG("", "", "start");
	}

	regex_token_iterator<string::iterator>   rItr(srcMessage.begin(), srcMessage.end(), exp1, 1);
	regex_token_iterator<string::iterator>   rItrEnd;
	while(rItr != rItrEnd)
	{
		result.push_back(rItr->str());
		++rItr;
	}

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + string("]: end (result length: ") + to_string(result.size()) + string(")"));
	}

	return result;
}

// input: ....
//		  includeReaders will add readers counter
string GetBookListInJSONFormat(string dbQuery, CMysql *db, bool includeReaders/* = false*/)
{
	struct BookClass {
		string	id, title, authorID, authorName, isbn10, isbn13, photoCoverFolder, PhotoCoverFilename, readersUserID;
	};

	ostringstream				   ostResult;
	int							 booksCount;
	vector<BookClass>			   bookList;


	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	booksCount = db->Query(dbQuery);
	if(booksCount)
	{
		for(int i = 0; i < booksCount; i++)
		{
			BookClass   bookListItem;
			bookListItem.id				 = db->Get(i, "id");
			bookListItem.title			  = db->Get(i, "title");
			bookListItem.authorID		   = db->Get(i, "authorID");
			bookListItem.isbn10			 = db->Get(i, "isbn10");
			bookListItem.isbn13			 = db->Get(i, "isbn13");
			bookListItem.photoCoverFolder   = db->Get(i, "coverPhotoFolder");
			bookListItem.PhotoCoverFilename = db->Get(i, "coverPhotoFilename");
			bookListItem.readersUserID	  = "";

			bookList.push_back(bookListItem);
		}

		for(int i = 0; i < booksCount; i++)
		{
				if(db->Query("select * from `book_author` where `id`=\"" + bookList.at(i).authorID + "\";"))
					bookList.at(i).authorName = db->Get(0, "name");

				if(includeReaders)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT `userID` from `users_books` WHERE `bookID`=\"" + bookList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "userID");
					}

					bookList.at(i).readersUserID = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"bookID\": \""				 << bookList.at(i).id << "\", "
						  << "\"bookTitle\": \""			  << bookList.at(i).title << "\", "
						  << "\"bookAuthorID\": \""		   << bookList.at(i).authorID << "\","
						  << "\"bookAuthorName\": \""		 << bookList.at(i).authorName << "\","
						  << "\"bookISBN10\": \""			 << bookList.at(i).isbn10 << "\","
						  << "\"bookISBN13\": \""			 << bookList.at(i).isbn13 << "\","
						  << "\"bookPhotoCoverFolder\": \""   << bookList.at(i).photoCoverFolder << "\","
						  << "\"bookPhotoCoverFilename\": \"" << bookList.at(i).PhotoCoverFilename << "\","
						  << "\"bookReadersUserID\": ["	   << bookList.at(i).readersUserID << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no books returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

// input: ....
//		  includeReaders will add readers counter
string GetComplainListInJSONFormat(string dbQuery, CMysql *db, bool includeReaders/* = false*/)
{
	struct ComplainClass {
		string	  id;
		string	  userID;
		string	  entityID;
		string	  type;
		string	  subtype;
		string	  complainComment;
		string	  resolveComment;
		string	  state;
		string	  openEventTimestamp;
		string	  closeEventTimestamp;
	};

	ostringstream				   ostResult;
	int							 complainsCount;
	vector<ComplainClass>		   complainList;


	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	complainsCount = db->Query(dbQuery);
	if(complainsCount)
	{
		for(int i = 0; i < complainsCount; i++)
		{
			ComplainClass   complainListItem;
			complainListItem.id				 = db->Get(i, "id");
			complainListItem.userID			 = db->Get(i, "userID");
			complainListItem.entityID		   = db->Get(i, "entityID");
			complainListItem.type			   = db->Get(i, "type");
			complainListItem.subtype			= db->Get(i, "subtype");
			complainListItem.complainComment	= db->Get(i, "complainComment");
			complainListItem.resolveComment	 = db->Get(i, "resolveComment");
			complainListItem.state			  = db->Get(i, "state");
			complainListItem.openEventTimestamp = db->Get(i, "openEventTimestamp");
			complainListItem.closeEventTimestamp= db->Get(i, "closeEventTimestamp");

			complainList.push_back(complainListItem);
		}

		for(int i = 0; i < complainsCount; i++)
		{
				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"id\": \""			  << complainList.at(i).id << "\", "
						  << "\"userID\": \""		  << complainList.at(i).userID << "\", "
						  << "\"entityID\": \""		<< complainList.at(i).entityID << "\","
						  << "\"type\": \""			<< complainList.at(i).type << "\","
						  << "\"subtype\": \""		 << complainList.at(i).subtype << "\","
						  << "\"complainComment\": \"" << complainList.at(i).complainComment << "\","
						  << "\"resolveComment\": \""  << complainList.at(i).resolveComment << "\","
						  << "\"state\": \""		   << complainList.at(i).state << "\","
						  << "\"openEventTimestamp\": \""  << complainList.at(i).openEventTimestamp << "\","
						  << "\"closeEventTimestamp\": \"" << complainList.at(i).closeEventTimestamp << "\""
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no complains returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

// input: ....
//		  includeDevoted will add student counter
string GetCertificationListInJSONFormat(string dbQuery, CMysql *db, bool includeDevoted/* = false*/)
{
	struct ItemClass {
		string	  id, title, photoFolder, photoFilename, devotedUserList;
		string	  vendorID, vendorName;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");
			item.vendorID		   = db->Get(i, "vendor_id");
			item.vendorName		 = "";
			item.photoFolder		= db->Get(i, "logo_folder");
			item.photoFilename	  = db->Get(i, "logo_filename");
			item.devotedUserList	= "";

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{
				if(db->Query("SELECT `name` FROM `company` WHERE `id`=\"" + itemsList.at(i).vendorID + "\";"))
					itemsList.at(i).vendorName = db->Get(0, "name");

				if(includeDevoted)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT * from `users_certifications` WHERE `track_id`=\"" + itemsList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "user_id");
					}

					itemsList.at(i).devotedUserList = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"certificationID\": \""				 << itemsList.at(i).id << "\", "
						  << "\"certificationTitle\": \""			  << itemsList.at(i).title << "\", "
						  << "\"certificationVendorID\": \""		   << itemsList.at(i).vendorID << "\", "
						  << "\"certificationVendorName\": \""		 << itemsList.at(i).vendorName << "\", "
						  << "\"certificationPhotoCoverFolder\": \""   << itemsList.at(i).photoFolder << "\","
						  << "\"certificationPhotoCoverFilename\": \"" << itemsList.at(i).photoFilename << "\","
						  << "\"certificationReadersUserID\": ["	   << itemsList.at(i).devotedUserList << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no certifications returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

// input: ....
//		  includeStudents will add student counter
string GetCourseListInJSONFormat(string dbQuery, CMysql *db, bool includeStudents/* = false*/)
{
	struct ItemClass {
		string	  id, title, photoFolder, photoFilename, studentUserList;
		string	  vendorID, vendorName;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");
			item.vendorID		   = db->Get(i, "vendor_id");
			item.vendorName		 = "";
			item.photoFolder		= db->Get(i, "logo_folder");
			item.photoFilename	  = db->Get(i, "logo_filename");
			item.studentUserList	= "";

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{
				if(db->Query("SELECT `name` FROM `company` WHERE `id`=\"" + itemsList.at(i).vendorID + "\";"))
					itemsList.at(i).vendorName = db->Get(0, "name");

				if(includeStudents)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT * from `users_courses` WHERE `track_id`=\"" + itemsList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "user_id");
					}

					itemsList.at(i).studentUserList = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"courseID\": \""				 << itemsList.at(i).id << "\", "
						  << "\"courseTitle\": \""			  << itemsList.at(i).title << "\", "
						  << "\"courseVendorID\": \""		   << itemsList.at(i).vendorID << "\", "
						  << "\"courseVendorName\": \""		 << itemsList.at(i).vendorName << "\", "
						  << "\"coursePhotoCoverFolder\": \""   << itemsList.at(i).photoFolder << "\","
						  << "\"coursePhotoCoverFilename\": \"" << itemsList.at(i).photoFilename << "\","
						  << "\"courseStudentsUserID\": ["	   << itemsList.at(i).studentUserList << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no courses returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

// input: ....
//		  includeStudents will add student counter
string GetLanguageListInJSONFormat(string dbQuery, CMysql *db, bool includeStudents/* = false*/)
{
	struct ItemClass {
		string	  id, title, photoFolder, photoFilename, studentUserList;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");
			item.photoFolder		= db->Get(i, "logo_folder");
			item.photoFilename	  = db->Get(i, "logo_filename");
			item.studentUserList	= "";

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{

				if(includeStudents)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT * from `users_language` WHERE `language_id`=\"" + itemsList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "user_id");
					}

					itemsList.at(i).studentUserList = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"languageID\": \""				 << itemsList.at(i).id << "\", "
						  << "\"languageTitle\": \""			  << itemsList.at(i).title << "\", "
						  << "\"languagePhotoCoverFolder\": \""   << itemsList.at(i).photoFolder << "\","
						  << "\"languagePhotoCoverFilename\": \"" << itemsList.at(i).photoFilename << "\","
						  << "\"languageStudentsUserID\": ["	   << itemsList.at(i).studentUserList << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no languages returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

string GetSkillListInJSONFormat(string dbQuery, CMysql *db)
{
	struct ItemClass {
		string	  id, title;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"id\": \""		<< itemsList.at(i).id << "\", "
						  << "\"title\": \""	<< itemsList.at(i).title << "\" "
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no skills returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}


// input: ....
//		  includeStudents will add student counter
string GetUniversityListInJSONFormat(string dbQuery, CMysql *db, bool includeStudents/* = false*/)
{
	struct ItemClass {
		string	  id, title, photoFolder, photoFilename, studentUserList;
		string	  regionID, regionTitle;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");
			item.regionID		   = db->Get(i, "geo_region_id");
			item.regionTitle		= "";
			item.photoFolder		= db->Get(i, "logo_folder");
			item.photoFilename	  = db->Get(i, "logo_filename");
			item.studentUserList	= "";

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{
				if(db->Query("SELECT `title` FROM `geo_region` WHERE `id`=\"" + itemsList.at(i).regionID + "\";"))
					itemsList.at(i).regionTitle = db->Get(0, "title");

				if(includeStudents)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT * from `users_university` WHERE `university_id`=\"" + itemsList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "user_id");
					}

					itemsList.at(i).studentUserList = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"universityID\": \""				 << itemsList.at(i).id << "\", "
						  << "\"universityTitle\": \""			  << itemsList.at(i).title << "\", "
						  << "\"universityRegionID\": \""		   << itemsList.at(i).regionID << "\", "
						  << "\"universityRegionName\": \""		 << itemsList.at(i).regionTitle << "\", "
						  << "\"universityPhotoCoverFolder\": \""   << itemsList.at(i).photoFolder << "\","
						  << "\"universityPhotoCoverFilename\": \"" << itemsList.at(i).photoFilename << "\","
						  << "\"universityStudentsUserID\": ["	   << itemsList.at(i).studentUserList << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no university's returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

// input: ....
//		  includeStudents will add student counter
string GetSchoolListInJSONFormat(string dbQuery, CMysql *db, bool includeStudents/* = false*/)
{
	struct ItemClass {
		string	  id, title, photoFolder, photoFilename, studentUserList;
		string	  regionID, regionTitle;
		string	  isComplained, complainedUserList;
	};

	ostringstream	   ostResult;
	int				 itemsCount;
	vector<ItemClass>   itemsList;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ostResult.str("");
	itemsCount = db->Query(dbQuery);
	if(itemsCount)
	{
		for(int i = 0; i < itemsCount; i++)
		{
			ItemClass   item;

			item.id				 = db->Get(i, "id");
			item.title			  = db->Get(i, "title");
			item.regionID		   = db->Get(i, "geo_locality_id");
			item.regionTitle		= "";
			item.photoFolder		= db->Get(i, "logo_folder");
			item.photoFilename	  = db->Get(i, "logo_filename");
			item.studentUserList	= "";

			itemsList.push_back(item);
		}

		for(int i = 0; i < itemsCount; i++)
		{
				if(db->Query("SELECT `title` FROM `geo_locality` WHERE `id`=\"" + itemsList.at(i).regionID + "\";"))
					itemsList.at(i).regionTitle = db->Get(0, "title");

				if(includeStudents)
				{
					string temp = "";

					for(int j = 0; j < db->Query("SELECT * from `users_school` WHERE `school_id`=\"" + itemsList.at(i).id + "\";"); ++j)
					{
						if(temp.length()) temp += ",";
						temp += db->Get(j, "user_id");
					}

					itemsList.at(i).studentUserList = temp;
				}

				if(ostResult.str().length()) ostResult << ", ";

				ostResult << "{"
						  << "\"schoolID\": \""				 << itemsList.at(i).id << "\", "
						  << "\"schoolTitle\": \""			  << itemsList.at(i).title << "\", "
						  << "\"schoolLocalityID\": \""		 << itemsList.at(i).regionID << "\", "
						  << "\"schoolLocalityTitle\": \""	   << itemsList.at(i).regionTitle << "\", "
						  << "\"schoolPhotoCoverFolder\": \""   << itemsList.at(i).photoFolder << "\","
						  << "\"schoolPhotoCoverFilename\": \"" << itemsList.at(i).photoFilename << "\","
						  << "\"schoolStudentsUserID\": ["	  << itemsList.at(i).studentUserList << "]"
						  << "}";
		} // --- for loop through user list
	} // --- if sql-query on user selection success
	else
	{
		MESSAGE_DEBUG("", "", "there are no school's returned by the request [" + dbQuery + "]");
	}

	{
		MESSAGE_DEBUG("", "", "finish (returning string length = " + to_string(ostResult.str().length()) + ")");
	}

	return ostResult.str();
}

string	AutodetectSexByName(string name, CMysql *db)
{
	string		result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	if(db->Query("SELECT * FROM `name_sex` WHERE `name`=\"" + name + "\";"))
		result = db->Get(0, "sex");

	{
		MESSAGE_DEBUG("", "", "finish (result = " + result + ")");
	}

	return result;
}

/*
string GetNewsFeedInJSONFormat(string whereStatement, int currPage, int newsOnSinglePage, CUser *user, CMysql *db)
{
		struct  ItemClass
		{
			string  action_category_title;
			string  action_category_title_female;
			string  action_category_title_male;
			string  action_types_title;
			string  action_types_title_female;
			string  action_types_title_male;
			string  feed_actionId;
			string  feed_actionTypeId;
			string  feed_eventTimestamp;
			string  feed_id;
			string  src_id;
			string  src_type;
			string  src_name;
			string  src_nameLast;
			string  src_sex;
			string  dst_id;
			string  dst_type;
			string  dst_name;
			string  dst_nameLast;
			string  dst_sex;
		};
		vector<ItemClass>   itemsList;

		ostringstream	  ostResult;
		int			  affected;
		vector<string>	vectorFriendList;

		ostResult.str("");
		ostResult << "SELECT `users_friends`.`friendID` \
				from `users_friends` \
				left join `users` on `users`.`id`=`users_friends`.`friendID` \
				where `users_friends`.`userID`='" << user->GetID() << "' and `users_friends`.`state`='confirmed' and `users`.`isactivated`='Y' and `users`.`isblocked`='N';";

		affected = db->Query(ostResult.str());
		for(int i = 0; i < affected; i++)
		{
			vectorFriendList.push_back(db->Get(i, "friendID"));
		}

		ostResult.str("");
		ostResult << "SELECT "
			<< "`feed`.`id` as `feed_id`, `feed`.`eventTimestamp` as `feed_eventTimestamp`, `feed`.`actionId` as `feed_actionId` , `feed`.`actionTypeId` as `feed_actionTypeId`, `feed`.`srcType` as `feed_srcType`, `feed`.`userId` as `feed_srcID`, `feed`.`dstType` as `feed_dstType`, `feed`.`dstID` as `feed_dstID`, "
			<< "`action_types`.`title` as `action_types_title`, "
			<< "`action_types`.`title_male` as `action_types_title_male`, "
			<< "`action_types`.`title_female` as `action_types_title_female`, "
			<< "`action_category`.`title` as `action_category_title`, "
			<< "`action_category`.`title_male` as `action_category_title_male`, "
			<< "`action_category`.`title_female` as `action_category_title_female` "
			<< "FROM `feed` "
			<< "INNER JOIN  `action_types`  ON `feed`.`actionTypeId`=`action_types`.`id` "
			<< "INNER JOIN  `action_category`   ON `action_types`.`categoryID`=`action_category`.`id` "
			<< "WHERE (" << whereStatement << ") and `action_types`.`isShowFeed`='1' "
			<< "ORDER BY  `feed`.`eventTimestamp` DESC LIMIT " << currPage * newsOnSinglePage << " , " << newsOnSinglePage;

		affected = db->Query(ostResult.str());
		for(int i = 0; i < affected; i++)
		{
			ItemClass   item;

			item.action_category_title = db->Get(i, "action_category_title");
			item.action_category_title_female = db->Get(i, "action_category_title_female");
			item.action_category_title_male = db->Get(i, "action_category_title_male");
			item.action_types_title = db->Get(i, "action_types_title");
			item.action_types_title_female = db->Get(i, "action_types_title_female");
			item.action_types_title_male = db->Get(i, "action_types_title_male");
			item.feed_actionId = db->Get(i, "feed_actionId");
			item.feed_actionTypeId = db->Get(i, "feed_actionTypeId");
			item.feed_eventTimestamp = db->Get(i, "feed_eventTimestamp");
			item.feed_id = db->Get(i, "feed_id");
			item.src_id = db->Get(i, "feed_srcID");
			item.src_type = db->Get(i, "feed_srcType");
			item.dst_id = db->Get(i, "feed_dstID");
			item.dst_type = db->Get(i, "feed_dstType");

			itemsList.push_back(item);
		}

		ostResult.str("");
		for(int i = 0; i < affected; i++)
		{
			ostringstream   ost1;
			string		  srcAvatarPath;
			string		  dstAvatarPath;
			string		  feedID = itemsList[i].feed_id;
			string		  feedActionTypeId = itemsList[i].feed_actionTypeId;
			string		  feedActionId = itemsList[i].feed_actionId;
			string		  feedMessageOwner = itemsList[i].src_id;
			string		  feedMessageTimestamp = itemsList[i].feed_eventTimestamp;
			string		  messageSrcObject = "";
			string		  messageDstObject = "";

			// --- avatar for srcObj
			if(itemsList[i].src_type == "user")
			{
				if(db->Query("SELECT `id`, `name`, `nameLast`, `sex` FROM `users` WHERE `id`=\"" + itemsList[i].src_id + "\";"))
				{
					messageSrcObject = "\"type\":\"" + itemsList[i].src_type + "\",\"id\":\"" + itemsList[i].src_id + "\",\"name\":\"" + db->Get(0, "name") + "\",\"nameLast\":\"" + db->Get(0, "nameLast") + "\",\"sex\":\"" + db->Get(0, "sex") + "\",\"link\":\"\"";

					if(db->Query("SELECT * FROM `users_avatars` WHERE `userid`='" + itemsList[i].src_id + "' and `isActive`='1';"))
						srcAvatarPath = "/images/avatars/avatars" + string(db->Get(0, "folder")) + "/" + string(db->Get(0, "filename"));
					else
						srcAvatarPath = "empty";

					messageSrcObject = messageSrcObject + ",\"avatar\":\"" + srcAvatarPath + "\"";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: user.id [" + itemsList[i].src_id + "] not found");
				}
			}
			else if(itemsList[i].src_type == "company")
			{
				if(db->Query("SELECT `id`, `type`, `name`, `link`, `logo_folder`, `logo_filename` FROM `company` WHERE `id`=\"" + itemsList[i].src_id + "\";"))
				{
					string  logo_filename = db->Get(0, "logo_filename");

					messageSrcObject = "\"type\":\"" + itemsList[i].src_type + "\",\"id\":\"" + itemsList[i].src_id + "\",\"companyType\":\"" + string(db->Get(0, "type")) + "\",\"name\":\"" + string(db->Get(0, "name")) + "\",\"nameLast\":\"\",\"sex\":\"F\",\"link\":\"" + db->Get(0, "link") + "\"";

					if(logo_filename.length())
						srcAvatarPath = "/images/companies/" + string(db->Get(0, "logo_folder")) + "/" + string(db->Get(0, "logo_filename"));
					else
						srcAvatarPath = "empty";

					messageSrcObject = messageSrcObject + ",\"avatar\":\"" + srcAvatarPath + "\"";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: company.id [" + itemsList[i].src_id + "] not found");
				}
			}
			else if(itemsList[i].src_type == "group")
			{
				if(db->Query("SELECT * FROM `groups` WHERE `id`=\"" + itemsList[i].src_id + "\";"))
				{
					string  logo_filename = db->Get(0, "logo_filename");

					messageSrcObject = "\"type\":\"" + itemsList[i].src_type + "\",\"id\":\"" + itemsList[i].src_id + "\",\"name\":\"" + db->Get(0, "title") + "\",\"nameLast\":\"\",\"sex\":\"F\",\"link\":\"" + db->Get(0, "link") + "\"";

					if(logo_filename.length())
						srcAvatarPath = "/images/groups/" + string(db->Get(0, "logo_folder")) + "/" + string(db->Get(0, "logo_filename"));
					else
						srcAvatarPath = "empty";

					messageSrcObject = messageSrcObject + ",\"avatar\":\"" + srcAvatarPath + "\"";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: company.id [" + itemsList[i].src_id + "] not found");
				}
			}
			else
			{
				CLog	log;
				log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: unknown srcType [" + itemsList[i].src_type + "]");
			}

			// --- avatar for dstObj
			if(itemsList[i].dst_type == "group")
			{
				if(db->Query("SELECT * FROM `groups` WHERE `id`=\"" + itemsList[i].dst_id + "\";"))
				{
					string  logo_filename = db->Get(0, "logo_filename");

					messageDstObject = "\"type\":\"" + itemsList[i].dst_type + "\",\"id\":\"" + itemsList[i].dst_id + "\",\"name\":\"" + db->Get(0, "title") + "\",\"nameLast\":\"\",\"sex\":\"F\",\"link\":\"" + db->Get(0, "link") + "\"";

					if(logo_filename.length())
						dstAvatarPath = "/images/groups/" + string(db->Get(0, "logo_folder")) + "/" + string(db->Get(0, "logo_filename"));
					else
						dstAvatarPath = "empty";

					messageDstObject = messageDstObject + ",\"avatar\":\"" + dstAvatarPath + "\"";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: company.id [" + itemsList[i].dst_id + "] not found");
				}
			}
			// --- avatar for dstObj
			if(itemsList[i].dst_type == "event")
			{
				if(db->Query("SELECT * FROM `events` WHERE `id`=\"" + itemsList[i].dst_id + "\";"))
				{
					string  logo_filename = db->Get(0, "logo_filename");

					messageDstObject = "\"type\":\"" + itemsList[i].dst_type + "\",\"id\":\"" + itemsList[i].dst_id + "\",\"name\":\"" + db->Get(0, "title") + "\",\"nameLast\":\"\",\"sex\":\"\",\"link\":\"" + db->Get(0, "link") + "\"";

					if(logo_filename.length())
						dstAvatarPath = "/images/events/" + string(db->Get(0, "logo_folder")) + "/" + string(db->Get(0, "logo_filename"));
					else
						dstAvatarPath = "empty";

					messageDstObject = messageDstObject + ",\"avatar\":\"" + dstAvatarPath + "\"";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: company.id [" + itemsList[i].dst_id + "] not found");
				}
			}

			if(feedActionTypeId == "11")
			{
				// --- 11 - message

				if(db->Query("SELECT * FROM `feed_message` WHERE `id`='" + feedActionId + "';"))
				{
					string  messageId = db->Get(0, "id");
					string  messageTitle = db->Get(0, "title");
					string  messageLink = db->Get(0, "link");
					string  messageMessage = db->Get(0, "message");
					string  messageImageSetID = db->Get(0, "imageSetID");
					string  messageAccessRights = db->Get(0, "access");
					string  messageImageList =			GetMessageImageList(messageImageSetID, db);
					string  messageParamLikesUserList =  GetMessageLikesUsersList(messageId, user, db);
					string  messageParamCommentsCount =  GetMessageCommentsCount(messageId, db);
					string  messageParamSpam =			GetMessageSpam(messageId, db);
					string  messageParamSpamMe =			GetMessageSpamUser(messageId, user->GetID(), db);

					if(AllowMessageInNewsFeed(user, feedMessageOwner, messageAccessRights, &vectorFriendList))
					{
						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"dstObj\":{"			 << messageDstObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female		<< "\",";
						ostResult << "\"actionTypesId\":\""	<< itemsList[i].feed_actionTypeId				   << "\",";

						ostResult << "\"messageId\":\""		<< messageId							 << "\",";
						ostResult << "\"messageTitle\":\""	  << messageTitle						  << "\",";
						ostResult << "\"messageLink\":\""		<< messageLink						   << "\",";
						ostResult << "\"messageMessage\":\""	  << messageMessage						<< "\",";
						ostResult << "\"messageImageSetID\":\""   << messageImageSetID					  << "\",";
						ostResult << "\"messageImageList\":["	<< messageImageList						 << "],";
						ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList			  << "],";
						ostResult << "\"messageCommentsCount\":\""<< messageParamCommentsCount			  << "\",";
						ostResult << "\"messageSpam\":\""		<< messageParamSpam						 << "\",";
						ostResult << "\"messageSpamMe\":\""	<< messageParamSpamMe					   << "\",";
						ostResult << "\"messageAccessRights\":\"" << messageAccessRights					  << "\",";

						ostResult << "\"eventTimestamp\":\""	  << itemsList[i].feed_eventTimestamp										<< "\",";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp))  << "\"";

						ostResult << "}";

						// if(i < (affected - 1)) ostResult << ",";
					} // --- Message Access Rights onot allow to post it into feed
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: can't get message [" + feedActionId + "] FROM feed_message");
				} // --- Message in news feed not found
			}
			else if((feedActionTypeId == "14") || (feedActionTypeId == "15") || (feedActionTypeId == "16"))
			{
				// --- 14 friendship established
				// --- 15 friendship broken
				// --- 16 friendship request sent

				string  friendID = feedActionId;

				ost1.str("");
				ost1 << "SELECT `users`.`name` as `users_name`, `users`.`nameLast` as `users_nameLast` FROM `users` WHERE `id`=\"" << friendID << "\" and `isblocked`='N';";
				if(db->Query(ost1.str()))
				{
					string  friendAvatar = "empty";
					string  friendName;
					string  friendNameLast;
					string  friendCompanyName;
					string  friendCompanyID;
					string  friendUsersCompanyPositionTitle;

					friendName = db->Get(0, "users_name");
					friendNameLast = db->Get(0, "users_nameLast");

					ost1.str("");
					ost1 << "SELECT * FROM `users_avatars` WHERE `userid`='" << friendID << "' and `isActive`='1';";
					if(db->Query(ost1.str()))
					{
						ost1.str("");
						ost1 << "/images/avatars/avatars" << db->Get(0, "folder") << "/" << db->Get(0, "filename");
						friendAvatar = ost1.str();
					}

					ost1.str("");
					ost1 << "SELECT `company_position`.`title` as `users_company_position_title`,  \
							`company`.`id` as `company_id`, `company`.`name` as `company_name` \
							FROM `users_company` \
							LEFT JOIN  `company_position` ON `company_position`.`id`=`users_company`.`position_title_id` \
							LEFT JOIN  `company`				ON `company`.`id`=`users_company`.`company_id` \
							WHERE `users_company`.`user_id`=\"" << friendID << "\" and `users_company`.`current_company`='1' \
							ORDER BY  `users_company`.`occupation_start` DESC ";
					if(db->Query(ost1.str()))
					{
						friendCompanyName = db->Get(0, "company_name");
						friendCompanyID = db->Get(0, "company_id");
						friendUsersCompanyPositionTitle = db->Get(0, "users_company_position_title");
					}
					else
					{
						MESSAGE_DEBUG("", "", "can't get information [" + itemsList[i].feed_actionId + "] about his/her employment");
					} // --- Message in news feed not found

					{
						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";
						ostResult << "\"avatar\":\""			  << srcAvatarPath									<< "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""	<< itemsList[i].feed_actionTypeId				   << "\",";

						ostResult << "\"friendAvatar\":\""	  << friendAvatar								  << "\",";
						ostResult << "\"friendID\":\""		  << friendID									  << "\",";
						ostResult << "\"friendName\":\""		  << friendName									<< "\",";
						ostResult << "\"friendNameLast\":\""	  << friendNameLast								<< "\",";
						ostResult << "\"friendCompanyID\":\""	<< friendCompanyID							   << "\",";
						ostResult << "\"friendCompanyName\":\""   << friendCompanyName							  << "\",";
						ostResult << "\"friendUsersCompanyPositionTitle\":\"" << friendUsersCompanyPositionTitle	  << "\",";

						ostResult << "\"eventTimestamp\":\""	  << itemsList[i].feed_eventTimestamp										<< "\",";
						// ostResult << "\"eventTimestampDelta\":\""  << GetHumanReadableTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)   << "\"";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp))  << "\"";
						ostResult << "}";

						// if(i < (affected - 1)) ostResult << ",";
					} // --- Message Access Rights onot allow to post it into feed
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: user [", friendID, "] not found or blocked");
				}
			}
			else if((feedActionTypeId == "41"))
			{
				// --- 41 skill added

				string  users_skillID = feedActionId;

				ost1.str("");
				ost1 << "SELECT * FROM `users_skill` WHERE `id`=\"" << users_skillID << "\";";
				if(db->Query(ost1.str()))
				{
					string  skillID = db->Get(0, "skill_id");

					ost1.str("");
					ost1 << "SELECT * FROM `skill` WHERE `id`=\"" << skillID << "\";";
					if(db->Query(ost1.str()))
					{
						string	skillTitle = db->Get(0, "title");

						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";
						ostResult << "\"avatar\":\""			  << srcAvatarPath									<< "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""	<< itemsList[i].feed_actionTypeId				   << "\",";

						ostResult << "\"skillID\":\""			<< skillID									   << "\",";
						ostResult << "\"skillTitle\":\""		  << skillTitle									<< "\",";

						ostResult << "\"eventTimestamp\":\""	  << itemsList[i].feed_eventTimestamp							<< "\",";
						// ostResult << "\"eventTimestampDelta\":\""  << GetHumanReadableTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)   << "\"";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp))  << "\"";
						ostResult << "}";

					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: skill_id [", skillID, "] not found");
					}

				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: users_skill_id [", users_skillID, "] not found");
				}
			}
			else if((feedActionTypeId == "54") || (feedActionTypeId == "53"))
			{
				// --- book read
				string  usersBooksID = itemsList[i].feed_actionId;
				string  bookID = "";
				string  readerUserID = "";
				string  bookReadTimestamp = "";

				if(db->Query("SELECT * FROM `users_books` WHERE `id`=\"" + feedActionId + "\";"))
				{
					bookID = db->Get(0, "bookID");
					readerUserID = db->Get(0, "userID");
					bookReadTimestamp = db->Get(0, "bookReadTimestamp");


					if(bookID.length() && db->Query("SELECT * FROM `book` WHERE `id`=\"" + bookID + "\";"))
					{
						string  bookTitle = db->Get(0, "title");
						string  bookAuthorID = db->Get(0, "authorID");
						string  bookISBN10 = db->Get(0, "isbn10");
						string  bookISBN13 = db->Get(0, "isbn13");
						string  bookCoverPhotoFolder = db->Get(0, "coverPhotoFolder");
						string  bookCoverPhotoFilename = db->Get(0, "coverPhotoFilename");

						if(bookAuthorID.length() && db->Query("SELECT * FROM `book_author` WHERE `id`=\"" + bookAuthorID + "\";"))
						{
							string  bookAuthorName = db->Get(0, "name");
							string  bookReadersRatingList =	 GetBookRatingList(bookID, db);
							string  messageParamLikesUserList = GetBookLikesUsersList(usersBooksID, user, db);
							string  messageParamCommentsCount = GetBookCommentsCount(bookID, db);
							string  bookMyRating = "0";

							if(db->Query("SELECT * FROM `users_books` WHERE `userID`=\"" + user->GetID() + "\" AND `bookID`=\"" + bookID + "\";"))
								bookMyRating = db->Get(0, "rating");

							if(ostResult.str().length() > 10) ostResult << ",";

							ostResult << "{";

							ostResult << "\"bookID\":\""			<< bookID << "\",";
							ostResult << "\"bookTitle\":\""	   << bookTitle << "\",";
							ostResult << "\"bookAuthorID\":\""	  << bookAuthorID << "\",";
							ostResult << "\"bookAuthorName\":\""	<< bookAuthorName << "\",";
							ostResult << "\"bookISBN10\":\""		<< bookISBN10 << "\",";
							ostResult << "\"bookISBN13\":\""		<< bookISBN13 << "\",";
							ostResult << "\"bookMyRating\":\""		<< bookMyRating << "\",";
							ostResult << "\"bookCoverPhotoFolder\":\""<< bookCoverPhotoFolder << "\",";
							ostResult << "\"bookCoverPhotoFilename\":\""<< bookCoverPhotoFilename << "\",";
							ostResult << "\"bookReadTimestamp\":\""   << bookReadTimestamp << "\",";
							ostResult << "\"bookReadersRatingList\":["<< bookReadersRatingList << "],";
							ostResult << "\"bookCommentsCount\":\""   << messageParamCommentsCount << "\",";

							ostResult << "\"usersBooksID\":\""	  << usersBooksID << "\",";
							ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

							ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
							ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
							ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
							ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
							ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
							ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
							ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
							ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
							ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
							ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
							ostResult << "\"actionID\":\""		  << itemsList[i].feed_actionId << "\",";
							ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
							ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
							ostResult << "}";
						}
						else
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: books_author_id [" + bookAuthorID + "] not found");
						}

					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: books_id [" + bookID + "] not found");
					}

				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_books_id [" + feedActionId + "] not found");
				}
			}
			else if((feedActionTypeId == "64") || (feedActionTypeId == "65"))
			{
				// --- group created / subscribed
				string  groupID = feedActionId;

				if(groupID.length())
				{
					if(db->Query("SELECT * FROM `groups` WHERE `id`=\"" + groupID + "\""))
					{
						string		isBlocked = db->Get(0, "isBlocked");

						if(isBlocked == "N")
						{
							if(ostResult.str().length() > 10) ostResult << ",";

							ostResult << "{";
							ostResult << "\"groups\":["			<< GetGroupListInJSONFormat("SELECT * FROM `groups` WHERE `id`=\"" + groupID + "\" AND `isBlocked`=\"N\";", db, user) << "],";
							ostResult << "\"avatar\":\""		<< srcAvatarPath << "\",";
							ostResult << "\"srcObj\":{"			<< messageSrcObject << "},";
							ostResult << "\"messageOwner\":{"	<< messageSrcObject << "},";
							ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
							ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
							ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
							ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
							ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
							ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
							ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
							ostResult << "\"actionID\":\""		  << itemsList[i].feed_actionId << "\",";
							ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
							ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
							ostResult << "}";
						}
						else
						{
							CLog	log;
							MESSAGE_DEBUG("", "", "`group`.`id` [" + groupID + "] blocked");
						}
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: `group`.`id` [" + groupID + "] not found");
					}
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: `group`.`id` [" + groupID + "] is empty");
				}
			}
			else if(feedActionTypeId == "63")
			{
				// --- group created / subscribed
				string  companyID = feedActionId;

				if(companyID.length() && db->Query("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\";"))
				{
					if(ostResult.str().length() > 10) ostResult << ",";

					ostResult << "{";
					ostResult << "\"companies\":["		<< GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\" AND `isBlocked`=\"N\";", db, user) << "],";
					ostResult << "\"avatar\":\""		<< srcAvatarPath << "\",";
					ostResult << "\"srcObj\":{"			<< messageSrcObject << "},";
					ostResult << "\"messageOwner\":{"	<< messageSrcObject << "},";
					ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
					ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
					ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
					ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
					ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
					ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
					ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
					ostResult << "\"actionID\":\""		  << itemsList[i].feed_actionId << "\",";
					ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
					ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
					ostResult << "}";
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: books_id [" + companyID + "] not found");
				}
			}
			else if(feedActionTypeId == "22")
			{
				// --- cretificate received

				if(db->Query("SELECT * FROM `users_certifications` WHERE `id`=\"" + feedActionId + "\";"))
				{
					string  certificationNumber = "";
					string  certificationTrackID = "";
					string  certifiedUserID = "";
					string  usersCertificationID = "";

					usersCertificationID = db->Get(0, "id");
					certificationNumber = db->Get(0, "certification_number");
					certificationTrackID = db->Get(0, "track_id");
					certifiedUserID = db->Get(0, "user_id");

					if(certificationTrackID.length() && db->Query("SELECT * FROM `certification_tracks` WHERE `id`=\"" + certificationTrackID + "\";"))
					{
						string  certificationVendorID = db->Get(0, "vendor_id");
						string  certificationVendorName = "";
						string  certificationTrackTitle = db->Get(0, "title");
						string  certificationVendorLogoFolder = db->Get(0, "logo_folder");
						string  certificationVendorLogoFilename = db->Get(0, "logo_filename");
						string  messageParamLikesUserList = GetCertificationLikesUsersList(usersCertificationID, user, db);
						string  messageParamCommentsCount = GetCertificateCommentsCount(certificationTrackID, db);

						if(certificationVendorID.length() && db->Query("SELECT * FROM `company` WHERE `id`=\"" + certificationVendorID + "\";"))
							certificationVendorName = db->Get(0, "name");
						else
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: certification vendor [" + certificationVendorID + "] not found");
						}

						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";

						ostResult << "\"certificationID\":\"" << usersCertificationID << "\",";
						ostResult << "\"certificationTrackID\":\"" << certificationTrackID << "\",";
						ostResult << "\"certificationNumber\":\"" << certificationNumber << "\",";
						ostResult << "\"certificationVendorID\":\"" << certificationVendorID << "\",";
						ostResult << "\"certificationVendorName\":\"" << certificationVendorName << "\",";
						ostResult << "\"certificationTrackTitle\":\"" << certificationTrackTitle << "\",";
						ostResult << "\"certificationVendorLogoFolder\":\""<< certificationVendorLogoFolder << "\",";
						ostResult << "\"certificationVendorLogoFilename\":\""<< certificationVendorLogoFilename << "\",";

						ostResult << "\"certificationCommentsCount\":\"" << messageParamCommentsCount << "\",";
						ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

						ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
						ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
						ostResult << "}";
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: certification track [" + certificationTrackID + "] not found");
					}


				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_certification_id [" + feedActionId + "] not found");
				}

			}
			else if(feedActionTypeId == "23")
			{
				// --- course received

				if(db->Query("SELECT * FROM `users_courses` WHERE `id`=\"" + feedActionId + "\";"))
				{
					string  courseTrackID = "";
					string  courseUserID = "";
					string  courseMyRating = "0";
					string  usersCourseID = "";
					string  eventTimestamp = "";

					usersCourseID = db->Get(0, "id");
					courseTrackID = db->Get(0, "track_id");
					courseUserID = db->Get(0, "user_id");
					eventTimestamp = db->Get(0, "eventTimestamp");

					if(courseTrackID.length() && db->Query("SELECT * FROM `certification_tracks` WHERE `id`=\"" + courseTrackID + "\";"))
					{
						string  courseVendorID = db->Get(0, "vendor_id");
						string  courseVendorName = "";
						string  courseTrackTitle = db->Get(0, "title");
						string  courseVendorLogoFolder = db->Get(0, "logo_folder");
						string  courseVendorLogoFilename = db->Get(0, "logo_filename");
						string  courseRatingList =			GetCourseRatingList(courseTrackID, db);
						string  messageParamLikesUserList = GetCourseLikesUsersList(usersCourseID, user, db);
						string  messageParamCommentsCount = GetCertificateCommentsCount(courseTrackID, db);

						if(courseVendorID.length() && db->Query("SELECT * FROM `company` WHERE `id`=\"" + courseVendorID + "\";"))
							courseVendorName = db->Get(0, "name");
						else
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: course vendor [" + courseVendorID + "] not found");
						}

						if(db->Query("SELECT * FROM `users_courses` WHERE `user_id`=\"" + user->GetID() + "\" AND `track_id`=\"" + courseTrackID + "\";"))
							courseMyRating = db->Get(0, "rating");

						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";

						ostResult << "\"courseID\":\""		  << courseTrackID << "\",";
						ostResult << "\"usersCourseID\":\""   << usersCourseID << "\",";
						ostResult << "\"courseTrackID\":\""   << courseTrackID << "\",";
						ostResult << "\"courseVendorID\":\""	<< courseVendorID << "\",";
						ostResult << "\"courseVendorName\":\""  << courseVendorName << "\",";
						ostResult << "\"courseTrackTitle\":\""  << courseTrackTitle << "\",";
						ostResult << "\"courseVendorLogoFolder\":\""<< courseVendorLogoFolder << "\",";
						ostResult << "\"courseVendorLogoFilename\":\""<< courseVendorLogoFilename << "\",";
						ostResult << "\"courseMyRating\":\""	<< courseMyRating << "\",";
						ostResult << "\"courseRatingList\":["  << courseRatingList << "],";
						ostResult << "\"courseEventTimestamp\":\""<< eventTimestamp << "\",";

						ostResult << "\"courseCommentsCount\":\"" << messageParamCommentsCount << "\",";
						ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

						ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
						ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
						ostResult << "}";
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: course track [" + courseTrackID + "] not found");
					}


				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_course_id [" + feedActionId + "] not found");
				}

			}
			else if(feedActionTypeId == "39")
			{
				// --- university degree received

				if(db->Query("SELECT * FROM `users_university` WHERE `id`=\"" + feedActionId + "\";"))
				{
					string  scienceDegreeTitle = "";
					string  scienceDegreeUniversityID = "";
					string  scienceDegreeStart = "";
					string  scienceDegreeFinish = "";
					string  degreedUserID = "";
					string  usersUniversityDegreeID = "";

					usersUniversityDegreeID = db->Get(0, "id");
					degreedUserID = db->Get(0, "user_id");
					scienceDegreeUniversityID = db->Get(0, "university_id");
					scienceDegreeTitle = db->Get(0, "degree");
					scienceDegreeStart = db->Get(0, "occupation_start");
					scienceDegreeFinish = db->Get(0, "occupation_finish");

					if(scienceDegreeUniversityID.length() && db->Query("SELECT * FROM `university` WHERE `id`=\"" + scienceDegreeUniversityID + "\";"))
					{
						string  scienceDegreeUniversityID = db->Get(0, "id");
						string  scienceDegreeUniversityTitle = db->Get(0, "title");
						string  scienceDegreeUniversityRegionID = db->Get(0, "geo_region_id");
						string  scienceDegreeUniversityRegionTitle = "";
						string  scienceDegreeUniversityCountryID = "";
						string  scienceDegreeUniversityCountryTitle = "";
						string  scienceDegreeUniversityLogoFolder = db->Get(0, "logo_folder");
						string  scienceDegreeUniversityLogoFilename = db->Get(0, "logo_filename");

						if(scienceDegreeUniversityRegionID.length() && db->Query("SELECT * FROM `geo_region` WHERE `id`=\"" + scienceDegreeUniversityRegionID + "\";"))
						{
							scienceDegreeUniversityRegionTitle = db->Get(0, "title");
							scienceDegreeUniversityCountryID = db->Get(0, "geo_country_id");
							if(scienceDegreeUniversityCountryID.length() && db->Query("SELECT * FROM `geo_country` WHERE `id`=\"" + scienceDegreeUniversityCountryID + "\";"))
							{
								scienceDegreeUniversityCountryTitle = db->Get(0, "title");
							}
							else
							{
								CLog	log;
								log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: university [" + scienceDegreeUniversityID + "] geo_region [" + scienceDegreeUniversityRegionID + "] geo_country [" + scienceDegreeUniversityCountryID + "] not found");
							}

						}
						else
						{
							CLog	log;
							log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]: university [" + scienceDegreeUniversityID + "] geo_region [" + scienceDegreeUniversityRegionID + "] not found");
						}

						{
							string  messageParamLikesUserList = GetUniversityDegreeLikesUsersList(usersUniversityDegreeID, user, db);
							string  messageParamCommentsCount = GetUniversityDegreeCommentsCount(scienceDegreeUniversityID, db);

							if(ostResult.str().length() > 10) ostResult << ",";

							ostResult << "{";

							ostResult << "\"scienceDegreeID\":\"" << usersUniversityDegreeID << "\",";
							ostResult << "\"scienceDegreeTitle\":\"" << scienceDegreeTitle << "\",";
							ostResult << "\"scienceDegreeUniversityTitle\":\"" << scienceDegreeUniversityTitle << "\",";
							ostResult << "\"scienceDegreeUniversityID\":\"" << scienceDegreeUniversityID << "\",";
							ostResult << "\"scienceDegreeUniversityRegionID\":\"" << scienceDegreeUniversityRegionID << "\",";
							ostResult << "\"scienceDegreeUniversityRegionTitle\":\"" << scienceDegreeUniversityRegionTitle << "\",";
							ostResult << "\"scienceDegreeUniversityCountryID\":\"" << scienceDegreeUniversityCountryID << "\",";
							ostResult << "\"scienceDegreeUniversityCountryTitle\":\"" << scienceDegreeUniversityCountryTitle << "\",";
							ostResult << "\"scienceDegreeUniversityLogoFolder\":\""<< scienceDegreeUniversityLogoFolder << "\",";
							ostResult << "\"scienceDegreeUniversityLogoFilename\":\""<< scienceDegreeUniversityLogoFilename << "\",";
							ostResult << "\"scienceDegreeStart\":\""<< scienceDegreeStart << "\",";
							ostResult << "\"scienceDegreeFinish\":\""<< scienceDegreeFinish << "\",";

							ostResult << "\"scienceDegreeCommentsCount\":\"" << messageParamCommentsCount << "\",";
							ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

							ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
							ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
							ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
							ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
							ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
							ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
							ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
							ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
							ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
							ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
							ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
							ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
							ostResult << "}";
						}

					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: university ID [" + scienceDegreeUniversityID + "] not found");
					}

				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_university_id [" + feedActionId + "] not found");
				}
			}
			else if(feedActionTypeId == "40")
			{
				// --- language improved

				if(db->Query("SELECT * FROM `users_language` WHERE `id`=\"" + feedActionId + "\";"))
				{
					string  usersLanguageID = "";
					string  languageID = "";
					string  languageUserID = "";
					string  languageLevel = "";

					usersLanguageID = db->Get(0, "id");
					languageID = db->Get(0, "language_id");
					languageUserID = db->Get(0, "user_id");
					languageLevel = db->Get(0, "level");

					if(languageID.length() && db->Query("SELECT * FROM `language` WHERE `id`=\"" + languageID + "\";"))
					{
						string  languageTitle = db->Get(0, "title");
						string  languageLogoFolder = db->Get(0, "logo_folder");
						string  languageLogoFilename = db->Get(0, "logo_filename");
						string  messageParamLikesUserList = GetLanguageLikesUsersList(usersLanguageID, user, db);
						string  messageParamCommentsCount = GetLanguageCommentsCount(languageID, db);

						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";

						ostResult << "\"languageID\":\""		  << languageID << "\",";
						ostResult << "\"usersLanguageID\":\""   << usersLanguageID << "\",";
						ostResult << "\"languageTitle\":\""   << languageTitle << "\",";
						ostResult << "\"languageLogoFolder\":\""<< languageLogoFolder << "\",";
						ostResult << "\"languageLogoFilename\":\""<< languageLogoFilename << "\",";
						ostResult << "\"languageLevel\":\""  << languageLevel << "\",";

						ostResult << "\"languageCommentsCount\":\"" << messageParamCommentsCount << "\",";
						ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

						ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
						ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
						ostResult << "}";
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: language[" + languageID + "] not found");
					}


				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_language_id [" + feedActionId + "] not found");
				}

			}
			else if(feedActionTypeId == "1")
			{
				// --- change employment

				if(db->Query("SELECT * FROM `users_company` WHERE `id`=\"" + feedActionId + "\";"))
				{
					string  usersCompanyID = "";
					string  companyID = "";
					string  companyUserID = "";
					string  companyPositionTitleID = "";

					usersCompanyID = db->Get(0, "id");
					companyID = db->Get(0, "company_id");
					companyUserID = db->Get(0, "user_id");
					companyPositionTitleID = db->Get(0, "position_title_id");

					if(companyID.length() && db->Query("SELECT * FROM `company` WHERE `id`=\"" + companyID + "\";"))
					{
						string  companyTitle = db->Get(0, "name");
						string  companyLogoFolder = db->Get(0, "logo_folder");
						string  companyLogoFilename = db->Get(0, "logo_filename");
						string  companyPositionTitle = "";
						string  messageParamLikesUserList = GetCompanyLikesUsersList(usersCompanyID, user, db);
						string  messageParamCommentsCount = GetCompanyCommentsCount(companyID, db);

						if(companyPositionTitleID.length() && db->Query("SELECT * FROM `company_position` WHERE `id`=\"" + companyPositionTitleID + "\";"))
							companyPositionTitle = db->Get(0, "title");

						if(ostResult.str().length() > 10) ostResult << ",";

						ostResult << "{";

						ostResult << "\"companyID\":\""		<< companyID << "\",";
						ostResult << "\"usersCompanyID\":\""	 << usersCompanyID << "\",";
						ostResult << "\"companyTitle\":\""  << companyTitle << "\",";
						ostResult << "\"companyLogoFolder\":\""<< companyLogoFolder << "\",";
						ostResult << "\"companyLogoFilename\":\""<< companyLogoFilename << "\",";
						ostResult << "\"companyPositionTitleID\":\""	<< companyPositionTitleID << "\",";
						ostResult << "\"companyPositionTitle\":\""	<< companyPositionTitle << "\",";

						ostResult << "\"companyCommentsCount\":\"" << messageParamCommentsCount << "\",";
						ostResult << "\"messageLikesUserList\":[" << messageParamLikesUserList << "],";

						ostResult << "\"avatar\":\""			<< srcAvatarPath << "\",";
						ostResult << "\"srcObj\":{"			 << messageSrcObject << "},";
						ostResult << "\"messageOwner\":{" << messageSrcObject << "},";
						ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
						ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
						ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
						ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
						ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
						ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
						ostResult << "\"actionTypesId\":\""   << itemsList[i].feed_actionTypeId << "\",";
						ostResult << "\"eventTimestamp\":\""	<< itemsList[i].feed_eventTimestamp << "\",";
						ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)) << "\"";
						ostResult << "}";
					}
					else
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: company[" + companyID + "] not found");
					}


				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: users_company_id [" + feedActionId + "] not found");
				}

			}
			else
			{

				if(ostResult.str().length() > 10) ostResult << ",";

				ostResult << "{";
				ostResult << "\"avatar\":\""			  << srcAvatarPath															<< "\",";
				ostResult << "\"srcObj\":{"	<< messageSrcObject << "},";
				ostResult << "\"actionCategoryTitle\":\"" << itemsList[i].action_category_title		   << "\",";
				ostResult << "\"actionCategoryTitleMale\":\"" << itemsList[i].action_category_title_male	<< "\",";
				ostResult << "\"actionCategoryTitleFemale\":\""<< itemsList[i].action_category_title_female   << "\",";
				ostResult << "\"actionTypesTitle\":\""  << itemsList[i].action_types_title			  << "\",";
				ostResult << "\"actionTypesTitleMale\":\""<< itemsList[i].action_types_title_male		 << "\",";
				ostResult << "\"actionTypesTitleFemale\":\""<< itemsList[i].action_types_title_female	 << "\",";
				ostResult << "\"actionTypesId\":\""	<< itemsList[i].feed_actionTypeId										   << "\",";
				ostResult << "\"eventTimestamp\":\""	  << itemsList[i].feed_eventTimestamp										<< "\",";
				// ostResult << "\"eventTimestampDelta\":\""  << GetHumanReadableTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp)   << "\"";
				ostResult << "\"eventTimestampDelta\":\"" << to_string(GetTimeDifferenceFromNow(itemsList[i].feed_eventTimestamp))  << "\"";
				ostResult << "}";

				// if(i < (affected - 1)) ostResult << ",";
			}
		}

		return ostResult.str();
}
*/

string GetUnreadChatMessagesInJSONFormat(CUser *user, CMysql *db)
{
	ostringstream	result, ost;
	int				affected;

	{
		MESSAGE_DEBUG("", "", "start");
	}

	result.str("");

	ost.str("");
	ost << "select * from `chat_messages` where `toID`='" << user->GetID() << "' and (`messageStatus`='unread' or `messageStatus`='delivered' or `messageStatus`='sent');";
	affected = db->Query(ost.str());
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			result << (i ? "," : "") << "{\
				\"id\": \""					<< db->Get(i, "id") << "\", \
				\"message\": \"" 			<< ReplaceDoubleQuoteToQuote(db->Get(i, "message")) << "\", \
				\"fromType\": \"" 			<< db->Get(i, "fromType") << "\",\
				\"fromID\": \""				<< db->Get(i, "fromID") << "\",\
				\"toType\": \""			 	<< db->Get(i, "toType") << "\",\
				\"toID\": \""	 			<< db->Get(i, "toID") << "\",\
				\"messageType\": \""		<< db->Get(i, "messageType") << "\",\
				\"messageStatus\": \""		<< db->Get(i, "messageStatus") << "\",\
				\"eventTimestamp\": \""		<< db->Get(i, "eventTimestamp") << "\"\
			}";
		}
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return	result.str();
}


// --- Function returns list of images belongs to imageSet
// --- input: imageSetID, db
// --- output: list of image objects
string GetMessageImageList(string imageSetID, CMysql *db)
{
	ostringstream	ost;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	if(imageSetID != "0")
	{
		int				affected;

		ost.str("");
		ost << "select * from `feed_images` where `setID`='" << imageSetID << "';";
		affected = db->Query(ost.str());
		if(affected > 0)
		{
			ost.str("");
			for(int i = 0; i < affected; i++)
			{
				if(i > 0) ost << "," << std::endl;
				ost << "{";
				ost << "\"id\":\"" << db->Get(i, "id") << "\",";
				ost << "\"folder\":\"" << db->Get(i, "folder") << "\",";
				ost << "\"filename\":\"" << db->Get(i, "filename") << "\",";
				ost << "\"mediaType\":\"" << db->Get(i, "mediaType") << "\",";
				ost << "\"isActive\":\"" << db->Get(i, "isActive") << "\"";
				ost << "}";
			}

			result = ost.str();
		}
	}

	{
		CLog			log;
		ostringstream	ost;

		ost.str();
		ost <<  "GetMessageImageList: end. returning string length " << result.length();
		log.Write(DEBUG, ost.str());
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

	{
		MESSAGE_DEBUG("", "", "start");
	}

	if(languageTitle.length())
	{
		if(db->Query("SELECT `id` FROM `language` WHERE `title`=\"" + languageTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;
			{
				CLog			log;
				MESSAGE_DEBUG("", "", "languageTitle [" + languageTitle + "] not found. Creating new one.");
			}

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
		{
			CLog			log;
			MESSAGE_DEBUG("", "", "languageTitle is empty");
		}
	}

	result = languageID;

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}


	return result;
}

string GetSkillIDByTitle(string skillTitle, CMysql *db)
{
	ostringstream   ost;
	string		  	result = "";
	string			languageID = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	if(skillTitle.length())
	{
		if(db->Query("SELECT `id` FROM `skill` WHERE `title`=\"" + skillTitle + "\";"))
		{
			languageID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;
			{
				CLog			log;
				MESSAGE_DEBUG("", "", "skillTitle [" + skillTitle + "] not found. Creating new one.");
			}

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
		{
			CLog			log;
			MESSAGE_DEBUG("", "", "skillTitle is empty");
		}
	}

	result = languageID;

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

string GetGeoLocalityIDByCityAndRegion(string regionName, string cityName, CMysql *db)
{
	ostringstream   ost;
	string		 	result = "";
	string			regionID = "", cityID = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	if(regionName.length())
	{
		if(db->Query("SELECT `id` FROM `geo_region` WHERE `title`=\"" + regionName + "\";"))
		{
			regionID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;
			{
				CLog			log;
				MESSAGE_DEBUG("", "", "region[" + regionName + "] not found. Creating new one.");
			}

			tmp = db->InsertQuery("INSERT INTO `geo_region` SET `geo_country_id`=\"0\", `title`=\"" + regionName + "\";");
			if(tmp)
				regionID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into geo_region_db");
			}
		}
	}
	else
	{
		{
			CLog			log;
			MESSAGE_DEBUG("", "", "regionName is empty");
		}
	}

	if(cityName.length())
	{
		if(db->Query("SELECT `id` FROM `geo_locality` WHERE `title`=\"" + cityName + "\" " + (regionID.length() ? " AND `geo_region_id`=\"" + regionID + "\" " : "") + ";"))
		{
			cityID = db->Get(0, "id");
		}
		else
		{
			long int 	tmp;
			{
				CLog			log;
				MESSAGE_DEBUG("", "", "region[" + cityName + "] not found. Creating new one.");
			}

			tmp = db->InsertQuery("INSERT INTO `geo_locality` SET " + (regionID.length() ? "`geo_region_id`=\"" + regionID + "\"," : "") + " `title`=\"" + cityName + "\";");
			if(tmp)
				cityID = to_string(tmp);
			else
			{
				CLog			log;
				log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: insert into geo_region_db");
			}
		}
		result = cityID;
	}
	else
	{
		{
			CLog			log;
			MESSAGE_DEBUG("", "", "cityName is empty");
		}
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns array of book rating
// --- input: bookID, db
// --- output: book rating array
string GetBookRatingList(string bookID, CMysql *db)
{
	int			 affected;
	string		  result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `users_books` where `bookID`=\"" + bookID + "\";");
	if(affected > 0)
	{
		for(int i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

// --- Function returns array of course rating
// --- input: courseID, db
// --- output: course rating array
string GetCourseRatingList(string courseID, CMysql *db)
{
	int				affected;
	string			result = "";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	affected = db->Query("select * from `users_courses` where `track_id`=\"" + courseID + "\";");
	if(affected > 0)
	{
		for(int i = 0; i < affected; ++i)
		{
			if(i) result += ",";
			result += db->Get(i, "rating");
		}
	}

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "finish (returning string length " + to_string(result.length()) + ")");
	}

	return result;
}

string GetMessageCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"message\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetCompanyCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"company\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetLanguageCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"language\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetBookCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"book\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetCertificateCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type` in (\"certification\", \"course\") and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetUniversityDegreeCommentsCount(string messageID, CMysql *db)
{
	ostringstream   ost;
	int			 affected;
	string		  result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_comment` where `type`=\"university\" and `messageID`='" << messageID << "';";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

string GetMessageSpam(string messageID, CMysql *db)
{
	ostringstream	ost;
	int				affected;
	string			result = "0";

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and messageID='" << messageID << "';";
	affected = db->Query(ost.str());


	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}

	{
		MESSAGE_DEBUG("", "", "finish");
	}

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

	{
		MESSAGE_DEBUG("", "", "start");
	}

	ost.str("");
	ost << "select count(*) as `counter` from `feed_message_params` where `parameter`='spam' and `messageID`='" << messageID << "' and `userID`='" << userID << "' ;";
	affected = db->Query(ost.str());
	if(affected > 0)
	{
		result = db->Get(0, "counter");
	}


	{
		MESSAGE_DEBUG("", "", "finish");
	}

	return result;
}

bool AllowMessageInNewsFeed(CUser *me, const string messageOwnerID, const string messageAccessRights, vector<string> *messageFriendList)
{

	{
		CLog			log;
		MESSAGE_DEBUG("", "", "parameters (user [" + me->GetID() + "], messageOwnerID [" + messageOwnerID + "], messageAccessRights [" + messageAccessRights + "]): start");
	}

	// --- messages belons to yourself must be shown unconditionally
	// --- must be checked before message access private
	if(me->GetID() == messageOwnerID) return true;

	if(messageAccessRights == "public") return true;
	if(messageAccessRights == "private") return false;

	// --- require to check friendship list;
	if(messageAccessRights == "friends")
	{
		for(auto it = messageFriendList->begin(); it != messageFriendList->end(); ++it)
		{
			if(*it == messageOwnerID) return true;
		}

		return false;
	}

	return true;
}

// --- rate-limit on sessid persistence
// --- input: REMOTE_ADDR
// --- output: true, if rate-limit required
// ---		 false, if rate-limit not required
bool isPersistenceRateLimited(string REMOTE_ADDR, CMysql *db)
{
	auto 			maxTime = BRUTEFORCE_TIME_INTERVAL, maxAttempts = BRUTEFORCE_ATTEMPTS;
	auto			rateLimitID = ""s;
	auto			affected = 0, attempts = 0;
	auto			result = false;

	MESSAGE_DEBUG("", "", "start (REMOTE_ADDR " + REMOTE_ADDR + ")");

	// --- cleanup rate-limit table
	db->Query("delete from `sessions_persistence_ratelimit` where `eventTimestamp` < (NOW() - interval "s + to_string(maxTime) + " second);");

	affected = db->Query("select `id`, `attempts` from `sessions_persistence_ratelimit` where `ip`='"s + REMOTE_ADDR + "';");
	if(affected)
	{
		MESSAGE_DEBUG("", "", "REMOTE_ADDR is in rate-limit table");

		rateLimitID = db->Get(0, "id");
		attempts = stoi(db->Get(0, "attempts"));

		db->Query("update `sessions_persistence_ratelimit` set `attempts`=`attempts`+1 where `id`='" + rateLimitID + "';");

		if(attempts > maxAttempts)
		{
			MESSAGE_ERROR("", "", "REMOTE_ADDR has tryed " + to_string(attempts) + " times during the last " + to_string(maxTime) + "sec. Needed to be rate-limited.")

			result = true;
		}
		else
		{
			MESSAGE_DEBUG("", "", "REMOTE_ADDR has tryed " + to_string(attempts) + " times during the last " + to_string(maxTime) + "sec. No need to rate-limit.")
			
			result = false;
		}
	}
	else
	{
		MESSAGE_DEBUG("", "", "REMOTE_ADDR not in rate-limit table.")

		db->Query("INSERT INTO `sessions_persistence_ratelimit` set `attempts`='1', `ip`='" + REMOTE_ADDR + "', `eventTimestamp`=NOW();");

		result = false;
	}

	MESSAGE_DEBUG("", "", "finish. "s + (result ? "rate-limit" : "no need rate-limit"))

	return result;
}

void CopyFile(const string src, const string dst)
{
	clock_t start, end;

	MESSAGE_DEBUG("", "", "start (" + src + ", " + dst + ")");

	start = clock();

	ifstream source(src.c_str(), ios::binary);
	ofstream dest(dst.c_str(), ios::binary);

	dest << source.rdbuf();

	source.close();
	dest.close();


	end = clock();

	MESSAGE_DEBUG("", "", "finish (time of file copying is " + to_string((end - start) / CLOCKS_PER_SEC) + " second)");
}

// --- admin function
string GetCompanyDuplicates(CMysql *db)
{
	// --- map<companyName, companyID>
	map<string, string>		companyMap;

	// --- map<companyID, companyName>
	map<string, string>		duplicatesMap;
	ostringstream			ost, ostResult;

	{
		CLog	log;
		ostringstream	ostTemp;

		ostTemp.str("");
		ostTemp << "GetCompanyDuplicates: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "select * from `company`;";
	for(int i = 0; i < db->Query(ost.str()); i++)
	{
		string		companyToTestName, companyToTestID;

		companyToTestID = db->Get(i, "id");
		companyToTestName = RemoveAllNonAlphabetSymbols(db->Get(i, "name"));

		auto it = companyMap.find(companyToTestName);
		if(it != companyMap.end())
		{
			// --- duplicate found
			duplicatesMap[companyToTestID] = companyToTestName;
			duplicatesMap[it->second] = it->first;
		}
		companyMap[companyToTestName] = companyToTestID;
	}

	for (const auto& it : duplicatesMap)
	{
		if(ostResult.str().length()) ostResult << ",";
		ost.str("");
		ost << "select * from `company` where `id`='" << it.first << "';";
		db->Query(ost.str());
		ostResult << "{\"id\":\"" << it.first << "\", \"companyName\":\"" << db->Get(0, "name") << "\"";

		ost.str("");
		ost << "select count(*) as `number_of_users` from `users_company` where `company_id`='" << it.first << "';";
		db->Query(ost.str());
		ostResult << ", \"usersInCompany\":\"" << db->Get(0, "number_of_users") << "\"}";
	}

	{
		CLog	log;
		ostringstream	ostTemp;

		ostTemp.str("");
		ostTemp << "GetCompanyDuplicates: end (number of duplicates: " << duplicatesMap.size() << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithEmptySet(CMysql *db)
{
	ostringstream		   ost, ostResult;
	int					 affected;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithEmptySet: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT * FROM `feed_images` where `setID`='0';";
	affected = db->Query(ost.str());
	for(int i = 0; i < affected; i++)
	{
		if(ostResult.str().length()) ostResult << ",";
		ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithEmptySet: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithUnknownMessage(CMysql *db)
{
	ostringstream					   ost, ostResult;
	int								 affected, lostCount = 0;
	unordered_set<unsigned long>		allImageSets;
	unordered_set<unsigned long>		lostImageSets;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownMessage: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT `setID` FROM `feed_images`;";
	affected = db->Query(ost.str());
	for(int i = 0; i < affected; i++)
	{
		allImageSets.insert(stol(db->Get(i, "setID")));
	}

	for(const unsigned long& id: allImageSets)
	{
		ost.str("");
		ost << "select count(*) as count from `feed_message` where `imageSetID`=\"" << id << "\";";
		db->Query(ost.str());
		if(!stoi(db->Get(0, "count")))
		{
			lostImageSets.insert(id);
		}
	}

	ostResult.str("");
	for(const unsigned long& id: lostImageSets)
	{
		ost.str("");
		ost << "select * from `feed_images` where `setID`=\"" << id << "\";";
		for(int i = 0; i < db->Query(ost.str()); i++, lostCount++)
		{
			if(ostResult.str().length()) ostResult << ",";
			ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"setID\":\"" << db->Get(i, "setID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
		}
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownMessage: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetPicturesWithUnknownUser(CMysql *db)
{
	ostringstream					   		ost, ostResult;
	int								 		affected, lostCount = 0;
	unordered_set<string>					allImageOwners;
	unordered_set<unsigned long>			lostImages;

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownUser: start ";
		log.Write(DEBUG, ostTemp.str());
	}

	ostResult.str("");
	ost.str("");
	ost << "SELECT `srcType`,`userID` FROM `feed_images`;";
	affected = db->Query(ost.str());
	for(int i = 0; i < affected; i++)
		allImageOwners.insert(string(db->Get(i, "srcType")) + string(db->Get(i, "userID")));

	for(auto &item: allImageOwners)
	{
		string				tableName = "";
		string				id;
		size_t				pos;

		if((pos = item.find("company")) != string::npos)
		{
			tableName = "company";
			id = item.substr(string("company").length(), item.length() - string("company").length());
		}
		else if((pos = item.find("user")) != string::npos)
		{
			tableName = "users";
			id = item.substr(string("user").length(), item.length() - string("user").length());
		}

		if(tableName.length() && !db->Query("select `id` from `" + tableName + "` where `id`=\"" + id + "\";"))
			lostImages.insert(stol(id));
	}

	ostResult.str("");
	for(const unsigned long& id: lostImages)
	{
		ost.str("");
		ost << "select * from `feed_images` where `userID`=\"" << id << "\";";
		for(int i = 0; i < db->Query(ost.str()); i++, lostCount++)
		{
			if(ostResult.str().length()) ostResult << ",";
			ostResult << "{\"id\":\"" << db->Get(i, "id") << "\",\"srcType\":\"" << db->Get(i, "srcType") << "\",\"userID\":\"" << db->Get(i, "userID") << "\",\"setID\":\"" << db->Get(i, "setID") << "\",\"folder\":\"" << db->Get(i, "folder") << "\",\"filename\":\"" << db->Get(i, "filename") << "\"}";
		}
	}

	{
		CLog	log;
		ostringstream   ostTemp;

		ostTemp.str("");
		ostTemp << "GetPicturesWithUnknownUser: end (# of lost pictures: " << affected << ")";
		log.Write(DEBUG, ostTemp.str());
	}

	return ostResult.str();
}

// --- admin function
string GetRecommendationAdverse(CMysql *db)
{
	ostringstream					   ost, ostResult, dictionaryStatement;
	int								 affected;

	dictionaryStatement.str("");
	ostResult.str("");
	ost.str("");
	ost << "SELECT * FROM `dictionary_adverse`;";
	affected = db->Query(ost.str());
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			if(i) dictionaryStatement << " or ";
			dictionaryStatement << "(`title` like \"%" << db->Get(i, "word") << "%\")";
		}
	}

	ost.str("");
	ost << "select * from `users_recommendation` where `state` = 'unknown' and (" << dictionaryStatement.str() << ");";
	affected = db->Query(ost.str());
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			if(i) ostResult << ",";
			ostResult << "{";
			ostResult << "\"recommendationId\":\"" << db->Get(i, "id") <<"\",";
			ostResult << "\"recommendationRecommended_userID\":\"" << db->Get(i, "recommended_userID") <<"\",";
			ostResult << "\"recommendationRecommending_userID\":\"" << db->Get(i, "recommending_userID") <<"\",";
			ostResult << "\"recommendationTitle\":\"" << db->Get(i, "title") <<"\",";
			ostResult << "\"recommendationEventTimestamp\":\"" << db->Get(i, "eventTimestamp") <<"\",";
			ostResult << "\"recommendationState\":\"" << db->Get(i, "state") <<"\"";
			ostResult << "}";
		}
	}

	return ostResult.str();
}

// --- User notification part
string GetUserAvatarByUserID(string userID, CMysql *db)
{
	ostringstream   ost;
	string		  userAvatar = "";

	ost.str("");
	ost << "select * from `users_avatars` where `userid`='" << userID << "' and `isActive`='1';";
	if(db->Query(ost.str()))
	{
		ost.str("");
		ost << "/images/avatars/avatars" << db->Get(0, "folder") << "/" << db->Get(0, "filename");
		userAvatar = ost.str();
	}

	return userAvatar;
}

// --- function removes message image from FileSystems and cleanup DB
// --- as input require SWL WHERE clause (because of using SELECT and DELETE statements)
// --- input params:
// --- 1) SQL WHERE statement
// --- 2) db reference
void	RemoveMessageImages(string sqlWhereStatement, CMysql *db)
{
	int			 affected;
	ostringstream   ost;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (sqlWhereStatement: " + sqlWhereStatement + ")");
	}

	ost.str("");
	ost << "select * from `feed_images` where " << sqlWhereStatement;
	affected = db->Query(ost.str());
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = db->Get(i, "mediaType");

			if(mediaType == "image" || mediaType == "video")
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(mediaType == "image") filename = IMAGE_FEED_DIRECTORY;
				if(mediaType == "video") filename = VIDEO_FEED_DIRECTORY;

				filename +=  "/";
				filename +=  db->Get(i, "folder");
				filename +=  "/";
				filename +=  db->Get(i, "filename");


				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file is not exists  [filename=" + filename + "]");
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		ost.str("");
		ost << "delete from `feed_images` where " << sqlWhereStatement;
		db->Query(ost.str());
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish");
	}
}

// --- function removes message image from FileSystems and cleanup DB
// --- as input require SWL WHERE clause (because of using SELECT and DELETE statements)
// --- input params:
// --- 1) SQL WHERE statement
// --- 2) db reference
void	RemoveBookCover(string sqlWhereStatement, CMysql *db)
{
	int			 affected;
	ostringstream   ost;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (sqlWhereStatement: " + sqlWhereStatement + ")");
	}

	ost.str("");
	ost << "select * from `book` where " << sqlWhereStatement;
	affected = db->Query(ost.str());
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = "image";

			if(mediaType == "image" || mediaType == "video")
			{

				if(mediaType == "image") filename = IMAGE_BOOKS_DIRECTORY;

				filename +=  "/";
				filename +=  db->Get(i, "coverPhotoFolder");
				filename +=  "/";
				filename +=  db->Get(i, "coverPhotoFilename");

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		ost.str("");
		ost << "UPDATE `book` SET `coverPhotoFolder`=\"\",`coverPhotoFilename`=\"\" where " << sqlWhereStatement;
		db->Query(ost.str());
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish");
	}
}

// --- function removes specified image from FileSystems and cleanup DB
// --- input params:
// --- 1) id
// --- 2) type
// --- 3) db reference
bool	RemoveSpecifiedCover(string itemID, string itemType, CMysql *db)
{
	int			 affected;
	bool			result = true;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: start (itemID = " + itemID + ", itemType = " + itemType + ")");
	}

	affected = db->Query(GetSpecificData_SelectQueryItemByID(itemID, itemType));
	if(affected)
	{
		for(int i = 0; i < affected; i++)
		{
			string  filename = "";
			string  mediaType = "image";

			if(mediaType == "image" || mediaType == "video")
			{

				if(mediaType == "image") filename = GetSpecificData_GetBaseDirectory(itemType);

				filename +=  "/";
				filename +=  db->Get(i, GetSpecificData_GetDBCoverPhotoFolderString(itemType).c_str());
				filename +=  "/";
				filename +=  db->Get(i, GetSpecificData_GetDBCoverPhotoFilenameString(itemType).c_str());

				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: file must be deleted [" + filename + "]");
				}

				if(isFileExists(filename))
				{
					unlink(filename.c_str());
				}
				else
				{
					result = false;
					{
						CLog	log;
						log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
					}
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: mediaType[" + mediaType + "] doesn't have local file");
				}

			}

		}
		// --- cleanup DB with images pre-populated for posted message
		db->Query(GetSpecificData_UpdateQueryItemByID(itemID, itemType, "", ""));
	}

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: finish (result = " + (result ? "true" : "false") + ")");
	}

	return result;
}


bool is_base64(BYTE c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64_encode(BYTE const* buf, unsigned int bufLen)
{
  std::string ret;
  int i = 0;
  int j = 0;
  BYTE char_array_3[3];
  BYTE char_array_4[4];

  while (bufLen--) {
	char_array_3[i++] = *(buf++);
	if (i == 3) {
	  char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	  char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	  char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	  char_array_4[3] = char_array_3[2] & 0x3f;

	  for(i = 0; (i <4) ; i++)
		ret += base64_chars[char_array_4[i]];
	  i = 0;
	}
  }

  if (i)
  {
	for(j = i; j < 3; j++)
	  char_array_3[j] = '\0';

	char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
	char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
	char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
	char_array_4[3] = char_array_3[2] & 0x3f;

	for (j = 0; (j < i + 1); j++)
	  ret += base64_chars[char_array_4[j]];

	while((i++ < 3))
	  ret += '=';
  }

  return ret;
}

string base64_decode(std::string const& encoded_string)
{
  size_t in_len = encoded_string.size();
  size_t i = 0;
  size_t j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
	char_array_4[i++] = encoded_string[in_]; in_++;
	if (i ==4) {
	  for (i = 0; i <4; i++)
		char_array_4[i] = static_cast<unsigned char>(base64_chars.find(char_array_4[i]));

	  char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	  char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	  char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	  for (i = 0; (i < 3); i++)
		ret += char_array_3[i];
	  i = 0;
	}
  }

  if (i) {
	for (j = i; j <4; j++)
	  char_array_4[j] = 0;

	for (j = 0; j <4; j++)
	  char_array_4[j] = static_cast<unsigned char>(base64_chars.find(char_array_4[j]));

	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
	char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

	for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

bool RedirStdoutToFile(string fname)
{
	bool		result = true;
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stdout);
	if(!fRes)
	{
		result = false;
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: redirect stderr to " + fname);
		}
	}

	return  result;
}

bool RedirStderrToFile(string fname)
{
	bool		result = true;
	FILE		*fRes;

	fRes = freopen(fname.c_str(), "w", stderr);
	if(!fRes)
	{
		result = false;
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: redirect stderr to " + fname);
		}
	}

	return  result;
}




int GetSpecificData_GetNumberOfFolders(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "university")				result = UNIVERSITYLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "school")					result = SCHOOLLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "language")					result = FLAG_NUMBER_OF_FOLDERS;
	else if(itemType == "book")						result = BOOKCOVER_NUMBER_OF_FOLDERS;
	else if(itemType == "company")					result = COMPANYLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "company_profile_logo")		result = COMPANYLOGO_NUMBER_OF_FOLDERS;
	else if(itemType == "gift")						result = GIFTIMAGE_NUMBER_OF_FOLDERS;
	else if(itemType == "event")					result = EVENTIMAGE_NUMBER_OF_FOLDERS;
	else if(itemType == "expense_line")				result = EXPENSELINE_NUMBER_OF_FOLDERS;
	else if(itemType == "template_sow")				result = TEMPLATE_SOW_NUMBER_OF_FOLDERS;
	else if(itemType == "template_psow")			result = TEMPLATE_PSOW_NUMBER_OF_FOLDERS;
	else if(itemType == "template_costcenter")		result = TEMPLATE_CC_NUMBER_OF_FOLDERS;
	else if(itemType == "template_company")			result = TEMPLATE_COMPANY_NUMBER_OF_FOLDERS;
	else if(itemType == "template_agreement_company") result = TEMPLATE_AGREEMENT_COMPANY_NUMBER_OF_FOLDERS;
	else if(itemType == "template_agreement_sow")	result = TEMPLATE_AGREEMENT_SOW_NUMBER_OF_FOLDERS;
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

int GetSpecificData_GetMaxFileSize(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_MAX_FILE_SIZE;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_MAX_FILE_SIZE;
	else if(itemType == "university")				result = UNIVERSITYLOGO_MAX_FILE_SIZE;
	else if(itemType == "school")					result = SCHOOLLOGO_MAX_FILE_SIZE;
	else if(itemType == "language")					result = FLAG_MAX_FILE_SIZE;
	else if(itemType == "book")						result = BOOKCOVER_MAX_FILE_SIZE;
	else if(itemType == "company")					result = COMPANYLOGO_MAX_FILE_SIZE;
	else if(itemType == "company_profile_logo")		result = COMPANYLOGO_MAX_FILE_SIZE;
	else if(itemType == "gift")						result = GIFTIMAGE_MAX_FILE_SIZE;
	else if(itemType == "event")					result = EVENTIMAGE_MAX_FILE_SIZE;
	else if(itemType == "expense_line")				result = EXPENSELINE_MAX_FILE_SIZE;
	else if(itemType == "template_sow")				result = TEMPLATE_SOW_MAX_FILE_SIZE;
	else if(itemType == "template_psow")			result = TEMPLATE_PSOW_MAX_FILE_SIZE;
	else if(itemType == "template_costcenter")		result = TEMPLATE_CC_MAX_FILE_SIZE;
	else if(itemType == "template_company")			result = TEMPLATE_COMPANY_MAX_FILE_SIZE;
	else if(itemType == "template_agreement_company") result = TEMPLATE_AGREEMENT_COMPANY_MAX_FILE_SIZE;
	else if(itemType == "template_agreement_sow")	result = TEMPLATE_AGREEMENT_SOW_MAX_FILE_SIZE;
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

unsigned int GetSpecificData_GetMaxWidth(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_MAX_WIDTH;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_MAX_WIDTH;
	else if(itemType == "university")				result = UNIVERSITYLOGO_MAX_WIDTH;
	else if(itemType == "school")					result = SCHOOLLOGO_MAX_WIDTH;
	else if(itemType == "language")					result = FLAG_MAX_WIDTH;
	else if(itemType == "book")						result = BOOKCOVER_MAX_WIDTH;
	else if(itemType == "company")					result = COMPANYLOGO_MAX_WIDTH;
	else if(itemType == "company_profile_logo")		result = COMPANYLOGO_MAX_WIDTH;
	else if(itemType == "gift")						result = GIFTIMAGE_MAX_WIDTH;
	else if(itemType == "event")					result = EVENTIMAGE_MAX_WIDTH;
	else if(itemType == "expense_line")				result = EXPENSELINE_IMAGE_MAX_WIDTH;
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

unsigned int GetSpecificData_GetMaxHeight(string itemType)
{
	int	  result = 0;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = CERTIFICATIONSLOGO_MAX_HEIGHT;
	else if(itemType == "course")					result = CERTIFICATIONSLOGO_MAX_HEIGHT;
	else if(itemType == "university")				result = UNIVERSITYLOGO_MAX_HEIGHT;
	else if(itemType == "school")					result = SCHOOLLOGO_MAX_HEIGHT;
	else if(itemType == "language")					result = FLAG_MAX_HEIGHT;
	else if(itemType == "book")						result = BOOKCOVER_MAX_HEIGHT;
	else if(itemType == "company")					result = COMPANYLOGO_MAX_HEIGHT;
	else if(itemType == "company_profile_logo")		result = COMPANYLOGO_MAX_HEIGHT;
	else if(itemType == "gift")	  					result = GIFTIMAGE_MAX_HEIGHT;
	else if(itemType == "event")	  				result = EVENTIMAGE_MAX_HEIGHT;
	else if(itemType == "expense_line")				result = EXPENSELINE_IMAGE_MAX_HEIGHT;
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + to_string(result) + ")");

	return result;
}

string GetSpecificData_GetBaseDirectory(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")					result = IMAGE_CERTIFICATIONS_DIRECTORY;
	else if(itemType == "course")					result = IMAGE_CERTIFICATIONS_DIRECTORY;
	else if(itemType == "university")				result = IMAGE_UNIVERSITIES_DIRECTORY;
	else if(itemType == "school")					result = IMAGE_SCHOOLS_DIRECTORY;
	else if(itemType == "language")					result = IMAGE_FLAGS_DIRECTORY;
	else if(itemType == "book")						result = IMAGE_BOOKS_DIRECTORY;
	else if(itemType == "company")					result = IMAGE_COMPANIES_DIRECTORY;
	else if(itemType == "company_profile_logo")		result = IMAGE_COMPANIES_DIRECTORY;
	else if(itemType == "gift")						result = IMAGE_GIFTS_DIRECTORY;
	else if(itemType == "event")					result = IMAGE_EVENTS_DIRECTORY;
	else if(itemType == "expense_line")				result = IMAGE_EXPENSELINES_DIRECTORY;
	else if(itemType == "template_sow")				result = TEMPLATE_SOW_DIRECTORY;
	else if(itemType == "template_psow")			result = TEMPLATE_PSOW_DIRECTORY;
	else if(itemType == "template_costcenter")		result = TEMPLATE_CC_DIRECTORY;
	else if(itemType == "template_company")			result = TEMPLATE_COMPANY_DIRECTORY;
	else if(itemType == "template_agreement_company") result = TEMPLATE_AGREEMENT_COMPANY_DIRECTORY;
	else if(itemType == "template_agreement_sow")	result = TEMPLATE_AGREEMENT_SOW_DIRECTORY;
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_GetFinalFileExtenstion(string itemType)
{
	string	  result = ".jpg";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "template_sow")						result = ".txt";
	else if(itemType == "template_psow")				result = ".txt";
	else if(itemType == "template_costcenter")			result = ".txt";
	else if(itemType == "template_company")				result = ".txt";
	else if(itemType == "template_agreement_company")	result = ".txt";
	else if(itemType == "template_agreement_sow")		result = ".txt";
	else
	{
		MESSAGE_DEBUG("", "", "default extension(" + result + ") taken");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_SelectQueryItemByID(string itemID, string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")						result = "SELECT * FROM `certification_tracks` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "course")						result = "SELECT * FROM `certification_tracks` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "university")					result = "SELECT * FROM `university` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "school")						result = "SELECT * FROM `school` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "language")						result = "SELECT * FROM `language` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "book")							result = "SELECT * FROM `book` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "company")						result = "SELECT * FROM `company` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "company_profile_logo")			result = "SELECT * FROM `company` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "gift")							result = "SELECT * FROM `gifts` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "event")						result = "SELECT * FROM `events` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "template_sow")					result = "SELECT * FROM `contract_sow_custom_fields` WHERE `id`=\"" + itemID + "\" AND `type`=\"file\";";
	else if(itemType == "template_psow")				result = "SELECT * FROM `contract_psow_custom_fields` WHERE `id`=\"" + itemID + "\" AND `type`=\"file\";";
	else if(itemType == "template_costcenter")			result = "SELECT * FROM `cost_center_custom_fields` WHERE `id`=\"" + itemID + "\" AND `type`=\"file\";";
	else if(itemType == "template_company")				result = "SELECT * FROM `company_custom_fields` WHERE `id`=\"" + itemID + "\" AND `type`=\"file\";";
	else if(itemType == "template_agreement_company")	result = "SELECT * FROM `company_agreement_files` WHERE `id`=\"" + itemID + "\";";
	else if(itemType == "template_agreement_sow")		result = "SELECT * FROM `contract_sow_agreement_files` WHERE `id`=\"" + itemID + "\";";
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_UpdateQueryItemByID(string itemID, string itemType, string folderID, string fileName)
{
	string		result = "";
	string		logo_folder = "";
	string		logo_filename = "";

	MESSAGE_DEBUG("", "", "start");

	logo_folder = GetSpecificData_GetDBCoverPhotoFolderString(itemType);
	logo_filename = GetSpecificData_GetDBCoverPhotoFilenameString(itemType);

	if(logo_filename.length())
	{
		if(itemType == "certification")						result = "UPDATE `certification_tracks` 		SET	`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "course")						result = "UPDATE `certification_tracks` 		SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "university")					result = "UPDATE `university`					SET	`" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "school")						result = "UPDATE `school`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "language")						result = "UPDATE `language`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "book")							result = "UPDATE `book`							SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "company")						result = "UPDATE `company`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "company_profile_logo")			result = "UPDATE `company`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "gift")							result = "UPDATE `gifts`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "event")						result = "UPDATE `events`						SET `" + logo_folder + "`='" + folderID + "', `" + logo_filename + "`='" + fileName + "' WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_sow")					result = "UPDATE `contract_sow_custom_fields`	SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_psow")				result = "UPDATE `contract_psow_custom_fields`	SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_costcenter")			result = "UPDATE `cost_center_custom_fields`	SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_company")				result = "UPDATE `company_custom_fields`		SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_agreement_company")	result = "UPDATE `company_agreement_files`		SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else if(itemType == "template_agreement_sow")		result = "UPDATE `contract_sow_agreement_files`	SET `" + logo_filename + "`='" + folderID + "/" + fileName + "', `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`=\"" + itemID + "\";";
		else
		{
			MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "logo_filename not found for itemType (" + itemType + ")");
	}


	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_GetDBCoverPhotoFolderString(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")	 					result = "logo_folder";
	else if(itemType == "course")	   					result = "logo_folder";
	else if(itemType == "university")   				result = "logo_folder";
	else if(itemType == "school")	   					result = "logo_folder";
	else if(itemType == "language")	 					result = "logo_folder";
	else if(itemType == "book")		 					result = "coverPhotoFolder";
	else if(itemType == "company")						result = "logo_folder";
	else if(itemType == "company_profile_logo")			result = "logo_folder";
	else if(itemType == "gift")	  						result = "logo_folder";
	else if(itemType == "event")	  					result = "logo_folder";
	else if(itemType == "template_sow")					result = "";
	else if(itemType == "template_psow")				result = "";
	else if(itemType == "template_costcenter")			result = "";
	else if(itemType == "template_company")				result = "";
	else if(itemType == "template_agreement_company")	result = "";
	else if(itemType == "template_agreement_sow")		result = "";
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_GetDBCoverPhotoFilenameString(string itemType)
{
	string	  result = "";

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "certification")						result = "logo_filename";
	else if(itemType == "course")						result = "logo_filename";
	else if(itemType == "university")					result = "logo_filename";
	else if(itemType == "school")						result = "logo_filename";
	else if(itemType == "language")						result = "logo_filename";
	else if(itemType == "book")							result = "coverPhotoFilename";
	else if(itemType == "company")						result = "logo_filename";
	else if(itemType == "company_profile_logo")			result = "logo_filename";
	else if(itemType == "gift")							result = "logo_filename";
	else if(itemType == "event")						result = "logo_filename";
	else if(itemType == "template_sow")					result = "value";
	else if(itemType == "template_psow")				result = "value";
	else if(itemType == "template_costcenter")			result = "value";
	else if(itemType == "template_company")				result = "value";
	else if(itemType == "template_agreement_company")	result = "filename";
	else if(itemType == "template_agreement_sow")		result = "filename";
	else
	{
		MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
	}

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}

string GetSpecificData_GetDataTypeByItemType(const string &itemType)
{
	auto	result = "image"s;

	MESSAGE_DEBUG("", "", "start");

	if(itemType == "template_sow")					result = "template";
	if(itemType == "template_psow")					result = "template";
	if(itemType == "template_costcenter")			result = "template";
	if(itemType == "template_company")				result = "template";
	if(itemType == "template_agreement_company")	result = "template";
	if(itemType == "template_agreement_sow")		result = "template";

	MESSAGE_DEBUG("", "", "finish (result = " + result + ")");

	return result;
}


// --- Does the owner user allowed to change it ?
// --- For example:
// ---	*) university or school logo can be changed by administartor only.
// ---	*) gift image could be changed by owner
bool GetSpecificData_AllowedToChange(string itemID, string itemType, CMysql *db, CUser *user)
{
	bool	  result = false;

	MESSAGE_DEBUG("", "", "start");


	if(itemType == "template_sow")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `agency_company_id` FROM `contracts_sow` WHERE `id`=("
								"SELECT `contract_sow_id` FROM `contract_sow_custom_fields` WHERE `id`=\"" + itemID + "\""
							")"
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change contract_sow_custom_fields.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") must be agency employee to change");
		}
		
	}
	else if(itemType == "template_psow")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `agency_company_id` FROM `contracts_sow` WHERE `id`=("
								"SELECT `contract_sow_id` FROM `contracts_psow` WHERE `id`=("
									"SELECT `contract_psow_id` FROM `contract_psow_custom_fields` WHERE `id`=\"" + itemID + "\""
								")"
							")"
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change contract_psow_custom_fields.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") must be agency employee to change");
		}
		
	}
	else if(itemType == "template_costcenter")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `agency_company_id` FROM `cost_centers` WHERE `id`=("
								"SELECT `cost_center_id` FROM `cost_center_custom_fields` WHERE `id`=\"" + itemID + "\""
							")"
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change cost_center_custom_fields.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") must be agency employee to change");
		}
		
	}
	else if(itemType == "template_company")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `company_custom_fields` WHERE `id`=\"" + itemID + "\""
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change company_custom_fields.id(" + itemID + ")");
			}
		}
		else if((user->GetType() == "subcontractor"))
		{
			if(db->Query("SELECT `id` FROM `company` WHERE `admin_userID`=\"" + user->GetID() + "\" AND `id`=("
							"SELECT `company_id` FROM `company_custom_fields` WHERE `id`=\"" + itemID + "\""
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change company_custom_fields.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") not allowerd to change company custom field");
		}
		
	}
	else if(itemType == "template_agreement_company")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `company_agreement_files` WHERE `id`=\"" + itemID + "\""
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change company_agreement_files.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") not allowerd to change company agreement files");
		}
	}
	else if(itemType == "template_agreement_sow")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `contract_sow_agreement_files` WHERE `id`=\"" + itemID + "\""
						");"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "user.id(" + user->GetID() + ") doesn't allowed to change contract_sow_agreement_files.id(" + itemID + ")");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") not allowerd to change contract_sow agreement files");
		}
	}
	else if(itemType == "company_profile_logo")
	{
		if((user->GetType() == "subcontractor"))
		{
			if(db->Query("SELECT `id` FROM `company` WHERE `id`=\"" + itemID + "\" AND `admin_userID`=\"" + user->GetID() + "\";"))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "subcontractor user.id(" + user->GetID() + ") doesn't allowed to change company.id(" + itemID + ") logo");
			}
		}
		else if((user->GetType() == "agency"))
		{
			if(db->Query(	"SELECT `id` FROM `company_employees` WHERE `company_id`=("
								"SELECT `company_id` FROM `company_employees` WHERE `user_id`=("
									"SELECT `admin_userID` FROM `company` WHERE `id`=\"" + itemID + "\""
								")"
							") AND `user_id`=\"" + user->GetID() + "\" AND `allowed_change_agency_data`=\"Y\""))
				result = true;
			else
			{
				MESSAGE_DEBUG("", "", "agency user.id(" + user->GetID() + ") doesn't allowed to change company.id(" + itemID + ") logo");
			}
		}
		else
		{
			MESSAGE_DEBUG("", "", "user.type(" + user->GetType() + ") not allowed to change");
		}

	}
	else if(db->Query(GetSpecificData_SelectQueryItemByID(itemID, itemType))) // --- item itemID exists ?
	{
		if((itemType == "course") || (itemType == "university") || (itemType == "school") || (itemType == "language") || (itemType == "book") || (itemType == "company") || (itemType == "certification"))
		{
			string	  coverPhotoFolder = db->Get(0, GetSpecificData_GetDBCoverPhotoFolderString(itemType).c_str());
			string	  coverPhotoFilename = db->Get(0, GetSpecificData_GetDBCoverPhotoFilenameString(itemType).c_str());

			if(coverPhotoFolder.empty() && coverPhotoFilename.empty())
				result = true;
			else
			{
				result = false;
				{
					CLog	log;
					MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, because logo already uploaded");
				}
			}
		}
		else if(itemType == "event")
		{
			if(user)
			{
				if(db->Query("SELECT `id` FROM `event_hosts` WHERE `event_id`=\"" + itemID + "\" AND `user_id`=\"" + user->GetID() + "\";"))
					result = true;
				else
				{
					result = false;
					{
						CLog	log;
						MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not the event host");
					}
				}
			}
			else
			{
				result = false;

				MESSAGE_ERROR("", "", "user object is NULL")
			}
		}
		else if(itemType == "gift")
		{
			string		user_id = db->Get(0, "user_id");

			if(user)
			{
				if(user_id == user->GetID())
					result = true;
				else
				{
					result = false;
					{
						CLog	log;
						MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not the gift owner");
					}
				}
			}
			else
			{
				result = false;

				MESSAGE_ERROR("", "", "user object is NULL")
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "itemType (" + itemType + ") is unknown");
		}
	}
	else
	{
		result = false;
		
		MESSAGE_ERROR("", "", itemType + "(" + itemID + ") not found");
	}

	MESSAGE_DEBUG("", "", "finish (result: " + (result ? "true" : "false") + ")");

	return result;
}

auto isAllowed_NoSession_Action(string action) -> bool
{
	auto			result = false;
	vector<string>	allowed_nosession_actions = ALLOWED_NO_SESSION_ACTION;

	MESSAGE_DEBUG("", "", "start (action " + action + ")");

	for(auto &allowed_nosession_action : allowed_nosession_actions)
	{
		if(action == allowed_nosession_action)
		{
			result = true;
			break;
		}
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

pair<struct tm, struct tm> GetFirstAndLastDateOfThisMonth()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm end_of_curr_mon, start_of_curr_mon;

	MESSAGE_DEBUG("", "", "start");

	start_of_curr_mon.tm_sec	= 0;   // seconds of minutes from 0 to 61
	start_of_curr_mon.tm_min	= 0;   // minutes of hour from 0 to 59
	start_of_curr_mon.tm_hour	= 0;  // hours of day from 0 to 24
	start_of_curr_mon.tm_mday	= 0;  // day of month from 1 to 31
	start_of_curr_mon.tm_mon	= 0;   // month of year from 0 to 11
	start_of_curr_mon.tm_year	= 0;  // year since 1900
	start_of_curr_mon.tm_wday	= 0;  // days since sunday
	start_of_curr_mon.tm_yday	= 0;  // days since January 1st
	start_of_curr_mon.tm_isdst	= 0; // hours of daylight savings time

	end_of_curr_mon.tm_sec	= 0;   // seconds of minutes from 0 to 61
	end_of_curr_mon.tm_min	= 0;   // minutes of hour from 0 to 59
	end_of_curr_mon.tm_hour	= 0;  // hours of day from 0 to 24
	end_of_curr_mon.tm_mday	= 0;  // day of month from 1 to 31
	end_of_curr_mon.tm_mon	= 0;   // month of year from 0 to 11
	end_of_curr_mon.tm_year	= 0;  // year since 1900
	end_of_curr_mon.tm_wday	= 0;  // days since sunday
	end_of_curr_mon.tm_yday	= 0;  // days since January 1st
	end_of_curr_mon.tm_isdst	= 0; // hours of daylight savings time

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	start_of_curr_mon.tm_year	= timeinfo->tm_year;
	start_of_curr_mon.tm_mon	= timeinfo->tm_mon;
	start_of_curr_mon.tm_mday	= 1;

	end_of_curr_mon.tm_year		= start_of_curr_mon.tm_year;
	end_of_curr_mon.tm_mon		= start_of_curr_mon.tm_mon + 1;
	end_of_curr_mon.tm_mday		= 0;

	mktime(&start_of_curr_mon);
	mktime(&end_of_curr_mon);

	MESSAGE_DEBUG("", "", "finish");

	return make_pair(start_of_curr_mon, end_of_curr_mon);
}

pair<struct tm, struct tm> GetFirstAndLastDateOfLastMonth()
{
	time_t rawtime;
	struct tm * timeinfo;

	struct tm end_of_last_mon, start_of_last_mon;

	MESSAGE_DEBUG("", "", "start");

	start_of_last_mon.tm_sec	= 0;   // seconds of minutes from 0 to 61
	start_of_last_mon.tm_min	= 0;   // minutes of hour from 0 to 59
	start_of_last_mon.tm_hour	= 0;  // hours of day from 0 to 24
	start_of_last_mon.tm_mday	= 0;  // day of month from 1 to 31
	start_of_last_mon.tm_mon	= 0;   // month of year from 0 to 11
	start_of_last_mon.tm_year	= 0;  // year since 1900
	start_of_last_mon.tm_wday	= 0;  // days since sunday
	start_of_last_mon.tm_yday	= 0;  // days since January 1st
	start_of_last_mon.tm_isdst	= 0; // hours of daylight savings time

	end_of_last_mon.tm_sec	= 0;   // seconds of minutes from 0 to 61
	end_of_last_mon.tm_min	= 0;   // minutes of hour from 0 to 59
	end_of_last_mon.tm_hour	= 0;  // hours of day from 0 to 24
	end_of_last_mon.tm_mday	= 0;  // day of month from 1 to 31
	end_of_last_mon.tm_mon	= 0;   // month of year from 0 to 11
	end_of_last_mon.tm_year	= 0;  // year since 1900
	end_of_last_mon.tm_wday	= 0;  // days since sunday
	end_of_last_mon.tm_yday	= 0;  // days since January 1st
	end_of_last_mon.tm_isdst	= 0; // hours of daylight savings time


	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	start_of_last_mon.tm_year	= timeinfo->tm_year;
	start_of_last_mon.tm_mon	= timeinfo->tm_mon - 1;
	start_of_last_mon.tm_mday	= 1;

	end_of_last_mon.tm_year		= start_of_last_mon.tm_year;
	end_of_last_mon.tm_mon		= start_of_last_mon.tm_mon + 1;
	end_of_last_mon.tm_mday		= 0;

	mktime(&start_of_last_mon);
	mktime(&end_of_last_mon);

	MESSAGE_DEBUG("", "", "finish");

	return make_pair(start_of_last_mon, end_of_last_mon);
}

auto GetSpellingFormattedDate(string date, string format) -> string
{
/*	MESSAGE_DEBUG("", "", "start");

	auto	result = ""s;
	char	buffer[100];
	auto	date_obj = GetTMObject(date);

	if(strftime(buffer, sizeof(buffer), format.c_str(), &date_obj))
	{
		result = buffer;
	}
	else
	{
		MESSAGE_ERROR("", "", "strftime returns 0");
	}


	MESSAGE_DEBUG("", "", "finish");

	return result;
*/
	return GetSpellingFormattedDate(GetTMObject(date), format);
}

auto GetSpellingFormattedDate(struct tm date_obj, string format) -> string
{
	MESSAGE_DEBUG("", "", "start");

	auto	result = ""s;
	char	buffer[100];

	mktime(&date_obj);

	if(strftime(buffer, sizeof(buffer), format.c_str(), &date_obj))
	{
		result = buffer;
	}
	else
	{
		MESSAGE_ERROR("", "", "strftime returns 0");
	}


	MESSAGE_DEBUG("", "", "finish");

	return result;
}



struct tm GetTMObject(string date)
{
	struct tm	result;
	smatch		sm;

	MESSAGE_DEBUG("", "", "start");
	
	result.tm_sec	= 0;   // seconds of minutes from 0 to 61
	result.tm_min	= 0;   // minutes of hour from 0 to 59
	result.tm_hour	= 0;  // hours of day from 0 to 24
	result.tm_mday	= 1;  // day of month from 1 to 31
	result.tm_mon	= 1;   // month of year from 0 to 11
	result.tm_year	= 70;  // year since 1900
	result.tm_wday	= 0;  // days since sunday
	result.tm_yday	= 0;  // days since January 1st
	result.tm_isdst	= 0; // hours of daylight savings time

	// --- normalize start, end format
	if(regex_match(date, sm, regex("^([[:digit:]]{1,2})\\/([[:digit:]]{1,2})\\/([[:digit:]]{2,4})$")))
	{
		result.tm_mday = stoi(sm[1]);
		result.tm_mon = stoi(sm[2]) - 1;
		if(stoi(sm[3]) < 100)
			result.tm_year = 2000 + stoi(sm[3]) - 1900;
		else
			result.tm_year = stoi(sm[3]) - 1900;
	}
	else if(regex_match(date, sm, regex("^([[:digit:]]{2,4})\\-([[:digit:]]{1,2})\\-([[:digit:]]{1,2})$")))
	{
		result.tm_mday = stoi(sm[3]);
		result.tm_mon = stoi(sm[2]) - 1;
		if(stoi(sm[1]) < 100)
			result.tm_year = 2000 + stoi(sm[1]) - 1900;
		else
			result.tm_year = stoi(sm[1]) - 1900;
	}
	else
	{
		MESSAGE_ERROR("", "", "incorrect date(" + date + ") format");
	}

	MESSAGE_DEBUG("", "", "finish");

	return result;
}

bool operator <(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) > 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator <=(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) >= 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator >(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) < 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator >=(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) <= 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

bool operator ==(const struct tm &param_1, const struct tm &param_2)
{
	bool		result = false;
	struct tm	tm_1 = param_1;
	struct tm	tm_2 = param_2;

	MESSAGE_DEBUG("", "", "start");

	if(difftime(mktime(&tm_2), mktime(&tm_1)) == 0) result = true;

	MESSAGE_DEBUG("", "", "finish (result = " + (result ? "true" : "false") + ")");

	return result;
}

string PrintDate(const struct tm &_tm)
{
	string	result = "";

	result = to_string(_tm.tm_mday) + "/" + to_string(_tm.tm_mon + 1) + "/" + to_string(_tm.tm_year + 1900);

	return result;
}

string PrintSQLDate(const struct tm &_tm)
{
	string	result = "";

	result = to_string(_tm.tm_year + 1900) + "-" + to_string(_tm.tm_mon + 1) + "-" + to_string(_tm.tm_mday);

	return result;
}

string PrintDateTime(const struct tm &_tm)
{
	return PrintTime(_tm, "%T");
}

string PrintTime(const struct tm &_tm, string format)
{
	char		buffer[256];
	string		result = "";
	struct tm	temp_tm = _tm;

	mktime(&temp_tm);
	memset(buffer, 0, sizeof(buffer));
	strftime(buffer, 255, format.c_str(), &temp_tm);
	result = buffer;

	return result;
}

