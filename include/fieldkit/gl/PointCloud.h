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