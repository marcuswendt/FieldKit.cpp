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
#include <boost/algorithm/string.hpp>
#include "cinder/Vector.h"

namespace fieldkit {

using ci::Vec2f;
using ci::Vec3f;
	
//! A simple configuration dictionary - loads XML
class ConfigDict {
public:
	void loadXML(ci::DataSourceRef source);
	void overrideWith(const std::vector<std::string>& args);
	void print();
	
	string gets(const string key, string alt="") { return get<string>(key, alt); };
	bool getb(const string key, bool alt=false) { return get<bool>(key, alt); };
	int geti(const string key, int alt=0) { return get<int>(key, alt); };
	float getf(const string key, float alt=0.0f) { return get<float>(key, alt); };	
	double getd(const string key, double alt=0.0) { return get<double>(key, alt); };	
	
	Vec2f get2f(const string key, Vec2f alt=Vec2f::zero());
	Vec3f get3f(const string key, Vec3f alt=Vec3f::zero());
	
private:
	map<string, string> settings;
	
	
	template <typename T>
	T get(const string key, const T defaultValue)
	{
		map<string, string>::iterator it;
		it = settings.find(key);
		if(it != settings.end()) {
			return fromString<T>((*it).second, defaultValue);
		} else {
			return defaultValue;
		}
	};
	
	template <typename T>
	T fromString(const string value, const T defaultValue) 
	{
		try {
			return boost::lexical_cast<T>(value);
		} catch(boost::bad_lexical_cast &) {
			return defaultValue;
		}
	}
};

} // namespace fieldkit