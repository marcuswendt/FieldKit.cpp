/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 16/06/2010.
 */

#pragma once

#include "fieldkit/fbx/FBXKit.h"

namespace fieldkit { namespace fbx {

	//! Utility class to create a new Scene from a FBX file
	class SceneImporter {
	public:
		SceneImporter() {};
		~SceneImporter() {};

		static Scene* load(const std::string &fileName);

	protected:
		static void convertNurbsAndPatch(KFbxSdkManager* sdkManager, KFbxScene* fbxScene);
		static void preparePointCacheData(KFbxScene* fbxScene);
		static void fillCameraArray(KFbxScene* fbxScene, KArrayTemplate<KFbxNode*>& cameras);
		static void fillCameraArrayRecursive(KFbxNode* node, KArrayTemplate<KFbxNode*>& cameras);
		static void fillPoseArray(KFbxScene* fbxScene, KArrayTemplate<KFbxPose*>& poses);
		static void loadTextures(KFbxScene* fbxScene, KArrayTemplate<Texture*> textures);
	};
} }