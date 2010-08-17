/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   Flow
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 29/06/2010.
 */

#include "fieldkit/gl/PointCloud.h"

using namespace fieldkit::gl;

void PointCloud::init(PointDataFormat format, int capacity, GlslProg shader)
{
	this->format = format;
	this->capacity = capacity;
	
	// calculate buffer size
	bytesPerParticle = 0;
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {
		bytesPerParticle += attr.bytes;
	}
	
	int bytesTotal = bytesPerParticle * capacity;
	
	// create buffer
	data = (GLfloat*)malloc(bytesTotal);
	
	// create gl objects
	vbo = new Vbo(GL_ARRAY_BUFFER);
	vbo->bufferData(bytesTotal, data, GL_DYNAMIC_DRAW); // or GL_STREAM_DRAW
	vbo->unbind();

	// when shader programm was given, try to load them
	if(shader != NULL) {
		this->shader = shader;

	// load default shaders
	} else {
		logger() << "PointCloud::init using default shaders" << std::endl;
		
		const char* POINTCLOUD_DEFAULT_VS = "\
		attribute vec3 InVertex;\
		attribute vec4 InColor;\
		attribute float InSize;\
		void main() {\
			vec4 vertex = vec4(InVertex, 1.0);\
			vec4 position = gl_ProjectionMatrix * gl_ModelViewMatrix * vertex;\
			gl_Position = position;\
			gl_PointSize = InSize;\
			gl_FrontColor = InColor;\
		}";
		
		const char* POINTCLOUD_DEFAULT_FS = "\
		void main() { \
			vec2 tc = gl_TexCoord[0].st;\
			float dist = distance(vec2(0.5, 0.5), tc);\
			if(dist > 0.5) discard;\
			gl_FragColor = gl_Color;\
		}";
		
		this->shader = GlslProg(POINTCLOUD_DEFAULT_VS, POINTCLOUD_DEFAULT_FS);
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

void PointCloud::put(Vec2f const& v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
}

void PointCloud::put(Vec3f const& v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
	*ptr = v.z; ++ptr;
}

void PointCloud::put(Vec4f const& v)
{
	*ptr = v.x; ++ptr;
	*ptr = v.y; ++ptr;
	*ptr = v.z; ++ptr;
	*ptr = v.w; ++ptr;
}

void PointCloud::put(ColorAf const& v)
{
	*ptr = v.r; ++ptr;
	*ptr = v.g; ++ptr;
	*ptr = v.b; ++ptr;
	*ptr = v.a; ++ptr;
}

void PointCloud::draw()
{
	// dont draw when no points were inserted
	if(size == 0) return;
	
	// enable states
	shader.bind();
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
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {		
		GLint loc = shader.getAttribLocation(attr.name);
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
	BOOST_FOREACH(PointDataFormat::Attribute attr, format.attributes) {		
		GLint loc = shader.getAttribLocation(attr.name);
		glDisableVertexAttribArray(loc);
	}
	
	// restore states
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);

	vbo->unbind();
	shader.unbind();
}

