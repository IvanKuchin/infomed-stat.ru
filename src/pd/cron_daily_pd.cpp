#include "cron_daily_pd.h"

bool CleanupRemovedSessions(CMysql *db)
{
	bool		result = true;

	MESSAGE_DEBUG("", "", "start");

	db->Query("DELETE FROM `sessions` WHERE `remove_flag_timestamp`< (UNIX_TIMESTAMP() - 3600*24*365) AND `remove_flag`=\"Y\";");
	
	MESSAGE_DEBUG("", "", "finish");

	return result;
}

bool RemoveOldCaptcha()
{
	DIR 		*dir;
	struct 		dirent *ent;
	bool		result = true;
	string		dirName = IMAGE_CAPTCHA_DIRECTORY;
	time_t		now;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	time(&now);

	if ((dir = opendir( dirName.c_str() )) != NULL)
	{
		/* print all the files and directories within directory */
		while ((ent = readdir (dir)) != NULL)
		{
			struct	stat	sb;
			string	fileName = dirName + ent->d_name;

			if(stat(fileName.c_str(), &sb) == 0)
			{
				double		secondsBetween = difftime(now, sb.st_mtime);


				if(secondsBetween > 2600 * 24)
				{
					{
						CLog	log;
						log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: remove file [" + ent->d_name + "] created " + to_string(secondsBetween) + " secs ago");
					}
					unlink(fileName.c_str());
				}
			}
			else
			{
				{
					CLog	log;
					log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) + "]:ERROR: file stat [" + ent->d_name + "]");
				}
			}
		}
		closedir (dir);
	}
	else
	{
		/* could not open directory */
		result = false;
	}



	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish");
	}

	return result;
}

bool RemoveTempMedia(CMysql *db)
{
	bool	result = true;
	int		affected;

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: start");
	}

	affected = db->Query("SELECT * FROM `temp_media` WHERE `mediaType`=\"image\" AND `eventTimestamp`<DATE_SUB(CURDATE(), INTERVAL 2 DAY);");
	for(int i = 0; i < affected; ++i)
	{
		string	filename = IMAGE_TEMP_DIRECTORY + "/" + db->Get(i, "folder") + "/" + db->Get(i, "filename");

		if(isFileExists(filename))
		{
			{
				CLog	log;
				log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) +  "]: deleting file [filename=" + filename + "]");
			}
			unlink(filename.c_str());
		}
		else
		{
			CLog	log;
			log.Write(ERROR, string(__func__) + "[" + to_string(__LINE__) +  "]:ERROR: file doesn't exists  [filename=" + filename + "]");
		}
	}

	db->Query("DELETE FROM `temp_media` WHERE `mediaType`=\"image\" AND `eventTimestamp`<DATE_SUB(CURDATE(), INTERVAL 2 DAY);");

	{
		CLog	log;
		log.Write(DEBUG, string(__func__) + "[" + to_string(__LINE__) + "]: finish (result = " + (result ? "true" : "false") + ")");
	}

	return result;
}

int main()
{
	CStatistics		appStat;  // --- CStatistics must be first statement to measure end2end param's
	c_config		config(CONFIG_DIR);
	CUser			user;
	CMysql			db;
	struct timeval	tv;

	{
		CLog	log;
		log.Write(DEBUG, __func__ + string("[") + to_string(__LINE__) + "]: " + __FILE__);
	}

	signal(SIGSEGV, crash_handler);

	gettimeofday(&tv, NULL);
	srand(tv.tv_sec * tv.tv_usec * 100000);

	try
	{

		if(db.Connect(&config) < 0)
		{
			CLog	log;

			log.Write(ERROR, string(__func__) + string("[") + to_string(__LINE__) + "]:ERROR: Can not connect to mysql database");
			throw CExceptionHTML("MySql connection");
		}

		//--- start of daily cron main functionality
		CleanupRemovedSessions(&db);

		//--- Remove temporarily media files
		RemoveTempMedia(&db);

		//--- Remove old captcha
		RemoveOldCaptcha();

		//--- end of daily cron main functionality
	}
	catch(CExceptionHTML &c)
	{
		CLog	log;

		c.SetLanguage("ru");
		c.SetDB(&db);

		log.Write(DEBUG, string(__func__) + ": catch CExceptionHTML: DEBUG exception reason: [", c.GetReason(), "]");

		return(0);
	}
	catch(CException &c)
	{
		CLog 	log;

		log.Write(ERROR, string(__func__) + ": catch CException: exception: ERROR  ", c.GetReason());

		return(-1);
	}
	catch(exception& e)
	{
		CLog 	log;
		log.Write(ERROR, string(__func__) + ": catch(exception& e): catch standard exception: ERROR  ", e.what());

		return(-1);
	}

	return(0);
}
