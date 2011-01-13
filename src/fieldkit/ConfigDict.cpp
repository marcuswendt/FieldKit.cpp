/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 11/01/2011.
 */

#include "fieldkit/ConfigDict.h"

#include "fieldkit/FieldKit.h"
#include "cinder/Xml.h"

using namespace fieldkit;
using namespace cinder;

void ConfigDict::loadXML(DataSourceRef source)
{
	// LOG_INFO("Config::loadXML "<< source->getFilePathHint());
	
	XmlTree doc = XmlTree(source);
	XmlTree root = *doc.begin();
	
	for(XmlTree::ConstIter setting = root.begin(); setting != root.end(); ++setting) {
		string key = setting->getTag();
		string value = setting->getValue();
		settings.insert(std::pair<string,string>(key, value) );
	}
}

void ConfigDict::print()
{
	LOG_INFO("Settings:");
	
	map<string, string>::iterator it;
	for(it=settings.begin() ; it != settings.end(); it++ )
		LOG_INFO("* " << (*it).first << " = " << (*it).second);
}
