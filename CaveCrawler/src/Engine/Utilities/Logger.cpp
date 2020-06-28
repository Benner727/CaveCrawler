#include "Logger.h"

namespace Square {

	const char* SeverityNames[NUM_SEVERITIES] = {
		"DEBUG", "INFO", "WARNING", "ERROR", "FATAL"
	};

	const char* LogMessage::LOG_FILE = "debug.log";

	LogMessage::LogMessage(Severity severtity, const char* fileName, const char* funcName, int line)
		: mSeverity(severtity), mFileName(fileName), mFuncName(funcName), mLine(line)
	{
		time_t t;
		struct tm* tmp;
		char MY_TIME[50];
		time(&t);

		tmp = localtime(&t);

		strftime(MY_TIME, sizeof(MY_TIME), "%x - %H:%M:%S", tmp);

		mTimestamp = std::string(MY_TIME);

		//std::remove(LOG_FILE);
	}

	LogMessage::~LogMessage()
	{
		Print();
	}

	void LogMessage::Print()
	{
		freopen(LOG_FILE, "a", stderr);
		fprintf(stderr, "%s [%s] %s : %s (%d) - '%s'\n", mTimestamp.c_str(), SeverityNames[mSeverity], mFileName, mFuncName, mLine, str().c_str());
		fclose(stderr);
	}

}