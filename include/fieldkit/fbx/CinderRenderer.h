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

namespace fieldkit { namespace fbx {

	//! Renders a FBX scene using the cinder library's opengl methods
	class CinderRenderer : 
		public MarkerRenderer,
		public SkeletonRenderer,
		public MeshRenderer,
		public LightRenderer,
		public NullRenderer,
		public CameraRenderer,
		public PerspectiveRenderer {
	public:
		CinderRenderer();
		~CinderRenderer() {};

		// Properties
		float markerSize;
		ci::ColorA markerColor;
		float markerWidth;

		ci::ColorA skeletonColor;
		float skeletonWidth;

		ci::ColorA nullColor;
		float nullSize;

		enum DrawMode { DRAW_MODE_WIREFRAME, DRAW_MODE_LIGHTED, DRAW_MODE_TEXTURED };
		DrawMode meshDrawMode;
		float meshWireframeWidth;
		ci::ColorA meshColor;

		// Perspective
		void setCameraPerspective(
			double fovY, double aspect, 
			double nearPlane, double farPlane, 
			KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up);

		virtual void getWindowSize(int& width, int& height);

		virtual void setCameraOrthogonal(
			double leftPlane, double rightPlane, 
			double bottomPlane, double topPlane, 
			double nearPlane, double farPlane, 
			KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up);

		// SceneGraph
		void drawMarker(KFbxXMatrix& position);

		void drawLimb(KFbxXMatrix& parentGlobalPosition, KFbxXMatrix& globalPosition);

		void drawMesh(Scene* scene, KFbxXMatrix& position, KFbxMesh* mesh, KFbxVector4* vertexArray);

		// Lights
		void drawSpotLight(KFbxColor& color, KFbxXMatrix& position, double coneAngle);
		void drawLight(KFbxColor& color, KFbxXMatrix& position);

		void drawCamera(KFbxXMatrix& position, double roll);
		void drawNull(KFbxXMatrix& position);

	};
} } // namespace fieldkit::fbx