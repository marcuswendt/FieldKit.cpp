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

#define LOG_INFO(MSG) \
	if(Logger::getInstance().getLogLevel() <= LOGLEVEL_INFO) { \
		Logger::getInstance().getStream() << Logger::getInstance().getLevelName(LOGLEVEL_INFO) << ": " << MSG; \
		Logger::getInstance().flush(); \
	}

#define LOG_WARN(MSG) \
	if(Logger::getInstance().getLogLevel() <= LOGLEVEL_WARN) { \
		Logger::getInstance().getStream() << Logger::getInstance().getLevelName(LOGLEVEL_WARN) << ": " << MSG; \
		Logger::getInstance().flush(); \
	}

#define LOG_ERROR(MSG) \
	if(Logger::getInstance().getLogLevel() <= LOGLEVEL_ERROR) { \
		Logger::getInstance().getStream() << Logger::getInstance().getLevelName(LOGLEVEL_ERROR) << ": " << MSG; \
		Logger::getInstance().flush(); \
	}
						
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
		
		std::stringstream& getStream();
		void flush();
		
		
		/**
		 * Add as many outputs as you like here
		 */
		void addLogOutput(LogOutput *logOutput);
		
		
		// singleton
		static Logger &getInstance();

		/**
		 * Set your log level here.
		 */
		void setLogLevel(LogLevel logLevel) { currLevel = logLevel; }
		
		
		LogLevel getLogLevel() { return currLevel; }
		std::string getLevelName(LogLevel logLevel);

		
	private:
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
		virtual void output(std::string out) { std::cout << out << std::endl; };
	};
	
	
	
	
	
	
	
	class FileLogger: public LogOutput {
	public:
		FileLogger(std::string _path) {
			path = _path;
		}

		virtual void output(std::string out) { 
			log.open(path.c_str(), std::ofstream::app);
			log << out << std::endl;
			log.close();
		}
		
	protected:
		std::ofstream log;
		std::string path;
	};
	
};