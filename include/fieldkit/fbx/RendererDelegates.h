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

//! Interfaces for renderer delegates that draw the FBX data to the screen
namespace fk { namespace fbx {
	
	class MarkerRenderer {
	public:
		virtual void drawMarker(KFbxXMatrix& position) = 0;
	};

	class SkeletonRenderer {
	public:
		virtual void drawLimb(KFbxXMatrix& parentGlobalPosition, KFbxXMatrix& globalPosition) = 0;
	};

	class MeshRenderer {
	public:
		virtual void drawMesh(Scene* scene, KFbxXMatrix& position, KFbxMesh* mesh, KFbxVector4* vertexArray) = 0;
	};

	class LightRenderer {
	public:
		virtual void drawSpotLight(KFbxColor& color, KFbxXMatrix& position, double coneAngle) = 0;

		// all other types of lights
		virtual void drawLight(KFbxColor& color, KFbxXMatrix& position) = 0;
	};

	class NullRenderer {
	public:
		virtual void drawNull(KFbxXMatrix& position) = 0;
	};

	class CameraRenderer {
	public:
		virtual void drawCamera(KFbxXMatrix& position, double roll) = 0;
	};

	class PerspectiveRenderer {
	public:
		virtual void setCameraPerspective(
						double fovY, double aspect, 
						double nearPlane, double farPlane, 
						KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up
					 ) = 0;

		virtual void getWindowSize(int& width, int& height) = 0;

		virtual void setCameraOrthogonal(
						double leftPlane, double rightPlane, 
						double bottomPlane, double topPlane, 
						double nearPlane, double farPlane, 
						KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up
					) = 0;
	};
} } // namespace fk::fbx