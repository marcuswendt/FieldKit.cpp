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

#include <boost/algorithm/string.hpp>

using namespace fieldkit;
using namespace cinder;

void ConfigDict::loadXML(DataSourceRef source)
{
	// LOG_INFO("Config::loadXML "<< source->getFilePathHint());
	
	XmlTree doc = XmlTree(source);
	XmlTree root = *doc.begin();
	
	for(XmlTree::ConstIter setting = root.begin(); setting != root.end(); ++setting) 
	{
		string key = setting->getTag();
		string value = setting->getValue();
		settings.insert(std::pair<string,string>(key, value) );
	}
}

void ConfigDict::overrideWith(const std::vector<std::string>& args)
{
	for(unsigned int i=1; i<args.size(); i+=1) 
	{
		// check if argument is a key=value pair
		std::string arg = args[i];
		std::vector<std::string> strs;
		boost::split(strs, arg, boost::is_any_of("="));
		if(strs.size() == 2) {
			string key = strs[0];
			// remove leading - if exists
			if(key.substr(0,1) == "-") key = key.substr(1);
			settings[key] = strs[1];
		}
	}
}

void ConfigDict::print()
{
	LOG_INFO("Settings:");
	
	map<string, string>::iterator it;
	for(it=settings.begin() ; it != settings.end(); it++ )
		LOG_INFO("* " << (*it).first << " = " << (*it).second);
}
