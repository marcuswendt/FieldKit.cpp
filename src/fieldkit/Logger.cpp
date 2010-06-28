/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "fieldkit/Logger.h"

#include "cinder/app/App.h"

//Logger & fieldkit::Logger::getInstance()
//{
//	if(instance == NULL) 
//		instance = new Logger();
//
//	return instance;
//}

std::ostream& fieldkit::logger()
{
	return ci::app::console();
}
