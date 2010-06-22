/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#pragma once

#include "fieldkit/fbx/FBXKit.h"

namespace fk { namespace fbx {
	class Texture {
	public:
		Texture() {};
		~Texture() {};

		//gl::Texture glTexture;
		KFbxTexture* refTexture;
	};
} } // namespace fk::fbx