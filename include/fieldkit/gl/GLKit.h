/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 30/06/2010.
 */

#pragma once

#include "fieldkit/gl/GLKit_Prefix.h"
#include "fieldkit/gl/PointDataFormat.h"
#include "fieldkit/gl/PointCloud.h"
#include "fieldkit/gl/ShaderSet.h"

namespace fieldkit { namespace gl {

	void drawCube(Vec3f const& center, Vec3f const& size);
	
} }