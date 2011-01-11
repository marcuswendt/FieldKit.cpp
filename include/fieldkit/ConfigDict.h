/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 11/01/2011.
 */

#pragma once

#include "cinder/DataSource.h"

#include <map>
using std::map;

#include <string>
using std::string;

#include <boost/lexical_cast.hpp>


namespace fieldkit {
	
//! A simple configuration dictionary - loads XML
class ConfigDict {
public:
	void loadXML(ci::DataSourceRef source);
	void print();
	
	string gets(const string key, string alt="") { return get<string>(key, alt); };
	bool getb(const string key, bool alt=false) { return get<bool>(key, alt); };
	int geti(const string key, int alt=0) { return get<int>(key, alt); };
	float getf(const string key, float alt=0.0f) { return get<float>(key, alt); };	
	double getd(const string key, double alt=0.0) { return get<double>(key, alt); };	
	
private:
	map<string, string> settings;
	
	
	template <typename T>
	T get(const string key, const T defaultValue)
	{
		map<string, string>::iterator it;
		it = settings.find(key);
		if(it != settings.end()) {
			return boost::lexical_cast<T>( (*it).second );
		} else {
			return defaultValue;
		}
	};
};

} // namespace fieldkit