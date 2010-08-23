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
	class Renderer {
	public:
		// Con/Destructors
		Renderer() {
			clearDelegates();
		};
		~Renderer() {};

		// Actions

		//! sets the modelview and perspective matrizes
		void drawPerspective(Scene* scene, double orthoCameraScale);

		//! draws the scene graph
		void drawScene(Scene* scene);

		//! remove all delegates
		void clearDelegates();

		// Accessors
		void setMarkerDelegate(MarkerRenderer* renderer) {
			this->markerRenderer = renderer;
		}
		void setSkeletonDelegate(SkeletonRenderer* renderer) {
			this->skeletonRenderer = renderer;
		}
		void setMeshDelegate(MeshRenderer* renderer) {
			this->meshRenderer = renderer;
		}
		void setLightDelegate(LightRenderer* renderer) {
			this->lightRenderer = renderer;
		}
		void setNullDelegate(NullRenderer* renderer) {
			this->nullRenderer = renderer;
		}
		void setCameraDelegate(CameraRenderer* renderer) {
			this->cameraRenderer = renderer;
		}
		void setPerspectiveDelegate(PerspectiveRenderer* renderer) {
			this->perspectiveRenderer = renderer;
		}
		
	protected:
		MarkerRenderer* markerRenderer;
		SkeletonRenderer* skeletonRenderer;
		MeshRenderer* meshRenderer;
		LightRenderer* lightRenderer;
		NullRenderer* nullRenderer;
		CameraRenderer* cameraRenderer;
		PerspectiveRenderer* perspectiveRenderer;

		// Scene rendering
		void drawNodeRecursive(Scene* scene, KFbxNode* node, KFbxXMatrix& parentGlobalPosition);
		void drawNode(Scene* scene, KFbxNode* node, 
					  KFbxXMatrix& parentGlobalPosition, KFbxXMatrix globalOffPosition,
					  KFbxPose* pose = NULL);

		void drawMarker(KFbxXMatrix& globalPosition);

		void drawSkeleton(KFbxNode* node, KFbxXMatrix& parentGlobalPosition,
						  KFbxXMatrix& globalPosition);

		void drawMesh(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition, KFbxPose* pose);

		void readVertexCacheData(KFbxMesh* mesh, KTime& time, KFbxVector4* vertexArray);
		void computeClusterDeformation(Scene* scene, KFbxXMatrix& globalPosition, KFbxMesh* mesh,
									   KFbxVector4* vertexArray, KFbxPose* pose);
		void computeShapeDeformation(Scene* scene, KFbxNode* node, KFbxMesh* mesh, KFbxVector4* vertexArray);

		//! draws the camera shape
		void drawCamera(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition);
		void drawLight(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition);
		void drawNull(KFbxXMatrix& globalPosition);
	};
} } // namespace fieldkit::fbx