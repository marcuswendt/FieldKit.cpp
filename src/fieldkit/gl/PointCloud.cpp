/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   Flow
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/06/2010.
 */

#include "fieldkit/gl/PointCloud.h"
#include "fieldkit/gl/PointDataFormat.h"

using namespace fieldkit::gl;

void PointCloud::init(PointDataFormat* format, int capacity,
					  DataSourceRef vertexShader, DataSourceRef fragmentShader)
{
	this->format = format;
	this->capacity = capacity;
	
	// calculate buffer size
	bytesPerParticle = 0;
	BOOST_FOREACH(PointDataFormat::Attribute attr, format->attributes) {
		bytesPerParticle += attr.bytes;
	}
	
	int bytesTotal = bytesPerParticle * capacity;
	
	// create buffer
	data = (GLfloat*)malloc(bytesTotal);
	
	// create gl objects
	vbo = new Vbo(GL_ARRAY_BUFFER);
	vbo->bufferData(bytesTotal, data, GL_DYNAMIC_DRAW); // or GL_STREAM_DRAW
	vbo->unbind();

	// when vertex and fragment shaders are given, try to load them
	if(vertexShader && fragmentShader) {
		shader = new GlslProg(vertexShader, fragmentShader);

	// load default shaders
	} else {
		logger() << "PointCloud::init using default shaders" << std::endl;
		shader = new GlslProg(POINTCLOUD_DEFAULT_VS, POINTCLOUD_DEFAULT_FS);
	}
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

void PointCloud::put(float v)
{
	*ptr = v; ++ptr;
}

void PointCloud::put(Vec2f v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
}

void PointCloud::put(Vec3f v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
	*ptr = v.z; ++ptr;
}

void PointCloud::put(Vec4f v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
	*ptr = v.z; ++ptr;
	*ptr = v.w; ++ptr;
}

void PointCloud::put(ColorAf v)
{
	*ptr = v.r; ++ptr;
	*ptr = v.g; ++ptr;
	*ptr = v.b; ++ptr;
	*ptr = v.a; ++ptr;
}

void PointCloud::draw()
{
	// enable states
	shader->bind();
	vbo->bind();
	
	// upload data
	vbo->bufferSubData(0, size * bytesPerParticle, data);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SPRITE);								
	glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnableClientState(GL_VERTEX_ARRAY);
	
	// set attribute pointers
	int offset = 0;
	BOOST_FOREACH(PointDataFormat::Attribute attr, format->attributes) {		
		GLint loc = shader->getAttribLocation(attr.name);
		if(loc == -1) {
			logger() << "WARNING: Couldnt find shader attribute '"<< attr.name << std::endl;
			return;
		}
		glEnableVertexAttribArray(loc);
		glVertexAttribPointer(loc, attr.size, attr.type, false, bytesPerParticle, (void*)offset);
		offset += attr.bytes;
	}
	
	// draw particles
	glDrawArrays(GL_POINTS, 0, size);

	// restore attribute pointers
	BOOST_FOREACH(PointDataFormat::Attribute attr, format->attributes) {		
		GLint loc = shader->getAttribLocation(attr.name);
		glDisableVertexAttribArray(loc);
	}
	
	// restore states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);

	vbo->unbind();
	shader->unbind();
}

