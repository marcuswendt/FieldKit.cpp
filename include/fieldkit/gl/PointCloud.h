/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/gl/GLKit.h"

namespace fieldkit { namespace gl {

	class PointDataFormat;

	static const char* POINTCLOUD_DEFAULT_VS = "\
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

	static const char* POINTCLOUD_DEFAULT_FS = "\
	void main() { \
		vec2 tc = gl_TexCoord[0].st;\
		float dist = distance(vec2(0.5, 0.5), tc);\
		if(dist > 0.5) discard;\
		gl_FragColor = gl_Color;\
	}";

	class PointCloud {
	public:
		PointCloud() {};
		~PointCloud() {};
		
		//! initializes this clouds buffer to a certain format
		void init(PointDataFormat* format, int capacity,
				  DataSourceRef vertexShader=DataSourceRef(), 
				  DataSourceRef fragmentShader=DataSourceRef());
		
		//! clears the buffer data
		void clear();

		//! inserts a float attribute for the current particle
		void put(float v);
		
		//! inserts a 2d vector attribute for the current particle
		void put(Vec2f v);
		
		//! inserts a 3d vector attribute for the current particle
		void put(Vec3f v);
		
		//! inserts a 4d vector attribute for the current particle
		void put(Vec4f v);
		
		//! inserts a color attribute for the current particle
		void put(ColorAf v);
		
		//! call this when all data for a single particle was inserted
		void insert();
		
		//! draws this cloud to screen
		void draw();

	protected:
		PointDataFormat* format;
		int capacity;
		int bytesPerParticle;
		GLfloat* data;
		
		int size;
		GLfloat* ptr;
		
		Vbo* vbo;
		GlslProg* shader;
	};
} } // namespace fieldkit::gl