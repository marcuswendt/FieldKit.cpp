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
	class PointCloud;

	class PointDataFormat {
	public:
		PointDataFormat() {};
		~PointDataFormat() {};
		
		void clear();
		void addAttribute(string name, int size, int type=GL_FLOAT);
						  
//	protected:
		//! defines a single attribute
		struct Attribute {
			string name;
			
			//! variable type of this attribute (GL_INT, GL_FLOAT)
			int type;
			
			//! number of elements 
			int size;
			
			//! size in bytes (depends on type)
			int bytes;
		};

		vector<PointDataFormat::Attribute> attributes;
	};
} } // namespace fieldkit::gl