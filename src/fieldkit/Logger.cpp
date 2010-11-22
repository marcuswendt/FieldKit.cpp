/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/Logger.h"

using namespace fieldkit;

Logger::Logger() {
	currContext = "";
	currLevel = LOGLEVEL_INFO;
	addLogOutput(new ConsoleLogger());
}

Logger::~Logger() {
	for(unsigned int i = 0; i < outputs.size(); i++) {
		delete outputs[i];
		outputs[i] = NULL;
	}
}

Logger &Logger::getInstance() {
	static Logger instance;
	return instance;
}

void Logger::addLogOutput(LogOutput *logOutput) {
	outputs.push_back(logOutput);
}

std::string Logger::getLevelName(LogLevel level) {
	switch(level) {
		case LOGLEVEL_INFO: return "INFO";
		case LOGLEVEL_WARN: return "WARN";
		case LOGLEVEL_ERROR: return "ERROR";
		default: return "";
	}
}

void Logger::flush() {
	for(unsigned int i = 0; i < outputs.size(); i++) {
		if(outputs[i])
			outputs[i]->output(logstream.str());
	}
	logstream.str(std::string());
}

std::stringstream& Logger::getStream() {
	return logstream;
	// could use the below instead
	//	return ci::app::console();
}


// -- Console Logger --------------------------------------------------

// replace this to remove cinder dependency
#include "cinder/app/App.h"

void ConsoleLogger::output( std::string out )
{
	//std::cout << out << std::endl; 
	ci::app::console() << out << std::endl;
}


// -- File Logger ----------------------------------------------------

void FileLogger::output( std::string out )
{
	log.open(path.c_str(), std::ofstream::app);
	log << out << std::endl;
	log.close();
}
