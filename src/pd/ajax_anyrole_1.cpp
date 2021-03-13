#include "ajax_anyrole.h"

int main(void)
{
	CStatistics		appStat;  // --- CStatistics must be first statement to measure end2end param's
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

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", action, "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect() < 0)
		{
			MESSAGE_ERROR("", action, "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);


		action = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("action"));
		{
			MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);
		}

		// --- generate common parts
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
			action = LogoutIfGuest(action, &indexPage, &db, &user);

		}
	// ------------ end generate common parts

		{
			MESSAGE_DEBUG("", "", "pre-condition if(action == \"" + action + "\")");
		}

		if((action.length() > 10) && (action.compare(action.length() - 9, 9, "_template") == 0))
		{
			ostringstream	ost;
			string			strPageToGet, strFriendsOnSinglePage;

			{
				MESSAGE_DEBUG("", action, "start");
			}

			{
				string		template_name = action.substr(0, action.length() - 9) + ".htmlt";

				if(!indexPage.SetTemplate(template_name))
				{
					MESSAGE_ERROR("", action, "can't find template " + template_name);
				} // if(!indexPage.SetTemplate("my_network.htmlt"))
			}

			{
				MESSAGE_DEBUG("", action, "finish");
			}
		}

		if(action == "AJAX_getCompanyInfo")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			tin = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("tin"));
			auto			template_name = "json_response.htmlt"s;
			auto			error_message = ""s;
			auto			company_obj = ""s;
			ostringstream	ostResult;

			ostResult.str("");

			if(id.length())
			{
				company_obj = GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `id`=\"" + id + "\";", &db, &user);

				if(company_obj.length())
				{
				}
				else
				{
					error_message = gettext("Company not found");
					MESSAGE_DEBUG("", "", "company.id(" + id + ") not found");
				}	
					
			}
			else if(tin.length())
			{
				company_obj = GetCompanyListInJSONFormat("SELECT * FROM `company` WHERE `tin`=\"" + tin + "\";", &db, &user);

				if(company_obj.length())
				{
				}
				else
				{
					error_message = gettext("Company not found");
					MESSAGE_DEBUG("", "", "company.tin(" + tin + ") not found");
				}	
					
			}
			else
			{
				error_message = "Некорректный номер kompanii";
				MESSAGE_DEBUG("", "", "fail to get company.id(" + id + ")");
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\",\"companies\":[" + company_obj + "]}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}
		if(action == "AJAX_isCompanyExists")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			tin = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("tin"));
			auto			company_type = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("company_type"));
			auto			template_name = "json_response.htmlt"s;
			auto			error_message = ""s;
			auto			success_message = ""s;
			ostringstream	ostResult;

			ostResult.str("");

			if(id.length())
			{
				if(db.Query("SELECT `id` FROM `company` WHERE `id`=\"" + id + "\""s + (company_type.length() ? " AND `type`=\"" + company_type + "\"" : "") + ";"))
				{
					success_message = ",\"isExists\":\"yes\"";
				}
				else
				{
					success_message = ",\"isExists\":\"no\"";
				}
			}
			else if(tin.length())
			{
				if(db.Query("SELECT `id` FROM `company` WHERE `tin`=\"" + tin + "\""s + (company_type.length() ? " AND `type`=\"" + company_type + "\"" : "") + ";"))
				{
					success_message = ",\"isExists\":\"yes\"";
				}
				else
				{
					success_message = ",\"isExists\":\"no\"";
				}					
			}
			else
			{
				error_message = "Некорректный номер kompanii";
				MESSAGE_DEBUG("", "", "fail to get company.id(" + id + ")");
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\"" + success_message + "}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_getGeoRegionName")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			string			template_name = "json_response.htmlt";
			string			error_message = "";
			ostringstream	ostResult;

			ostResult.str("");

			if(id.length())
			{
				if(db.Query("SELECT `title` FROM `geo_region` WHERE `id`=\"" + id + "\";"))
					ostResult << "{\"result\":\"success\",\"geo_region\":{\"id\":\"" + id + "\",\"name\":\"" + db.Get(0, "title") + "\"}}";
				else
				{
					error_message = "Регион не найден";
					MESSAGE_DEBUG("", "", "region_id(" + id + ") not found");
				}	
					
			}
			else
			{
				error_message = "Некорректный номер региона";
				MESSAGE_DEBUG("", "", "fail to get region_id(" + id + ")");
			}

			if(error_message.empty())
			{
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_getGeoRegionAndLocalityNames")
		{
			string			country = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("country"));
			string			zip = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("zip"));
			string			template_name = "json_response.htmlt";
			string			error_message = "";
			ostringstream	ostResult;

			ostResult.str("");

			if(country.length() && zip.length())
			{
				if(db.Query(
					"SELECT `id` FROM `geo_zip` WHERE `zip`=\"" + zip + "\" and `geo_locality_id` IN ("
						"SELECT `id` FROM `geo_locality` WHERE `geo_region_id` IN ("
							"SELECT `id` FROM `geo_region` WHERE `geo_country_id` IN ("
								"SELECT `id` FROM `geo_country` WHERE `title`=\"" + country + "\""
							")"
						")"
					");"
					))
				{
					ostResult << "{\"result\":\"success\","
							  << "\"geo_zip\":[" << GetZipInJSONFormat(db.Get(0, "id"), &db, &user) << "]"
							  << "}";
				}
				else
				{
					error_message = "Индекс не найден";
					MESSAGE_DEBUG("", "", "country and zip not found");
				}	
					
			}
			else
			{
				error_message = "Некорректный параметры";
				MESSAGE_DEBUG("", "", "fail to get country name or zip code");
			}

			if(error_message.empty())
			{
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}
 
		if(action == "AJAX_getRegionAutocompleteList")
		{
			string			country = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("country"));
			string			region = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("region"));
			string			template_name = "json_response.htmlt";
			string			error_message = "";
			ostringstream	ostResult;

			ostResult.str("");

			if(region.length())
			{
				int	affected = db.Query(
					"SELECT `id`, `title` FROM `geo_region` WHERE `title` LIKE \"%" + region + "%\" AND `geo_country_id` IN ("
						"SELECT `id` FROM `geo_country` WHERE `title` LIKE \"%" + country + "%\""
					") LIMIT 0, 20;"
					);
				if(affected)
				{
					ostResult << "{\"result\":\"success\","
							  << "\"autocomplete_list\":[";
					for(int i = 0; i < affected; ++i)
					{
						if(i) ostResult << ",";
						ostResult << "{\"id\":\"" << db.Get(i, "id") << "\","
								  << "\"label\":\"" << db.Get(i, "title") << "\"}";
					}
					ostResult << "]}";
				}
				else
				{
					error_message = "Регион не найден";
					MESSAGE_DEBUG("", "", "country and region pair not found");
				}	
					
			}
			else
			{
				error_message = "Некорректный параметры";
				MESSAGE_DEBUG("", "", "fail to get country name or region code");
			}

			if(error_message.empty())
			{
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_getLocalityAutocompleteList")
		{
			string			country = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("country"));
			string			region = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("region"));
			string			locality = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("locality"));
			string			template_name = "json_response.htmlt";
			string			error_message = "";
			ostringstream	ostResult;

			ostResult.str("");

			if(locality.length())
			{
				int	affected = db.Query(
					"SELECT `id`, `title` FROM `geo_locality` WHERE `title` LIKE \"%" + locality + "%\" AND `geo_region_id` IN ("
						"SELECT `id` FROM `geo_region` WHERE `title` LIKE \"%" + region + "%\" AND `geo_country_id` IN ("
							"SELECT `id` FROM `geo_country` WHERE `title` LIKE \"%" + country + "%\""
						")"
					") LIMIT 0, 20;"
					);
				if(affected)
				{
					ostResult << "{\"result\":\"success\","
							  << "\"autocomplete_list\":[";
					for(int i = 0; i < affected; ++i)
					{
						if(i) ostResult << ",";
						ostResult << "{\"id\":\"" << db.Get(i, "id") << "\","
								  << "\"label\":\"" << db.Get(i, "title") << "\"}";
					}
					ostResult << "]}";
				}
				else
				{
					error_message = "Город не найден";
					MESSAGE_DEBUG("", "", "country, region and locality not found");
				}	
					
			}
			else
			{
				error_message = "Некорректный параметры";
				MESSAGE_DEBUG("", "", "fail to get country name or region code");
			}

			if(error_message.empty())
			{
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_submitNewGeoZip")
		{
			string			country = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("country"));
			string			region = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("region"));
			string			locality = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("locality"));
			string			zip = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("zip"));
			string			template_name = "json_response.htmlt";
			string			error_message = "";
			ostringstream	ostResult;

			ostResult.str("");

			if(country.length() && region.length() && locality.length() && zip.length())
			{
				if(db.Query("SELECT `id` FROM `geo_zip` WHERE `zip`=\"" + zip + "\";") == 0)
				{
					string	country_id = "";

					if(db.Query("SELECT `id` FROM `geo_country` WHERE `title`=\"" + country + "\";")) country_id = db.Get(0, "id");
					else
					{
						long int temp = db.InsertQuery("INSERT INTO `geo_country` (`title`) VALUES (\"" + country + "\");");
						if(temp)
						{
							country_id = to_string(temp);
							MESSAGE_ERROR("", action, "new country.id(" + country_id + ") added");
						}
						else
						{
							MESSAGE_ERROR("", action, "fail to add new country");
						}
					}

					if(country_id.length())
					{
						string	region_id = "";

						if(db.Query("SELECT `id` FROM `geo_region` WHERE `title`=\"" + region + "\";")) region_id = db.Get(0, "id");
						else
						{
							long int temp = db.InsertQuery("INSERT INTO `geo_region` (`geo_country_id`,`title`) VALUES (\"" + country_id + "\",\"" + region + "\");");

							if(temp)
							{
								region_id = to_string(temp);
								MESSAGE_ERROR("", action, "new region.id(" + region_id + ") added");
							}
							else
							{
								MESSAGE_ERROR("", action, "fail to add new region");
							}
						}

						if(region_id.length())
						{
							string	locality_id = "";

							if(db.Query("SELECT `id` FROM `geo_locality` WHERE `title`=\"" + locality + "\";")) locality_id = db.Get(0, "id");
							else
							{
								long int temp = db.InsertQuery("INSERT INTO `geo_locality` (`geo_region_id`,`title`) VALUES (\"" + region_id + "\",\"" + locality + "\");");

								if(temp)
								{
									locality_id = to_string(temp);
									MESSAGE_ERROR("", action, "new locality.id(" + locality_id + ") added");
								}
								else
								{
									MESSAGE_ERROR("", action, "fail to add new locality");
								}
							}

							if(locality_id.length())
							{
								string	zip_id = "";
								long int temp = db.InsertQuery("INSERT INTO `geo_zip` (`geo_locality_id`,`zip`) VALUES (\"" + locality_id + "\",\"" + zip + "\");");

								if(temp)
								{
									zip_id = to_string(temp);
									MESSAGE_ERROR("", action, "new zip.id(" + zip_id + ") added");
								}
								else
								{
									MESSAGE_ERROR("", action, "fail to add new zip");
								}

								if(zip_id.length())
								{
									ostResult << "{\"result\":\"success\","
											  << "\"geo_zip\":[" << GetZipInJSONFormat(zip_id, &db, &user) << "]"
											  << "}";
								}
								else
								{
									error_message = "Не удалось добавить индекс";
									MESSAGE_ERROR("", action, "fail to add new zip");
								}
							}
							else
							{
								error_message = "Не удалось добавить город";
								MESSAGE_ERROR("", action, "fail to add new locality");
							}
						}
						else
						{
							error_message = "Не удалось добавить регион";
							MESSAGE_ERROR("", action, "fail to add new region");
						}

					}
					else
					{
						error_message = "Не удалось добавить страну";
						MESSAGE_ERROR("", action, "fail to add new country");
					}

				}
				else
				{
					error_message = "Индекс уже существует в БД";
					MESSAGE_DEBUG("", "", "zip(" + zip + ") already exists in DB");
				}	
					
			}
			else
			{
				error_message = "Некорректный параметры";
				MESSAGE_DEBUG("", "", "mandatory parameter missed");
			}

			if(error_message.empty())
			{
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
		}

		if(action == "AJAX_getRussianLocalityByZip")
		{
			MESSAGE_DEBUG("", action, "start");

			auto			error_message = ""s;
			auto			zip = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("zip"));
			auto			success_message = ""s;


			if(zip.length() == 6)
			{
				if(db.Query("SELECT `id` FROM `geo_zip` WHERE `zip`=" + quoted(zip) + ";"))
				{
					success_message = "\"zip\":" + GetZipInJSONFormat(db.Get(0, 0), &db, &user);
				}
				else
				{
					error_message = gettext("not found");
					MESSAGE_DEBUG("", "", error_message);
				}
			}
			else
			{
				error_message = gettext("mandatory parameter missed");
				MESSAGE_DEBUG("", "", error_message);
			}

			AJAX_ResponseTemplate(&indexPage, success_message, error_message);

			MESSAGE_DEBUG("", action, "finish");
		}


		if(action == "AJAX_addNewCompany")
		{
			auto			id = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto			legal_geo_zip		= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("geo_zip_legal"));
			auto			legal_address		= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("legal_address"));
			auto			mailing_geo_zip		= CheckHTTPParam_Number	(indexPage.GetVarsHandler()->Get("geo_zip_mailing"));
			auto			mailing_address		= CheckHTTPParam_Text	(indexPage.GetVarsHandler()->Get("mailing_address"));
			auto			template_name = "json_response.htmlt"s;
			auto			error_message = ""s;
			auto			company_obj = ""s;
			C_Company		company(&db, &user);

			ostringstream	ostResult;

			ostResult.str("");

			if(mailing_geo_zip.empty()) mailing_geo_zip = legal_geo_zip;
			if(mailing_address.empty()) mailing_address = legal_address;

			company.SetName				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_title")));
			company.SetType				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("type")));
			company.SetLegalGeoZip		(legal_geo_zip);
			company.SetLegal_address	(legal_address);
			company.SetMailingGeoZip	(mailing_geo_zip);
			company.SetMailing_address	(mailing_address);
			company.SetBIK				(CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("bank_bik")));
			company.SetAccount			(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_account")));
			company.SetTIN				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_tin")));
			company.SetVAT				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_vat")));
			company.SetOGRN				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_ogrn")));
			company.SetKPP				(CheckHTTPParam_Text  (indexPage.GetVarsHandler()->Get("company_kpp")));

			if((error_message = company.CheckValidity()).empty())
			{
				if((error_message = company.InsertToDB()).empty())
				{
					if(company.GetType() == "agency")
					{
						auto	company_employee_id = db.InsertQuery(
																"INSERT INTO `company_employees` (`user_id`, `company_id`, `allowed_change_agency_data`,`allowed_change_sow`,`eventTimestamp`) "
																"VALUES "
																" (\"" + user.GetID() + "\", \"" + company.GetID() + "\", \"Y\", \"Y\", UNIX_TIMESTAMP())"
															);
						if(company_employee_id == 0)
						{
							MESSAGE_ERROR("", action, "fail to insert to company_employee table");
						}
					}
				}
				else
				{
					MESSAGE_ERROR("", "", "company InsertToDB failed");
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "company CheckValidity failed");
			}

			if(error_message.empty())
			{
				ostResult << "{\"result\":\"success\",\"company_id\":\"" + company.GetID() + "\"}";
			}
			else
			{
				MESSAGE_DEBUG("", action, "failed");
				ostResult.str("");
				ostResult << "{\"result\":\"error\",\"description\":\"" + error_message + "\"}";
			}

			indexPage.RegisterVariableForce("result", ostResult.str());

			if(!indexPage.SetTemplate(template_name)) MESSAGE_ERROR("", action, "can't find template " + template_name);
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
