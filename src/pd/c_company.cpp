#include "c_company.h"

C_Company::C_Company() {}

C_Company::C_Company(CMysql *param1, CUser *param2) : db(param1), user(param2) {}

auto	C_Company::CheckValidity() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(db)
	{
		if(user)
		{
			if(GetName					().empty()) { error_message = gettext("name is empty"); }
			if(GetType					().empty()) { error_message = gettext("type is empty"); }
			if(GetLegalGeoZip			().empty()) { error_message = gettext("legal_geo_zip_id is empty"); }
			if(GetMailingGeoZip			().empty()) { error_message = gettext("mailing_geo_zip_id is empty"); }
			if(GetLegal_address			().empty()) { error_message = gettext("legal_address is empty"); }
			if(GetTIN					().empty()) { error_message = gettext("tin is empty"); }
			if(GetBIK					().empty()) { error_message = gettext("bank_id is empty"); }
			if(GetAccount				().empty()) { error_message = gettext("account is empty"); }
			if(GetKPP					().empty()) { error_message = gettext("kpp is empty"); }
			if(GetOGRN					().empty()) { error_message = gettext("ogrn is empty"); }

			if(error_message.empty())
			{
				if(db->Query("SELECT * FROM `company` WHERE `tin`=\"" + GetTIN() + "\";"))
				{
					auto	id_from_db		= db->Get(0, "id");
					auto	kpp_from_db		= db->Get(0, "kpp");
					auto	ogrn_from_db	= db->Get(0, "ogrn");
					auto	bank_id_from_db	= db->Get(0, "bank_id");
					auto	account_from_db	= db->Get(0, "account");

					if(db->Query("SELECT `bik` FROM `banks` WHERE `id`=\"" + bank_id_from_db + "\";"))
					{
						auto	bik_from_db = db->Get(0, "bik");

						if(GetKPP() != kpp_from_db)
						{
							error_message = gettext("tin already exists") + ", "s + gettext("but KPP is different");
							MESSAGE_ERROR("", "", "banks.tin(" + GetTIN() + ") already exists but KPP is different(" + GetKPP() + " != " + kpp_from_db + ")");
						}
						else if(GetOGRN() != ogrn_from_db)
						{
							error_message = gettext("tin already exists") + ", "s + gettext("but OGRN is different");
							MESSAGE_ERROR("", "", "banks.tin(" + GetTIN() + ") already exists but OGRN is different(" + GetOGRN() + " != " + ogrn_from_db + ")");
						}
						else if(GetBIK() != bik_from_db)
						{
							error_message = gettext("tin already exists") + ", "s + gettext("but BIK is different");
							MESSAGE_ERROR("", "", "banks.tin(" + GetTIN() + ") already exists but BIK is different(" + GetBIK() + " != " + bik_from_db + ")");
						}
						else if(GetAccount() != account_from_db)
						{
							error_message = gettext("tin already exists") + ", "s + gettext("but account is different");
							MESSAGE_ERROR("", "", "banks.tin(" + GetTIN() + ") already exists but account is different(" + GetAccount() + " != " + account_from_db + ")");
						}
						else
						{
							SetID(id_from_db);
						}
					}
					else
					{
						error_message = gettext("bik issue");
						MESSAGE_ERROR("", "", "can't find BIK by banks.id(" + bank_id_from_db + ")");
					}
				}
				else
				{
					MESSAGE_DEBUG("", "", "company with TIN(" + GetTIN() + ") doesn't exists in DB");
				}
			}

		}
		else
		{
			error_message = gettext("user is not initialized");
			MESSAGE_ERROR("", "", "user is not initialized");
		}
	}
	else
	{
		error_message = gettext("db is not initialized");
		MESSAGE_ERROR("", "", "db is not initialized");
	}

	MESSAGE_DEBUG("", "", "finish(error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto	C_Company::InsertToDB() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(GetID().empty())
	{
		if(db->Query("SELECT `id` FROM `banks` WHERE `bik`=\"" + GetBIK() + "\";"))
		{
			SetBankID(db->Get(0, "id"));

			if(db->Query("SELECT `id` FROM `geo_zip` WHERE `zip`=\"" + GetLegalGeoZip() + "\";"))
			{
				SetLegalGeoZipID(db->Get(0, "id"));

				if(db->Query("SELECT `id` FROM `geo_zip` WHERE `zip`=\"" + GetMailingGeoZip() + "\";"))
				{
					SetMailingGeoZipID(db->Get(0, "id"));

					if(db)
					{
						auto	insert_query = "INSERT INTO `company` ("s + 
													"`type`,"
													"`name`,"
													"`legal_geo_zip_id`,"
													"`legal_address`,"
													"`mailing_geo_zip_id`,"
													"`mailing_address`,"
													"`tin`,"
													"`vat`,"
													"`bank_id`,"
													"`account`,"
													"`kpp`,"
													"`ogrn`,"
													"`admin_userID`,"
													"`webSite`,"
													"`description`,"
													"`logo_folder`,"
													"`logo_filename`,"
													"`eventTimestamp`"
												") VALUES (" +
													quoted(GetType()) + "," +
													quoted(GetName()) + "," +
													quoted(GetLegalGeoZipID()) + "," +
													quoted(GetLegal_address()) + "," +
													quoted(GetMailingGeoZipID()) + "," +
													quoted(GetMailing_address()) + "," +
													quoted(GetTIN()) + "," +
													quoted(GetVAT()) + "," +
													quoted(GetBankID()) + "," +
													quoted(GetAccount()) + "," +
													quoted(GetKPP()) + "," +
													quoted(GetOGRN()) + "," +
													quoted(user->GetID()) + "," +
													quoted(GetWebSite()) + "," +
													quoted(GetDescription()) + "," +
													quoted(GetLogo_folder()) + "," +
													quoted(GetLogo_filename()) + "," +
													"UNIX_TIMESTAMP()"
												")";
						auto	temp_id = db->InsertQuery(insert_query);

						if(temp_id)
						{
							SetID(to_string(temp_id));
						}
						else
						{
							MESSAGE_ERROR("", "", "fail to insert to db");
							error_message = gettext("fail to insert to db");
						}
					}
					else
					{
						error_message = gettext("db is not initialized");
						MESSAGE_ERROR("", "", "db is not initialized");
					}
				}
				else
				{
					error_message = gettext("zip not found");
					MESSAGE_ERROR("", "", "geo_zip.id not found by zip(" + GetMailingGeoZipID() + ")");
				}
			}
			else
			{
				error_message = gettext("zip not found");
				MESSAGE_ERROR("", "", "geo_zip.id not found by zip(" + GetLegalGeoZipID() + ")");
			}
		}
		else
		{
			error_message = gettext("bik not found");
			MESSAGE_ERROR("", "", "bank.id not found by bik(" + GetBIK() + ")");
		}
	}
	else
	{
		// --- company id already preloaded
	}

	MESSAGE_DEBUG("", "", "finish(error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}



ostream& operator<<(ostream& os, const C_Company &var)
{
	os << "object C_Company [empty for now]";

	return os;
}

