#pragma once

#include <string>
#include <sstream>
#include <ctime>  
#include <chrono>

namespace Square {

	enum Severity {
		DEBUG = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
		FATAL = 4,
		NUM_SEVERITIES
	};

	class LogMessage : public std::ostringstream
	{
	public:
		LogMessage(Severity severity, const char* fileName, const char* funcName, int line);
		~LogMessage();

	protected:
		static const char* LOG_FILE;

		void Print();

	private:
		Severity mSeverity;
		std::string mTimestamp;
		const char* mFileName;
		const char* mFuncName;
		int mLine;
	};

#define LOG_DEBUG if(_DEBUG) Square::LogMessage(Square::DEBUG, __FILE__, __FUNCTION__, __LINE__).flush()
#define LOG_INFO Square::LogMessage(Square::INFO, __FILE__, __FUNCTION__, __LINE__).flush()
#define LOG_WARNING Square::LogMessage(Square::WARNING, __FILE__, __FUNCTION__, __LINE__).flush()
#define LOG_ERROR Square::LogMessage(Square::ERROR, __FILE__, __FUNCTION__, __LINE__).flush()
#define LOG_FATAL Square::LogMessage(Square::FATAL, __FILE__, __FUNCTION__, __LINE__).flush()
#define LOG(severity) LOG_##severity

}