/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#pragma once

#include "fieldkit/fbx/FBX.h"

#include <cinder/Vector.h>

//! Utility functions & classes used throughout the fbx library
namespace fk { namespace fbx {

	//! Scale all the elements of a matrix.
	inline void MatrixScale(KFbxXMatrix& matrix, double value) {
		int i,j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				matrix[i][j] *= value;
			}
		}
	};

	//! Scale all the elements of a matrix.
	inline void MatrixAddToDiagonal(KFbxXMatrix& matrix, double value) {
		matrix[0][0] += value;
		matrix[1][1] += value;
		matrix[2][2] += value;
		matrix[3][3] += value;
	};

	//! Sum two matrices element by element.
	inline void MatrixAdd(KFbxXMatrix& dstMatrix, KFbxXMatrix& srcMatrix) {
		int i,j;
		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				dstMatrix[i][j] += srcMatrix[i][j];
			}
		}
	};

	inline ci::Vec3f KFbxVector4ToVec3f(KFbxVector4 v) {
		return ci::Vec3f((float)v[0], (float)v[1], (float)v[2]);
	}

	// Used in SceneImporter
	template<class KString> 
	class DeletionPolicy {
	public:
		static inline void DeleteIt(KString** ptr) {
			if(*ptr != NULL) {
				delete *ptr;
				*ptr = NULL;
			}
		}
	};
} };