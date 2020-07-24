#include "index.h"


// --- Generating image with text "randStr"
// --- Input: randStr - text needs to be written on the image
// --- Output: path to the file
static string GenerateImage(string randStr)
{
	string	fileName = "dessin.gif", fileFont, fileResultFull, fileResult;
	string	annotateFlag = "yes";

	fileName = IMAGE_DIRECTORY;
	fileName += "pages/login/dessin.gif";

	if(!fileName.empty())
	{
		if(annotateFlag == "yes")
		{
			string				fileNameWater;
			Magick::Image		imageMaster, imageDest;
			// ImageInfo	*image1_info, *anotherInfo;
			// char			geometry[128];
			ostringstream 	ost;

			Magick::InitializeMagick(NULL);

			try
			{
				bool 		fileFlagExist;

				imageMaster.read(fileName);
				imageDest = imageMaster;
				imageDest.fontPointsize(14);
				imageDest.addNoise(Magick::GaussianNoise);
				imageDest.addNoise(Magick::GaussianNoise);
				ost.str("");
				ost << "+" << 1 + (int)(rand()/(RAND_MAX + 1.0) * 45) << "+" << 13 + (int)(rand()/(RAND_MAX + 1.0) * 10);
				imageDest.annotate(randStr, Magick::Geometry(ost.str()));


				fileFlagExist = true;
				do {
					MESSAGE_DEBUG("", "", "checking captha file existance");

					fileResult = "_";
					fileResult += GetRandom(10);
					fileResult += ".gif";
					fileResultFull = IMAGE_CAPTCHA_DIRECTORY;
					fileResultFull += fileResult;
					int fh = open(fileResultFull.c_str(), O_RDONLY);
					if(fh < 0)
					{
						fileFlagExist = false;

						MESSAGE_DEBUG("", "", ": trying file " + fileResultFull + " -> can be used for writing");
					}
					else
					{
						close(fh);

						MESSAGE_DEBUG("", "", ": trying file " + fileResultFull + " -> can't be used, needs another one");
					}
				} while(fileFlagExist == true);

				MESSAGE_DEBUG("", "", ": write captcha-image to " + fileResultFull);

				imageDest.write(fileResultFull);

			}
			catch(Magick::Exception &error_)
			{
				MESSAGE_ERROR("", "", "Caught exception: " + error_.what())

				fileResult = "";
			}
		}
	}
	return fileResult;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be firts statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	{
		MESSAGE_DEBUG("", action, __FILE__);
	}

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	try
	{

		indexPage.ParseURL();
		indexPage.AddCookie("lng", "ru", nullptr, "", "/");

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(DB_NAME, DB_LOGIN, DB_PASSWORD) < 0)
		{
			MESSAGE_ERROR("", action, "can't connect to DB");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		{
			MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);
		}

	// ------------ generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &indexPage, &db, &user);
		}
	// ------------ end generate common parts

		{
			MESSAGE_DEBUG("", action, "start (action's == \"" + action + "\") condition");
		}

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

			if(!indexPage.SetTemplate(template_name))
			{
				MESSAGE_DEBUG("", action, "can't find template " + template_name);
			} // if(!indexPage.SetTemplate("my_network.htmlt"))

			{
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(
			(action == LOGGEDIN_DOCTOR_DEFAULT_ACTION) ||
			(action == LOGGEDIN_NOROLE_DEFAULT_ACTION)
		   )
		{
			auto		template_name = action + ".htmlt";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");
				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(!indexPage.SetTemplate(template_name))
			{
				MESSAGE_DEBUG("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "setlang")
		{
			string		lng;

			lng = indexPage.GetVarsHandler()->Get("lng");
			indexPage.AddCookie("lng", lng, nullptr, "", "/");

			if(!indexPage.SetTemplate("index.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file index.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		if(action == "autologin")
		{
			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(!indexPage.SetTemplate("autologin.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file index.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		// --- JSON part has started
	    if(action == "AJAX_getUserWall")
	    {
	        ostringstream   ost;
	        string          strPageToGet, strNewsOnSinglePage;
	        int             currPage = 0, newsOnSinglePage = 0;
	        vector<int>     vectorFriendList;
	        string			userLogin = "", userID = "", result = "";

			{
			    
			    MESSAGE_DEBUG("", action, "start");
			}

	        userID			    = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("id"));
	        userLogin		    = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
	        strNewsOnSinglePage = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
	        strPageToGet        = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));

	        if(strPageToGet.empty()) strPageToGet = "0";

	        {
	            
	            MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
	        }

	        try{
	            currPage = stoi(strPageToGet);
	        } catch(...) {
	            currPage = 0;
	        }

	        try{
	            newsOnSinglePage = stoi(strNewsOnSinglePage);
	        } catch(...) {
	            newsOnSinglePage = 30;
	        }

/*
	        if(user.GetLogin() == "Guest")
	        {
	            ostringstream   ost;

	            {
	                
	                MESSAGE_DEBUG("", action, "re-login required");
	            }

	            indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
	        }
*/

	        // --- define userID from userLogin
	        if(userID.length())
	        {

	        }
	        else if(userLogin.length())
	        {
				if(db.Query("SELECT `id` FROM `users` WHERE `login`=\"" + userLogin + "\" AND `isblocked`=\"N\";"))
					userID = db.Get(0, "id");
				else
				{
					{
						
						MESSAGE_DEBUG("", action, "attempt to get a wall of unknown/blocked user (" + userLogin + ")");
					}
				}
	        }
	        else
	        {
	        	userID = user.GetID();
	        }

			if(userID.length() && db.Query("SELECT `id` FROM `users` WHERE `id`=\"" + userID + "\" AND `isblocked`=\"N\";"))
			{
	        	string		whereQuery = "((`feed`.`userId` in (" + userID + ")) AND (`feed`.`srcType` = \"user\") AND (`feed`.`dstType` = \"\"))";

				result = GetNewsFeedInJSONFormat(whereQuery, currPage, newsOnSinglePage, &user, &db);
		        indexPage.RegisterVariableForce("result", "{"
		        											"\"status\":\"success\","
		        											"\"feed\":[" + result + "]"
		        											"}");
			}
			else
			{
				{
	                
	                MESSAGE_DEBUG("", action, "attempt to get a wall of unknown/blocked user (" + userLogin + ")");
				}

		        indexPage.RegisterVariableForce("result", "{\"status\":\"error\",\"description\":\"user blocked or not found\"}");
			}


	        if(!indexPage.SetTemplate("json_response.htmlt"))
	        {
	            MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
	            throw CExceptionHTML("user not activated");
	        } // if(!indexPage.SetTemplate("json_response.htmlt"))

			{
			    
			    MESSAGE_DEBUG("", action, "finish");
			}
	    }

	    // --- JSON news feed
	    if(action == "AJAX_getNewsFeed")
	    {
	        string          strPageToGet, strNewsOnSinglePage;
	        auto			strFriendList = ""s;
	        auto			strCompaniesList = ""s;
	        auto			strGroupsList = ""s;
	        auto			strEventsList = ""s;
	        auto            currPage = 0, newsOnSinglePage = 0;

	        strNewsOnSinglePage = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
	        strPageToGet        = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));

	        if(strPageToGet.empty()) strPageToGet = "0";

            MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");

	        try{
	            currPage = stoi(strPageToGet);
	        } catch(...) {
	            currPage = 0;
	        }

	        try{
	            newsOnSinglePage = stoi(strNewsOnSinglePage);
	        } catch(...) {
	            newsOnSinglePage = 30;
	        }

	        if(user.GetLogin() == "Guest")
	        {
				MESSAGE_DEBUG("", action, "re-login required");

		        indexPage.RegisterVariableForce("result", "{\"status\":\"error\",\"description\":\"re-login required\",\"link\":\"/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10) + "\"}");
	        }
	        else
	        {
		        vector<string>  vectorFriendList;
		        ostringstream   ost;
		        string			companies_i_own_list = "";
		        int             affected;

		        // --- building friend list
		        {
			        ost.str("");
			        ost << "SELECT `users_friends`.`friendID` "
			                "FROM `users_friends` "
			                "LEFT JOIN `users` ON `users`.`id`=`users_friends`.`friendID` "
			                "WHERE `users_friends`.`userID`='" << user.GetID() << "' and `users_friends`.`state`=\"confirmed\" and `users`.`isactivated`=\"Y\" and `users`.`isblocked`=\"N\";";

			        affected = db.Query(ost.str());
			        for(int i = 0; i < affected; i++)
			            vectorFriendList.push_back(db.Get(i, "friendID"));

			        ost.str("");
			        for(auto it = vectorFriendList.begin(); it != vectorFriendList.end(); ++it)
			        {
			            if(it != vectorFriendList.begin()) ost << ",";
			            ost << *it;
			        }
			        strFriendList = ost.str();
			        if(strFriendList.length() > 0) strFriendList += ",";
			        strFriendList += user.GetID();
		        }

		        // --- building subscription company list
		        {
		        	affected = db.Query("SELECT `id` FROM `company` WHERE `isBlocked`=\"N\" AND `id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\" AND `entity_type`=\"company\");");

					strCompaniesList = "";
			        for(int i = 0; i < affected; i++)
			        {
			        	if(strCompaniesList.length()) strCompaniesList += ",";
			        	strCompaniesList += db.Get(i, "id");
			        }
		        }

		        // --- building subscription group list
		        {
		        	affected = db.Query("SELECT `id` FROM `groups` WHERE `isBlocked`=\"N\" AND `id` IN (SELECT `entity_id` FROM `users_subscriptions` WHERE `user_id`=\"" + user.GetID() + "\" AND `entity_type`=\"group\");");

					strGroupsList = "";
			        for(int i = 0; i < affected; i++)
			        {
			        	if(strGroupsList.length()) strGroupsList += ",";
			        	strGroupsList += db.Get(i, "id");
			        }
		        }

		        // --- building company owning list
		        {
		        	affected = db.Query("SELECT `id` FROM `company` WHERE `isBlocked`=\"N\" AND `admin_userID`=\"" + user.GetID() + "\";");

					companies_i_own_list = "";
			        for(int i = 0; i < affected; i++)
			        {
			        	if(companies_i_own_list.length()) companies_i_own_list += ",";
			        	companies_i_own_list += db.Get(i, "id");
			        }
		        }

		        // --- building event list
		        {
		        	affected = db.Query("SELECT `id` FROM `events` WHERE `isBlocked`=\"N\""
		        														" AND ("
		        															"`id` IN (SELECT `event_id` FROM `event_guests` WHERE `user_id`=\"" + user.GetID() + "\")"
			        														" OR "
		        															"`id` IN (SELECT `event_id` FROM `event_hosts` WHERE `user_id`=\"" + user.GetID() + "\")"
		        														" );");

					strEventsList = "";
			        for(int i = 0; i < affected; i++)
			        {
			        	if(strEventsList.length()) strEventsList += ",";
			        	strEventsList += db.Get(i, "id");
			        }
		        }


		        {
		        	string		whereQuery;

		        	whereQuery = "((`feed`.`userId` in (" + strFriendList + ")) AND (`feed`.`srcType` = \"user\") AND (`feed`.`dstType` = \"\"))";

		        	if(strCompaniesList.length())
		        		whereQuery += " OR ((`feed`.`userId` in (" + strCompaniesList + ")) AND (`feed`.`srcType` = \"company\"))";

		        	if(strGroupsList.length())
		        		whereQuery += " OR ((`feed`.`dstID` in (" + strGroupsList + ")) AND (`feed`.`dstType` = \"group\"))";

		        	if(strEventsList.length())
		        		whereQuery += " OR ((`feed`.`dstID` in (" + strEventsList + ")) AND (`feed`.`dstType` = \"event\"))";

			        indexPage.RegisterVariableForce("result", "{"
			        											"\"status\":\"success\","
			        											"\"my_companies\":[" + companies_i_own_list + "],"
			        											"\"feed\":[" + GetNewsFeedInJSONFormat(whereQuery, currPage, newsOnSinglePage, &user, &db) + "]"
			        											"}");
		        }
		    }

	        if(!indexPage.SetTemplate("json_response.htmlt"))
	        {
	            MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
	            throw CExceptionHTML("user not activated");
	        } // if(!indexPage.SetTemplate("json_response.htmlt"))
	    }

		// --- AJAX delete message FROM news feed
		if(action == "AJAX_deleteNewsFeedMessage")
		{
			ostringstream	ostFinal;
			CMysql			db1;
			string			messageID;

			messageID	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageID"));

			MESSAGE_DEBUG("", action, "start [" + messageID + "]");

			if(messageID.empty())
			{
				
				MESSAGE_DEBUG("", action, "messageID is not defined");

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"messageID is empty\"";
				ostFinal << "}";
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					{
						
						MESSAGE_DEBUG("", action, "re-login required");
					}

					indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
				}


				if(AmIMessageOwner(messageID, &user, &db))
				{
					pair<string, string> 	messageOwner = GetMessageOwner(messageID, &user, &db);
					string					messageOwnerType = messageOwner.first;
					string					messageOwnerID = messageOwner.second;

					if(messageOwnerType.length() && messageOwnerID.length())
					{
						db.Query("DELETE FROM `feed` WHERE `srcType`=\"" + messageOwnerType + "\" AND `userId`='" + messageOwnerID + "' AND `actionTypeId`='11' and `actionId`='" + messageID + "';");

						if(db.Query("SELECT `imageSetID` FROM `feed_message` WHERE `id`='" + messageID + "';"))
						{
							ostringstream	ost;
							string			imageSetID = db.Get(0, "imageSetID");

							ost.str("");
							ost << " `setID`='" << imageSetID << "' AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" << messageOwnerID << "\" ";
							RemoveMessageImages(ost.str(), &db);
						} // --- if ("SELECT * FROM `feed_message` WHERE `id`='" << messageID << "';";)

						db.Query("DELETE FROM `feed_message` WHERE `id`=\"" + messageID + "\";");

						// --- removing likes / dislikes and notifications
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"50\" and `actionId`='" + messageID + "';");
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId` in (SELECT `id` FROM `feed_message_params` WHERE `messageID`='" + messageID + "' and `parameter`=\"like\");");
						db.Query("DELETE FROM `feed_message_params` WHERE `messageID`='" + messageID + "';");

						// --- removing comments and notifications
						db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"19\" and `actionId` in (SELECT `id` FROM `feed_message_comment` WHERE `messageID`='" + messageID + "' and `type`=\"message\");");
						db.Query("DELETE FROM `feed_message_comment` WHERE `messageID`='" + messageID + "' and `type`=\"message\";");

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"\"";
						ostFinal << "}";
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
						ostFinal << "}";
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "message doesn't belongs to you");
					}

					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"Ошибка: Вы не можете удалить чужое сообщение.\"";
					ostFinal << "}";
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish (messageID " + messageID + ")");
			}

		}

		// --- AJAX delete comment FROM message
		if(action == "AJAX_deleteNewsFeedComment")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			commentID;

			commentID	= indexPage.GetVarsHandler()->Get("commentID");

			MESSAGE_DEBUG("", action, "start [" + commentID + "]");

			if(commentID.empty())
			{
				
				MESSAGE_DEBUG("", action, "commentID is not defined");

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"commentID is empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					{
						
						MESSAGE_DEBUG("", action, "re-login required");
					}

					indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
				}

				ost.str("");
				ost << "DELETE FROM `feed_message_comment` WHERE `id`='" << commentID << "' and `userID`='" << user.GetID() << "';";
				db.Query(ost.str());

				ost.str("");
				ost << "DELETE FROM `users_notification` WHERE `actionId`='" << commentID << "' and `actionTypeId`='19';";
				db.Query(ost.str());

				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"success\"," << std::endl;
				ostFinal << "\"description\" : \"\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX get data for profile (called from JS Worker)
		if(action == "AJAX_getDataForProfile")
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			ost.str("");
			ost << "SELECT * FROM `geo_country`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_country is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_country", ost.str());

			ost.str("");
			ost << "SELECT * FROM `geo_region`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_country_id\":\"" << db.Get(i, "geo_country_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_region is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_region", ost.str());

			ost.str("");
			ost << "SELECT * FROM `geo_locality`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
	//				ost << "{ \"label\": \"" << db.Get(i, "title") << "\", \"category\": \"" << db.Get(i, "area") << "\" }";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_region_id\":\"" << db.Get(i, "geo_region_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table geo_locality is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("geo_locality", ost.str());

			ost.str("");
			ost << "SELECT * FROM `company_position`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table company_position is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("company_position", ost.str());

/*
			ost.str("");
			ost << "SELECT * FROM `site_themes`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					if(i) ost << ",";
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"name\":\"" << db.Get(i, "name") << "\",\"path\":\"" << db.Get(i, "path") << "\"}";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table site_themes is empty");
				ost.str("");
			}
			indexPage.RegisterVariableForce("themes", ost.str());

			ost.str("");
			ost << "SELECT * FROM `school`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"geo_locality_id\":\"" << db.Get(i, "geo_locality_id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table school is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("school", ost.str());

			ost.str("");
			ost << "SELECT * FROM `language`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table language is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("language", ost.str());

			ost.str("");
			ost << "SELECT * FROM `skill`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{\"id\":\"" << db.Get(i, "id") << "\",\"title\":\"" << db.Get(i, "title") << "\"}";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "table skill is empty");
				ost.str("");
			}

			indexPage.RegisterVariableForce("skill", ost.str());
*/
			if(!indexPage.SetTemplate("ajax_getDataForProfile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_getDataForProfile.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

/*
		// --- AJAX get URL meta data
		if(action == "AJAX_getURLMetaData")
		{
			ostringstream	ost, ostFinal;
			CMysql			db1;
			string			url, imageTempSet;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			url = indexPage.GetVarsHandler()->Get("url");
			imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");
			try{
				imageTempSet = to_string(stol(imageTempSet));
			} catch(...) {
				{
					MESSAGE_ERROR("", action, "imageTempSet [" + string(indexPage.GetVarsHandler()->Get("imageTempSet")) + "] can't be converted to number");
				}
				imageTempSet = "";
			}

			url = CleanUPText(url);
			if(imageTempSet.empty() || url.empty() || url.length() > HTML_MAX_POLL_URL_LENGTH)
			{
				if(imageTempSet.empty())
				{
					MESSAGE_ERROR("", action, "imageTempSet is empty");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"проблема с выбором imageTempSet\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					
					MESSAGE_DEBUG("", action, "url is empty OR too long OR imageTempSet is empty");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"ссылка пустая или слишком длинная\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				if(user.GetLogin() == "Guest")
				{
					ostringstream	ost;

					{
						
						MESSAGE_DEBUG("", action, "re-login required");
					}

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"Ваша сессия истекла. Необходимо выйти и повторно зайти.\"" << std::endl;
					ostFinal << "}" << std::endl;
				}


				{
					// --- !!! IMPORTANT !!! Create and destroy CHTML just once in the app
					CHTML		html;

					if(html.PerformRequest(url))
					{
						unsigned long		feed_imageID = 0;
						string			  feed_imageURL = "";
						string				feed_mediaType = "";

						if(html.isEmbedVideoHostedOnYoutube())
						{
							ostringstream   ost;

							{
								
								MESSAGE_DEBUG("", action, "youtube video found");
							}

							ost.str("");
							ost << "insert into `feed_images` set "
								<< "`tempSet`='" << imageTempSet << "', "
								<< "`userID`='" << user.GetID() << "',  "
								<< "`folder`='', "
								<< "`mediaType`=\"youtube_video\",  "
								<< "`filename`=\"" << html.GetEmbedVideoURL() << "\";";

							 feed_imageID = db.InsertQuery(ost.str());
							 feed_mediaType = "youtube_video";
							 feed_imageURL = html.GetEmbedVideoURL();
						}
						else if((!html.GetPreviewImageFolder().empty()) && (!html.GetPreviewImagePrefix().empty()) && (!html.GetPreviewImageExtention().empty()))
						{
							// --- 1) check that image actually image
							// --- 2) move it to finalFoldef
							// --- 3) submit to imageTempSet in DB
							struct ExifInfo exifInfo;
							string			finalFile, tmpFile2Check, tmpImageJPG;
							ostringstream	ost;

							{
								
								MESSAGE_DEBUG("", action, "preview image found");
							}

#ifndef IMAGEMAGICK_DISABLE
							Magick::InitializeMagick(NULL);
#endif
							// --- init variables
							ost.str("");
							ost << IMAGE_FEED_DIRECTORY << "/" << html.GetPreviewImageFolder() << "/" << html.GetPreviewImagePrefix() << ".jpg";
							finalFile = ost.str();

							ost.str("");
							ost << "/tmp/tmp_" << html.GetPreviewImagePrefix() << html.GetPreviewImageExtention();
							tmpFile2Check = ost.str();

							ost.str("");
							ost << "/tmp/" << html.GetPreviewImagePrefix() << ".jpg";
							tmpImageJPG = ost.str();

							if(ImageSaveAsJpgToFeedFolder(tmpFile2Check, tmpImageJPG, exifInfo))
							{

								{
									MESSAGE_DEBUG("", action, "" + action + ": choosen filename for feed image [" + finalFile + "]");
								}

								CopyFile(tmpImageJPG, finalFile);

								ost.str("");
								ost << "insert into `feed_images` set "
									<< "`tempSet`='" << imageTempSet << "', "
									<< "`userID`='" << user.GetID() << "',  "
									<< "`folder`='" << html.GetPreviewImageFolder() << "', "
									<< "`filename`='" << html.GetPreviewImagePrefix() << ".jpg', "
									<< "`mediaType`='image',  "
									<< ((exifInfo.DateTime.length() && exifInfo.DateTime != "unknown") ? (string)("`exifDateTime`='") + exifInfo.DateTime + "', " : "")
									<< "`exifGPSAltitude`='" << ParseGPSAltitude(exifInfo.GPSAltitude) << "', "
									<< "`exifGPSLatitude`='" << ParseGPSLatitude(exifInfo.GPSLatitude) << "', "
									<< "`exifGPSLongitude`='" << ParseGPSLongitude(exifInfo.GPSLongitude) << "', "
									<< "`exifGPSSpeed`='" << ParseGPSSpeed(exifInfo.GPSSpeed) << "', "
									<< "`exifModel`='" << exifInfo.Model << "', "
									<< "`exifAuthors`='" << exifInfo.Authors << "', "
									<< "`exifApertureValue`='" << exifInfo.ApertureValue << "', "
									<< "`exifBrightnessValue`='" << exifInfo.BrightnessValue << "', "
									<< "`exifColorSpace`='" << exifInfo.ColorSpace << "', "
									<< "`exifComponentsConfiguration`='" << exifInfo.ComponentsConfiguration << "', "
									<< "`exifCompression`='" << exifInfo.Compression << "', "
									<< "`exifDateTimeDigitized`='" << exifInfo.DateTimeDigitized << "', "
									<< "`exifDateTimeOriginal`='" << exifInfo.DateTimeOriginal << "', "
									<< "`exifExifImageLength`='" << exifInfo.ExifImageLength << "', "
									<< "`exifExifImageWidth`='" << exifInfo.ExifImageWidth << "', "
									<< "`exifExifOffset`='" << exifInfo.ExifOffset << "', "
									<< "`exifExifVersion`='" << exifInfo.ExifVersion << "', "
									<< "`exifExposureBiasValue`='" << exifInfo.ExposureBiasValue << "', "
									<< "`exifExposureMode`='" << exifInfo.ExposureMode << "', "
									<< "`exifExposureProgram`='" << exifInfo.ExposureProgram << "', "
									<< "`exifExposureTime`='" << exifInfo.ExposureTime << "', "
									<< "`exifFlash`='" << exifInfo.Flash << "', "
									<< "`exifFlashPixVersion`='" << exifInfo.FlashPixVersion << "', "
									<< "`exifFNumber`='" << exifInfo.FNumber << "', "
									<< "`exifFocalLength`='" << exifInfo.FocalLength << "', "
									<< "`exifFocalLengthIn35mmFilm`='" << exifInfo.FocalLengthIn35mmFilm << "', "
									<< "`exifGPSDateStamp`='" << exifInfo.GPSDateStamp << "', "
									<< "`exifGPSDestBearing`='" << exifInfo.GPSDestBearing << "', "
									<< "`exifGPSDestBearingRef`='" << exifInfo.GPSDestBearingRef << "', "
									<< "`exifGPSImgDirection`='" << exifInfo.GPSImgDirection << "', "
									<< "`exifGPSImgDirectionRef`='" << exifInfo.GPSImgDirectionRef << "', "
									<< "`exifGPSInfo`='" << exifInfo.GPSInfo << "', "
									<< "`exifGPSTimeStamp`='" << exifInfo.GPSTimeStamp << "', "
									<< "`exifISOSpeedRatings`='" << exifInfo.ISOSpeedRatings << "', "
									<< "`exifJPEGInterchangeFormat`='" << exifInfo.JPEGInterchangeFormat << "', "
									<< "`exifJPEGInterchangeFormatLength`='" << exifInfo.JPEGInterchangeFormatLength << "', "
									<< "`exifMake`='" << exifInfo.Make << "', "
									<< "`exifMeteringMode`='" << exifInfo.MeteringMode << "', "
									<< "`exifOrientation`='" << exifInfo.Orientation << "', "
									<< "`exifResolutionUnit`='" << exifInfo.ResolutionUnit << "', "
									<< "`exifSceneCaptureType`='" << exifInfo.SceneCaptureType << "', "
									<< "`exifSceneType`='" << exifInfo.SceneType << "', "
									<< "`exifSensingMethod`='" << exifInfo.SensingMethod << "', "
									<< "`exifShutterSpeedValue`='" << exifInfo.ShutterSpeedValue << "', "
									<< "`exifSoftware`='" << exifInfo.Software << "', "
									<< "`exifSubjectArea`='" << exifInfo.SubjectArea << "', "
									<< "`exifSubSecTimeDigitized`='" << exifInfo.SubSecTimeDigitized << "', "
									<< "`exifSubSecTimeOriginal`='" << exifInfo.SubSecTimeOriginal << "', "
									<< "`exifWhiteBalance`='" << exifInfo.WhiteBalance << "', "
									<< "`exifXResolution`='" << exifInfo.XResolution << "', "
									<< "`exifYCbCrPositioning`='" << exifInfo.YCbCrPositioning << "', "
									<< "`exifYResolution`='" << exifInfo.YResolution << "';";
								feed_imageID = db.InsertQuery(ost.str());

								if(feed_imageID)
								{
									feed_imageURL = html.GetPreviewImageFolder() + "/" + html.GetPreviewImagePrefix() + ".jpg";
									feed_mediaType = "image";
								}
								else
								{
									{
										MESSAGE_ERROR("", action, "inserting image info into feed_images table");
									}
								}

								// --- Delete temporarily files
								unlink(tmpFile2Check.c_str());
								unlink(tmpImageJPG.c_str());
							}
							else
							{
								{
									MESSAGE_ERROR("", action, "image [" + tmpFile2Check + "] is not valid image format (looks like attack)");
								}
							}
						}
						else
						{
							
							MESSAGE_DEBUG("", action, "there are no (youtube_video, preview image)");
						}
						ostFinal.str("");
						ostFinal << "{" << std::endl;
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"title\" : \"" << CleanUPText(html.GetTitle()) << "\",";
						ostFinal << "\"description\" : \"" << CleanUPText(html.GetDescription()) << "\",";
						ostFinal << "\"imageURL\" : \"" << feed_imageURL << "\",";
						ostFinal << "\"imageID\" : \"" << feed_imageID << "\",";
						ostFinal << "\"mediaType\" : \"" << feed_mediaType << "\"";
						ostFinal << "}" << std::endl;
					}
					else
					{
						{
							
							MESSAGE_DEBUG("", action, "can't perform request [" + string(url) + "]");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"error\",";
						ostFinal << "\"description\" : \"Не удалось получить данные с сайта, проверьте корректность ссылки\"";
						ostFinal << "}";
					}
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}
*/


		// --- AJAX delete preview avatar
		if(action == "AJAX_deleteAvatar")
		{
			ostringstream	result;
			ostringstream	ost;
			string		sessid, avatarID;
			int			affected;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			avatarID = indexPage.GetVarsHandler()->Get("id");

			ost << "SELECT * FROM `users_avatars` WHERE `id`=\"" << avatarID << "\";";
			affected = db.Query(ost.str());
			if((affected = db.Query(ost.str())) > 0)
			{
				if(db.Get(0, "userid") == user.GetID())
				{
					string filename;

					filename += IMAGE_AVATAR_DIRECTORY;
					filename += "/avatars";
					filename += db.Get(0, "folder");
					filename += "/";
					filename += db.Get(0, "filename");

					{
						MESSAGE_DEBUG("", action, "" + action + ": removing avatar [id=" + avatarID + "] belongs to user " + user.GetLogin() + " [filename=" + filename + "]");
					}

					ost.str("");
					ost << "DELETE FROM `users_avatars` WHERE `id`=\"" << avatarID << "\";";
					db.Query(ost.str());

					if(isFileExists(filename))
					{
						unlink(filename.c_str());

						// --- Update live feed
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"9\", \"0\", NOW())";
						if(db.InsertQuery(ost.str()))
						{
							result.str("");
							result << "{ \"result\":\"success\", \"description\":\"\" }";
						}
						else
						{
							result.str("");
							result << "{ \"result\":\"error\", \"description\":\"ERROR inserting into DB `feed`\" }";

							{
								MESSAGE_ERROR("", action, "inserting into `feed` (" + ost.str() + ")");
							}
						}
					}
					else
					{
						result.str("");
						result << "{ \"result\":\"error\", \"description\":\"ERROR file is not exists\" }";

						{
							MESSAGE_ERROR("", action, "file is not exists  [filename=" + filename + "] belongs to user " + user.GetLogin());
						}
					}
				}
				else
				{
					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"avatar do not belongs to you\" }";

					{
						MESSAGE_ERROR("", action, "avatar [id=" + avatarID + "] do not belongs to user " + user.GetLogin());
					}
				}
			}
			else
			{
				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"there is no avatar\" }";

				MESSAGE_DEBUG("", action, "there is no avatar [id=" + avatarID + "]");
			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX change in friendship status
		if(action == "AJAX_setFindFriend_FriendshipStatus")
		{
			ostringstream	ost, result;
			string			sessid, friendID, currentFriendshipStatus, requestedFriendshipStatus;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			friendID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("friendID"));
			requestedFriendshipStatus = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("status"));
			result.str("");

			if((friendID.length() == 0) || (requestedFriendshipStatus.length() == 0))
			{
				ostringstream	ost2;
				ost2.str("");
				ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": friendID [" << friendID << "] or status [" << requestedFriendshipStatus << "] is empty";

				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"" << ost2.str() << "\" }";

				{
					CLog	log;
					log.Write(ERROR, ost2.str());
				}
			}
			else
			{
				if(db.Query("SELECT * FROM `users_friends` WHERE `userID`='" + user.GetID() + "' and `friendID`='" + friendID + "';"))
					currentFriendshipStatus = db.Get(0, "state");
				else
					currentFriendshipStatus = "empty";

				if((requestedFriendshipStatus == "requested") || (requestedFriendshipStatus == "requesting"))
				{
					if((currentFriendshipStatus == "empty") || (currentFriendshipStatus == "ignored"))
					{
						ost.str("");
						ost << "START TRANSACTION;";
						ost << "insert into `users_friends` (`userID`, `friendID`, `state`, `date`) \
								VALUES (\
								\"" << user.GetID() << "\", \
								\"" << friendID << "\", \
								\"requesting\", \
								NOW() \
								);";
						ost << "insert into `users_friends` (`userID`, `friendID`, `state`, `date`) \
								VALUES (\
								\"" << friendID << "\", \
								\"" << user.GetID() << "\", \
								\"requested\", \
								NOW() \
								);";
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) \
								VALUES(\
								\"\", \
								\"" << user.GetID() << "\", \
								\"16\", \
								\"" << friendID << "\",  \
								NOW());";
						ost << "COMMIT;";
						db.Query(ost.str());

						result.str("");
						result << "{ \"result\":\"ok\", \"description\":\"\" }";

					}
					else
					{
						ostringstream	ost2;
						ost2.str("");
						ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": can't move "
								"FROM (" << user.GetID() << " " << currentFriendshipStatus << " " << friendID << ") "
								"TO (" << friendID << " " << requestedFriendshipStatus << " " << user.GetID() << ") ";

						result.str("");
						result << "{ \"result\":\"error\", \"description\":\"" << ost2.str() << "\" }";

						{
							CLog	log;
							log.Write(ERROR, ost2.str());
						} // CLog
					}
				}
				else if (requestedFriendshipStatus == "disconnect")
				{

					if((currentFriendshipStatus == "requested") || (currentFriendshipStatus == "requesting") || (currentFriendshipStatus == "confirmed") || (currentFriendshipStatus == "blocked"))
					{
						ost.str("");
						ost << "DELETE FROM `users_friends` WHERE  \
								(`userID`=\"" << user.GetID() << "\" and `friendID`=\"" << friendID << "\" ) \
								OR \
								(`friendID`=\"" << user.GetID() << "\" and `userID`=\"" << friendID << "\" );";
						db.Query(ost.str());

						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"15\", \"" << friendID << "\", NOW())";
						db.Query(ost.str());

						result.str("");
						result << "{ \"result\":\"ok\", \"description\":\"\" }";
					}
					else
					{
						ostringstream	ost2;
						ost2.str("");
						ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": can't move FROM " << currentFriendshipStatus << " to " << requestedFriendshipStatus << " ";

						result.str("");
						result << "{ \"result\":\"error\", \"description\":\"" << ost2.str() << "\" }";

						{
							CLog	log;
							log.Write(ERROR, ost2.str());
						} // CLog
					}

				}
				else if(requestedFriendshipStatus == "confirm")
				{
					if((currentFriendshipStatus == "requested") || (currentFriendshipStatus == "requesting") || (currentFriendshipStatus == "confirmed") || (currentFriendshipStatus == "blocked"))
					{
						ost.str("");
						ost << "update `users_friends` set `state`='confirmed', `date`=NOW() WHERE  \
								(`userID`=\"" << user.GetID() << "\" and `friendID`=\"" << friendID << "\") \
								OR \
								(`friendID`=\"" << user.GetID() << "\" and `userID`=\"" << friendID << "\" );";
						db.Query(ost.str());

						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"14\", \"" << friendID << "\", NOW())";
						db.Query(ost.str());
						ost.str("");
						ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << friendID << "\", \"14\", \"" << user.GetID() << "\", NOW())";
						db.Query(ost.str());

						result.str("");
						result << "{ \"result\":\"ok\", \"description\":\"\" }";
					}
					else
					{
						ostringstream	ost2;
						ost2.str("");
						ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": can't move FROM " << currentFriendshipStatus << " to " << requestedFriendshipStatus << " ";

						result.str("");
						result << "{ \"result\":\"error\", \"description\":\"" << ost2.str() << "\" }";

						{
							CLog	log;
							log.Write(ERROR, ost2.str());
						} // CLog
					}
				}
				else
				{
					ostringstream	ost2;
					ost2.str("");
					ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": requested friendship status is unknown [" << requestedFriendshipStatus << "] friendID [" << friendID << "]";

					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"" << ost2.str() << "\" }";

					{
						CLog	log;
						log.Write(ERROR, ost2.str());
					} // CLog
				} // Check friendship status
			} // if((friendID.length() == 0) || (status.length() == 0))

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- AJAX block user account
		if(action == "AJAX_block_user_account")
		{
			ostringstream	ost, result;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "update `users` set `isblocked`='Y' WHERE `id`='" << user.GetID() << "';";
			db.Query(ost.str());
			ost.str("");
			ost << "SELECT `isblocked` FROM `users` WHERE `id`='" << user.GetID() << "';";
			if(db.Query(ost.str()))
			{
				string	blockStatus = db.Get(0, "isblocked");

				if(blockStatus == "Y")
				{

					// --- update news feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"17\", \"0\", NOW());";
					db.Query(ost.str());

					result.str("");
					result << "{ \"result\":\"success\", \"description\":\"\" }";
				}
				else
				{
					ostringstream	ost2;
					ost2.str("");
					ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": userID [" << user.GetID() << "] can't block";

					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] can't block\" }";

					{
						CLog	log;
						log.Write(ERROR, ost2.str());
					} // CLog
				}
			}
			else
			{
				ostringstream	ost2;
				ost2.str("");
				ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": userID [" << user.GetID() << "] has not been found";

				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] has not been found\" }";

				{
					CLog	log;
					log.Write(ERROR, ost2.str());
				} // CLog
			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}

		// --- AJAX enable user account
		if(action == "AJAX_enable_user_account")
		{
			ostringstream	ost, result;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "update `users` set `isblocked`='N' WHERE `id`='" << user.GetID() << "';";
			db.Query(ost.str());
			ost.str("");
			ost << "SELECT `isblocked` FROM `users` WHERE `id`='" << user.GetID() << "';";
			if(db.Query(ost.str()))
			{
				string	blockStatus = db.Get(0, "isblocked");

				if(blockStatus == "N")
				{

					// --- update news feed
					ost.str("");
					ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"18\", \"0\", NOW());";
					db.Query(ost.str());

					result.str("");
					result << "{ \"result\":\"success\", \"description\":\"\" }";
				}
				else
				{
					ostringstream	ost2;
					ost2.str("");
					ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": userID [" << user.GetID() << "] can't enable";

					result.str("");
					result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] can't enable\" }";

					{
						CLog	log;
						log.Write(ERROR, ost2.str());
					} // CLog
				}
			}
			else
			{
				ostringstream	ost2;
				ost2.str("");
				ost2 << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": userID [" << user.GetID() << "] has not been found";

				result.str("");
				result << "{ \"result\":\"error\", \"description\":\"userID [" << user.GetID() << "] has not been found\" }";

				{
					CLog	log;
					log.Write(ERROR, ost2.str());
				} // CLog
			}

			indexPage.RegisterVariableForce("result", result.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}


		// --- JSON like handler
		if(action == "JSON_ClickLikeHandler")
		{
			ostringstream	ost, ostFinal;
			string			sessid, messageId, messageLikeType, userList = "";
			string		  failReason = "";

			{
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			messageId = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageId"));
			messageLikeType = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("messageLikeType"));
			if(messageLikeType.empty()) messageLikeType = "like";

			if(messageId.length())
			{
				int				affected;
				bool			isSuccess = false;

				messageId = to_string(stol(messageId));

				affected = db.Query("SELECT * FROM `feed_message_params` WHERE `parameter`=\"" + messageLikeType + "\" and `messageID`=\"" + messageId + "\" and `userID`=\"" + user.GetID() + "\";");
				if(affected)
				{
					// --- disliked message
					string  feed_message_params_id_to_remove = db.Get(0, "id");

					isSuccess = true;
					// --- remove "my like"
					db.Query("DELETE FROM `feed_message_params` WHERE `id`=\"" + feed_message_params_id_to_remove + "\";");

					// --- remove it FROM users_notifications
					db.Query("DELETE FROM `users_notification` WHERE `actionTypeId`=\"49\" and `actionId`='" + feed_message_params_id_to_remove + "';");
				}
				else
				{
					// --- add "my like"
					unsigned long   feed_message_params_id = db.InsertQuery("INSERT INTO  `feed_message_params` (`parameter`, `messageID`, `userID`) VALUES (\"" + messageLikeType + "\", \"" + messageId + "\", \"" + user.GetID() + "\");");

					if(feed_message_params_id)
					{
						string	  userIDtoNotify = "";

						isSuccess = true;

						if((messageLikeType == "like") && db.Query("SELECT * FROM `feed` WHERE `actionTypeId`=\"11\" and `actionId`=\"" + messageId + "\";"))
						{
							string	srcType = db.Get(0, "srcType");
							string	srcID = db.Get(0, "userId");

							if(srcType == "user")
							{
								userIDtoNotify = srcID;
							}
							else if(srcType == "company")
							{
								if(db.Query("SELECT `admin_userID` FROM `company` WHERE `id`=\"" + srcID + "\" AND `isBlocked`=\"N\";"))
								{
									userIDtoNotify = db.Get(0, "admin_userID");
								}
								else
								{
									MESSAGE_ERROR("", action, "unknown company.id(" + srcID + ") or blocked");
								}
							}
							else if(srcType == "group")
							{
								if(db.Query("SELECT `owner_id` FROM `groups` WHERE `id`=\"" + srcID + "\" AND `isBlocked`=\"N\";"))
								{
									userIDtoNotify = db.Get(0, "owner_id");
								}
								else
								{
									MESSAGE_ERROR("", action, "unknown group.id(" + srcID + ") or blocked");
								}
							}
							else
							{
								MESSAGE_ERROR("", action, "unknown srcType(" + srcType + ")");
							}
						}
						if((messageLikeType == "likeBook") && db.Query("SELECT * FROM `users_books` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "userID");
						if((messageLikeType == "likeCertification") && db.Query("SELECT * FROM `users_certifications` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeCourse") && db.Query("SELECT * FROM `users_courses` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeUniversityDegree") && db.Query("SELECT * FROM `users_university` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeCompany") && db.Query("SELECT * FROM `users_company` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");
						if((messageLikeType == "likeLanguage") && db.Query("SELECT * FROM `users_language` WHERE `id`=\"" + messageId + "\";"))
							userIDtoNotify = db.Get(0, "user_id");

						// --- send notification to message owner
						if(userIDtoNotify.length() && (userIDtoNotify != "0"))
						{
							// --- user notification for message like

							if(db.InsertQuery("insert into `users_notification` (`userId`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES (\"" + userIDtoNotify + "\", \"49\", \"" + to_string(feed_message_params_id) + "\", UNIX_TIMESTAMP());"))
							{
							}
							else
							{
								MESSAGE_ERROR("", action, "inserting into users_notification table");
							}
						}
						else
						{
							{
								MESSAGE_ERROR("", action, "can't find \"liked message\" owner");
							}

						}
					}
					else
					{
						isSuccess = false;
						failReason = "inserting into feed_message_params table";

						{
							MESSAGE_ERROR("", action, "inserting into feed_message_params table");
						}
					}
				}

				affected = db.Query("SELECT * FROM `feed_message_params` WHERE `parameter`=\"" + messageLikeType + "\" and `messageID`=\"" + messageId + "\";");
				if(affected)
				{
					ost.str("");
					ost << "SELECT * FROM `users` WHERE `id` in (";
					if(affected)
					{
						for(int i = 0; i < affected; i++)
						{
							if(i > 0) ost << ",";
							ost << db.Get(i, "userID");
						}
					}
					ost << ");";
					userList = GetUserListInJSONFormat(ost.str(), &db, &user);
				}
				else
				{
					{
						
						MESSAGE_DEBUG("", action, "noone " + messageLikeType + " id:" + messageId + " anymore");
					}

					userList = "";
				}

				ostFinal.str("");
				if(isSuccess)
				{
					ostFinal << "{";
					ostFinal << "\"result\" : \"success\",";
					ostFinal << "\"description\" : \"\",";
					ostFinal << "\"messageLikesUserList\" : [" << userList << "]";
					ostFinal << "}";
				}
				else
				{
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"" << failReason << "\"";
					ostFinal << "}";
				}

			}
			else
			{
				// --- message ID is not number
				{
					MESSAGE_ERROR("", action, "messageID[" + indexPage.GetVarsHandler()->Get("messageId") + "] is empty");
				}

				ostFinal.str("");
				ostFinal << "{";
				ostFinal << "\"result\" : \"error\",";
				ostFinal << "\"description\" : \"messageID is empty\"";
				ostFinal << "}";
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}


		// --- JSON returns shake hands in view profile
		// ---		common friends
		// ---		common companies
		if(action == "JSON_getShakeHands")
		{
			ostringstream	ost, ostFinal;
			string			sessid, lookForKey;
			string			user1, user2, handshakeUserStatus = "";
			string			user1Data, user2Data, hopUserList;
			CMysql			db1;
			vector<int>		vectorFriendList1, vectorFriendList2, vectorFriendList3;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			user1 = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("user1"));
			user2 = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("user2"));

			if(user1 != user2)
			{
				int				affected;

				ost.str("");
				ost << "SELECT `users_friends`.`friendID` FROM `users_friends` \
						RIGHT JOIN `users` on `users_friends`.`friendID` = `users`.`id` \
						WHERE `users_friends`.`userID`='" << user1 << "' and `users`.`isblocked`='N' and  `users`.`isactivated`='Y' and `users_friends`.`state`='confirmed';";
				affected = db.Query(ost.str());
				if(affected)
				{
					for(int i = 0; i < affected; i++)
					{
						vectorFriendList1.push_back(stoi(db.Get(i, "friendID")));

						if(atoi(user2.c_str()) == stoi(db.Get(i, "friendID")))
						{
							handshakeUserStatus = "directFriends";
						}
					}


					if(handshakeUserStatus == "")
					{

						ost.str("");
						ost << "SELECT `users_friends`.`friendID` FROM `users_friends` \
								RIGHT JOIN `users` on `users_friends`.`friendID` = `users`.`id` \
								WHERE `users_friends`.`userID`='" << user2 << "' and `users`.`isblocked`='N' and  `users`.`isactivated`='Y' and `users_friends`.`state`='confirmed';";
						affected = db.Query(ost.str());
						if(affected)
						{
							for(int i = 0; i < affected; i++)
							{
								vectorFriendList2.push_back(stoi(db.Get(i, "friendID")));
							}
						}

						for(vector<int>::iterator it1 = vectorFriendList1.begin(); it1 != vectorFriendList1.end(); ++it1)
						{
							for(vector<int>::iterator it2 = vectorFriendList2.begin(); it2 != vectorFriendList2.end(); ++it2)
							{
								if(*it1 == *it2)
								{
									vectorFriendList3.push_back(*it1);
								}
							}

						}

						if(vectorFriendList3.size() > 0)
						{
							handshakeUserStatus = "1hop";

							ost.str("");
							ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` in (";
							for(vector<int>::iterator it = vectorFriendList3.begin(); it != vectorFriendList3.end(); ++it)
							{
								if(it != vectorFriendList3.begin())	ost << ",";
								ost << *it;
							}
							ost << ");";
							hopUserList = GetUserListInJSONFormat(ost.str(), &db, &user);
						}
						else
						{
							handshakeUserStatus = "noConnection";
						}
					}
				}
				else
				{
					handshakeUserStatus = "noConnection";
				}
			}
			else
			{
				handshakeUserStatus = "sameUser";
			}

			ost.str("");
			ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`='" << user1 << "'";
			user1Data = GetUserListInJSONFormat(ost.str(), &db, &user);

			ost.str("");
			ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`='" << user2 << "'";
			user2Data = GetUserListInJSONFormat(ost.str(), &db, &user);


			ostFinal.str("");
			ostFinal << "{" << std::endl;
			ostFinal << "\"result\" : \"success\"," << std::endl;
			ostFinal << "\"description\" : \"\"," << std::endl;
			ostFinal << "\"handshakeUserStatus\" : \"" << handshakeUserStatus << "\"," << std::endl;
			ostFinal << "\"handshakeUsers\" : [" << std::endl << hopUserList << std::endl << "]," << std::endl;
			ostFinal << "\"user1\" : " << std::endl << user1Data << std::endl << "," << std::endl;
			ostFinal << "\"user2\" : " << std::endl << user2Data << std::endl << "" << std::endl;
			ostFinal << "}" << std::endl;

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		// --- JSON FindFriend by ID
		if(action == "JSON_getFindFriendByID")
		{
			ostringstream	ost, ostFinal;
			string			sessid, lookForKey, userList;
			CMysql			db1;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(db1.Connect(DB_NAME, DB_LOGIN, DB_PASSWORD) < 0)
			{
				CLog	log;
				log.Write(ERROR, "Can not connect to mysql database");
				return(1);
			}

			lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

			// --- Clean-up the text
			lookForKey = ReplaceDoubleQuoteToQuote(lookForKey);
			lookForKey = DeleteHTML(lookForKey);
			lookForKey = SymbolReplace(lookForKey, "\r\n", "");
			lookForKey = SymbolReplace(lookForKey, "\r", "");
			lookForKey = SymbolReplace(lookForKey, "\n", "");

			ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`=\"" << lookForKey << "\" ;";

			userList = GetUserListInJSONFormat(ost.str(), &db, &user);

			ostFinal.str("");
			ostFinal << "[" << std::endl << userList << std::endl << "]" << std::endl;

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		// --- JSON friend list for autocomplete
		if((action == "JSON_getFindFriendsListAutocomplete") || (action == "JSON_getFindFriendsListAutocompleteIncludingMyself") || (action == "JSON_getFindFriendsList"))
		{
			ostringstream	ost, ostFinal;
			string			sessid, lookForKey, userList;
			vector<string>	searchWords;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			// --- Initialization
			ostFinal.str("");

			{

				lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

				if(qw(lookForKey, searchWords))
				{

					ostFinal << "[";
					{
						// --- Logging
						CLog			log;
						MESSAGE_DEBUG("", action, "" + action + ": qw4(" + lookForKey + ")");
		/*				for(vector<string>::iterator it = searchWords.begin(); it != searchWords.end(); ++it, i++)
						{
							ost << string(__func__) + "[" + to_string(__LINE__) << "]" + action + ": parsed value at index " << i << " [" << *it << "]" << endl;
							log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) << "]" + action + ": parsed value at index " << i << " [" << *it << "]" << endl);
						}
		*/
					}

					// --- single search word can be name, surname, company name
					if(searchWords.size() == 1)
					{
						{
							
							MESSAGE_DEBUG("", action, "single word search");
						}

						// --- Looking through name, surname
						ost.str("");
						ost << "SELECT * FROM `users` WHERE "
								" `isActivated`='Y' and `isblocked`='N' "
								<< (action == "JSON_getFindFriendsListAutocompleteIncludingMyself" ? "" : " and `users`.`id`!=\"" + user.GetID() + "\" ") <<
								" and (`name` like \"%" 	<< lookForKey << "%\" or `nameLast` like \"%" 	<< lookForKey << "%\");";

						userList = GetUserListInJSONFormat(ost.str(), &db, &user);
						if(userList.length() > 0)
						{
							// --- comma required only if previous text is exists
							if(ostFinal.str().length() > 10) ostFinal << ",";
							ostFinal  << std::endl << userList;
						}

						// --- Looking through company title
						ost.str("");
						ost << "SELECT * FROM `users` "
								"left join `users_company` on `users_company`.`user_id` = `users`.`id` "
								"left join `company` on `company`.`id`=`users_company`.`company_id` "
								"where "
								"`users`.`isActivated`='Y' "
								" and `users`.`isblocked`='N' "
								<< (action == "JSON_getFindFriendsListAutocompleteIncludingMyself" ? "" : " and `users`.`id`!=\"" + user.GetID() + "\" ") <<
								" and `users_company`.`current_company`='1' "
								" and `company`.`name` like \"%" 	<< lookForKey << "%\";";

						userList = GetUserListInJSONFormat(ost.str(), &db, &user);
						if(userList.length() > 0)
						{
							// --- comma required only if previous text is exists
							if(ostFinal.str().length() > 10) ostFinal << ",";
							ostFinal  << std::endl << userList;
						}

					}

					// --- two words searching through DB
					if(searchWords.size() == 2)
					{
						{
							
							MESSAGE_DEBUG("", action, "two words search");
						}

						// --- Looking through user name,surname and company title
						ost.str("");
						ost << "SELECT * FROM `users` "
								" left join `users_company` on `users_company`.`user_id` = `users`.`id` "
								" left join `company` on `company`.`id`=`users_company`.`company_id` "
								" where "
								" `users`.`isActivated`='Y' "
								" and `users`.`isblocked`='N' "
								<< (action == "JSON_getFindFriendsListAutocompleteIncludingMyself" ? "" : " and `users`.`id`!=\"" + user.GetID() + "\" ") <<
								" and `users_company`.`current_company`='1' "
								" and ( "
								" 	`company`.`name` like \"%" 	<< searchWords[0] << "%\" or "
								" 	`company`.`name` like \"%" 	<< searchWords[1] << "%\" "
								" ) and ( "
								" 	`users`.`name` like \"%" 		<< searchWords[0] << "%\" or "
								" 	`users`.`name` like \"%" 		<< searchWords[1] << "%\" or "
								" 	`users`.`nameLast` like \"%" 	<< searchWords[0] << "%\" or "
								" 	`users`.`nameLast` like \"%" 	<< searchWords[1] << "%\" "
								" );";

						userList = GetUserListInJSONFormat(ost.str(), &db, &user);
						if(userList.length() > 0)
						{
							// --- comma required only if previous text is exists
							if(ostFinal.str().length() > 10) ostFinal << ",";
							ostFinal  << std::endl << userList;
						}
						else
						{
							// --- here code will be run only if multiwork search was not sucessfull on previous step
							// --- earlier: user _and_ company is not success
							// --- here: user _or_ company
							{
								
								MESSAGE_DEBUG("", action, "(user _and_ company) has fail, try (user _without_ company) ");
							}

							ost.str("");
							ost << "SELECT * FROM `users` "
									" WHERE `isActivated`='Y' "
									" and `isblocked`='N' "
									<< (action == "JSON_getFindFriendsListAutocompleteIncludingMyself" ? "" : " and `users`.`id`!=\"" + user.GetID() + "\" ") <<
									" and ( "
									" ( "
									" 	`users`.`name` like \"%" 		<< searchWords[1] << "%\" and "
									" 	`users`.`nameLast` like \"%" 	<< searchWords[0] << "%\" "
									" ) "
									" or "
									" ( "
									" 	`users`.`name` like \"%" 		<< searchWords[0] << "%\" and "
									" 	`users`.`nameLast` like \"%" 	<< searchWords[1] << "%\" "
									" ) "
									" );";

							userList = GetUserListInJSONFormat(ost.str(), &db, &user);
							if(userList.length() > 0)
							{
								// --- comma required only if previous text is exists
								if(ostFinal.str().length() > 10) ostFinal << ",";
								ostFinal  << std::endl << userList;
							}
						}

					}

					// --- three words searching through DB
					if(searchWords.size() == 3)
					{
						{
							
							MESSAGE_DEBUG("", action, "three words search");
						}

						// --- Looking through user name,surname and company title
						ost.str("");
						ost << "SELECT * FROM `users` \
								left join `users_company` on `users_company`.`user_id` = `users`.`id` \
								left join `company` on `company`.`id`=`users_company`.`company_id` \
								where \
								`users`.`isActivated`='Y' and `users`.`isblocked`='N' and `users`.`id`!=\"" << user.GetID() << "\" and \
								`users_company`.`current_company`='1' and \
								( \
									`company`.`name` like \"%" 		<< searchWords[0] << "%\" or \
									`company`.`name` like \"%" 		<< searchWords[1] << "%\" or \
									`company`.`name` like \"%" 		<< searchWords[2] << "%\" \
								) and ( \
									`users`.`name` like \"%" 		<< searchWords[0] << "%\" or \
									`users`.`name` like \"%" 		<< searchWords[1] << "%\" or \
									`users`.`name` like \"%" 		<< searchWords[2] << "%\" or \
									`users`.`nameLast` like \"%" 	<< searchWords[0] << "%\" or \
									`users`.`nameLast` like \"%" 	<< searchWords[1] << "%\" or \
									`users`.`nameLast` like \"%" 	<< searchWords[2] << "%\" \
								);";

						userList = GetUserListInJSONFormat(ost.str(), &db, &user);
						if(userList.length() > 0)
						{
							// --- comma required only if previous text is exists
							if(ostFinal.str().length() > 10) ostFinal << ",";
							ostFinal  << std::endl << userList;
						}
						else
						{
							// --- here code will be run only if multiwork search was not sucessfull on previous step
							// --- earlier: user _and_ company is not success
							// --- here: user _or_ company
							{
								
								MESSAGE_DEBUG("", action, "(user _and_ company) has fail, try (user _without_ company) ");
							}

							ost.str("");
							ost << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id`!=\"" << user.GetID() << "\" and ( \
									( \
										`users`.`name` like \"%" 		<< searchWords[1] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[0] << "%\" \
									) \
									or \
									( \
										`users`.`name` like \"%" 		<< searchWords[0] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[1] << "%\" \
									) \
									or \
									( \
										`users`.`name` like \"%" 		<< searchWords[2] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[0] << "%\" \
									) \
									or \
									( \
										`users`.`name` like \"%" 		<< searchWords[0] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[2] << "%\" \
									) \
									or \
									( \
										`users`.`name` like \"%" 		<< searchWords[1] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[2] << "%\" \
									) \
									or \
									( \
										`users`.`name` like \"%" 		<< searchWords[2] << "%\" and \
										`users`.`nameLast` like \"%" 	<< searchWords[1] << "%\" \
									) \
									);";

							userList = GetUserListInJSONFormat(ost.str(), &db, &user);
							if(userList.length() > 0)
							{
								// --- comma required only if previous text is exists
								if(ostFinal.str().length() > 10) ostFinal << ",";
								ostFinal  << std::endl << userList;
							}
						}

					}

					ostFinal << std::endl << "]";
				}

				MESSAGE_DEBUG("", action, "final response [" + ostFinal.str() + "]");

				indexPage.RegisterVariableForce("result", ostFinal.str());
			}

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON industy list for autocomplete
		if(action == "JSON_getIndustryListAutocomplete")
		{
			ostringstream   ost, ostFinal;
			string		  sessid, lookForKey, industriesList;
			vector<string>  searchWords;

			MESSAGE_DEBUG("", action, "start");

			// --- Initialization
			ostFinal.str("");

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			lookForKey = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lookForKey"));

			{
				auto			tmpStr = ""s;
				auto			affected = db.Query("SELECT * FROM `company_industry` WHERE `name` LIKE \"%" + lookForKey + "%\" LIMIT 0, 20;");

				for(int i = 0; i < affected; ++i)
				{
					if(i) tmpStr += ",";
					tmpStr += string("{\"id\":\"") + string(db.Get(i, "id")) + "\",";
					tmpStr += string("\"name\":\"") + string(db.Get(i, "name")) + "\"}";
				}
				// industriesList = GetCompanyListInJSONFormat(ost.str(), &db, &user);
				ostFinal << "{\"status\":\"success\",\"industries\":[" << tmpStr << "]}";

			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "final response [" + ostFinal.str() + "]");
		}

		// --- JSON get list of my friends
		if((action == "JSON_getMyNetworkFriendList") || (action == "JSON_getWhoWatchedONMeList"))
		{
			ostringstream	ost, ostFinal, friendsSqlQuery;
			string			sessid, lookForKey, userList = "";
			CMysql			db1;
			int				affected;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(db1.Connect(DB_NAME, DB_LOGIN, DB_PASSWORD) < 0)
			{
				CLog	log;
				log.Write(ERROR, "Can not connect to mysql database");
				return(1);
			}

			friendsSqlQuery.str("");
			ost.str("");
			if(action == "JSON_getMyNetworkFriendList")
				ost << "SELECT `friendID` FROM `users_friends` WHERE `userID`='" << user.GetID() << "';";
			if(action == "JSON_getWhoWatchedONMeList")
				ost << "SELECT `watching_userID` as `friendID` FROM `users_watched` WHERE `watched_userID`='" << user.GetID() << "';";
			affected = db.Query(ost.str());
			if(affected)
			{
				friendsSqlQuery << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` IN (";
				for(int i = 0; i < affected; i++)
				{
					friendsSqlQuery << (i > 0 ? ", " : "") << db.Get(i, "friendID");
				}
				friendsSqlQuery << ");";

				MESSAGE_DEBUG("", action, "query for JSON prepared [" + friendsSqlQuery.str() + "]");

				userList = GetUserListInJSONFormat(friendsSqlQuery.str(), &db, &user);
			}


			ostFinal.str("");
			ostFinal << "[" << std::endl << userList << std::endl << "]" << std::endl;

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		if(action == "JSON_chatGetInitialData")
		{
			ostringstream	ost, ostFinal, friendsSqlQuery, chatMessageQuery;
			string			sessid, lookForKey, userArray = "", messageArray = "";

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ostFinal << "result: fail";
			}
			else
			{
				int				affected;

				friendsSqlQuery.str("");
				ost.str("");
				ost << "SELECT `friendID` FROM `users_friends` WHERE `userID`='" << user.GetID() << "';";
				affected = db.Query(ost.str());
				if(affected)
				{
					friendsSqlQuery << "SELECT * FROM `users` WHERE `isActivated`='Y' and `isblocked`='N' and `id` IN (";
					for(int i = 0; i < affected; i++)
					{
						friendsSqlQuery << (i > 0 ? ", " : "") << db.Get(i, "friendID");
					}
					friendsSqlQuery << ");";

					{
						
						MESSAGE_DEBUG("", action, "query for JSON prepared [" + friendsSqlQuery.str() + "]");
					}
					userArray = GetUserListInJSONFormat(friendsSqlQuery.str(), &db, &user);

					chatMessageQuery.str("");
					chatMessageQuery << "SELECT * FROM `chat_messages` WHERE `toID`='" << user.GetID() << "' or `fromID`='" << user.GetID() << "';";
					messageArray = GetChatMessagesInJSONFormat(chatMessageQuery.str(), &db);
				}


				ostFinal.str("");
				ostFinal << "\"result\": \"success\"," << std::endl;
				ostFinal << "\"userArray\": [" << userArray << "]," << std::endl;
				ostFinal << "\"messageArray\": [" << messageArray << "]";
			}


			indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}


		// --- JSON avatar list
		if(action == "JSON_getAvatarList")
		{
			ostringstream	ost;
			string		sessid;
			int			affected;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			ost << "SELECT * FROM `users_avatars` WHERE `userid`=\"" << user.GetID() << "\";";
			affected = db.Query(ost.str());
			if(affected > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{ \"folder\": \"" << db.Get(i, "folder") << "\", \"filename\": \"" << db.Get(i, "filename") << "\", \"isActive\": \"" << db.Get(i, "isActive") << "\", \"avatarID\": \"" << db.Get(i, "id") << "\" }";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				CLog	log;
				ost.str("");
				MESSAGE_DEBUG("", action, "there are no avatars for user " + user.GetLogin());
			}

			indexPage.RegisterVariableForce("result", "[" + ost.str() + "]");

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get user CV
		if(action == "JSON_getUserCV")
		{
			ostringstream	ost, ostResult;
			string		sessid;
			int			affected;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}

			affected = db.Query("SELECT `cv` FROM `users` WHERE `id`=\"" + user.GetID() + "\";");
			if(affected > 0)
			{
				ostResult << "{ \"result\": \"success\", \"cv\": \"" << db.Get(0, "cv") << "\" }";
			}
			else
			{
				ostResult << "{ \"result\": \"error\", \"description\": \"error receiving CV FROM DB\" }";
				{
					MESSAGE_ERROR("", action, "receiving users CV [" + user.GetID() + "]");
				}
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get comments list on message
		if((action == "JSON_getCommentsOnMessage") ||
			(action == "JSON_getCommentsOnBook") ||
			(action == "JSON_getCommentsOnCertification") ||
			(action == "JSON_getCommentsOnCompany") ||
			(action == "JSON_getCommentsOnLanguage") ||
			(action == "JSON_getCommentsOnScienceDegree"))
		{
			ostringstream	ost, ostList;
			string			messageID;
			string		  commentType;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			messageID = indexPage.GetVarsHandler()->Get("messageID");

			// --- ensure that it is number
			try {
				messageID = to_string(stol(messageID));
			} catch(...) {
				{
					MESSAGE_ERROR("", action, "can't convert messageID[" + messageID + "] to number");
				}
				messageID = "";
			}

			if(action == "JSON_getCommentsOnMessage") commentType = "\"message\"";
			if(action == "JSON_getCommentsOnBook") commentType = "\"book\"";
			if(action == "JSON_getCommentsOnCertification") commentType = "\"certification\", \"course\"";
			if(action == "JSON_getCommentsOnScienceDegree") commentType = "\"university\"";
			if(action == "JSON_getCommentsOnCompany") commentType = "\"company\"";
			if(action == "JSON_getCommentsOnLanguage") commentType = "\"language\"";

			if(messageID != "")
			{
				int				affected;

				ostList.str("");
				ost.str("");
				ost << "SELECT "
					<< "`feed_message_comment`.`id` as `feed_message_comment_id`, "
					<< "`feed_message_comment`.`messageID` as `feed_message_comment_messageID`, "
					<< "`feed_message_comment`.`userID` as `feed_message_comment_userID`, "
					<< "`feed_message_comment`.`comment` as `feed_message_comment_comment`, "
					<< "`feed_message_comment`.`eventTimestamp` as `feed_message_comment_eventTimestamp`, "
					<< "`users`.`id` as `users_id`, "
					<< "`users`.`name` as `users_name`, "
					<< "`users`.`nameLast` as `users_nameLast` "
					<< "from `feed_message_comment` "
					<< "inner join `users` on `feed_message_comment`.`userID`=`users`.`id` "
					<< "where `messageID`=\"" << messageID << "\" and `feed_message_comment`.`type` in (" + commentType + ") and `users`.`isactivated`='Y' and `users`.`isblocked`='N'";

				affected = db.Query(ost.str());
				if(affected)
				{
					ostList.str("");
					for(int i = 0; i < affected; i++)
					{
						if(i > 0) ostList << "," << std::endl;
						ostList << "{";
						ostList << "\"id\" : \"" << db.Get(i, "feed_message_comment_id") << "\",";
						ostList << "\"messageID\" : \"" << db.Get(i, "feed_message_comment_messageID") << "\",";
						ostList << "\"user\" : {";
						ostList << 		"\"userID\" : \"" << db.Get(i, "users_id") << "\",";
						ostList << 		"\"name\" : \"" << db.Get(i, "users_name") << "\",";
						ostList << 		"\"nameLast\" : \"" << db.Get(i, "users_nameLast") << "\"";
						ostList << "},";
						ostList << "\"comment\" : \"" << db.Get(i, "feed_message_comment_comment") << "\",";
						ostList << "\"eventTimestamp\" : \"" << db.Get(i, "feed_message_comment_eventTimestamp") << "\",";
						ostList << "\"eventTimestampDelta\":\"" << GetHumanReadableTimeDifferenceFromNow(db.Get(i,"feed_message_comment_eventTimestamp")) << "\"";
						ostList << "}";
					}
				}

				ost.str("");
				ost << "{" << std::endl;
				ost << "\"result\": \"success\",";
				ost << "\"commentsList\": [" << ostList.str() << "]" << std::endl;
				ost << "}" << std::endl;
			}
			else
			{
				// --- Empty messageID
				ost.str("");
				ost << "{";
				ost << "\"result\": \"error\",";
				ost << "\"description\": \"can't get comments list due to messageID is empty\"";
				ost << "}";

				MESSAGE_ERROR("", action, "can't convert messageID[" + indexPage.GetVarsHandler()->Get("messageID") + "] to number");
			} // --- if(messageID != "")

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("template page missing");
			} // if(!indexPage.SetTemplate("json_response.htmlt"))

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_chatPostMessage")
		{
			ostringstream	ost, ostFinal;
			string			sessid, message = "", toID = "";

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ostFinal << "result: fail";
			}
			else
			{
				message = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("message"));
				toID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("toID"));

				MESSAGE_DEBUG("", action, "message [" + message + "]");

				if(message.length() && toID.length())
				{
					int				affected;

					ost.str("");
					ost << "insert into `chat_messages` (`message`, `fromType`, `fromID`, `toType`, `toID`, `messageStatus`, `eventTimestamp`) \
							VALUES (\
							\"" << message << "\", \
							\"fromUser\", \
							\"" << user.GetID() << "\", \
							\"toUser\", \
							\"" << toID << "\", \
							\"unread\", \
							NOW() \
							);";

					db.Query(ost.str());

					ost.str("");
					ost << "SELECT * FROM `chat_messages` WHERE `fromID`='" << user.GetID() << "' and `toID`='" << toID << "' and `messageStatus`='unread' ORDER BY `id` DESC LIMIT 0,1;";
					affected = db.Query(ost.str());
					if(affected)
					{
						string		tempMessage = db.Get(0, "message");

						if(tempMessage == message)
						{
							ostringstream	chatMessageQuery;
							string			messageObj;

							chatMessageQuery.str("");
							chatMessageQuery << "SELECT * FROM `chat_messages` WHERE `id`='" << db.Get(0, "id") << "';";
							messageObj = GetChatMessagesInJSONFormat(chatMessageQuery.str(), &db);

							if(messageObj.length())
							{
								ostFinal.str("");
								ostFinal << "\"result\": \"success\"," << std::endl;
								ostFinal << "\"messageObj\": " << messageObj << std::endl;

							}
							else
							{
								{
									CLog			log;
									ostringstream	ost;

									ost.str("");
									ost << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": in getting chat message by ID FROM DB (" << db.Get(0, "id") << ")";
									log.Write(ERROR, ost.str());
								}
								ostFinal.str("");
								ostFinal << "\"result\": \"error\"," << std::endl;
								ostFinal << "\"description\": \"проблема с базой данных (сообщите администрации)\"" << std::endl;
							}
						}

						else
						{
							{
								CLog			log;
								ostringstream	ost;

								ost.str("");
								ost << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": can't find message after inserting into DB (original message: [" << message << "], DB message: [" << tempMessage << "])";
								log.Write(ERROR, ost.str());
							}
							ostFinal.str("");
							ostFinal << "\"result\": \"error\"," << std::endl;
							ostFinal << "\"description\": \"проблема с базой данных (сообщите администрации)\"" << std::endl;
						}

					}
					else
					{
						{
							MESSAGE_ERROR("", action, "can't find message after inserting into DB");
						}
						ostFinal.str("");
						ostFinal << "\"result\": \"error\"," << std::endl;
						ostFinal << "\"description\": \"проблема с базой данных (сообщите администрации)\"" << std::endl;
					}


				}
				else
				{
					{
						
						MESSAGE_DEBUG("", action, "message_body or message_recipient is empty");
					}
					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"сообщение должно содержать текст\"" << std::endl;
				}





			}


			indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_chatMarkMessageReadByMessageID")
		{
			ostringstream	ost, ostFinal;
			string			messageID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				messageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageid"));

				if(messageID.length())
				{
					db.Query("UPDATE `chat_messages` SET `messageStatus`=\"read\" WHERE `toID`='" + user.GetID() + "' and `id`='" + messageID + "';");

					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"" << std::endl;
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "messageID is empty");

					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"messageID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_notificationMarkMessageReadByMessageID")
		{
			ostringstream	ost, ostFinal;
			string			notificationID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_ERROR("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				notificationID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("notificationID"));

				if(notificationID.length())
				{
					db.Query("UPDATE `users_notification` SET `notificationStatus`=\"read\" WHERE `userId`='" + user.GetID() + "' and `id`='" + notificationID + "';");

					if(!db.isError())
					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"" << std::endl;
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "updating users_notification table");
						}

						ostFinal.str("");
						ostFinal << "\"result\": \"fail\"" << std::endl;
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "notificationID is empty");
					}
					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"notificationID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_chatMarkMessageReadByUserID")
		{
			ostringstream	ost, ostFinal;
			string			userID = "";

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				userID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("userID"));

				if(userID.length())
				{
					ost.str("");
					ost << "UPDATE `chat_messages` SET `messageStatus`=\"read\" \
							WHERE `fromID`='" << userID << "' AND `toID`='" << user.GetID() << "' AND `messageStatus`='unread';";

					db.Query(ost.str());

					{
						ostFinal.str("");
						ostFinal << "\"result\": \"success\"," << std::endl;
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "userID is empty");
					}
					ostFinal.str("");
					ostFinal << "\"result\": \"error\"," << std::endl;
					ostFinal << "\"description\": \"userID is empty\"" << std::endl;
				}

				indexPage.RegisterVariableForce("result", "{" + ostFinal.str() + "}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- cleanup news feed image after closing the modal window
		if((action == "AJAX_cleanupFeedImages") || (action == "AJAX_editCleanupFeedImages"))
		{
			string		imageTempSet;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{
				imageTempSet = indexPage.GetVarsHandler()->Get("imageTempSet");
				if(imageTempSet.length() > 0)
				{
					RemoveMessageImages("`tempSet`=\"" + imageTempSet + "\" AND `userID`=\"" + user.GetID() + "\" AND `setID`=\"0\"", &db);

					// --- cleanup DB tempSet for edit images
					db.Query("UPDATE `feed_images` SET `tempSet`='0', `removeFlag`=\"keep\" WHERE `tempSet`=\"" + imageTempSet + "\" AND `userID`=\"" + user.GetID() + "\";");
					if(db.isError())
					{
						MESSAGE_ERROR("", action, "DB error message(" + db.GetErrorMessage() + ")");
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "imageTempSet is empty");
					}
				}

				indexPage.RegisterVariableForce("result", "{\"result\" : \"success\"}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}


		// --- prepare new feed image to new image set loading
		if(action == "AJAX_prepareFeedImages")
		{
			string		currentEmployment, companyId;
			ostringstream	ost;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10) + "\""
															"}");

			}
			else
			{

				ost.str("");
				ost << "update `feed_images` set `tempSet`='0' WHERE `userID`='" << user.GetID() << "';";
				db.Query(ost.str());

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"success\","
															"\"companies\":[" + GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `isBlocked`='N' and (`admin_userID`=\"" + user.GetID() + "\");", &db, &user) + "]"
															"}");
			}


			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- prepare edit feed image to new image set loading
		if(action == "AJAX_prepareEditFeedImages")
		{
			string			imageTempSet, messageID;
			ostringstream	ost;

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "" + action + ": re-login required");

				indexPage.RegisterVariableForce("result", "{"
															"\"result\":\"error\","
															"\"description\":\"re-login required\","
															"\"location\":\"/login?rand=" + GetRandom(10) + "\""
															"}");
			}
			else
			{

				messageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageID"));
				imageTempSet = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("imageTempSet"));

				if((messageID.length() > 0) and (messageID != "0") and (imageTempSet.length() > 0) and (imageTempSet != "0"))
				{
					if(AmIMessageOwner(messageID, &user, &db))
					{
						pair<string, string> 	messageOwner = GetMessageOwner(messageID, &user, &db);
						string					messageOwnerType = messageOwner.first;
						string					messageOwnerID = messageOwner.second;


						if(messageOwnerType.length() && messageOwnerID.length())
						{
							string  mediaSetID = "";

							// --- "where `tempSet`!='0'" using to speed up lookup through `feed_images` table
							db.Query("UPDATE `feed_images` set `tempSet`='0' WHERE `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" and `tempSet`!=\"0\";");

							if(db.Query("SELECT  `imageSetID` FROM  `feed_message` WHERE  `id` = \"" + messageID + "\";"))
							{
								string	imageSetID = db.Get(0, "imageSetID");

								// --- this condition avoid assigning "lost pictures" to a new message
								// --- how to reproduce:
								// --- 1) create "lost picture" (`feedImages` table: `imageSet`=0, `userID`=XXXX)
								// --- 2) craft post without images
								// --- 3) edit post title
								// ---		PROBLEM: :lost pictures" having imageSet=0 which will be reassigned to edited post
								if(imageSetID != "0")
								{
									db.Query("UPDATE `feed_images` SET `tempSet`=\"" + imageTempSet + "\", `removeFlag`=\"keep\" WHERE `setID`=\"" + imageSetID + "\";");
								}
								else
								{
									{
										MESSAGE_DEBUG("", action, "" + action + ": there is no media in this post [messageID: " + messageID + string("]"));
									}
								}
							}

							indexPage.RegisterVariableForce("result", "{\"result\" : \"success\"}");
						}
						else
						{

							{
								MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");
							}

							ost.str("");
							ost << "{";
							ost << "\"result\" : \"error\",";
							ost << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
							ost << "}";
						}
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "message.id(" + messageID + ") doesn't belongs to user.id(" + user.GetID() + ") or his companies");
						}
						indexPage.RegisterVariableForce("result", "{\"result\" : \"error\", \"description\" : \"вы не можете редактировать сообщение\"}");
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "messageID(" + messageID + ") or imageTempSet(" + imageTempSet + ") is empty");
					}
					indexPage.RegisterVariableForce("result", "{\"result\" : \"error\", \"description\" : \"issue with messageID or imageTempSet parameters (empty or '0')\"}");
				}

			}

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get current employment
		if(action == "JSON_getCurrentEmployment")
		{
			string		currentEmployment, companyId;
			ostringstream	ost;
			int			affected;

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "SELECT `company`.`name` as `company_name` FROM `company`, `users_company` WHERE `users_company`.`occupation_finish`=\"0000-00-00\" and `users_company`.`company_id`=`company`.`id`;";
			if((affected = db.Query(ost.str())) > 0)
			{
				ost.str("");
				for(int i = 0; i < affected; i++)
				{
					ost << "{ \"folder\": \"" << db.Get(i, "folder") << "\", \"filename\": \"" << db.Get(i, "filename") << "\", \"isActive\": \"" << db.Get(i, "isActive") << "\" }";
					if(i < (affected-1)) ost << ", ";
				}
			}
			else
			{
				{
					MESSAGE_DEBUG("", action, "there are no company of current employmant of user " + user.GetID());
				}
			}
			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("ajax_getJobTitles.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}

		}
		// --- JSON part has eneded


		// --- AJAX part has started

		// --- AJAX post message to news feed
		if(action == "AJAX_postNewsFeedMessage")
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage;
			string			newsFeedMessageDstType;
			string			newsFeedMessageDstID;
			string			newsFeedMessageSrcType;
			string			newsFeedMessageSrcID;
			string			newsFeedMessageTitle;
			string			newsFeedMessageLink;
			string			newsFeedMessageText;
			string			newsFeedMessageRights;
			string			newsFeedMessageImageTempSet;
			string			newsFeedMessageImageSet;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ost.str("");
				ost << "[{\"result\": \"error\"}, {\"description\": \"session lost. Need to relogin\"}]";

				indexPage.RegisterVariableForce("result", ost.str());

				if(!indexPage.SetTemplate("json_response.htmlt"))
				{
					MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
					throw CExceptionHTML("user not activated");
				} // if(!indexPage.SetTemplate("json_response.htmlt"))
			}
			else
			{
				bool			admittedToPost = false;

				// --- Authorized user
				newsFeedMessageDstType = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageDstType"));
				newsFeedMessageDstID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("newsFeedMessageDstID"));
				newsFeedMessageSrcType = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageSrcType"));
				newsFeedMessageSrcID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("newsFeedMessageSrcID"));
				newsFeedMessageTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageTitle"));
				newsFeedMessageLink = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageLink"));
				newsFeedMessageText = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageText"));
				newsFeedMessageRights = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageRights"));
				newsFeedMessageImageTempSet = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("newsFeedMessageImageTempSet"));
				newsFeedMessageImageSet = "";

				if(newsFeedMessageDstID == "") newsFeedMessageDstID = "0";

				// --- check src if permitted to post message
				if((newsFeedMessageSrcType == "") || (newsFeedMessageSrcType == "user"))
				{
					if(newsFeedMessageSrcID != user.GetID())
					{
						MESSAGE_ERROR("", action, "spoofed userID");
					}

					newsFeedMessageSrcType = "user";
					newsFeedMessageSrcID = user.GetID();
					admittedToPost = true;
				}
				else if(newsFeedMessageSrcType == "company")
				{
					if(db.Query("SELECT `id`, `admin_userID` FROM `company` WHERE `id`=\"" + newsFeedMessageSrcID + "\" AND `admin_userID`=\"" + user.GetID() + "\";"))
						admittedToPost = true;
					else
					{
						MESSAGE_ERROR("", action, "userID[" + user.GetID() + "] spoofed companyID[" + newsFeedMessageSrcID + "]");
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "unknown newsFeedMessageSrcType[" + newsFeedMessageSrcType + "]");
					}
				}

				// --- check dst if permitted to post message
				if(newsFeedMessageDstType == "group")
				{
					if(db.Query("SELECT `id` FROM `groups` WHERE `id`=\"" + newsFeedMessageDstID + "\" AND `isBlocked`=\"N\";"))
					{
						// --- no change in post admittance
						db.Query("UPDATE `groups` SET `eventTimestampLastPost`=UNIX_TIMESTAMP() WHERE `id`=\"" + newsFeedMessageDstID + "\";");
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "DB error message(" + db.GetErrorMessage() + ")");
						}
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "group.id[" + newsFeedMessageDstID + "] not found or blocked");
						}

						admittedToPost = false;
					}
				}


				if(admittedToPost)
				{
					// --- handle images assigned to message
					if(newsFeedMessageImageTempSet.length() && (newsFeedMessageImageTempSet != "0"))
					{
						// --- remove all images subjected to removal
						RemoveMessageImages("`tempSet`=\"" + newsFeedMessageImageTempSet + "\" and `userID`=\"" + user.GetID() + "\" and `removeFlag`=\"remove\";", &db);

						// --- update images assigned to message
						if(db.Query("SELECT `id` FROM `feed_images` WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" and `userID`=\"" + user.GetID() + "\";"))
						{
							newsFeedMessageImageSet = db.Get(0, "id");

							db.Query("UPDATE  `feed_images` SET "
										"`setID`=\"" + newsFeedMessageImageSet + "\","
										"`tempSet`=\"0\","
										"`srcType`=\"" + newsFeedMessageSrcType + "\","
										"`userID`=\"" + newsFeedMessageSrcID + "\" "
										"WHERE  `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `userID`=\"" + user.GetID() + "\";");
						}
						else
						{
							MESSAGE_DEBUG("", action, "" + action + ": DEBUG: there are no images associated with `tempSet`='" + newsFeedMessageImageTempSet + "' and `userID`='" + user.GetID() + "'");
						}
					}
					else
					{
						MESSAGE_ERROR("", action, "imageTempSet is empty or 0;");
					}

					if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImageSet == "")))
					{
						unsigned long	feed_messageID = 0;
						ost.str("");
						ost << "INSERT INTO `feed_message` (`title`, `link`, `message`, `imageSetID`, `access`) \
								VALUES (\
								\"" << newsFeedMessageTitle << "\", \
								\"" << newsFeedMessageLink << "\", \
								\"" << newsFeedMessageText << "\", \
								\"" << (newsFeedMessageImageSet.length() ? newsFeedMessageImageSet : "0") << "\", \
								\"" << newsFeedMessageRights << "\" \
								);";
						feed_messageID = db.InsertQuery(ost.str());
						if(feed_messageID)
						{
							ost.str("");
							ost << "INSERT INTO `feed` (`title`, `dstType`, `dstID`, `srcType`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << newsFeedMessageDstType << "\",\"" << newsFeedMessageDstID << "\",\"" << newsFeedMessageSrcType << "\",\"" << newsFeedMessageSrcID << "\", \"11\", " << feed_messageID << ", NOW());";

							if(db.InsertQuery(ost.str()))
							{
								ost.str("");
								ost << "[";
								ost << "{";
								ost << "\"result\": \"success\",";
								ost << "\"description\": \"message has been posted\"";
								ost << "}";
								ost << "]";
							}
							else
							{
								{
									MESSAGE_ERROR("", action, "inserting into feed");
								}
								ost.str("");
								ost << "[";
								ost << "{";
								ost << "\"result\": \"error\",";
								ost << "\"description\": \"error inserting into feed\"";
								ost << "}";
								ost << "]";
							}


						}
						else
						{
							{
								MESSAGE_ERROR("", action, "inserting into feed_message");
							}
							ost.str("");
							ost << "[";
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"error inserting creating message\"";
							ost << "}";
							ost << "]";
						}

						{
							MESSAGE_DEBUG("", action, "" + action + ": end (message FROM " + newsFeedMessageSrcType + ".id[" + newsFeedMessageSrcID + "] has been posted)");
						}
					} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
					else
					{
						// --- Empty title, message and image
						{
							MESSAGE_ERROR("", action, "can't post message due to title, text and image is empty ");
						}

						ost.str("");
						ost << "[";
						ost << "{";
						ost << "\"result\": \"error\",";
						ost << "\"description\": \"can't post message due to title, text and image is empty \"";
						ost << "}";
						ost << "]";

					} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
				}
				else
				{
					ost.str("");
					ost << "[";
					ost << "{";
					ost << "\"result\": \"error\",";
					ost << "\"description\": \"prohibited to post message\"";
					ost << "}";
					ost << "]";
					{
						MESSAGE_ERROR("", action, "" + newsFeedMessageSrcType + ".id[" + newsFeedMessageSrcID + "] prohibited to post");
					}
				}
			} // if(user.GetLogin() == "Guest")


			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("json_response.htmlt"))

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		// --- AJAX update message to news feed
		if(action == "AJAX_updateNewsFeedMessage")
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage;
			string			newsFeedMessageID;
			string			newsFeedMessageTitle;
			string			newsFeedMessageLink;
			string			newsFeedMessageText;
			string			newsFeedMessageRights;
			string			newsFeedMessageImageTempSet;
			string			newsFeedMessageImageSet;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ost.str("");
				ost << "[{\"result\": \"error\"}, {\"description\": \"session lost. Need to relogin\"}]";

				indexPage.RegisterVariableForce("result", ost.str());

				if(!indexPage.SetTemplate("json_response.htmlt"))
				{
					MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
					throw CExceptionHTML("user not activated");
				}
			}
			else
			{
				// --- Authorized user
				newsFeedMessageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("newsFeedMessageID"));
				newsFeedMessageTitle = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageTitle"));
				newsFeedMessageLink = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageLink"));
				newsFeedMessageText = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageText"));
				newsFeedMessageRights = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("newsFeedMessageRights"));
				newsFeedMessageImageTempSet = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("newsFeedMessageImageTempSet"));
				newsFeedMessageImageSet = "";

				// --- messageID defined
				if(newsFeedMessageID.length() && (AmIMessageOwner(newsFeedMessageID, &user, &db)))
				{
					pair<string, string> 	messageOwner = GetMessageOwner(newsFeedMessageID, &user, &db);
					string					messageOwnerType = messageOwner.first;
					string					messageOwnerID = messageOwner.second;


					if(messageOwnerType.length() && messageOwnerID.length())
					{
						if(!isAdverseWordsHere(newsFeedMessageText, &db) && !isAdverseWordsHere(newsFeedMessageTitle, &db))
						{
							// --- handle images assigned to message
							// --- remove all images subjected to removal
							RemoveMessageImages(" `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" AND `removeFlag`=\"remove\" ", &db);

							// --- define new SetID FROM existing images or newly uploaded
							if(db.Query("SELECT `id`,`setID` FROM `feed_images` WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\" LIMIT 0,1;"))
							{
								newsFeedMessageImageSet = db.Get(0, "setID");

								// --- if there were no media uploaded during message crafting, but new media uploaded in message editing
								// --- creating new uniq imageSetID
								if(newsFeedMessageImageSet == "0") newsFeedMessageImageSet = db.Get(0, "id");
							}
							else
							{
								MESSAGE_DEBUG("", action, "" + action + ": there is no media attached to the message.id [" + newsFeedMessageID + "]");
							}

							if(newsFeedMessageImageSet.length() > 0)
								db.Query("UPDATE `feed_images` SET `setID`=\"" + newsFeedMessageImageSet + "\", `tempSet`=\"0\" WHERE `tempSet`=\"" + newsFeedMessageImageTempSet + "\" AND `srcType`=\"" + messageOwnerType + "\" AND `userID`=\"" + messageOwnerID + "\";");

							if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImageTempSet == "") && (newsFeedMessageImageSet == "")))
							{
								string		messageId;

								ost.str("");
								ost << "UPDATE `feed_message` SET  "
									<< "`title`=\"" << newsFeedMessageTitle << "\", "
									<< "`link`=\"" << newsFeedMessageLink << "\", "
									<< "`message`=\"" << newsFeedMessageText << "\", "
									<< "`imageSetID`=\"" << (newsFeedMessageImageSet == "" ? "0" : newsFeedMessageImageSet) << "\", "
									<< "`access`=\"" << newsFeedMessageRights << "\" "
									<< "WHERE `id`='" << newsFeedMessageID << "';";
								db.Query(ost.str());

								ost.str("");
								ost << "[";
								ost << "{";
								ost << "\"result\": \"success\",";
								ost << "\"description\": \"message has been updated\"";
								ost << "}";
								ost << "]";

								{
									MESSAGE_DEBUG("", action, "" + action + ": message [id = " + newsFeedMessageID + "] FROM " + messageOwnerType + ".id[" + messageOwnerID + "] has been posted");
								}


								indexPage.RegisterVariableForce("result", ost.str());

								if(!indexPage.SetTemplate("json_response.htmlt"))
								{
									MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
									throw CExceptionHTML("user not activated");
								} // if(!indexPage.SetTemplate("json_response.htmlt"))
							} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
							else
							{
								// --- Empty title, message and image
								ost.str("");
								ost << "[";
								ost << "{";
								ost << "\"result\": \"error\",";
								ost << "\"description\": \"can't post message due to title, text and image is empty \"";
								ost << "}";
								ost << "]";


								{
									MESSAGE_ERROR("", action, "can't post message due to title, text and image is empty ");
								}
							} // if(!((newsFeedMessageTitle == "") && (newsFeedMessageText == "") && (newsFeedMessageImage == "")))
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "[";
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"присутствуют запрещенные слова\"";
							ost << "}";
							ost << "]";

							{
								MESSAGE_ERROR("", action, "can't post message due to adverse words");
							}
						}
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "message owner error (type:" + messageOwnerType + ", id:" + messageOwnerID + ")");
						}

						ost.str("");
						ost << "{";
						ost << "\"result\" : \"error\",";
						ost << "\"description\" : \"Ошибка: Не получилось определить владельца сообщения\"";
						ost << "}";
					}

				} // if(newsFeedMessageID.length() > 0)
				else
				{
					// --- Empty title, message and image
					ost.str("");
					ost << "[";
					ost << "{";
					ost << "\"result\": \"error\",";
					ost << "\"description\": \"Ошибка: сообщение не найдено или не ваше\"";
					ost << "}";
					ost << "]";

					MESSAGE_ERROR("", action, "can't update message due to messageID(" + newsFeedMessageID + ") is empty or not belongs to you");
				}
			} // if(user.GetLogin() == "Guest")

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}

		}

		// --- AJAX comment on message in news feed
		if((action == "AJAX_commentOnMessageInNewsFeed") ||
			(action == "AJAX_commentOnBookInNewsFeed") ||
			(action == "AJAX_commentOnCertificationInNewsFeed") ||
			(action == "AJAX_commentOnCourseInNewsFeed") ||
			(action == "AJAX_commentOnCompanyInNewsFeed") ||
			(action == "AJAX_commentOnLanguageInNewsFeed") ||
			(action == "AJAX_commentOnScienceDegreeInNewsFeed"))
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream   ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ost.str("");
				ost << "{\"result\": \"error\", \"description\": \"session lost. Need to relogin\"}";

				indexPage.RegisterVariableForce("result", ost.str());

				if(!indexPage.SetTemplate("json_response.htmlt"))
				{
					MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
					throw CExceptionHTML("Template file was missing");
				} // if(!indexPage.SetTemplate("json_response.htmlt"))
			}
			else
			{
				string			newsFeedMessageID;
				string			newsFeedMessageComment;
				string			newsFeedMessageCommentType;

				// --- "new" used due to possibility of drop "standard exception"
				char			*convertBuffer = new char[ 1024 * 1024];

				// --- This line will not be reached in case of error in memory allocation
				// --- To avoid throw std exception use char *a = new(std::nothrow) char[ 0x7FFFFFFF ];
				if(!convertBuffer)
				{
					MESSAGE_ERROR("", action, "can't allocate memory");
				}

				// --- Authorized user
				newsFeedMessageID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("messageID"));
				newsFeedMessageComment = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("comment"));

				// --- messageID defined
				if(newsFeedMessageID.length())
				{
					unsigned long	feed_message_comment_id;
					string			messageOwnerID = "";
					string			messageOwnerType = "user";

					if(action == "AJAX_commentOnMessageInNewsFeed")
					{
						newsFeedMessageCommentType = "message";

						// --- check that message exists
						if(db.Query("SELECT `id` FROM `feed_message` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							// --- looking for message owner
							if(db.Query("SELECT * FROM `feed` WHERE `actionTypeId`=\"11\" and `actionId`=\"" + newsFeedMessageID + "\";"))
							{
								messageOwnerID = db.Get(0, "userId");
								messageOwnerType = db.Get(0, "srcType");
							}
							else
							{
								{
									MESSAGE_ERROR("", action, "finding messageID in feed");
								}

								ost.str("");
								ost << "{";
								ost << "\"result\": \"error\",";
								ost << "\"description\": \"ERROR finding messageID in feed\"";
								ost << "}";
							}
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"messageID[" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't post message due to missing messageID");
						}
					} // --- action == "AJAX_commentOnMessageInNewsFeed")
					if(action == "AJAX_commentOnBookInNewsFeed")
					{
						newsFeedMessageCommentType = "book";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_books` WHERE `id`=\"" + newsFeedMessageID + "\";"))
						{
							messageOwnerID = db.Get(0, "userID");

							// --- comments must belongs to book rather than users/book
							newsFeedMessageID = db.Get(0, "bookID");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"usersBookID[" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_books table");
						}
					} // --- action == "AJAX_commentOnBookInNewsFeed"
					if(action == "AJAX_commentOnCertificationInNewsFeed")
					{
						newsFeedMessageCommentType = "certification";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_certifications` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to certification rather than users/certification
							newsFeedMessageID = db.Get(0, "track_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_certifications.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_certifications table");
						}
					} // --- action == "AJAX_commentOnCertificationInNewsFeed"
					if(action == "AJAX_commentOnCourseInNewsFeed")
					{
						newsFeedMessageCommentType = "course";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_courses` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to course rather than users/course
							newsFeedMessageID = db.Get(0, "track_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_courses.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_courses table");
						}
					} // --- action == "AJAX_commentOnCourseInNewsFeed"
					if(action == "AJAX_commentOnLanguageInNewsFeed")
					{
						newsFeedMessageCommentType = "language";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_language` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to language rather than users/language
							newsFeedMessageID = db.Get(0, "language_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_languages.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_languages table");
						}
					} // --- action == "AJAX_commentOnLanguageInNewsFeed"
					if(action == "AJAX_commentOnCompanyInNewsFeed")
					{
						newsFeedMessageCommentType = "company";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_company` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to company rather than users/company
							newsFeedMessageID = db.Get(0, "company_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_companys.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_companys table");
						}
					} // --- action == "AJAX_commentOnCompanyInNewsFeed"
					if(action == "AJAX_commentOnScienceDegreeInNewsFeed")
					{
						newsFeedMessageCommentType = "university";

						// --- check that message exists
						if(db.Query("SELECT * FROM `users_university` WHERE `id`='" + newsFeedMessageID + "';"))
						{
							messageOwnerID = db.Get(0, "user_id");

							// --- comments must belongs to university rather than users/university
							newsFeedMessageID = db.Get(0, "university_id");
						}
						else
						{
							// --- Empty title, message and image
							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"users_university.id [" << newsFeedMessageID << "] is not exists\"";
							ost << "}";

							MESSAGE_ERROR("", action, "can't find ID in users_university table");
						}
					} // --- action == "AJAX_commentOnScienceDegreeInNewsFeed"


					if(messageOwnerType == "company")
					{
						if(db.Query("SELECT `admin_userID` from `company` WHERE `id`=\"" + messageOwnerID + "\";"))
						{
							messageOwnerID = db.Get(0, "admin_userID");
						}
						else
						{
							{
								MESSAGE_ERROR("", action, "defining company(id: " + messageOwnerID + ") admin userID");
							}

							messageOwnerID = "";

							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"ERROR defining company owner\"";
							ost << "}";
						}
					}

					if(messageOwnerID.length())
					{
						// --- insert comment
						ost.str("");
						ost << "insert into `feed_message_comment` (`messageID`, `userID`, `comment`, `type`, `eventTimestamp`) VALUES (\"" << newsFeedMessageID << "\", \"" << user.GetID() << "\", \"" << newsFeedMessageComment << "\", \"" << newsFeedMessageCommentType << "\", NOW());";
						feed_message_comment_id = db.InsertQuery(ost.str());
						if(feed_message_comment_id)
						{
							vector<string>	  replyToUsers = GetUserTagsFromText(newsFeedMessageComment);

							// --- if comment written by other users to your message
							if(messageOwnerID != user.GetID())
							{
								auto	it = find(replyToUsers.begin(), replyToUsers.end(), messageOwnerID);
								if(it == replyToUsers.end())
									replyToUsers.push_back(messageOwnerID);
							}

							for(auto &replyUserID: replyToUsers)
							{
								ost.str("");
								ost << "insert into `users_notification` (`userID`, `actionTypeId`, `actionId`, `eventTimestamp`) VALUES ('" << replyUserID << "', \"19\", '" << feed_message_comment_id << "', UNIX_TIMESTAMP() );";
								if(db.InsertQuery(ost.str()))
								{
									{
										
										MESSAGE_DEBUG("", action, "success comment submission to userID[" + replyUserID + "]");
									}

								}
								else
								{
									{
										MESSAGE_ERROR("", action, "inserting into users_notification table");
									}
								}
							}

							ost.str("");
							ost << "{";
							ost << "\"result\": \"success\",";
							ost << "\"description\": \"\"";
							ost << "}";
						}
						else
						{
							{
								MESSAGE_ERROR("", action, "inserting into feed_message_comment");
							}

							ost.str("");
							ost << "{";
							ost << "\"result\": \"error\",";
							ost << "\"description\": \"ERROR inserting into feed_message_comment\"";
							ost << "}";
						}
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "finding commenting message owner");
						}

						ost.str("");
						ost << "{";
						ost << "\"result\": \"error\",";
						ost << "\"description\": \"ERROR finding commenting message owner\"";
						ost << "}";
					}
				} // if(newsFeedMessageID.length() > 0)
				else
				{
					// --- Empty title, message and image
					ost.str("");
					ost << "{";
					ost << "\"result\": \"error\",";
					ost << "\"description\": \"can't update message due to messageID is not defined\"";
					ost << "}";

					MESSAGE_ERROR("", action, "can't update message due to messageID is not defined");
				}
			} // if(user.GetLogin() == "Guest")

			indexPage.RegisterVariableForce("result", ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}


		// --- AJAX change user password
		if(action == "AJAX_changeUserPassword")
		{
			string			newPassword, cleanedPassword;
			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				error_message = "re-login required";
				MESSAGE_DEBUG("", action, error_message);
			}
			else
			{
				// --- Authorized user
				newPassword = indexPage.GetVarsHandler()->Get("password");
				cleanedPassword = CheckHTTPParam_Text(newPassword);

				if(cleanedPassword != newPassword)
				{
					error_message = "Пароль не должен содержать символов [(кавычки), (перевод строки), '<>]";
					MESSAGE_DEBUG("", action, error_message);
				}
				else
				{
					error_message = user.ChangePasswordTo(newPassword);
				}
			} // if(user.GetLogin() == "Guest")

			AJAX_ResponseTemplate(&indexPage, "", error_message);
		}


		if(action == "ajax_regNewUser_checkUser") {
			ostringstream	ost;
			string		sessid;
			string		randomValue = GetRandom(4);
			string 		userToCheck;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			userToCheck = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("regEmail"));

			if(CheckUserEmailExisting(userToCheck, &db))
			{
				{
					MESSAGE_DEBUG("", action, "" + action + ": login or email already registered");
				}
				indexPage.RegisterVariableForce("result", "already used");
			}
			else
			{
				{
					MESSAGE_DEBUG("", action, "" + action + ": login or email not yet exists");
				}
				indexPage.RegisterVariableForce("result", "free");
			}

			if(!indexPage.SetTemplate("ajax_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file ajax_response.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		if(action == "view_profile")
		{
			ostringstream	ost;
			string			sessid, friendID;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

/*
			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}
*/
			friendID = indexPage.GetVarsHandler()->Get("userid");
			if(friendID.length() && atol(friendID.c_str()))
			{
				if(friendID != user.GetID())
				{
					// --- update user watched page
					if(db.Query("SELECT `id` FROM `users_watched` WHERE `watched_userID`=\"" + friendID + "\" and `watching_userID`=\"" + user.GetID() + "\";"))
					{
						string		profile_watched_id = db.Get(0, "id");

						db.Query("update `users_watched` set `eventTimestamp`=UNIX_TIMESTAMP() WHERE `id`='" + profile_watched_id + "';");
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "updating table users_watched");
						}
					}
					else
					{
						if(user.GetID().length())
						{
							ost.str("");
							ost << "insert `users_watched` (`watched_userID`, `watching_userID`, `eventTimestamp`) VALUE (\"" << friendID << "\", \"" << user.GetID() << "\", UNIX_TIMESTAMP());";
							if(!db.InsertQuery(ost.str()))
							{
								MESSAGE_ERROR("", action, "inserting into table users_watched");
							}
						}
						else
						{
							MESSAGE_DEBUG("", action, "" + action + ": not registered user watched profile, no need to insert to DB");
						}
					}
				}

				// --- get user profile details
				ost.str("");
				ost << "SELECT * FROM `users` WHERE `id`='" << friendID << "' and `isactivated`='Y' and `isblocked`='N';";
				if(db.Query(ost.str()))
				{

					string		friendID = db.Get(0, "id");
					string		friendLogin = db.Get(0, "login");
					string		friendEmail = db.Get(0, "email");
					string		friendName = db.Get(0, "name");
					string		friendNameLast = db.Get(0, "nameLast");
					string		friendCV = db.Get(0, "cv");
					string		friendLastOnline = db.Get(0, "last_online");
					string		friendIP = db.Get(0, "ip");
					string		friendActivated = db.Get(0, "activated");
					string		friendFriendshipState = "empty";
					int			affected = 0;
					string		current_company = "0";

					friendCV = ReplaceCRtoHTML(friendCV);
					indexPage.RegisterVariableForce("friendID", friendID);
					indexPage.RegisterVariableForce("friendLogin", friendLogin);
					indexPage.RegisterVariableForce("friendEmail", friendEmail);
					indexPage.RegisterVariableForce("friendName", friendName);
					indexPage.RegisterVariableForce("friendNameLast", friendNameLast);
					indexPage.RegisterVariableForce("friendCV", friendCV);
					indexPage.RegisterVariableForce("friendLastOnline", friendLastOnline);
					indexPage.RegisterVariableForce("friendIP", friendIP);
					indexPage.RegisterVariableForce("friendActivated", friendActivated);
					indexPage.RegisterVariableForce("friendActivatedDifferenceFromNow", GetHumanReadableTimeDifferenceFromNow(friendActivated));

					if(GetTimeDifferenceFromNow(friendLastOnline) < 60)
					{
						indexPage.RegisterVariableForce("friendLastOnlineStatement", "менее минуты назад");
					}
					else
					{
						ostringstream		ost;

						ost.str("");
						ost << GetHumanReadableTimeDifferenceFromNow(friendLastOnline);
						indexPage.RegisterVariableForce("friendLastOnlineStatement", ost.str());
					}


					if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + friendID + "\" and `isActive`=\"1\";"))
						indexPage.RegisterVariableForce("friendAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));

					ost.str("");
					ost << "SELECT * FROM `users_friends` WHERE `userid`='" << user.GetID() << "' and `friendID`='" << friendID << "';";
					if(db.Query(ost.str()))
					{
						friendFriendshipState = db.Get(0, "state");
					}
					indexPage.RegisterVariableForce("friendFriendshipState", friendFriendshipState);


					ost.str("");
					ost << "SELECT `users_company`.`id` as `users_company_id`, `company`.`name` as `company_name`, `occupation_start`, `occupation_finish`, `current_company`, `responsibilities`, `company_position`.`title` as `title` \
							FROM  `company` ,  `users_company` ,  `company_position` \
							WHERE  `user_id` =  '" << friendID << "' \
							AND  `company`.`id` =  `users_company`.`company_id`  \
							AND  `company_position`.`id` =  `users_company`.`position_title_id`  \
							ORDER BY  `users_company`.`occupation_start` DESC ";
					affected = db.Query(ost.str());
					if(affected > 0) {
							ostringstream	ost1, ost2, ost3, ost4;
							string			occupationFinish;
							ost1.str("");
							ost2.str("");
							ost3.str("");
							ost4.str("");

							for(int i = 0; i < affected; i++, current_company = "0") {
								occupationFinish = db.Get(i, "occupation_finish");
								if(occupationFinish == "0000-00-00") {
									current_company = "1";
									ost2.str("");
									ost2 << indexPage.GetVarsHandler()->Get("currentCompany");
									if(ost2.str().length() > 1) ost2 << ", ";
									ost2 << db.Get(i, "company_name");
									indexPage.RegisterVariableForce("currentCompany", ost2.str());
								}

								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-md-4'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\"" : "") << ">с ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_start' class='occupation_start datePick'>" << db.Get(i, "occupation_start") << "</span> по ";
								ost1 << "<span data-id='" << db.Get(i, "users_company_id") << "' data-action='update_occupation_finish' class='occupation_finish editableSpan'>" << (occupationFinish == "0000-00-00" ? "настоящее время" : occupationFinish)  << "</span></p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-8'>";
								ost1 << "<p" << (current_company == "1" ? " class=\"current_company\" " : "") << "> \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateJobTitle' class='jobTitle editableSpan'>"  << db.Get(i, "title") << "</span> в \
								<span data-id='" << db.Get(i, "users_company_id") << "' data-action='updateCompanyName' class='companyName editableSpan'>" << db.Get(i, "company_name") << "</span>";
								// ost1 << (current_company == "1" ? " (текущее место работы)" : "") << "</p>";
								ost1 << "</div>\n";
								ost1 << "</div> <!-- row -->\n\n";
								ost1 << "<div class='row'>\n";
								ost1 << "<div class='col-md-1'>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-9'>";
								ost1 << "<p>"  << db.Get(i,"responsibilities") << "</p>";
								ost1 << "</div>\n";
								ost1 << "<div class='col-md-1'>";
								ost1 << "</div>\n";
								ost1 << "</div>\n\n";
							}
							indexPage.RegisterVariableForce("carrierPath", ost1.str());
					}
					else
					{
						indexPage.RegisterVariableForce("carrierPath", "Нет данных");
					}
				}
				else
				{
					MESSAGE_ERROR("", action, "user not found , not activated or blocked");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "userID is missing or equal zero");
			}

// TODO: remove Dec 1 '18
/*
			if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + user.GetID() + "\" and `isActive`=\"1\";"))
				indexPage.RegisterVariableForce("myUserAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));
*/
			indexPage.RegisterVariableForce("myUserAvatar", user.GetAvatar());

			if(!indexPage.SetTemplate("view_profile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file view_profile.htmlt was missing");
				throw CException("Template file was missing");
			}
		}

		// --- JSON get user notifications
		if(action == "AJAX_getUserNotification")
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage, strFriendList;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			strNewsOnSinglePage	= indexPage.GetVarsHandler()->Get("NewsOnSinglePage");
			strPageToGet 		= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}


			ost.str("");
			ost << "SELECT `users_notification`.`eventTimestamp` as `feed_eventTimestamp`, `users_notification`.`actionId` as `feed_actionId` , `users_notification`.`actionTypeId` as `feed_actionTypeId`, "
				" `users_notification`.`id` as `users_notification_id`, `users_notification`.`notificationStatus` as `users_notification_notificationStatus`, `users_notification`.`title` as `users_notification_title`, "
				" `users`.`id` as `user_id`, `users`.`name` as `user_name`, `users`.`nameLast` as `user_nameLast`, `users`.`sex` as `user_sex`, `users`.`email` as `user_email`, "
				" `action_types`.`title` as `action_types_title`, `action_types`.`title_male` as `action_types_title_male`, `action_types`.`title_female` as `action_types_title_female`, "
				" `action_category`.`title` as `action_category_title`, `action_category`.`title_male` as `action_category_title_male`, `action_category`.`title_female` as `action_category_title_female`, `action_category`.`id` as `action_category_id` "
				" FROM `users_notification` "
				" INNER JOIN  `action_types` 		ON `users_notification`.`actionTypeId`=`action_types`.`id` "
				" INNER JOIN  `action_category` 	ON `action_types`.`categoryID`=`action_category`.`id` "
				" INNER JOIN  `users` 			ON `users_notification`.`userId`=`users`.`id` "
				" WHERE `users_notification`.`userId`=\"" << user.GetID() << "\" AND `action_types`.`isShowNotification`='1' "
				" ORDER BY  `users_notification`.`eventTimestamp` DESC LIMIT " << stoi(strPageToGet, nullptr, 10) * stoi(strNewsOnSinglePage, nullptr, 10) << " , " << stoi(strNewsOnSinglePage, nullptr, 10);

			indexPage.RegisterVariableForce("result", GetUserNotificationInJSONFormat(ost.str(), &db, &user));

			ost.str("");
			ost << "update `users_notification` set `notificationStatus`=\"read\" WHERE `userId`=\"" << user.GetID() << "\";";
			db.Query(ost.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("json_response.htmlt"))

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}


		if(action == "user_notifications")
		{
			ostringstream	ost;
			string			sessid, activeUserID;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(!indexPage.SetTemplate("user_notifications.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file user_notifications.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "chat")
		{
			ostringstream	ost;
			string			sessid, activeUserID;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(!indexPage.SetTemplate("chat.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file chat.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "login")
		{
			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;
				string		sessid;
				string		randomValue = GetRandom(4);
				string 		captchaFile = GenerateImage(randomValue);
				int 		affected;
				auto		template_name = "login.htmlt"s;

				sessid = indexPage.GetCookie("sessid");

				if(sessid.length() < 5) {
					MESSAGE_ERROR("", action, "session.id [" + sessid + "] must be 20 symbols long");
					throw CException("Please enable cookie in browser.");
				}

				{
					MESSAGE_DEBUG("", action, "get login captcha for session " + sessid);
				}


				ost.str("");
				ost << "SELECT `id` FROM `captcha` WHERE `session`=\"" << sessid << "\" and `purpose`='regNewUser'";

				if((affected = db.Query(ost.str())) > 0) {
					// ------ Update session
					MESSAGE_DEBUG("", action, "update session.id(" + sessid + ") captcha");

					ost.str("");
					ost << "UPDATE `captcha` SET `code`='" << randomValue << "', `filename`='" << captchaFile << "', `timestamp`=NOW() WHERE `session`=\"" << sessid << "\" AND `purpose`='regNewUser'";
				}
				else
				{
					// ------ Create new session
					MESSAGE_DEBUG("", action, "create new session.id(" + sessid + ") captcha");

					ost.str("");
					ost << "INSERT INTO  `captcha` (`session` ,`code` ,`filename` ,`purpose`, `timestamp`) VALUES ('" << sessid << "',  '" << randomValue << "',  '" << captchaFile << "',  'regNewUser', NOW());";
				}
				db.Query(ost.str());

				indexPage.RegisterVariableForce("noun_list", GetPasswordNounsList(&db));
				indexPage.RegisterVariableForce("adjectives_list", GetPasswordAdjectivesList(&db));

				indexPage.RegisterVariableForce("title", "Добро пожаловать");
				indexPage.RegisterVariable("regEmail_checked", "0");


				indexPage.RegisterVariableForce("securityFile", captchaFile);


				if(isDemoDomain()) template_name = "login_demo.htmlt";

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_ERROR("", action, "template file " + template_name + " was missing");
					throw CException("Template file was missing");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity error to monitor) registered user(" + user.GetLogin() + ") attempts to access login page, redirect to default page");

				indexPage.Redirect("/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" + GetRandom(10));
			}

			MESSAGE_DEBUG("", action, "finish");
		}
		if(action == "logout")
		{
			ostringstream	ost;
			string		sessid;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() > 0)
			{
				db.Query("UPDATE `sessions` SET `user_id`=(SELECT `id` FROM `users` WHERE `login`=\"Guest\"), `expire`=\"1\" WHERE `id`=\"" + sessid + "\";");

				if(!indexPage.Cookie_Expire()) {
					MESSAGE_ERROR("", action, "in session expiration");
				} // --- if(!indexPage.Cookie_Expire())

			}

			if(!indexPage.SetTemplate("logout.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file logout.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				MESSAGE_DEBUG("", action, "" + action + ": end");
			}
		}

		if(action == "forget_password_page")
		{
			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(!indexPage.SetTemplate("forget_password.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file forgot_password_page.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}

		}

		if(action == "AJAX_forgetPassword")
		{
			string		login, lng, sessid;
			CUser		user;
			ostringstream	ost1, ostResult;

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				
				MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"session ID derived FROM cookie is wrong\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"url\": \"/login?rand=" << GetRandom(10) << "\"";
				ostResult << "}";
			}
			else
			{
				login = indexPage.GetVarsHandler()->Get("email");
				lng = indexPage.GetLanguage();

				login = ReplaceDoubleQuoteToQuote(login);
				login = DeleteHTML(login);
				login = SymbolReplace(login, "\r\n", "<br>");
				login = SymbolReplace(login, "\r", "<br>");
				login = SymbolReplace(login, "\n", "<br>");

				user.SetDB(&db);
				if(!user.GetFromDBbyEmail(login))
				{
					MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");

					// --- don't alert that user is missing, it make reconaissance attack easier
					ostResult.str("");
					ostResult << "{";
					ostResult << "\"result\": \"error\",";
					ostResult << "\"description\": \"Проверьте свой e-mail\"";
					ostResult << "}";
				}
				else
				{

					if(!user.isActive())
					{
						MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

						ostResult.str("");
						ostResult << "{";
						ostResult << "\"result\": \"error\",";
						ostResult << "\"description\": \"пользователь неактивирован. Проверьте e-mail и активируйте аккаунт.\"";
						ostResult << "}";
					}
					else
					{
						CMailLocal	mail;
						string		activator_id = GetRandom(20);

						MESSAGE_DEBUG("", action, "" + action + ": sending mail message with password to user (" + user.GetLogin() + ")");

						db.Query("INSERT INTO `activators` SET `id`=\"" + activator_id + "\", `user`=\"" + user.GetEmail() + "\", `type`=\"password_recovery\", `date`=NOW();");
						if(db.isError())
						{
							MESSAGE_ERROR("", action, "fail to insert activator for user.login(" + user.GetLogin() + ")");

							ostResult.str("");
							ostResult << "{";
							ostResult << "\"result\": \"error\",";
							ostResult << "\"description\": \"Ошибка БД. Мы исправим ее в течение 24 часов.\"";
							ostResult << "}";
						}
						else
						{
							indexPage.RegisterVariableForce("login", user.GetLogin());
							indexPage.RegisterVariableForce("name", user.GetName());
							indexPage.RegisterVariableForce("nameLast", user.GetNameLast());
							indexPage.RegisterVariableForce("activator_id", activator_id);
							mail.Send(user.GetLogin(), "forget", indexPage.GetVarsHandler(), &db);

							ost1.str("");
							ost1 << "/login?rand=" << GetRandom(10) << "&signinInputEmail=" << user.GetEmail();

							ostResult.str("");
							ostResult << "{";
							ostResult << "\"result\": \"success\",";
							ostResult << "\"description\": \"\",";
							ostResult << "\"url\": \"" << ost1.str() << "\",";
							ostResult << "\"email\": \"" << user.GetEmail() << "\"";
							ostResult << "}";
						}

					}  // if(!user.isActive())
				}  // if(!user.isFound())
			} // if(sessid.length() < 5)

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_recoverPassword")
		{
			string			login, lng, sessid;
			CUser			user;
			ostringstream	ost1, ostResult;
			string			error_message;
			string			redirect_url;

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				error_message = "Устаревшая сессия";
				redirect_url = "/login?rand=" + GetRandom(10) + "\"";
				MESSAGE_ERROR("", action, "with session id derived FROM cookies");
			}
			else
			{
				auto		activator_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("activator_id"));
				auto		password_hash = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password_hash"));

				if(activator_id.length())
				{
					if(db.Query("SELECT `user` FROM `activators` WHERE `id`=\"" + activator_id + "\" and `type`=\"password_recovery\";"))
					{
						auto		user_email = db.Get(0, "user");
						CUser		user(&db);

						if(user.GetFromDBbyEmail(user_email))
						{
							if((error_message = user.ChangePasswordTo(password_hash)).empty())
							{
								db.Query("DELETE FROM `activators` WHERE `user`=\"" + user_email + "\" AND `type`=\"password_recovery\";");
								if(db.isError())
								{
									MESSAGE_ERROR("", "", "fail to clean-up table activators with user.email(" + user_email + ") and type(password_recovery)");
								}

								redirect_url = "/login?rand=" + GetRandom(10) + "&signinInputEmail=" + user_email;
							}
							else
							{
								MESSAGE_ERROR("", action, error_message);
							}
						}
						else
						{
							error_message = gettext("user not found");
							MESSAGE_ERROR("", action, error_message);
						}
					}
					else
					{
						error_message = "Неизвестный токен активации";
						MESSAGE_ERROR("", action, "activator_id(" + activator_id + ") with type(password_recovery) not found in the activator table");
					}
				}
				else
				{
					error_message = "Токен активации пустой";
					MESSAGE_ERROR("", action, "activator_id is empty");
				}
			} // if(sessid.length() < 5)

			if(error_message.empty())
			{
				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"success\",";
				ostResult << "\"redirect_url\": \"" + redirect_url + "\"";
				ostResult << "}";
			}
			else
			{
				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"" + error_message + "\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"redirect_url\": \"" + redirect_url + "\"";
				ostResult << "}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_loginUser")
		{
			MESSAGE_DEBUG("", action, "start");

			vector<pair<string, string>>	error_message;
			auto							success_message = ""s;
			auto							sessid = indexPage.GetCookie("sessid");

			if(sessid.length() < 5)
			{
				MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

				error_message.push_back(make_pair("description", "session ID derived FROM cookie is wrong"));
				error_message.push_back(make_pair("type", "redirect"));
				error_message.push_back(make_pair("url", "/login?rand=" + GetRandom(10) + ""));
			}
			else
			{
				auto	login				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
				auto	password			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password"));
				auto	confirmation_code	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("confirmation_code"));
				auto	rememberMe			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("signinRemember"));
				auto	country_code		= ""s;
				auto	phone_number		= ""s;
				CUser	user;

				user.SetDB(&db);

				if(confirmation_code.length())
				{
					tie(country_code, phone_number, password) = GetCountryCodeAndPhoneNumberBySMSCode(confirmation_code, indexPage.SessID_Get_FromHTTP() , &db);
					error_message = CheckPhoneConfirmationCode(confirmation_code, indexPage.SessID_Get_FromHTTP(), &db, &user);
				}

				if(error_message.empty())
				{
					if	(
							((login.find("@") != string::npos) && user.GetFromDBbyEmail(login)) ||
							(country_code.length() && phone_number.length() && password.length() && user.GetFromDBbyPhone(country_code, phone_number)) || 
							(user.GetFromDBbyLogin(login))
						)
					{

						if(!user.isActive())
						{
							error_message.push_back(make_pair("description", "пользователь неактивирован, необходима активация"));
							MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");
						}
						else
						{
							if((password != user.GetPasswd()) || (user.GetPasswd() == ""))
							{
								if(db.Query("SELECT * FROM `users_passwd` WHERE `userID`=\"" + user.GetID() + "\" and `passwd`=\"" + password + "\";"))
								{
									// --- earlier password is user for user login
									error_message.push_back(make_pair("description", "этот пароль был изменен " + GetHumanReadableTimeDifferenceFromNow(db.Get(0, "eventTimestamp"))));
									MESSAGE_DEBUG("", action, "old password has been used for user [" + user.GetLogin() + "] login");
								}
								else
								{
									// --- password is wrong for user
									error_message.push_back(make_pair("description", "логин или пароль указаны не верно"));
									MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] failed to login due to passwd error");
								}

							}
							else
							{
								MESSAGE_DEBUG("", action, "" + action + ": switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");

								db.Query("UPDATE `sessions` SET `user_id`=\"" + user.GetID() + "\", `ip`=\"" + getenv("REMOTE_ADDR") + "\", `expire`=\"" + (rememberMe == "remember-me" ? "0" : to_string(SESSION_LEN * 60)) + "\" WHERE `id`=\"" + sessid + "\";");

								if(db.isError())
								{
									error_message.push_back(make_pair("description", gettext("SQL syntax error")));
									MESSAGE_ERROR("", action, "updating `sessions` table");
								}
								else
								{
									if(rememberMe == "remember-me")
									{
										if(!indexPage.CookieUpdateTS("sessid", 0))
										{
											MESSAGE_ERROR("", action, "in setting up expiration sessid cookie to infinite");
										}
									}

									indexPage.RegisterVariableForce("loginUser", user.GetLogin());
									indexPage.RegisterVariableForce("menu_main_active", "active");

									MESSAGE_DEBUG("", action, "redirect to \"/" + GetDefaultActionFromUserType(&user, &db) + "?rand=xxxxxx\"");

									success_message = 	"\"result\": \"success\","
														"\"description\": \"\","
														"\"url\": \"/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" + GetRandom(10) + "\"";
								}
							}
						}
					}
					else
					{
						error_message.push_back(make_pair("description", "Почта или Пароль указаны не верно."));
						MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "error_message is not empty");
				}

			} // if(sessid.length() < 5)

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}
/*
		if(action == "AJAX_loginUser")
		{
			string		login, password, lng, sessid, rememberMe;
			CUser		user;
			ostringstream	ost1, ostResult;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				
				MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

				ostResult.str("");
				ostResult << "{";
				ostResult << "\"result\": \"error\",";
				ostResult << "\"description\": \"session ID derived FROM cookie is wrong\",";
				ostResult << "\"type\": \"redirect\",";
				ostResult << "\"url\": \"/login?rand=" << GetRandom(10) << "\"";
				ostResult << "}";
			}
			else
			{
				login		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
				password	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("password"));
				rememberMe	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("remember"));
				lng			= CheckHTTPParam_Text(indexPage.GetLanguage());

				user.SetDB(&db);

				if	(
						((login.find("@") != string::npos) && user.GetFromDBbyEmail(login)) ||
						((login.find_first_not_of("1234567890+()- ") == string::npos) && user.GetFromDBbyPhone(login)) || 
						(user.GetFromDBbyLogin(login))
					)
				{

					if(!user.isActive())
					{
						MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

						ostResult.str("");
						ostResult << "{"
									"\"result\": \"error\","
									"\"description\": \"пользователь неактивирован, необходима активация\""
									"}";
					}
					else
					{
						if((password != user.GetPasswd()) || (user.GetPasswd() == ""))
						{
							if(db.Query("SELECT * FROM `users_passwd` WHERE `userID`=\"" + user.GetID() + "\" and `passwd`=\"" + password + "\";"))
							{
								// --- earlier password is user for user login
								MESSAGE_DEBUG("", action, "old password has been used for user [" + user.GetLogin() + "] login");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"этот пароль был изменен " << GetHumanReadableTimeDifferenceFromNow(db.Get(0, "eventTimestamp")) << "\"";
								ostResult << "}";
							}
							else
							{
								// --- password is wrong for user
								MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] failed to login due to passwd error");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"логин или пароль указаны не верно\"";
								ostResult << "}";
							}

						}
						else
						{
							MESSAGE_DEBUG("", action, "" + action + ": switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");

							db.Query("UPDATE `sessions` SET `user_id`=\"" + user.GetID() + "\", `ip`=\"" + getenv("REMOTE_ADDR") + "\", `expire`=\"" + (rememberMe == "remember-me" ? "0" : to_string(SESSION_LEN * 60)) + "\" WHERE `id`=\"" + sessid + "\";");

							if(db.isError())
							{
								MESSAGE_ERROR("", action, "updating `sessions` table");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"error\",";
								ostResult << "\"description\": \"ошибка БД\"";
								ostResult << "}";
							}
							else
							{
								if(rememberMe == "remember-me")
								{
									if(!indexPage.CookieUpdateTS("sessid", 0))
									{
										MESSAGE_ERROR("", action, "in setting up expiration sessid cookie to infinite");
									}
								}

								indexPage.RegisterVariableForce("loginUser", user.GetLogin());
								indexPage.RegisterVariableForce("menu_main_active", "active");

								MESSAGE_DEBUG("", action, "redirect to \"/" + GetDefaultActionFromUserType(&user, &db) + "?rand=xxxxxx\"");

								ostResult.str("");
								ostResult << "{";
								ostResult << "\"result\": \"success\",";
								ostResult << "\"description\": \"\",";
								ostResult << "\"url\": \"/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" << GetRandom(10) << "\"";
								ostResult << "}";
							}
						} // if(password != user.GetPasswd())
					}  // if(!user.isActive())
				}
				else
				{
					MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");

					ostResult.str("");
					ostResult << "{";
					ostResult << "\"result\": \"error\",";
					ostResult << "\"description\": \"Почта или Пароль указаны не верно.\"";
					ostResult << "}";
				}
			} // if(sessid.length() < 5)

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template json_response.htmlt");
				throw CExceptionHTML("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}
*/
		if(action == "regNewUser")
		{
			string			actID, sessid;
			CActivator		act;
			ostringstream	ost;
			int 			affected;
			string			error_message = "";
			string			redirect_url = "";
			string			result = "";

			MESSAGE_DEBUG("", action, "start");

			sessid = indexPage.GetCookie("sessid");
			if(sessid.length() < 5)
			{
				MESSAGE_ERROR("", action, "in session id [" + sessid + "]");
				error_message = "Разрешите cookie в браузере";
			}
			else
			{
					string 			regSecurityCode, regPassword, regEmail;

					regEmail = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("regEmail"));
					regPassword = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("regPassword"));
					regSecurityCode = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("regSecurityCode"));

					if(regEmail.length() <= 3)
					{
						MESSAGE_DEBUG("", action, "email is incorrect [" + regEmail + "]");
						error_message = "Введен некорректный email";
					}
					else
					{
						if(CheckUserEmailExisting(regEmail, &db))
						{
							MESSAGE_DEBUG("", action, "login or email already registered");
							error_message = "Пользователь с этим адресом уже зарегистрирован";
						}
						else
						{
							// ----- Check captcha and session coincidence
							if((affected = db.Query("SELECT id FROM `captcha` WHERE `purpose`='regNewUser' and `code`='" + regSecurityCode + "' and `session`='" + sessid + "' and `timestamp` > NOW() - INTERVAL " + to_string(SESSION_LEN) + " MINUTE")) == 0)
							{
								MESSAGE_DEBUG("", action, "check captcha fail");

								error_message = "Некорректный код с картинки";
								redirect_url = "/cgi-bin/index.cgi?action=weberror_captcha_template&regEmail=" + regEmail + "&rand=" + GetRandom(10);
							}
							else
							{
								CActivator 	act;
								CMailLocal	mail;
								CUser		userTemporary;
								string		remoteIP;

								MESSAGE_DEBUG("", action, "check captcha success");

								remoteIP = getenv("REMOTE_ADDR");

								affected = db.Query("DELETE FROM `captcha` WHERE `purpose`='regNewUser' and `code`=\"" + regSecurityCode + "\" and `session`=\"" + sessid + "\";");
								if(affected != 0)
								{
									MESSAGE_ERROR("", action, "in cleanup captcha table for type=regNewUser and captcha=" + regSecurityCode + ", [affected rows = " + to_string(affected) + "]");
								}

								// --- Create temporarily user
								userTemporary.SetLogin(regEmail);
								userTemporary.SetEmail(regEmail);
								userTemporary.SetPasswd(regPassword);
								userTemporary.SetCountry(indexPage.GetCountry());
								userTemporary.SetCity(indexPage.GetCity());
								userTemporary.SetType("no role");
								userTemporary.SetIP(getenv("REMOTE_ADDR"));
								userTemporary.SetLng(indexPage.GetLanguage());
								userTemporary.SetDB(&db);
								userTemporary.Create();


								// -----  Create activator for new user
								act.SetCgi(&indexPage);
								act.SetDB(&db);
								act.SetUser(indexPage.GetVarsHandler()->Get("regEmail"));
								act.SetType("regNewUser");
								act.Save();
								// act.Activate();

								indexPage.RegisterVariableForce("activator_regNewUser", act.GetID());
								mail.Send(regEmail, "activator_regNewUser", indexPage.GetVarsHandler(), &db);

								if(!indexPage.SetTemplate("activator_regNewUser.htmlt"))
								{
									MESSAGE_ERROR("", action, "template file index.htmlt was missing");
									throw CException("Template file was missing");
								} // if(!indexPage.SetTemplate("activator_regNewUser.htmlt"))

							} // if captcha correct

						} // if(CheckUserEmailExisting(regEmail))
					}

			}

			if(error_message.empty())
			{
				result = "{\"result\":\"success\",\"redirect_url\":\"/cgi-bin/index.cgi?action=activator_regNewUser_template&rand=" + GetRandom(10) + "\"}";
			}
			else
			{
				result = "{\"result\":\"error\",\"description\":\"" + error_message + "\", \"redirect_url\":\"" + redirect_url + "\"}";
			}


			// --- scoping
			{
				string			template_name = "json_response.htmlt";
				indexPage.RegisterVariableForce("result", result);

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_DEBUG("", action, "can't find template " + template_name);
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "activateNewUser")
		{
			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				CActivator 		act;
				auto			activatorID = indexPage.GetVarsHandler()->Get("activator");


				act.SetCgi(&indexPage);
				act.SetDB(&db);
				if(!act.Load(activatorID)) 
				{
					MESSAGE_DEBUG("", action, "failed to Load activator [" + activatorID + "]");

					if(!indexPage.SetTemplate("weberror_activator_notfound.htmlt"))
					{
						CLog	log;

						MESSAGE_ERROR("", action, "template file weberror_activator_notfound.htmlt was missing");
						throw CExceptionHTML("Template file was missing");
					}

				}
				else 
				{
					// --- account activated
					act.Activate();

					// --- improve the user expirience by automatically sign-in user
					// --- automatic sing-in
					string		sessid, login, rememberMe, lng;
					CUser		user;

					sessid = indexPage.GetCookie("sessid");
					if(sessid.length() < 5)
					{
						MESSAGE_DEBUG("", action, "with session id derived FROM cookies");

						if(!indexPage.SetTemplate("weberror_cookie_disabled.htmlt.htmlt"))
						{
							CLog	log;
							MESSAGE_ERROR("", action, "template weberror_cookie_disabled.htmlt can't be found");
							throw CExceptionHTML("cookies");
						}

					} // --- if(sessid.length() < 5)
					else 
					{
						login = act.GetUser();
						rememberMe = "remember-me";
						lng = indexPage.GetLanguage();

						user.SetDB(&db);
						if(!user.GetFromDBbyEmail(login)) 
						{
							MESSAGE_DEBUG("", action, "user [" + user.GetLogin() + "] not found");

							if(!indexPage.SetTemplate("weberror_user_not_found.htmlt"))
							{
								throw CExceptionHTML("template page missing");
							}

						}
						else 
						{

							if(!user.isActive()) 
							{
								CLog	log;
								MESSAGE_ERROR("", action, "user [" + user.GetLogin() + "] not activated");

								if(!indexPage.SetTemplate("weberror_user_not_activared.htmlt"))
								{
									throw CExceptionHTML("template page missing");
								}
							}
							else 
							{
								MESSAGE_DEBUG("", action, "switching session (" + sessid + ") FROM Guest to user (" + user.GetLogin() + ")");

								// --- 2delete if login works till Nov 1
								// ost1.str("");
								// ost1 << "update `users` set `last_online`=NOW(), `ip`='" << getenv("REMOTE_ADDR") << "' WHERE `login`='" << user.GetLogin() << "';";
								// db.Query(ost1.str());

								db.Query("UPDATE `sessions` SET `user_id`='" + user.GetID() + "', `ip`='" + getenv("REMOTE_ADDR") + "', `expire`=" + to_string(rememberMe == "remember-me" ? 0 : SESSION_LEN * 60) + " WHERE `id`='" + sessid + "';");

								if(rememberMe == "remember-me") 
								{
									if(!indexPage.CookieUpdateTS("sessid", 0))
									{
										MESSAGE_ERROR("", action, "sets expiration sessid cookie to infinite");
									}
								}

								indexPage.RegisterVariableForce("loginUser", user.GetLogin());
								indexPage.RegisterVariableForce("menu_main_active", "active");


								MESSAGE_DEBUG("", action, "redirection to default action to user role");

								indexPage.Redirect("/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" + GetRandom(10));

							}  // if(!user.isActive()) 
						}  // if(!user.isFound()) 
					} // if(sessid.length() < 5)
				} // if(!act.Load(activatorID))
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity should be monitor) registered user(" + user.GetLogin() + ") attempts to access activateNewUser page, redirect to default page");

				indexPage.Redirect("/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" + GetRandom(10));
			}


			MESSAGE_DEBUG("", action, "finish");
		}

		// --- Account properties
		if(action == "user_account_properties")
		{
			ostringstream	ost;
			int		affected;
			string		userID, name, nameLast, age, cv, pass, address, phone, email, isBlocked, avatarFileName, avatarFolderName, current_company;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			ost.str("");
			ost << "SELECT \
						`users`.`id` 				as `users_id`, \
						`users`.`name` 				as `users_name`, \
						`users`.`nameLast`			as `users_nameLast`, \
						`users`.`cv` 				as `users_cv`, \
						`users_passwd`.`passwd` 	as `users_passwd_passwd`, \
						`users`.`address`			as `users_address`, \
						`users`.`phone`				as `users_phone`, \
						`users`.`email`				as `users_email`, \
						`users`.`isblocked`			as `users_isblocked` \
					FROM `users` \
					INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
					WHERE `users`.`id`='" << user.GetID() << "' AND `users_passwd`.`isActive`='true';";
			affected = db.Query(ost.str());
			if(affected)
			{
				indexPage.RegisterVariableForce("menu_profile_active", "active");


				userID = db.Get(0, "users_id");
				name = db.Get(0, "users_name"); 		indexPage.RegisterVariableForce("name", name);
										  				indexPage.RegisterVariableForce("myFirstName", name);
				nameLast = db.Get(0, "users_nameLast"); indexPage.RegisterVariableForce("nameLast", nameLast);
														indexPage.RegisterVariableForce("myLastName", nameLast);
				cv = db.Get(0, "users_cv");				indexPage.RegisterVariableForce("cv", cv);
				pass = db.Get(0, "users_passwd_passwd");indexPage.RegisterVariableForce("pass", pass);
				address = db.Get(0, "users_address");	indexPage.RegisterVariableForce("address", address);
				phone = db.Get(0, "users_phone");		indexPage.RegisterVariableForce("phone", phone);
				email = db.Get(0, "users_email"); 		indexPage.RegisterVariableForce("email", email);
				isBlocked = db.Get(0, "users_isblocked");

				if(isBlocked == "Y")
				{
					indexPage.RegisterVariableForce("isblocked", "<button type=\"button\" class=\"btn btn-danger user-account-properties-visible form-control\" id=\"ButtonAccountEnable1\">Аккаунт заблокирован</button> <button type=\"button\" class=\"btn btn-success user-account-properties-hidden form-control\" id=\"ButtonAccountBlock1\">Аккаунт активен</button>");
				}
				else
				{
					indexPage.RegisterVariableForce("isblocked", "<button type=\"button\" class=\"btn btn-danger user-account-properties-hidden form-control\" id=\"ButtonAccountEnable1\">Аккаунт заблокирован</button> <button type=\"button\" class=\"btn btn-success user-account-properties-visible form-control\" id=\"ButtonAccountBlock1\">Аккаунт активен</button>");
				}

				{
					
					MESSAGE_DEBUG("", action, "user details isBlocked:[" + isBlocked + "]");
				}
			}

			if(!indexPage.SetTemplate("user_account_properties.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file user_account_properties.htmlt was missing");
				throw CExceptionHTML("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

// --- Deprecated item
/*
		if(action == "edit_profile")
		{
			ostringstream	ost;
			string		userID, name, nameLast, age, cv, pass, address, phone, email, isBlocked, avatarFileName, avatarFolderName, current_company;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");
				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}
			else
			{
				int		affected;

				ost.str("");
				ost << "SELECT \
							`users`.`id` 				as `users_id`, \
							`users`.`name` 				as `users_name`, \
							`users`.`nameLast`			as `users_nameLast`, \
							`users`.`cv` 				as `users_cv`, \
							`users_passwd`.`passwd` 	as `users_passwd_passwd`, \
							`users`.`address`			as `users_address`, \
							`users`.`phone`				as `users_phone`, \
							`users`.`email`				as `users_email`, \
							`users`.`isblocked`			as `users_isblocked` \
						FROM `users` \
						INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
						WHERE `users`.`id`='" << user.GetID() << "' AND `users_passwd`.`isActive`='true';";
				affected = db.Query(ost.str());
				if(affected)
				{
					indexPage.RegisterVariableForce("menu_profile_active", "active");

					userID = db.Get(0, "users_id");
					name = db.Get(0, "users_name"); 		indexPage.RegisterVariableForce("name", name);
											  				indexPage.RegisterVariableForce("myFirstName", name);
					nameLast = db.Get(0, "users_nameLast"); indexPage.RegisterVariableForce("nameLast", nameLast);
															indexPage.RegisterVariableForce("myLastName", nameLast);
					cv = db.Get(0, "users_cv");
					if(cv == "") cv = "Напишите несколько слов о себе";
															indexPage.RegisterVariableForce("cv", cv);
					address = db.Get(0, "users_address");	indexPage.RegisterVariableForce("address", address);
					phone = db.Get(0, "users_phone");		indexPage.RegisterVariableForce("phone", phone);
					email = db.Get(0, "users_email"); 		indexPage.RegisterVariableForce("email", email);
					isBlocked = db.Get(0, "users_isblocked");

					{
						
						MESSAGE_DEBUG("", action, "user details isBlocked:[", isBlocked, "]");
					}

				}
				else
				{
					{
						MESSAGE_ERROR("", action, "there is no user in DB [" + indexPage.GetVarsHandler()->Get("loginUser") + "]");
					}
					CExceptionHTML("no user");
				}

				indexPage.RegisterVariableForce("title", "Моя страница");
			}

			if(!indexPage.SetTemplate("edit_profile.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file edit_profile.htmlt was missing");
				throw CException("Template file edit_profile.htmlt was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}
*/
/*
		// --- no reference to this code
		if(action == "edit_company")
		{
			ostringstream	ost;
			string		userID, name, nameLast, age, cv, pass, address, phone, email, isBlocked, avatarFileName, avatarFolderName, current_company;

			{
				
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			indexPage.RegisterVariableForce("title", "Редактирование данных компании");

			if(!indexPage.SetTemplate("edit_company.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file edit_company.htmlt was missing");
				throw CException("Template file edit_company.htmlt was missing");
			}  // if(!indexPage.SetTemplate("edit_company.htmlt"))

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		} 	// if(action == "edit_company")
*/
		if(action == "JSON_getUserProfile")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	temp_userID			= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto	userID				= (temp_userID.length() ? temp_userID : user.GetID());
			auto	success_message		= "\"users\":[" + GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=\"" + userID + "\";", &db, &user) + "]";
			auto	error_message		= ""s;

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_newsFeedMarkImageToRemove")
		{
			ostringstream	ostFinal;
			string			imageIDMarkToRemove;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			imageIDMarkToRemove = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("imageID"));

			if(imageIDMarkToRemove.length() > 0)
			{
				ostringstream	ost;
				string			messageID = "";

				// --- defining messageID by image
				if(db.Query("SELECT `setID` FROM `feed_images` WHERE `id`=\"" + imageIDMarkToRemove + "\";"))
				{
					string		setID = db.Get(0, "setID");

					if(db.Query("SELECT `id` FROM `feed_message` WHERE `imageSetID`=\"" + setID + "\";"))
					{
						messageID = db.Get(0, "id");
					}
					else
					{
						{
							MESSAGE_ERROR("", action, "can't find message by imageSetID(" + setID + ")");
						}
					}
				}
				else
				{
					{
						MESSAGE_ERROR("", action, "can't find imageSet by image.id(" + imageIDMarkToRemove + ")");
					}
				}

				if(AmIMessageOwner(messageID, &user, &db))
				{
					int				affected;

					affected = db.Query("SELECT * FROM `feed_images` WHERE `id`=\"" + imageIDMarkToRemove + "\";");
					if(affected > 0)
					{
						string	  mediaType = db.Get(0, "mediaType");
						string	  dbQuery;

						if((mediaType == "video") || (mediaType == "youtube_video"))
						{
							string	  setID = db.Get(0, "setID");
							string	  tempSet = db.Get(0, "tempSet");

							if(tempSet != "0")
								dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `tempSet`=\"" + tempSet + "\";";
							else if(setID != "0")
								dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `setID`=\"" + setID + "\";";
							else
							{
								MESSAGE_ERROR("", action, "video media can't be found because tempSet=0 and setID=0");
							}
						} else if(mediaType == "image")
							dbQuery = "update `feed_images` set `removeFlag`=\"remove\" WHERE `id`=\"" + imageIDMarkToRemove + "\";";
						else
						{
							MESSAGE_ERROR("", action, "unknown mediaType [" + mediaType + "]");
						}

						if(dbQuery.length())
						{
							db.Query(dbQuery);
							if(!db.isError())
							{
								ostFinal.str("");
								ostFinal << "{" << std::endl;
								ostFinal << "\"result\" : \"success\"," << std::endl;
								ostFinal << "\"description\" : \"\"" << std::endl;
								ostFinal << "}" << std::endl;
							}
							else
							{
								MESSAGE_ERROR("", action, "marking imageID for removal (" + db.GetErrorMessage() + ")");

								ostFinal.str("");
								ostFinal << "{";
								ostFinal << "\"result\" : \"error\",";
								ostFinal << "\"description\" : \"ERROR marking imageID to removal\"";
								ostFinal << "}";

							}
						}
						else
						{
							{
								MESSAGE_ERROR("", action, "update dbQuery is empty");
							}

							ostFinal.str("");
							ostFinal << "{";
							ostFinal << "\"result\" : \"error\",";
							ostFinal << "\"description\" : \"ERROR with DB operations\"";
							ostFinal << "}";
						}

					}
					else
					{
						{
							MESSAGE_DEBUG("", action, "" + action + ": `feed_images`.`tempSet` [" + imageIDMarkToRemove + "]  is empty or doesn't belongs to you");
						}

						ostFinal.str("");
						ostFinal << "{";
						ostFinal << "\"result\" : \"success\",";
						ostFinal << "\"description\" : \"there is no media files assigned to message or they belongs to another user\"";
						ostFinal << "}";
					}

				}
				else
				{
					{
						MESSAGE_ERROR("", action, "can't edit message.id(" + messageID + ") such as neighter you(user.id: " + user.GetID() + ") nor your companies are not owning message");
					}
					ostFinal.str("");
					ostFinal << "{";
					ostFinal << "\"result\" : \"error\",";
					ostFinal << "\"description\" : \"Вам запрещено редактировать это сообщение\"";
					ostFinal << "}";
				}

			}
			else
			{
				{
					CLog	log;
					ostringstream	ost;

					ost.str("");
					ost << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": imageIDMarkToRemove [" << imageIDMarkToRemove << "]  is unknown/empty";
					log.Write(ERROR, ost.str());
				}
				ostFinal.str("");
				ostFinal << "{" << std::endl;
				ostFinal << "\"result\" : \"error\"," << std::endl;
				ostFinal << "\"description\" : \"imageIDMarkToRemove [" << imageIDMarkToRemove << "]  is unknown/empty\"" << std::endl;
				ostFinal << "}" << std::endl;
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}



		// --- AJAX_updateFirstName
		if(action == "AJAX_updateFirstName")
		{
			string			firstName;
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			firstName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			ostFinal.str("");

			if((firstName.length() > 0))
			{
				ostringstream	ost;

				{
					CLog	log;
					ostringstream	ost;

					ost.str("");
					ost << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": name [" << firstName << "]";
					log.Write(DEBUG, ost.str());
				}

				db.Query("update `users` set `name`=\"" + firstName + "\" , `sex`=\"" + AutodetectSexByName(firstName, &db) + "\"  WHERE `id`=\"" + user.GetID() + "\";");

				// --- Update live feed
				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"5\", \"0\", NOW())"))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{

					{
						CLog			log;
						ostringstream	ostTmp;

						ostTmp.str("");
						ostTmp << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": inserting into DB (" << ost.str() << ")";
						log.Write(ERROR, ostTmp.str());
					}

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				ostringstream	ost;
				{
					CLog	log;
					ost.str("");
					ost << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": firstName [" << firstName << "] is empty";
					log.Write(DEBUG, ost.str());
				}

				ost.str("");
				ost << "update `users` set `name`=\"\" WHERE `id`='" << user.GetID() << "'";
				db.Query(ost.str());

				// --- Update live feed
				ost.str("");
				ost << "INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" << user.GetID() << "\", \"6\", \"0\", NOW())";
				if(db.InsertQuery(ost.str()))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{

					{
						CLog			log;
						ostringstream	ostTmp;

						ostTmp.str("");
						ostTmp << string(__func__) + "[" + to_string(__LINE__) + "]" + action + ": inserting into DB (" << ost.str() << ")";
						log.Write(ERROR, ostTmp.str());
					}

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}

		}

		// --- AJAX_updateLastName
		if(action == "AJAX_updateLastName")
		{
			string			lastName;
			ostringstream	ostFinal;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			lastName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if((lastName.length() > 0))
			{
				db.Query("update users set `nameLast`=\"" + lastName + "\" WHERE `id`='" + user.GetID() + "'");

				// --- Update live feed
				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"4\", \"0\", NOW())"))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", "fail insert to DB");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				db.Query("update users set `nameLast`=\"\" WHERE `id`='" + user.GetID() + "'");

				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"7\", \"0\", NOW())"))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", "fail insert to DB");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}

			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			{
				
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		// --- AJAX_updateFirstLastName
		if(action == "AJAX_updateFirstLastName")
		{
			ostringstream	ostFinal;
			string			firstName, lastName;

			MESSAGE_DEBUG("", action, "start");

			if(user.GetLogin() == "Guest")
			{
				MESSAGE_DEBUG("", action, "re-login required");

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			firstName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("firstName"));
			lastName = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("lastName"));

			if((lastName.length() > 0))
			{
				db.Query("update users set `nameLast`=\"" + lastName + "\" WHERE `id`='" + user.GetID() + "'");

				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"4\", \"0\", NOW())"))
				{
				}
				else
				{
					MESSAGE_ERROR("", "", "fail insert to DB");
				}
			}
			else
			{
				db.Query("update users set `nameLast`=\"\" WHERE `id`='" + user.GetID() + "'");

				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"7\", \"0\", NOW())"))
				{
				}
				else
				{
					MESSAGE_ERROR("", "", "fail insert to DB");
				}

			}

			if((firstName.length() > 0))
			{
				db.Query("update `users` set `name`=\"" + firstName + "\", `sex`=\"" + AutodetectSexByName(firstName, &db) + "\" WHERE `id`='" + user.GetID() + "'");

				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"5\", \"0\", NOW())"))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{

					MESSAGE_ERROR("", "", "fail insert to DB");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
			}
			else
			{
				db.Query("update `users` set `name`=\"\" WHERE `id`='" + user.GetID() + "'");

				if(db.InsertQuery("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"6\", \"0\", NOW())"))
				{
					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"success\"," << std::endl;
					ostFinal << "\"description\" : \"\"" << std::endl;
					ostFinal << "}" << std::endl;
				}
				else
				{
					MESSAGE_ERROR("", "", "fail insert to DB");

					ostFinal.str("");
					ostFinal << "{" << std::endl;
					ostFinal << "\"result\" : \"error\"," << std::endl;
					ostFinal << "\"description\" : \"error inserting into user feed\"" << std::endl;
					ostFinal << "}" << std::endl;
				}


			}

			indexPage.RegisterVariableForce("result", ostFinal.str());

			if(!indexPage.SetTemplate("json_response.htmlt"))
			{
				MESSAGE_ERROR("", action, "template file json_response.htmlt was missing");
				throw CException("Template file was missing");
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateActiveAvatar")
		{
			MESSAGE_DEBUG("", action, "start");

			auto 			avatarID = (indexPage.GetVarsHandler()->Get("id") == "-1" ? "" : CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id")));
			auto			success_message = ""s;
			auto			error_message = ""s;

			if(user.GetLogin() == "Guest")
			{
				error_message = gettext("re-login required");
				MESSAGE_DEBUG("", action, error_message);
			}

			if(avatarID.length() > 0)
			{
				db.Query("update `users_avatars` set `isActive`=\"0\" WHERE `userid`='" + user.GetID() + "';");
				db.Query("update `users_avatars` set `isActive`=\"1\" WHERE `id`=\"" + avatarID + "\" and `userid`=\"" + user.GetID() + "\";");
				db.Query("INSERT INTO `feed` (`title`, `userId`, `actionTypeId`, `actionId`, `eventTimestamp`) values(\"\",\"" + user.GetID() + "\", \"8\", \"" + avatarID + "\", NOW())");
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "" + action + ": finish");
		}

		if(action == "AJAX_updateSiteTheme")
		{
			string			strPageToGet, strFriendsOnSinglePage;
			ostringstream	ostResult;

			MESSAGE_DEBUG("", action, "start");

			ostResult.str("");
			if(user.GetLogin() == "Guest")
			{
				{
					MESSAGE_DEBUG("", action, "re-login required");
				}

				ostResult << "{\"result\":\"error\",\"description\":\"re-login required\"}";
			}
			else
			{
				string		template_name = "json_response.htmlt";
				string		error_message = "";
				string		theme_id = "";

				theme_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("theme_id"));

				if(theme_id.length())
				{
					if(db.Query("SELECT `id` FROM `site_themes` WHERE `id`=\"" + theme_id + "\";"))
					{
						db.Query("UPDATE `users` SET `site_theme_id`=\"" + theme_id + "\" WHERE `id`=\"" + user.GetID() + "\";");

						if(db.isError())
						{
							MESSAGE_ERROR("", action, "DB update failed (" + db.GetErrorMessage() + ")");
							error_message = "Ошибка БД";
						}
					}
					else
					{
						MESSAGE_DEBUG("", action, "theme_id(" + theme_id + ") not found");
						error_message = "Некорректный идентификатор темы";
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, "theme_id is empty");
					error_message = "Не указан идентификатор";
				}


				if(error_message.empty())
				{
					ostResult << "{\"status\":\"success\"}";
				}
				else
				{
					MESSAGE_DEBUG("", action, "business trip validity check failed");
					ostResult << "{\"status\":\"error\",\"description\":\"" + error_message + "\"}";
				}

				indexPage.RegisterVariableForce("result", ostResult.str());

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_DEBUG("", action, "can't find template " + template_name);
				}
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		// --- !!! IMPORTANT !!!
		// --- action _news_feed_ MUST BE BELOW action _login_user_
		if((action == "news_feed") || (action == "getUserWall"))
		{
			ostringstream	ost;
			string			strPageToGet, strNewsOnSinglePage;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			if((user.GetLogin() == "Guest") && (action == "news_feed"))
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			strNewsOnSinglePage	= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("NewsOnSinglePage"));
			strPageToGet 		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("page"));
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

// TODO: remove Dec 1 '18
/*			if(db.Query("SELECT * FROM `users_avatars` WHERE `userID`=\"" + user.GetID() + "\" and `isActive`=\"1\";"))
				indexPage.RegisterVariableForce("myUserAvatar", "/images/avatars/avatars" + string(db.Get(0, "folder")) + "/" + string(db.Get(0, "filename")));
*/
			indexPage.RegisterVariableForce("userLogin", user.GetLogin());
			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());
			indexPage.RegisterVariableForce("myUserAvatar", user.GetAvatar());

			indexPage.RegisterVariableForce("action", action);

			if(!indexPage.SetTemplate("news_feed.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template news_feed.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("news_feed.htmlt"))

			{
				MESSAGE_DEBUG("", action, "" + action + ": finish");
			}
		}

		if(action == "my_network")
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}

			if(action == "my_network") indexPage.RegisterVariableForce("title_head", "Мои друзья");
			if(action == "who_watched_on_me") indexPage.RegisterVariableForce("title_head", "Кто просматривал мой профиль");
			if(action == "companies_i_own_list") indexPage.RegisterVariableForce("title_head", "Мои компании");

			strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
			strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


			if(!indexPage.SetTemplate("my_network.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template my_network.htmlt");
				throw CExceptionHTML("user not activated");
			} // if(!indexPage.SetTemplate("my_network.htmlt"))

			{
				MESSAGE_DEBUG("", action, "" + action + ": finish");
			}
		}

		if(action == "find_friends")
		{
			string			strPageToGet, strFriendsOnSinglePage, searchText;

/*
			if(user.GetLogin() == "Guest")
			{
				ostringstream	ost;

				{
					
					MESSAGE_DEBUG("", action, "re-login required");
				}

				indexPage.Redirect("/" + GUEST_USER_DEFAULT_ACTION + "?rand=" + GetRandom(10));
			}
*/

			indexPage.RegisterVariableForce("title_head", "Поиск друзей");
			strFriendsOnSinglePage	= indexPage.GetVarsHandler()->Get("FriendsOnSinglePage");
			strPageToGet 			= indexPage.GetVarsHandler()->Get("page");
			if(strPageToGet.empty()) strPageToGet = "0";
			{
				
				MESSAGE_DEBUG("", action, "page " + strPageToGet + " requested");
			}

			indexPage.RegisterVariableForce("myFirstName", user.GetName());
			indexPage.RegisterVariableForce("myLastName", user.GetNameLast());


			if(!indexPage.SetTemplate("find_friends.htmlt"))
			{
				MESSAGE_ERROR("", action, "can't find template find_friends.htmlt");
				throw CExceptionHTML("user not activated");
			}
		}
/*
		if(action == "showmain")
		{
			if(user.GetLogin() == "Guest")
			{
				indexPage.RegisterVariableForce("title", "Добро пожаловать");
				if(!indexPage.SetTemplate("main.htmlt"))
				{
					MESSAGE_ERROR("", action, "template file anketa.htmlt was missing");
					throw CException("Template file was missing");
				}
			}
			else
			{
				MESSAGE_ERROR("", action, "(not an error, severity error to monitor) registered user(" + user.GetLogin() + ") attempts to access showmain page, redirect to default page");

				indexPage.Redirect("/" + GetDefaultActionFromUserType(&user, &db) + "?rand=" + GetRandom(10));
			}
		}
*/
		if(action == "forget")
		{
			string		login;
			CMailLocal	mail;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			login = RemoveQuotas(indexPage.GetVarsHandler()->Get("login"));
			if(login.length() > 0)
			{
				ostringstream	ost;
				int				affected;

				ost.str("");
				ost << "SELECT \
							`users`.`login` 			as `users_login`, \
							`users`.`email` 			as `users_email`, \
							`users_passwd`.`passwd`		as `users_passwd_passwd` \
						FROM `users` \
						INNER JOIN `users_passwd` ON `users_passwd`.`userID`=`users`.`id` \
						WHERE (`users`.`login`=\"" << login << "\"  OR  `users`.`email`=\"" << login << "\")  AND (`users_passwd`.`isActive`='true');";

				affected = db.Query(ost.str());
				if(affected)
				{
					indexPage.RegisterVariableForce("login", db.Get(0, "users_login"));
					indexPage.RegisterVariableForce("passwd", db.Get(0, "users_passwd_passwd"));
					indexPage.RegisterVariableForce("ip", getenv("REMOTE_ADDR"));
					mail.Send(db.Get(0, "users_email"), "forget", indexPage.GetVarsHandler(), &db);
				}
			}
			else
			{
				{
					MESSAGE_ERROR("", action, "login is not defined");
				}
			}

			indexPage.RegisterVariableForce("content", "На ваш почтовый ящик выслан пароль !");
		}

		{
			MESSAGE_DEBUG("", action, "finish (action's == \"" + action + "\") condition");
		}

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		MESSAGE_DEBUG("", action, "catch CExceptionHTML: exception reason: [" + c.GetReason() + "]");

		if(c.GetTemplate().length())
		{
			if(!indexPage.SetTemplate(c.GetTemplate()))
			{
				MESSAGE_ERROR("", action, "catch CExceptionHTML: template file " + c.GetTemplate() + " missing");
				return(-1);
			}
			indexPage.RegisterVariable("content", c.GetReason());
			indexPage.OutTemplate();
		}

		return(0);
	}
	catch(CException &c)
	{
		CLog 	log;

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}

		MESSAGE_ERROR("", "", "catch CException: exception:  " + c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		MESSAGE_ERROR("", "", "catch(exception& e): catch standard exception:  " + e.what());

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", e.what());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}

