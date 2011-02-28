/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 11/01/2011.
 */

#pragma once

#include <map>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include "cinder/DataSource.h"
#include "fieldkit/math/Vector.h"

namespace fieldkit {
	    
using std::map;
using std::string;

//! A simple configuration dictionary
class ConfigDict {
public:
	void loadXML(ci::DataSourceRef source);
    //! override with cinder-style args
	void overrideWith(const std::vector<std::string>& args);
    //! override with c style args
    void overrideWith(int argc, const char* argv[]);
	void print();
	
	string gets(const string key, string alt="");
	bool getb(const string key, bool alt=false);
	int geti(const string key, int alt=0);
	float getf(const string key, float alt=0.0f);
	double getd(const string key, double alt=0.0);
	
	Vec2f get2f(const string key, Vec2f alt=Vec2f::zero());
	Vec3f get3f(const string key, Vec3f alt=Vec3f::zero());
	Vec2i get2i(const string key, Vec2i alt=Vec2i::zero());
	
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
		} catch(boost::bad_lexical_cast &e) {
			return defaultValue;
		}
	}
};

} // namespace fieldkit