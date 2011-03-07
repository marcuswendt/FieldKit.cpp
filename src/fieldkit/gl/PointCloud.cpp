/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   Flow
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/06/2010.
 */

#include "fieldkit/gl/PointCloud.h"
#include "fieldkit/Logger.h"
#include <boost/foreach.hpp>

using namespace fieldkit::gl;

PointCloud::PointCloud()
{
	capacity = 0;
	bytesPerParticle = 0;
	size = 0;

	data = NULL;
	ptr = NULL;
}

PointCloud::~PointCloud()
{
	vbo.reset();
	shader.reset();

	delete data;
	data = NULL;
	ptr = NULL;
}

void PointCloud::init(PointDataFormat const format, int capacity, GlslProg const shader)
{
	this->format = format;
	this->capacity = capacity;
	this->shader = shader;
    
	// calculate buffer size
	bytesPerParticle = 0;
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {
		bytesPerParticle += attr.bytes;
	}
	
	int bytesTotal = bytesPerParticle * capacity;
	
	// create buffer
	data = (GLfloat*)malloc(bytesTotal);
	clear();

	// create gl objects
	vbo = Vbo(GL_ARRAY_BUFFER);
	vbo.bufferData(bytesTotal, data, GL_DYNAMIC_DRAW); // or GL_STREAM_DRAW
	vbo.unbind();
}

void PointCloud::clear() 
{
	size = 0;
	ptr = data;
}

void PointCloud::insert()
{
	size += 1;
}

void PointCloud::put(Vec2f const& v)
{
	put(v.x); 
	put(v.y);
}

void PointCloud::put(Vec3f const& v)
{
	put(v.x); 
	put(v.y);
	put(v.z);
}

void PointCloud::put(Vec4f const& v)
{
	put(v.x);
	put(v.y);
	put(v.z);
	put(v.w);
}

void PointCloud::put(ColorAf const& v)
{
	put(v.r);
	put(v.g);
	put(v.b);
	put(v.a);
}

void PointCloud::draw()
{
	// dont draw when no points were inserted
	if(size == 0) return;
	
	// enable states
	shader.bind();
	vbo.bind();
	
	// upload data
	vbo.bufferSubData(0, size * bytesPerParticle, data);
	
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	
	glEnableClientState(GL_VERTEX_ARRAY);

	// set attribute pointers
	int offset = 0;
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {		
		GLint loc = shader.getAttribLocation(attr.name);
		if(loc == -1) {
			LOG_WARN( "Couldnt find shader attribute '"<< attr.name);
			return;
		}
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, attr.size, attr.type, false, bytesPerParticle, (void*)offset);
		offset += attr.bytes;
	}
	
	// draw particles
	glDrawArrays(GL_POINTS, 0, size);

	// restore attribute pointers
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {		
		GLint loc = shader.getAttribLocation(attr.name);
		glDisableVertexAttribArray(loc);
	}
	
	// restore states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glDisable(GL_POINT_SPRITE);

	vbo.unbind();
	shader.unbind();
}

