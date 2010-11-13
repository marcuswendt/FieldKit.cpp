/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 16/06/2010.
 */

#include "fieldkit/fbx/SceneImporter.h"

using namespace fieldkit::fbx;

Scene* SceneImporter::load(const string &fileName_)
{
	// Create the scene
	LOG_INFO("fieldkit::fbx::SceneImporter::load " << fileName_);
	
	Scene* scene = new Scene();
	
	scene->init();

	KFbxScene* fbxScene = scene->fbxScene;
	KFbxSdkManager* sdkManager = scene->sdkManager;

	// Create the importer.
	LOG_INFO("fieldkit::fbx::SceneImporter::load - Create the importer 1");
	int fileFormat = -1;
	KScopedPtr<KString, fbx::DeletionPolicy<KString> > fileName;
	fileName.Reset(new KString());

	*fileName = fileName_.c_str();

	LOG_INFO("fieldkit::fbx::SceneImporter::load - Create the importer 2");
	KFbxImporter* importer = KFbxImporter::Create(sdkManager,"");
	if(!sdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(*fileName, fileFormat) ) {
		// Unrecognizable file format. Try to fall back to KFbxImporter::eFBX_BINARY
		LOG_INFO("fieldkit::fbx::SceneImporter::load - Create the importer 3");
		fileFormat = sdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}

	// Initialize the importer by providing a filename.
	if(importer->Initialize(fileName->Buffer(), fileFormat) == true) {
		LOG_INFO( "Loading file: " << fileName->Buffer());

		// Import scene
		if(importer->Import(fbxScene) == true) {
			// Convert Axis System to what is used in this example, if needed
			KFbxAxisSystem SceneAxisSystem = fbxScene->GetGlobalSettings().GetAxisSystem();
			KFbxAxisSystem OurAxisSystem(KFbxAxisSystem::YAxis, KFbxAxisSystem::ParityOdd, KFbxAxisSystem::RightHanded);
			if( SceneAxisSystem != OurAxisSystem ) {
				OurAxisSystem.ConvertScene(fbxScene);
			}

			// Convert Unit System to what is used in this example, if needed
			KFbxSystemUnit SceneSystemUnit = fbxScene->GetGlobalSettings().GetSystemUnit();
			if( SceneSystemUnit.GetScaleFactor() != 1.0 ) {
				KFbxSystemUnit OurSystemUnit(1.0);
				OurSystemUnit.ConvertScene(fbxScene);
			}

			// Nurbs and patch attribute types are not supported yet.
			// Convert them into mesh node attributes to have them drawn.
			convertNurbsAndPatch(sdkManager, fbxScene);

			// Convert any .PC2 point cache data into the .MC format for 
			// vertex cache deformer playback.
			preparePointCacheData(fbxScene);

			// Get the list of all the cameras in the scene.
			fillCameraArray(fbxScene, scene->cameras);

			// Get the list of all the animation stack.
			fbxScene->FillAnimStackNameArray(scene->animationNames);

			// Get the list of pose in the scene
			fillPoseArray(fbxScene, scene->poses);

			// Load the texture data in memory (for supported formats)
			loadTextures(fbxScene, scene->textures);

			// find active anim stack index
			scene->setAnimation(scene->getCurrentAnimationIndex());

			// Initialize the frame period.
			scene->period.SetTime(0, 0, 0, 1, 0, fbxScene->GetGlobalSettings().GetTimeMode());
		}

	} else {
		LOG_ERROR("Unable to open file!")
		LOG_INFO( "Error reported: " << importer->GetLastErrorString())
		exit(-1001);
	}

	// Destroy the importer to release the file.
	importer->Destroy();

	return scene;
}

// -- Helpers ----------------------------------------------------------------
void SceneImporter::convertNurbsAndPatch(KFbxSdkManager* sdkManager, KFbxScene* fbxScene) {
	LOG_WARN( "Scene::convertNurbsAndPatch() - not implemented at the moment!");
}

void SceneImporter::preparePointCacheData(KFbxScene* fbxScene) {
	LOG_WARN( "Scene::preparePointCacheData() - not implemented at the moment!");
}

void SceneImporter::fillCameraArray(KFbxScene* fbxScene, KArrayTemplate<KFbxNode*>& cameras) {
	cameras.Clear();
	fillCameraArrayRecursive(fbxScene->GetRootNode(), cameras);
}

void SceneImporter::fillCameraArrayRecursive(KFbxNode* node, KArrayTemplate<KFbxNode*>& cameras) {
	int i, lCount;

	if (node) {
		if (node->GetNodeAttribute()) {
			if (node->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eCAMERA) {
				cameras.Add(node);
			}
		}

		lCount = node->GetChildCount();

		for (i = 0; i < lCount; i++) {
			fillCameraArrayRecursive(node->GetChild(i), cameras);
		}
	}
}

void SceneImporter::fillPoseArray(KFbxScene* fbxScene, KArrayTemplate<KFbxPose*>& poses) {
	int i, lPoseCount;

	for (i=0, lPoseCount = fbxScene->GetPoseCount(); i < lPoseCount; i++) {
		poses.Add(fbxScene->GetPose(i));
	}
} 

void SceneImporter::loadTextures(KFbxScene* fbxScene, KArrayTemplate<Texture*> textures) 
{
	LOG_WARN( "Scene::loadTextures() - not implemented at the moment!");
}