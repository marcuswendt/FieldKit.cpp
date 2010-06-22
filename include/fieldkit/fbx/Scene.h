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
	class Scene {
	public:
		friend class SceneController;
		friend class SceneImporter;
		friend class Renderer;

		// Con/Destructors
		Scene();
		~Scene();

		void init();

		// Actions
		void setAnimation(int index);
		int getCurrentAnimationIndex();

		// Accessors
		KFbxXMatrix getGlobalPosition(KFbxNode* node, KTime& time, 
									  KFbxXMatrix* parentGlobalPosition = NULL);

		KFbxXMatrix getGlobalPosition(KFbxNode* node, KTime& time, KFbxPose* pose, 
									  KFbxXMatrix* parentGlobalPosition = NULL);

		KFbxXMatrix getPoseMatrix(KFbxPose* pose, int nodeIndex);

		KFbxXMatrix getGeometry(KFbxNode* node);

		KFbxCamera* getCurrentCamera(KTime time);
		void getCameraAnimatedParameters(KFbxNode* node, KTime time);
		bool isProducerCamera(KFbxCamera* camera);

		KArrayTemplate<Texture*> getTextures() { return textures; }

	protected:	
		KFbxSdkManager* sdkManager;
		KFbxScene* fbxScene;
		KFbxAnimLayer* currentAnimationLayer;

		KArrayTemplate<KFbxNode*> cameras;
		KArrayTemplate<KString*> animationNames;
		KArrayTemplate<KFbxPose*> poses;
		KArrayTemplate<Texture*> textures;

		KTime period, start, stop, currentTime;
	};
} } // namespace fieldkit::fbx