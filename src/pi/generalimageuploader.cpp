#include "generalimageuploader.h"	 

static bool ImageSaveAsJpg(const string src, const string dst, string itemType, c_config *config)
{
	MESSAGE_DEBUG("", "", "start (" + src + ", " + dst + ")");

#ifndef IMAGEMAGICK_DISABLE
	// Construct the image object. Separating image construction from the
	// the read operation ensures that a failure to read the image file
	// doesn't render the image object useless.
	try {
		Magick::Image		   image;
		Magick::OrientationType imageOrientation;
		Magick::Geometry		imageGeometry;

		// Read a file into image object
		image.read( src );

		imageGeometry = image.size();
		imageOrientation = image.orientation();

		MESSAGE_DEBUG("", "", "imageOrientation = " + to_string(imageOrientation) + ", xRes = " + to_string(imageGeometry.width()) + ", yRes = " + to_string(imageGeometry.height()));

		if(imageOrientation == Magick::TopRightOrientation) image.flop();
		if(imageOrientation == Magick::BottomRightOrientation) image.rotate(180);
		if(imageOrientation == Magick::BottomLeftOrientation) { image.flop(); image.rotate(180); }
		if(imageOrientation == Magick::LeftTopOrientation) { image.flop(); image.rotate(-90); }
		if(imageOrientation == Magick::RightTopOrientation) image.rotate(90);
		if(imageOrientation == Magick::RightBottomOrientation) { image.flop(); image.rotate(90); }
		if(imageOrientation == Magick::LeftBottomOrientation) image.rotate(-90);

		if((imageGeometry.width() > stod_noexcept(config->GetFromFile("image_max_width", itemType))) || (imageGeometry.height() > stod_noexcept(config->GetFromFile("image_max_height", itemType))))
		{
			int   newHeight, newWidth;
			if(imageGeometry.width() >= imageGeometry.height())
			{
				newWidth = stod_noexcept(config->GetFromFile("image_max_width", itemType));
				newHeight = newWidth * imageGeometry.height() / imageGeometry.width();
			}
			else
			{
				newHeight = stod_noexcept(config->GetFromFile("image_max_height", itemType));
				newWidth = newHeight * imageGeometry.width() / imageGeometry.height();
			}

			image.resize(Magick::Geometry(newWidth, newHeight, 0, 0));
		}

		// Write the image to a file
		image.write( dst );
	}
	catch( Magick::Exception &error_ )
	{
		MESSAGE_DEBUG("", "", "exception in read/write operation [" + error_.what() + "]");
		
		return false;
	}

	MESSAGE_DEBUG("", "", "finish (image(" + src + " -> " + dst + ") has been successfully converted to .jpg format)");

	return true;
#else
	MESSAGE_DEBUG("", "", "start (" + src + " -> " + dst + ") simple file coping, because ImageMagick++ is not activated");

	CopyFile(src, dst);

	MESSAGE_DEBUG("", "", "finish (image(" + src + " -> " + dst + ")");

	return  true;
#endif
}

static bool BlindCopy(const string src, const string dst, string itemType, c_config *config)
{
	MESSAGE_DEBUG("", "", "start (" + src + " -> " + dst + ")");

	CopyFile(src, dst);

	MESSAGE_DEBUG("", "", "finish (image(" + src + " -> " + dst + ")");

	return  true;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be a first statement to measure end2end param's
	CCgi			indexPage(EXTERNAL_TEMPLATE);
	c_config		config(CONFIG_DIR);
	CUser			user;
	string			action;
	CMysql			db;
	struct timeval	tv;
	ostringstream	ostJSONResult/*(static_cast<ostringstream&&>(ostringstream() << "["))*/;

	MESSAGE_DEBUG("", action, __FILE__);

    signal(SIGSEGV, crash_handler); 

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);
	ostJSONResult.clear();
		
	try
	{
		indexPage.ParseURL();

		if(!indexPage.SetTemplate("index.htmlt"))
		{
			MESSAGE_ERROR("", "", "template file was missing");
			throw CException("Template file was missing");
		}

		if(db.Connect(&config) < 0)
		{
			MESSAGE_ERROR("", "", "Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		indexPage.SetDB(&db);

		action = indexPage.GetVarsHandler()->Get("action");
		{
			MESSAGE_DEBUG("", "", "action taken from HTTP is " + action);
		}



	// ------------ generate common parts
		{

// TODO: remove now
/*
			indexPage.RegisterVariableForce("rand", GetRandom(10));
			indexPage.RegisterVariableForce("random", GetRandom(10));
			indexPage.RegisterVariableForce("style", "style.css");
*/

#ifndef IMAGEMAGICK_DISABLE
			Magick::InitializeMagick(NULL);
#endif

			if(RegisterInitialVariables(&indexPage, &db, &user))
			{
			}
			else
			{
				MESSAGE_ERROR("", "", "RegisterInitialVariables failed, throwing exception");
				throw CExceptionHTML("environment variable error");
			}

			action = GenerateSession(action, &config, &indexPage, &db, &user);
		}
	// ------------ end generate common parts


		if((user.GetID().length()) && (user.GetName() != "Guest"))
		{
			auto	  itemID = CheckHTTPParam_Number(indexPage.GetVarsHandler()->Get("id"));
			auto	  itemType = CheckHTTPParam_Text(indexPage.GetVarsHandler()->Get("type"));

			if(itemID.length() && itemType.length())
			{
				auto	scoped_error_message = isItemAllowedToChange(itemID, itemType, &config, &db, &user);
				if(scoped_error_message.empty())
				{
					if(indexPage.GetFilesHandler()->Count() == 1)
					{
						// --- number uploaded files = 1
						for(auto filesCounter = 0; filesCounter < indexPage.GetFilesHandler()->Count(); ++filesCounter)
						{
							FILE			*f;
							auto			folderID = 0;
							auto			filePrefix = GetRandom(20);
							auto			finalFilename = ""s;
							auto			originalFilename = ""s;
							auto			preFinalFilename = ""s;
							auto			fileName = ""s;
							auto			fileExtension = ""s;
							auto			save_func = ImageSaveAsJpg;

							if(indexPage.GetFilesHandler()->GetSize(filesCounter) > stod_noexcept(config.GetFromFile("max_file_size", itemType)))
							{
								MESSAGE_ERROR("", "", "file [" + indexPage.GetFilesHandler()->GetName(filesCounter) + "] size exceed permitted maximum: " + to_string(indexPage.GetFilesHandler()->GetSize(filesCounter)) + " > " + config.GetFromFile("max_file_size", itemType));

								ostJSONResult.str("");
								ostJSONResult << "{";
								ostJSONResult << "\"result\": \"error\",";
								ostJSONResult << "\"textStatus\": \"" << gettext("file size") << " " << indexPage.GetFilesHandler()->GetSize(filesCounter) / 1024 << "K " << gettext("exceeds allowed size") << " " << config.GetFromFile("max_file_size", itemType) << "bytes\",";
								ostJSONResult << "\"fileName\": \"\" ,";
								ostJSONResult << "\"jqXHR\": \"\"";
								ostJSONResult << "}";

								break;
							}

							//--- check logo file existing
							do
							{
								auto		  	tmp					= indexPage.GetFilesHandler()->GetName(filesCounter);
								auto		  	foundPos			= tmp.rfind(".");
								auto			number_of_folders	= stod_noexcept(config.GetFromFile("number_of_folders", itemType));

								folderID	= (int)(rand()/(RAND_MAX + 1.0) * number_of_folders) + 1;
								filePrefix	= GetRandom(20);

								if(foundPos != string::npos)
								{
									fileExtension = tmp.substr(foundPos);

						            // --- filter wrong fileExtension (for ex: .com?action=fake_action) 
						            if(fileExtension.find("jpeg")     != string::npos) fileExtension = ".jpeg";
						            else if(fileExtension.find("png") != string::npos) fileExtension = ".png";
						            else if(fileExtension.find("gif") != string::npos) fileExtension = ".gif";
						            else if(fileExtension.find("svg") != string::npos) fileExtension = ".svg";
						            else if(fileExtension.find("xml") != string::npos) fileExtension = ".xml";
						            else if(fileExtension.find("txt") != string::npos) fileExtension = ".txt";
						            else fileExtension = ".jpg";
								}
								else
								{
									fileExtension = ".jpg";
								}

								originalFilename = "/tmp/tmp_" + filePrefix + fileExtension;
								preFinalFilename = "/tmp/" + filePrefix + "." + fileExtension;
								finalFilename = config.GetFromFile("image_folders", itemType) + "/" + to_string(folderID) + "/" + filePrefix + config.GetFromFile("file_extension", itemType);

							} while(isFileExists(finalFilename) || isFileExists(originalFilename) || isFileExists(preFinalFilename));

							MESSAGE_DEBUG("", "", "Save file to /tmp for checking of image validity [" + originalFilename + "]");

							// --- Save file to "/tmp/" for checking of image validity
							f = fopen(originalFilename.c_str(), "w");
							if(f == NULL)
							{
								MESSAGE_ERROR("", "", "fail to write file " + originalFilename);
								throw CExceptionHTML("file write error", indexPage.GetFilesHandler()->GetName(filesCounter));
							}
							fwrite(indexPage.GetFilesHandler()->Get(filesCounter), indexPage.GetFilesHandler()->GetSize(filesCounter), 1, f);
							fclose(f);

							// --- scoping
							{
								auto	file_type = config.GetFromFile("data_type", itemType);

								if(file_type == "image")			save_func = ImageSaveAsJpg;
								else if(file_type == "template")	save_func = BlindCopy;
							}

							if(save_func(originalFilename, preFinalFilename, itemType, &config))
							{

								MESSAGE_DEBUG("", "", "final filename is " + finalFilename + "");

								auto	affected = 0;

								{
									map<string, string> vars		= {{"itemType", itemType}, {"itemID", itemID}};
									auto				db_query	= config.GetFromFile("db_select_all_by_id", {itemType}, vars)[itemType];

									affected = db.Query(db_query);
								}

								// --- Don't forget to remove previous logo
								if(affected)
								{
									auto  currLogo = config.GetFromFile("image_folders", itemType) + (config.GetFromFile("db_field_name_photo_folder", itemType).length() ? "/"s + db.Get(0, config.GetFromFile("db_field_name_photo_folder", itemType)) : "") + "/" + db.Get(0, config.GetFromFile("db_field_name_photo_filename", itemType));

									if(isFileExists(currLogo)) 
									{
										MESSAGE_DEBUG("", "", "unlink current file " + currLogo);
										unlink(currLogo.c_str());
									}
									else
									{
										MESSAGE_DEBUG("", "", "file " + currLogo + " doesn't exists");
									}
								}

								CopyFile(preFinalFilename, finalFilename);


								{
									map<string, string> vars		= {{"itemType", itemType}, {"itemID", itemID}, {"folderName", to_string(folderID)}, {"fileName", filePrefix  + config.GetFromFile("file_extension", itemType)}};
									auto				db_query	= config.GetFromFile("db_update_logo_by_id", {itemType}, vars)[itemType];

									db.Query(db_query);

									// db.Query(GetSpecificData_UpdateQueryItemByID(itemID, itemType, to_string(folderID), filePrefix  + config.GetFromFile("file_extension", itemType)));
								}

								if(filesCounter == 0) ostJSONResult << "[";
								if(filesCounter  > 0) ostJSONResult << ",";
								ostJSONResult << "{";
								ostJSONResult << "\"result\": \"success\",";
								ostJSONResult << "\"textStatus\": \"\",";
								ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ,";
								ostJSONResult << "\"jqXHR\": \"\",";
								if(config.GetFromFile("db_field_name_photo_folder", itemType).length())
								{
									ostJSONResult << "\"" + config.GetFromFile("db_field_name_photo_folder", itemType) + "\": \"" << folderID << "\",";
									ostJSONResult << "\"" + config.GetFromFile("db_field_name_photo_filename", itemType) + "\": \"" << filePrefix << config.GetFromFile("file_extension", itemType) << "\"";
								}
								else
								{
									ostJSONResult << "\"" + config.GetFromFile("db_field_name_photo_filename", itemType) + "\": \"" << folderID << "/" << filePrefix << config.GetFromFile("file_extension", itemType) << "\"";
								}
								ostJSONResult << "}";
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";

							}
							else
							{
								MESSAGE_DEBUG("", "", "fail to save " + indexPage.GetFilesHandler()->GetName(filesCounter) + ". Probably it is not an image file.");

								if(filesCounter == 0) ostJSONResult << "[";
								if(filesCounter  > 0) ostJSONResult << ",";
								ostJSONResult << "{";
								ostJSONResult << "\"result\": \"error\",";
								ostJSONResult << "\"textStatus\": \"" << (gettext("wrong format")) << "\",";
								ostJSONResult << "\"fileName\": \"" << indexPage.GetFilesHandler()->GetName(filesCounter) << "\" ,";
								ostJSONResult << "\"jqXHR\": \"\"";
								ostJSONResult << "}";
								if(filesCounter == (indexPage.GetFilesHandler()->Count() - 1)) ostJSONResult << "]";
							}
							// --- Delete temporarily files
							unlink(originalFilename.c_str());
							unlink(preFinalFilename.c_str());

						} // --- Loop through all uploaded files

					} // --- if number uploaded files = 1
					else
					{
						{
							MESSAGE_ERROR("", "", "number uploaded images is " + to_string(indexPage.GetFilesHandler()->Count()) + ", but must be 1");
						}

						ostJSONResult.str("");
						ostJSONResult << "{";
						ostJSONResult << "\"result\": \"error\",";
						ostJSONResult << "\"textStatus\": \"" << (gettext("number uploaded images must be 1")) << "\",";
						ostJSONResult << "\"fileName\": \"\" ,";
						ostJSONResult << "\"jqXHR\": \"\"";
						ostJSONResult << "}";
					} // --- if number uploaded files = 1
				}
				else
				{
					MESSAGE_ERROR("", "", scoped_error_message);

					ostJSONResult.str("");
					ostJSONResult << "{";
					ostJSONResult << "\"result\": \"error\",";
					ostJSONResult << "\"textStatus\": " + quoted(scoped_error_message) + ",";
					ostJSONResult << "\"fileName\": \"\" ,";
					ostJSONResult << "\"jqXHR\": \"\"";
					ostJSONResult << "}";
				}

				indexPage.RegisterVariableForce("result", ostJSONResult.str());
			} // --- parameter itemID exists and not empty
			else
			{
				{
					MESSAGE_ERROR("", "", "itemID or itemType parameter missed");
				}

				ostringstream   ost;
				
				ost.str("");
				ost << "{";
				ost << "\"result\": \"error\",";
				ost << "\"textStatus\": \"" << (gettext("mandatory parameter missed")) << "\",";
				ost << "\"fileName\": \"\" ,";
				ost << "\"jqXHR\": \"\"";
				ost << "}";

				indexPage.RegisterVariableForce("result", ost.str());
			} // --- parameter itemID exists and not empty
		} // --- user not Guest
		else
		{
			{
				MESSAGE_ERROR("", "", "user not found or not logged in");
			}


			ostringstream   ost;
			
			ost.str("");
			ost << "{";
			ost << "\"result\": \"error\",";
			ost << "\"textStatus\": \"" << indexPage.GetVarsHandler()->Get("ErrorDescription") << "\",";
			ost << "\"fileName\": \"\" ,";
			ost << "\"jqXHR\": \"\"";
			ost << "}";

			indexPage.RegisterVariableForce("result", ost.str());
		} // --- user not Guest

		if(!indexPage.SetTemplate("json_response.htmlt"))
		{
			MESSAGE_ERROR("", "", "template file missed (json_response_with_braces.htmlt)");
			throw CException("Template file was missing");
		}

		indexPage.OutTemplate();

	}
	catch(CExceptionHTML &c)
	{
		c.SetLanguage(indexPage.GetLanguage());
		c.SetDB(&db);

		MESSAGE_ERROR("", "", "catch CExceptionHTML: exception reason (" + c.GetReason() + ")");

		if(c.GetReason() == "file write error")
		{
			ostringstream   ost;

			ost.str("");
			ost << "\"result\": \"error\",";
			ost << "\"textStatus\": \"" << c.GetReason() << ", file: (" << c.GetParam1() << ")\",";
			ost << "\"jqXHR\": \"\"";
			indexPage.RegisterVariableForce("result", ost.str());
		}
		if(c.GetReason() == "file size exceed")
		{
			ostringstream   ost;

			ost.str("");
			ost << "\"result\": \"error\",";
			ost << "\"textStatus\": \"" << c.GetReason() << ", file: (" << c.GetParam1() << ")\",";
			ost << "\"jqXHR\": \"\"";
			indexPage.RegisterVariableForce("result", ost.str());
		}


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

		MESSAGE_ERROR("", "", "catch CException: exception: " + c.GetReason());

		indexPage.RegisterVariable("content", c.GetReason());
		indexPage.OutTemplate();
		return(-1);
	}

	return(0);
}

