#include "utilities.h"

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

string GetSpecificData_GetFinalFileExtension(string itemType)
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
auto GetSpecificData_AllowedToChange(string itemID, string itemType, CMysql *db, CUser *user) -> string
{
	auto	  error_message = ""s;

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
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change contract_sow_custom_fields.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") must be an agency employee to change";
			MESSAGE_DEBUG("", "", error_message);
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
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change contract_psow_custom_fields.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") must be agency employee to change";
			MESSAGE_DEBUG("", "", error_message);
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
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change cost_center_custom_fields.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") must be agency employee to change";
			MESSAGE_DEBUG("", "", error_message);
		}
		
	}
	else if(itemType == "template_company")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `company_custom_fields` WHERE `id`=\"" + itemID + "\""
						");"))
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change company_custom_fields.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else if((user->GetType() == "subcontractor"))
		{
			if(db->Query("SELECT `id` FROM `company` WHERE `admin_userID`=\"" + user->GetID() + "\" AND `id`=("
							"SELECT `company_id` FROM `company_custom_fields` WHERE `id`=\"" + itemID + "\""
						");"))
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change company_custom_fields.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") not allowerd to change company custom field";
			MESSAGE_DEBUG("", "", error_message);
		}
		
	}
	else if(itemType == "template_agreement_company")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `company_agreement_files` WHERE `id`=\"" + itemID + "\""
						");"))
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change company_agreement_files.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") not allowerd to change company agreement files";
			MESSAGE_DEBUG("", "", error_message);
		}
	}
	else if(itemType == "template_agreement_sow")
	{
		if((user->GetType() == "agency"))
		{
			if(db->Query("SELECT `id` FROM `company_employees` WHERE `user_id`=\"" + user->GetID() + "\" AND `allowed_change_sow`=\"Y\" AND `company_id`=("
							"SELECT `company_id` FROM `contract_sow_agreement_files` WHERE `id`=\"" + itemID + "\""
						");"))
			{
			}
			else
			{
				error_message = "user.id(" + user->GetID() + ") doesn't allowed to change contract_sow_agreement_files.id(" + itemID + ")";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") not allowerd to change contract_sow agreement files";
			MESSAGE_DEBUG("", "", error_message);
		}
	}
	else if(itemType == "company_profile_logo")
	{
		if((user->GetType() == "subcontractor"))
		{
			if(db->Query("SELECT `id` FROM `company` WHERE `id`=\"" + itemID + "\" AND `admin_userID`=\"" + user->GetID() + "\";"))
			{
			}
			else
			{
				error_message = "subcontractor user.id(" + user->GetID() + ") doesn't allowed to change company.id(" + itemID + ") logo";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else if((user->GetType() == "agency"))
		{
			if(db->Query(	"SELECT `id` FROM `company_employees` WHERE `company_id`=("
								"SELECT `company_id` FROM `company_employees` WHERE `user_id`=("
									"SELECT `admin_userID` FROM `company` WHERE `id`=\"" + itemID + "\""
								")"
							") AND `user_id`=\"" + user->GetID() + "\" AND `allowed_change_agency_data`=\"Y\""))
			{
			}
			else
			{
				error_message = "agency user.id(" + user->GetID() + ") doesn't allowed to change company.id(" + itemID + ") logo";
				MESSAGE_DEBUG("", "", error_message);
			}
		}
		else
		{
			error_message = "user.type(" + user->GetType() + ") not allowed to change";
			MESSAGE_DEBUG("", "", error_message);
		}

	}
	else if(db->Query(GetSpecificData_SelectQueryItemByID(itemID, itemType))) // --- item itemID exists ?
	{
			if((itemType == "course") || (itemType == "university") || (itemType == "school") || (itemType == "language") || (itemType == "book") || (itemType == "company") || (itemType == "certification"))
			{
				string	  coverPhotoFolder = db->Get(0, GetSpecificData_GetDBCoverPhotoFolderString(itemType).c_str());
				string	  coverPhotoFilename = db->Get(0, GetSpecificData_GetDBCoverPhotoFilenameString(itemType).c_str());

				if(coverPhotoFolder.empty() && coverPhotoFilename.empty()) {}
				else
				{
					error_message = "logo already uploaded";

					MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, because logo already uploaded");
				}
			}
			else if(itemType == "event")
			{
				if(user)
				{
					if(db->Query("SELECT `id` FROM `event_hosts` WHERE `event_id`=\"" + itemID + "\" AND `user_id`=\"" + user->GetID() + "\";")) {}
					else
					{
						error_message = "you are not the event host";

						MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not an event host");
					}
				}
				else
				{
					error_message = "user object is NULL";

					MESSAGE_ERROR("", "", error_message);
				}
			}
			else if(itemType == "gift")
			{
				string		user_id = db->Get(0, "user_id");

				if(user)
				{
					if(user_id == user->GetID()) {}
					else
					{
						error_message = "you are not the gift owner";

						MESSAGE_DEBUG("", "", "access to " + itemType + "(" + itemID + ") denied, you are not a gift owner");
					}
				}
				else
				{
					error_message = "user object is NULL";

					MESSAGE_ERROR("", "", error_message);
				}
			}
			else
			{
				error_message = "itemType [" + itemType + "] unknown";

				MESSAGE_ERROR("", "", error_message);
			}
	}
	else
	{
		error_message = itemType + "(" + itemID + ") not found";

		MESSAGE_ERROR("", "", error_message);
	}

	MESSAGE_DEBUG("", "", "finish (error_message: " + error_message + ")");

	return error_message;
}
