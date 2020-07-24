#include "cstatistics.h"

CStatistics::CStatistics(): logFilename (LOG_FILE_NAME)
{
	startClock = clock();
	startTime = time(NULL);
	startChrono = std::chrono::duration_cast<std::chrono::microseconds> (system_clock::now().time_since_epoch());
}

CStatistics::~CStatistics()
{
	endClock = clock();
	endTime = time(NULL);
	endChrono = std::chrono::duration_cast<std::chrono::microseconds> (system_clock::now().time_since_epoch());

	{
		CLog			log(logFilename);
		ostringstream	ost;

		ost.str("");
		ost	<< "CStatistics::~CStatistics: time duration (in ticks) = " << (float)(endClock - startClock) /  CLOCKS_PER_SEC;
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost	<< "CStatistics::~CStatistics: sec duration = " << (endTime - startTime);
		log.Write(DEBUG, ost.str());

		ost.str("");
		ost	<< "CStatistics::~CStatistics: chrono duration = " << to_string(endChrono.count() - startChrono.count()) << " microseconds";
		log.Write(DEBUG, ost.str());
	}
}