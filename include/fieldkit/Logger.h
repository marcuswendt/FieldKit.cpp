/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#pragma once

#include <ostream>

namespace fieldkit {

	// TODO turn this into a more fully featured logging class

	////! A simple Logging Utility
	//class Logger {
	//public:
	//	static Logger &getInstance();

	//	operator <<()
	//protected:
	//	Logger() {};
	//	~Logger() {};
	//	static Logger &instance;
	//};

	//Logger &logger() { return Logger::getInstance(); }

	std::ostream& logger();
}