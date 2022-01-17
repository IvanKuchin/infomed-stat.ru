#include "doctor.h"

// --- function stays here because utilities_infomed do not includes ccgi.h
auto CheckMedicalRecordValidity(CCgi *indexPage, CMysql *db) -> string
{
	MESSAGE_DEBUG("", "action", "start");

	auto	error_message = ""s;
	auto	first_name = CheckHTTPParam_Text(indexPage->GetVarsHandler()->Get("___first_name"));
	auto	last_name = CheckHTTPParam_Text(indexPage->GetVarsHandler()->Get("___last_name"));
	auto	middle_name = CheckHTTPParam_Text(indexPage->GetVarsHandler()->Get("___middle_name"));
	auto	birthdate = CheckHTTPParam_Text(indexPage->GetVarsHandler()->Get("___birthdate"));
	auto	patient_id = CheckHTTPParam_Text(indexPage->GetVarsHandler()->Get("patient_id"));


	if(first_name.length() && last_name.length() && middle_name.length())
	{
		if(birthdate.length())
		{
			error_message = IsPatientExists(first_name, middle_name, last_name, birthdate, patient_id, db);
		}
		else
		{
			error_message = gettext("birthday isn't defined");
			MESSAGE_DEBUG("", "", error_message);
		}
	}
	else
	{
		error_message = gettext("first or last name isn't defined");
		MESSAGE_DEBUG("", "", error_message);
	}

	MESSAGE_DEBUG("", "action", "finish (error_message = " + error_message + ")");

	return error_message;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	c_config		config(CONFIG_DIR);
	CUser			user;
	string			action, partnerID;
	CMysql			db;
	struct timeval	tv;

	MESSAGE_DEBUG("", action, __FILE__);

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);    /* Flawfinder: ignore */

	try
	{

		indexPage.ParseURL();
		indexPage.AddCookie("lng", "ru", nullptr, "", "/");

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(&config) < 0)
		{
			MESSAGE_ERROR("", action, "can't connect to DB");
			throw CException("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		{
			MESSAGE_DEBUG("", action, "action taken from HTTP is " + action);
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
			action = GenerateSession(action, &config, &indexPage, &db, &user);
			action = LogoutIfGuest(action, &config, &indexPage, &db, &user);
		}
	// ------------ end generate common parts

		MESSAGE_DEBUG("", action, "start (action's == \"" + action + "\") condition");

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

		if(action == config.GetFromFile("default_action", "user"))
		{
			auto		template_name = action + ".htmlt";

			MESSAGE_DEBUG("", action, "start");

			if(!indexPage.SetTemplate(template_name))
			{
				MESSAGE_DEBUG("", action, "can't find template " + template_name);
			}

			MESSAGE_DEBUG("", action, "finish");
		}


		if(action == "AJAX_getDashboardData")
		{
			MESSAGE_DEBUG("", action, "start");

			auto		error_message = ""s;
			auto		affected = db.Query("SELECT COUNT(*) FROM `medical_records` WHERE `company_id` IN ("
												"SELECT `company_id_data_available` FROM `company_data_access` WHERE `company_id` IN ("
													"SELECT `company_id` FROM `company_employment` WHERE `user_id`=" + quoted(user.GetID()) +
												")"
											");");
			auto		number_of_medical_records = affected ? db.Get(0, 0) : "0";
			auto		success_message =	"\"number_of_medical_records\":" + quoted(number_of_medical_records) + ","
											"\"users\":[" + GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=" + user.GetID() + ";", &db, &user) + "]";

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getMedicalRecords")
		{
			MESSAGE_DEBUG("", action, "start");

			auto		error_message	= ""s;
			auto		patient_id		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("patient_id"));
			auto		sql_query		= "SELECT * FROM `medical_records` WHERE `company_id` IN ("
												"SELECT `company_id_data_available` FROM `company_data_access` WHERE `company_id` IN ("
													"SELECT `company_id` FROM `company_employment` WHERE `user_id`=" + quoted(user.GetID()) +
												")"
											")" + (patient_id.length() ? " AND `id`=" + quoted(patient_id) + " AND `submitter_user_id`=" + quoted(user.GetID()) : "");
			auto		success_message	= "\"medical_records\":[" + GetMedicalRecordsInJSONFormat(sql_query, &db, &user) + "]";

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_getCompanyList")
		{

			MESSAGE_DEBUG("", action, "start");


			{
				auto		error_message = ""s;
				auto		success_message = 
									"\"companies\":[" + GetCompanyListInJSONFormat("SELECT * FROM `company`", &db, &user) + "],"
									// --- user is required to understand AAA
									"\"users\":[" + GetUserListInJSONFormat("SELECT * FROM `users` WHERE `id`=" + quoted(user.GetID()) + ";", &db, &user) + "]";

									AJAX_ResponseTemplate(&indexPage, success_message, error_message);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addCompany")
		{

			MESSAGE_DEBUG("", action, "start");


			{
				auto		title = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("title"));
				auto		error_message = ""s;
				auto		success_message = ""s;

				if(GetUserAAARole(&user, &db) == "admin")
				{
					if(db.Query("SELECT `id` FROM `company` WHERE `title`=" + quoted(title) + ";"))
					{
						error_message = gettext("Company already exists");
						MESSAGE_DEBUG("", action, error_message);
					}
					else
					{
						auto	company_id = db.InsertQuery("INSERT INTO `company` (`title`, `eventTimestamp`) VALUES (" + quoted(title) + ", UNIX_TIMESTAMP())");

						if(company_id)
						{
							error_message = AddAccessDataRight(to_string(company_id), to_string(company_id), &db);
						}
						else
						{
							error_message = gettext("SQL syntax error");
							MESSAGE_DEBUG("", action, error_message);
						}
					}
				}
				else
				{
					error_message = gettext("You are not authorized");
					MESSAGE_DEBUG("", action, error_message);
				}

				AJAX_ResponseTemplate(&indexPage, success_message, error_message);
			}

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_deleteCompany")
		{
			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			success_message = ""s;

			MESSAGE_DEBUG("", action, "start");


			if(GetUserAAARole(&user, &db) == "admin")
			{

				if((error_message = CouldCompanyBeDeleted(id, &db, &user)).empty())
				{
					if((error_message = RemoveAccessDataRight(id, &db)).empty())
					{
						db.Query("DELETE FROM `company_data_access` WHERE `company_id_data_available`=" + quoted(id) + ";");
						if(db.isError())
						{
							error_message = gettext("fail to clean up") + " company_data_access"s;
							MESSAGE_ERROR("", action, error_message);
						}
						else
						{
							db.Query("DELETE FROM `company` WHERE `id`=" + quoted(id) + ";");
						}

					}
					else
					{
						MESSAGE_DEBUG("", action, error_message);
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, error_message);
				}

			}
			else
			{
				error_message = gettext("You are not authorized");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateCompanyTitle")
		{
			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			auto			success_message = ""s;

			MESSAGE_DEBUG("", action, "start");


			if(GetUserAAARole(&user, &db) == "admin")
			{

				if((error_message = CouldCompanyTitleBeChanged(id, value, &db, &user)).empty())
				{
					db.Query("UPDATE `company` SET `title`=" + quoted(value) + " WHERE `id`=" + quoted(id) + ";");
				}
				else
				{
					MESSAGE_DEBUG("", action, error_message);
				}

			}
			else
			{
				error_message = gettext("You are not authorized");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_updateUserType_SwitcherInitiated")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));

			if(value == "N")
			{
				indexPage.RegisterVariableForce("value", "");
				action = "AJAX_updateUserType";
			}
			else if(value == "Y")
			{
				indexPage.RegisterVariableForce("value", "admin");
				action = "AJAX_updateUserType";
			}
			else
			{
				MESSAGE_ERROR("", action, "unknown value(" + value + ")");
			}

			MESSAGE_DEBUG("", action, "finish");
		}
		// --- must be after AJAX_updateUserType_SwitcherInitiated
		if(action == "AJAX_updateUserType")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			user_id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			auto			success_message = ""s;


			if(user.GetID() == user_id)
			{
				error_message = gettext("can't change your own permissions");
				MESSAGE_DEBUG("", action, error_message);
			}
			else if(GetUserAAARole(&user, &db) == "admin")
			{

				if(value == "admin")
				{
					db.Query("DELETE FROM `users_aaa` WHERE `user_id`=" + quoted(user_id) + ";");
					db.Query("INSERT INTO `users_aaa` SET `user_id`=" + quoted(user_id) + ", `role`=\"admin\";");
				}
				else
				{
					db.Query("DELETE FROM `users_aaa` WHERE `user_id`=" + quoted(user_id) + ";");
				}
			}
			else
			{
				error_message = gettext("You are not authorized");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(
			(action == "AJAX_updateDoctorLastName")		||
			(action == "AJAX_updateDoctorFirstName")	||
			(action == "AJAX_updateDoctorMiddleName")	||
			(action == "AJAX_updateDoctorLogin")		||
			(action == "AJAX_updateDoctorEmail")		||
			(action == "AJAX_updateDoctorPhone")		||
			(action == "AJAX_updateDoctorPassword")		||
			(action == "AJAX_getCompanyDataAccess")		||
			(action == "AJAX_deleteDoctor")				||
			(action == "AJAX_updateDataAccess")
		)
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			auto			success_message = ""s;


			if(id.length() && value.length())
			{
				if(GetUserAAARole(&user, &db) == "admin")
				{
					if(action == "AJAX_updateDoctorLastName")	db.Query("UPDATE `users` SET `nameLast`="	+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDoctorFirstName")	db.Query("UPDATE `users` SET `name`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDoctorMiddleName")	db.Query("UPDATE `users` SET `nameMiddle`="	+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDoctorLogin")		db.Query("UPDATE `users` SET `login`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDoctorEmail")		db.Query("UPDATE `users` SET `email`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDoctorPhone")		db.Query("UPDATE `users` SET `phone`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
					if(action == "AJAX_updateDataAccess")
					{
																auto	companies_id = split(value, ',');

																error_message = RemoveAccessDataRight(id, &db);
																for(auto &company_id: companies_id)
																{
																	if(error_message.empty())
																		error_message = AddAccessDataRight(id, company_id, &db);
																}

					}
					if(action == "AJAX_updateDoctorPassword")
					{
																CUser		user(&db);

																if(user.GetFromDBbyID(id))
																{
																	error_message = user.ChangePasswordTo(value);
																}
																else
																{
																	error_message = gettext("user not found");
																	MESSAGE_ERROR("", action, error_message);
																}
					}
					if(action == "AJAX_deleteDoctor")
					{
																auto	affected = db.Query("SELECT `id` FROM `medical_records` WHERE `submitter_user_id`=" + quoted(id) + ";");
																if(affected)
																{
																	char	buffer[50];    /* Flawfinder: ignore */
																	sprintf(buffer, ngettext("%d records", "%d records", affected), affected);    /* Flawfinder: ignore */

																	error_message = gettext("doctor submitted") + " "s + buffer;
																	MESSAGE_DEBUG("", action, error_message);
																}
																else
																{
																	CUser		user(&db);

																	if(user.GetFromDBbyID(id))
																	{
																		db.Query("DELETE FROM `company_employment` WHERE `user_id`=" + quoted(id) + ";");
																		error_message = user.DeleteFromDB();
																	}
																	else
																	{
																		error_message = gettext("user not found");
																		MESSAGE_ERROR("", action, error_message);
																	}
																}
					}
					if(action == "AJAX_getCompanyDataAccess")
					{
																auto affected = db.Query("SELECT `company_id_data_available` FROM `company_data_access` WHERE `company_id`=" + quoted(id) + ";");
																for(auto i = 0; i < affected; ++i)
																{
																	if(i) success_message += ",";
																	success_message += db.Get(i, 0);
																}

																success_message = "\"company_id\":[" + success_message + "]";

					}

					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", action, error_message);
					}

				}
				else
				{
					error_message = gettext("You are not authorized");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_addDoctor")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			login = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("login"));
			auto			email = CheckHTTPParam_Email(indexPage.GetVarsHandler()->Get("email"));
			auto			phone = SymbolReplace_KeepDigitsOnly(indexPage.GetVarsHandler()->Get("phone"));
			auto			country_code = SymbolReplace_KeepDigitsOnly(indexPage.GetVarsHandler()->Get("country_code"));
			auto			password = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("pass"));
			auto			hospital = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("hospital"));

			auto			success_message = ""s;

			if(hospital.length())
			{
				if(login.length() || email.length() || phone.length())
				{
					if(password.length())
					{
						CUser	user(&db);

						if(
							(login.length() && user.GetFromDBbyLogin(login)) ||
							(email.length() && user.GetFromDBbyEmail(email)) ||
							(phone.length() && user.GetFromDBbyPhone(country_code, phone))
						)
						{
							error_message = gettext("user already exists");
							MESSAGE_DEBUG("", action, error_message);
						}
						else
						{
							user.SetLogin(login);
							user.SetEmail(email);
							user.SetType("user");
							user.SetIsActivated("Y");
							user.SetCountryCode(country_code);
							user.SetPhone(phone);
							user.SetPasswd(password);
							user.Create();

							if(user.GetID() == "0")
							{
								error_message = gettext("fail to create user");
								MESSAGE_ERROR("", action, error_message);
							}
							else
							{
								if(db.InsertQuery("INSERT INTO `company_employment` (`user_id`, `company_id`) VALUES (" + quoted(user.GetID()) + "," + quoted(hospital) + ");"))
								{

								}
								else
								{
									error_message = gettext("SQL syntax error");
									MESSAGE_ERROR("", action, error_message);
								}
							}
						}
					}
					else
					{
						error_message = gettext("password required");
						MESSAGE_DEBUG("", action, error_message);
					}
				}
				else
				{
					error_message = gettext("login or email or phone is required");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("hospital is required");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_submitMedicalRecord")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	patient_id		= CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("patient_id"));
			auto	company_id		= GetCompanyID(&user, &db);
			auto	error_message	= ""s;
			auto	success_message	= ""s;

			if(company_id.length())
			{
				if((error_message = CheckMedicalRecordValidity(&indexPage, &db)).empty())
				{
					if(
						patient_id.empty() || // --- empty patient_id means create another patient
						IsUserAllowedToChangePatientRecord(patient_id, &user, &db) // --- patient.id belongs to user
						)
					{
						regex	regex1("___");
						auto	sql_query = ""s;
						auto	http_params = indexPage.GetVarsHandler();

						auto	medical_record_item_name = http_params->GetNameByRegex(regex1);
						while(medical_record_item_name.length() && error_message.empty())
						{
							if(sql_query.length()) sql_query += ",";

							sql_query += "`" + CheckHTTPParam_Text(medical_record_item_name) + "`=" + quoted(CheckHTTPParam_Text(http_params->Get(medical_record_item_name)));

							http_params->Delete(medical_record_item_name);
							medical_record_item_name = http_params->GetNameByRegex(regex1);
						}


						if(patient_id.empty())
						{
							sql_query = "INSERT INTO `medical_records` SET "
												"`submitter_user_id`=" + user.GetID() + ","
												"`company_id`=" + company_id + ","
												"`eventTimestamp`=UNIX_TIMESTAMP(),"
												+ sql_query;
						}
						else
						{
							sql_query = "UPDATE `medical_records` SET "
												"`submitter_user_id`=" + user.GetID() + ","
												"`company_id`=" + company_id + ","
												"`eventTimestamp`=UNIX_TIMESTAMP(),"
												+ sql_query + " "
												"WHERE `id`=" + quoted(patient_id) + ";";

						}

						db.Query(sql_query);
						if(db.isError())
						{
							error_message = gettext("SQL syntax error");
							MESSAGE_ERROR("", "", error_message);
						}
						else
						{
							// --- good2go
						}

					}
					else
					{
						error_message = gettext("you are not authorized");
						MESSAGE_ERROR("", action, error_message);
					}
				}
				else
				{
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("you are not employed at any medical center");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(action == "AJAX_checkPatientExistence")
		{
			MESSAGE_DEBUG("", action, "start");

			auto	error_message = IsPatientExists(
											CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("___first_name")),
											CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("___middle_name")),
											CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("___last_name")),
											CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("___birthdate")),
											CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("patient_id")),
											&db);

			AJAX_ResponseTemplate(&indexPage, "", error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if(
			(action == "AJAX_updatePatientLastName")	||
			(action == "AJAX_updatePatientFirstName")	||
			(action == "AJAX_updatePatientMiddleName")	||
			(action == "AJAX_updatePatientBirthdate")
		)
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			value = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("value"));
			auto			success_message = ""s;


			if(id.length() && value.length())
			{
				if(IsUserAllowedToChangePatientRecord(id, &user, &db))
				{
					if(db.Query("SELECT * FROM `medical_records` WHERE `id`=" + quoted(id) + ";"))
					{
						auto	first_name	= db.Get(0, "___first_name");
						auto	last_name	= db.Get(0, "___last_name");
						auto	middle_name	= db.Get(0, "___middle_name");
						auto	birthdate	= db.Get(0, "___birthdate");

						if(db.Query("SELECT `id` FROM `medical_records` WHERE "
										"`___last_name`="	+ quoted(action == "AJAX_updatePatientLastName"		? value	:	last_name) + " AND "
										"`___first_name`="	+ quoted(action == "AJAX_updatePatientFirstName"	? value	:	first_name) + " AND "
										"`___middle_name`="	+ quoted(action == "AJAX_updatePatientMiddleName"	? value	:	middle_name) + " AND "
										"`___birthdate`="	+ quoted(action == "AJAX_updatePatientBirthdate"	? value	:	birthdate) + 
										";"
							))
						{
							error_message = gettext("patient already exists");
							MESSAGE_DEBUG("", action, error_message);
						}
						else
						{
							if(action == "AJAX_updatePatientLastName")		db.Query("UPDATE `medical_records` SET `___last_name`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
							if(action == "AJAX_updatePatientFirstName")		db.Query("UPDATE `medical_records` SET `___first_name`="	+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
							if(action == "AJAX_updatePatientMiddleName")	db.Query("UPDATE `medical_records` SET `___middle_name`="	+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");
							if(action == "AJAX_updatePatientBirthdate")		db.Query("UPDATE `medical_records` SET `___birthdate`="		+ quoted(value) + " WHERE `id`=" + quoted(id) + ";");

							if(db.isError())
							{
								error_message = gettext("SQL syntax error");
								MESSAGE_ERROR("", action, error_message);
							}
						}
					}
					else
					{
						error_message = gettext("patient not found");
						MESSAGE_DEBUG("", action, error_message);
					}
				}
				else
				{
					error_message = gettext("You are not authorized");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}

		if((action == "AJAX_deletePatient"))
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			success_message = ""s;


			if(id.length())
			{
				if(IsUserAllowedToChangePatientRecord(id, &user, &db))
				{
					db.Query("DELETE FROM `medical_records` WHERE `id`=" + quoted(id) + ";");
					
					if(db.isError())
					{
						error_message = gettext("SQL syntax error");
						MESSAGE_ERROR("", action, error_message);
					}
				}
				else
				{
					error_message = gettext("You are not authorized");
					MESSAGE_DEBUG("", action, error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", action, error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}


		MESSAGE_DEBUG("", action, "finish (action's == \"" + action + "\") condition");

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{
		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		MESSAGE_DEBUG("", action, "catch CExceptionHTML: DEBUG exception reason: [" + c.GetReason() + "]");

		if(!indexPage.SetTemplate(c.GetTemplate()))
		{
			MESSAGE_ERROR("", "", "template (" + c.GetTemplate() + ") not found");
			return(-1);
		}

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();

		return(-1);
	}
	catch(CException &c)
	{
		MESSAGE_ERROR("", action, "catch CException: exception: ERROR  " + c.GetReason());

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			MESSAGE_ERROR("", "", "template not found");
			return(-1);
		}

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();

		return(-1);
	}
	catch(exception& e)
	{
		MESSAGE_ERROR("", action, "catch(exception& e): catch standard exception: ERROR  " + e.what());

		if(!indexPage.SetTemplateFile("templates/error.htmlt"))
		{
			MESSAGE_ERROR("", "", "template not found");
			return(-1);
		}
		
		indexPage.RegisterVariable("content", e.what());
		indexPage.OutTemplate();

		return(-1);
	}

	return(0);
}
