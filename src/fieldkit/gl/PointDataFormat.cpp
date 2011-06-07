/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/06/2010.
 */

#include "fieldkit/gl/PointDataFormat.h"
#include <boost/lexical_cast.hpp>

using std::string;
using namespace fieldkit::gl;

void PointDataFormat::clear()
{
	attributes.clear();
}

void PointDataFormat::addAttribute(string name, int size, int type)
{
	PointDataFormat::Attribute a;

#ifndef CINDER_COCOA_TOUCH
	int typeBytes = 0;
	if(type == GL_INT) {
		typeBytes = sizeof(GLint);
		
	} else if(type == GL_FLOAT) {
		typeBytes = sizeof(GLfloat);
		
	} else {
		throw string("WARN: Invalid type ") + boost::lexical_cast<string>(type);
	}
#else 
    type = GL_FLOAT;
	int typeBytes = sizeof(GLfloat);
#endif
    
	a.name = name;
	a.type = type;
	a.size = size;
	a.bytes = size * typeBytes;
	
	attributes.push_back(a);
}