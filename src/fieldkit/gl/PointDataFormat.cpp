/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   Flow
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/06/2010.
 */

#include "fieldkit/gl/PointDataFormat.h"

using namespace fieldkit::gl;

void PointDataFormat::clear()
{
	attributes.clear();
}

void PointDataFormat::addAttribute(string name, int size, int type)
{
	PointDataFormat::Attribute a;
	
	int typeBytes = 0;
	if(type == GL_INT) {
		typeBytes = sizeof(GLint);
		
	} else if(type == GL_FLOAT) {
		typeBytes = sizeof(GLfloat);
		
	} else {
		logger() << "WARN: Invalid type "<< type << std::endl;
	}
	
	a.name = name;
	a.type = type;
	a.size = size;
	a.bytes = size * typeBytes;
	
	attributes.push_back(a);
}