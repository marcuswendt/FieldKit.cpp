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
#include "fieldkit/gl/PointDataFormat.h"

namespace fieldkit { namespace gl {

	class PointCloud {
	public:
		PointCloud() {};
		~PointCloud() {};
		
		//! initializes this clouds buffer to a certain format
		void init(PointDataFormat format, int capacity, GlslProg shader=NULL);
		
		//! clears the buffer data
		void clear();

		//! inserts a float attribute for the current particle
		void put(float v);
		
		//! inserts a 2d vector attribute for the current particle
		void put(Vec2f const& v);
		
		//! inserts a 3d vector attribute for the current particle
		void put(Vec3f const& v);
		
		//! inserts a 4d vector attribute for the current particle
		void put(Vec4f const& v);
		
		//! inserts a color attribute for the current particle
		void put(ColorAf const& v);
		
		//! call this when all data for a single particle was inserted
		void insert();
		
		//! draws this cloud to screen
		void draw();

	protected:
		PointDataFormat format;
		int capacity;
		int bytesPerParticle;
		GLfloat* data;
		
		int size;
		GLfloat* ptr;
		
		Vbo* vbo;
		GlslProg shader;
	};
} } // namespace fieldkit::gl