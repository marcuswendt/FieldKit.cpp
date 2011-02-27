/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 *  
 * 
 *   Usage:
 *		// actual logging
 *  	LOG(ERROR, "This is an error");
 *		LOG(WARN, "This is a warning");
 * 		LOG(INFO, "This is just some info");
 * 
 *		// setting up writing to file
 *      Logger::getInstance()->addLogOutput(new FileLogger("/path/to/file.log"));
 */

#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>


#define LOG_OUTPUT(LEVEL, MSG) \
	if(fieldkit::Logger::Instance()->getLogLevel() <= LEVEL) { \
		fieldkit::Logger::Instance()->getStream() << fieldkit::Logger::Instance()->getLevelName(LEVEL) << ": " << MSG; \
		fieldkit::Logger::Instance()->flush(); \
	}

#define LOG_INFO(MSG) LOG_OUTPUT(fieldkit::LOGLEVEL_INFO, MSG)
#define LOG_WARN(MSG) LOG_OUTPUT(fieldkit::LOGLEVEL_WARN, MSG)
#define LOG_ERROR(MSG) LOG_OUTPUT(fieldkit::LOGLEVEL_ERROR, MSG)

namespace fieldkit {

	enum LogLevel {
		LOGLEVEL_INFO = 1,    // this is the default log level
		LOGLEVEL_WARN,
		LOGLEVEL_ERROR
	};
	
	/** 
	 * Implement this class for custom logging
	 */
	class LogOutput {
	public:
		virtual void output(std::string out) = 0;
	};
	
	class Logger {
	public:
		//! singleton accessor
		static Logger* Instance();

		std::stringstream& getStream();
		void flush();

		//! Add as many outputs as you like here
		void addLogOutput(LogOutput *logOutput);
		
		//! Set your log level here.
		void setLogLevel(LogLevel logLevel) { currLevel = logLevel; }
		LogLevel getLogLevel() { return currLevel; }
		std::string getLevelName(LogLevel logLevel);

	private:
		static Logger* instance;
		std::vector<LogOutput*> outputs;
		std::stringstream logstream;
		
		Logger();
		~Logger();
		
		LogLevel currLevel;
		std::string currContext;
	};
	
	
	/**
	 * Standard out console logger.
	 */
	class ConsoleLogger: public LogOutput {
	public:
		void output(std::string out);
	};

	
	/**
	 * File logger
	 */	
	class FileLogger: public LogOutput {
	public:
		FileLogger(std::string _path) {
			path = _path;
		}

		void output(std::string out);
		
	protected:
		std::ofstream log;
		std::string path;
	};
	
};