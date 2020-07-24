#include "initial_wizard.h"

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	CUser			user;
	CMysql			db;
	string			action = "";

	MESSAGE_DEBUG("", action, __FILE__);

	signal(SIGSEGV, crash_handler);

	// --- randomization
	{
		struct timeval	tv;
		gettimeofday(&tv, NULL);
		srand(tv.tv_sec * tv.tv_usec * 100000);
	}

	try
	{

		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(DB_NAME, DB_LOGIN, DB_PASSWORD) < 0)
		{
			MESSAGE_ERROR("", action, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));

		MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);

		// --- generate common parts
		{
			// --- it has to be run before session, because session relay upon Apache environment variable
			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", action, "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			//------- Generate session
			action = GenerateSession(action, &indexPage, &db, &user);
			action = LogoutIfGuest(action, &indexPage, &db, &user);
		}
	// ------------ end generate common parts

		MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			MESSAGE_DEBUG("", action, "start");

			{
				string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_ERROR("", action, "can't find template " + template_name);
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updatePersonalData")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name	= "json_response.htmlt"s;
			auto			error_message	= ""s;
			auto			type			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("type"));
			auto			first_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("first_name"));
			auto			last_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("last_name"));
			auto			middle_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("middle_name"));

			ostResult.str("");

			if(type == "agency_employee")		type = "agency";
			else if(type == "agency_owner")		type = "agency";
			else if(type == "subc")				type = "subcontractor";
			else if(type == "approver")			type = "approver";
			else
			{
				MESSAGE_ERROR("", action, "unknown user type(" + type + ")");
				type = "subcontractor";
			}

			if(first_name.length() && last_name.length())
			{
				db.Query("UPDATE `users` SET `name`=\"" + first_name + "\",`nameLast`=\"" + last_name + "\",`nameMiddle`=\"" + middle_name + "\",`type`=\"" + type + "\" WHERE `id`=\"" + user.GetID() + "\";");
				if(db.isError())
				{
					error_message = gettext("SQL syntax error");
					MESSAGE_ERROR("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("first name and last name are mandatory");
				MESSAGE_ERROR("", action, error_message);
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);


			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updatePassportData")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name	= "json_response.htmlt"s;
			auto			error_message	= ""s;
			auto			passport_series				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("passport_series"));
			auto			passport_number				= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("passport_number"));
			auto			passport_issue_date			= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("passport_issue_date"));
			auto			passport_issue_authority	= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("passport_issue_authority"));

			ostResult.str("");

			db.Query("UPDATE `users` SET `passport_series`=\"" + passport_series + "\",`passport_number`=\"" + passport_number + "\",`passport_issue_authority`=\"" + passport_issue_authority + "\",`passport_issue_date`=STR_TO_DATE(\"" + passport_issue_date + "\",\"%d/%m/%Y\") WHERE `id`=\"" + user.GetID() + "\";");
			if(db.isError())
			{
				error_message = gettext("SQL syntax error");
				MESSAGE_ERROR("", action, error_message);
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);


			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_notifyAgencyAboutSubcRegitration")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name	= "json_response.htmlt"s;
			auto			error_message	= ""s;
			auto			agency_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("agency_to_notify"));
			auto			affected = db.Query("SELECT `id` FROM `company` WHERE `name` LIKE \"%" + agency_name + "%\" AND `type`=\"agency\";");

			ostResult.str("");


			if(affected == 1) // --- single agency found with this name
			{
				auto	agency_company_id = db.Get(0, 0);

				if(db.Query("SELECT `id` FROM `company` WHERE `admin_userID`=\"" + user.GetID() + "\";"))
				{
					auto	subc_company_id = db.Get(0, 0);
					auto	temp_error_message = NotifyAgencyAboutChanges(agency_company_id, to_string(NOTIFICATION_AGENGY_ABOUT_SUBC_REGISTRATION), subc_company_id, &db, &user);

					if(temp_error_message.length()) MESSAGE_ERROR("", action, "fail to notify agency company.id(" + agency_company_id + ")")
				}
				else
				{
					error_message = gettext("Company not found");
					MESSAGE_ERROR("", action, error_message + " by admin_user.id(" + user.GetID() + ")");
				}
			}
			else
			{
				// --- severity must be DEBUG, I set it to ERROR just for trial period 
				MESSAGE_ERROR("", action, "found more than 1 agency can't determine which one must be notified");
			}


			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);


			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_notifyAgencyAboutApproverRegitration")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name	= "json_response.htmlt"s;
			auto			error_message	= ""s;
			auto			agency_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("agency_to_notify"));
			auto			affected = db.Query("SELECT `id` FROM `company` WHERE `name` LIKE \"%" + agency_name + "%\" AND `type`=\"agency\";");

			ostResult.str("");


			if(affected == 1) // --- single agency found with this name
			{
				auto	agency_company_id = db.Get(0, 0);
				auto	temp_error_message = NotifyAgencyAboutChanges(agency_company_id, to_string(NOTIFICATION_AGENGY_ABOUT_APPROVER_REGISTRATION), user.GetID(), &db, &user);

				if(temp_error_message.length()) MESSAGE_ERROR("", action, "fail to notify agency company.id(" + agency_company_id + ")")
			}
			else
			{
				// --- severity must be DEBUG, I set it to ERROR just for trial period 
				MESSAGE_ERROR("", action, "found more than 1 agency can't determine which one must be notified");
			}


			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);


			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_notifyAgencyAboutEmployeeRegitration")
		{
			MESSAGE_DEBUG("", action, "start");

			ostringstream	ostResult;
			auto			template_name	= "json_response.htmlt"s;
			auto			error_message	= ""s;
			auto			agency_name		= CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("agency_to_notify"));
			auto			affected = db.Query("SELECT `id` FROM `company` WHERE `name` LIKE \"%" + agency_name + "%\" AND `type`=\"agency\";");

			ostResult.str("");


			if(affected == 1) // --- single agency found with this name
			{
				auto	agency_company_id = db.Get(0, 0);
				auto	temp_error_message = NotifyAgencyAboutChanges(agency_company_id, to_string(NOTIFICATION_AGENGY_ABOUT_EMPLOYEE_REGISTRATION), user.GetID(), &db, &user);

				if(temp_error_message.length()) MESSAGE_ERROR("", action, "fail to notify agency company.id(" + agency_company_id + ")")
			}
			else
			{
				// --- severity must be DEBUG, I set it to ERROR just for trial period 
				MESSAGE_ERROR("", action, "found more than 1 agency can't determine which one must be notified");
			}


			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);


			MESSAGE_DEBUG("", action, "finish");
		}








		MESSAGE_DEBUG("", "", "post-condition if(action == \"" + action + "\")");

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{

		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);
		MESSAGE_DEBUG("", action, "catch CExceptionHTML: DEBUG exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplate(c.GetTemplate()))
		{
			return(-1);
		}
		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(0);
	}
	catch(CException &c)
	{
		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			return(-1);
		}
		{
			MESSAGE_ERROR("", action, "catch CException: exception: ERROR  " + c.GetReason());
		}

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}
	catch(exception& e)
	{
		{
			MESSAGE_ERROR("", action, "catch(exception& e): catch standard exception: ERROR  " + e.what());
		}

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
