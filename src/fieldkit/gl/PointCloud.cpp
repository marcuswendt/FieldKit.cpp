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
	ptr = NULL;
}

PointCloud::~PointCloud()
{
	vbo.reset();
	shader.reset();
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
    GLfloat* data = (GLfloat*)malloc(bytesTotal);

	// create gl objects
	vbo = Vbo(GL_ARRAY_BUFFER);
	vbo.bufferData(bytesTotal, data, GL_DYNAMIC_DRAW); // or GL_STREAM_DRAW
	vbo.unbind();
    
    delete data;
}


#ifndef CINDER_COCOA_TOUCH
void PointCloud::map()
{
    size = 0;
    ptr = reinterpret_cast<GLfloat*>( vbo.map(GL_WRITE_ONLY) );
}

void PointCloud::unmap()
{
    vbo.unmap();
}

void PointCloud::draw()
{
	// dont draw when no points were inserted
	if(size == 0) return;
	
	// enable states
	shader.bind();
	vbo.bind();
	
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

#else 

void PointCloud::map()
{
}

void PointCloud::unmap()
{
}

void PointCloud::draw()
{
}

#endif

