/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#include "fieldkit/fbx/Scene.h"

using namespace fk::fbx;

// -- Constructor --------------------------------------------------------------
Scene::Scene() {}

Scene::~Scene()
{
	// Delete the FBX SDK manager. All the objects that have been allocated 
	// using the FBX SDK manager and that haven't been explicitly destroyed 
	// are automatically destroyed at the same time.
	if(sdkManager) sdkManager->Destroy();
	sdkManager = NULL;

	// Delete the array used to associate menu ids with animation stack names.
	FbxSdkDeleteAndClear(animationNames);

	// Delete any texture allocated
	FbxSdkDeleteAndClear(textures);
}


// -- Init ---------------------------------------------------------------------
void Scene::init() {
	// The first thing to do is to create the FBX SDK manager which is the 
    // object allocator for almost all the classes in the SDK.
    sdkManager = KFbxSdkManager::Create();
	
    if (!sdkManager) {
		console() << ("Unable to create the FBX SDK manager\n");
        exit(-1000);
    }
	
	// create an IOSettings object
	KFbxIOSettings * ios = KFbxIOSettings::Create(sdkManager, IOSROOT);
	sdkManager->SetIOSettings(ios);
	
	// Load plugins from the executable directory
	#if defined(KARCH_ENV_WIN)
		KString lExtension = "dll";
	#elif defined(KARCH_ENV_MACOSX)
		KString lExtension = "dylib";
	#elif defined(KARCH_ENV_LINUX)
		KString lExtension = "so";
	#endif
	
	#if defined(KARCH_ENV_WIN)
		KString lPath = KFbxGetApplicationDirectory();
		sdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());
	
	#elif defined(KARCH_ENV_MACOSX)
		char* lPath = (char*)App::get()->getAppPath().c_str();
		sdkManager->LoadPluginsDirectory(lPath, lExtension.Buffer());
	
	#endif	

    // Create the entity that will hold the scene.
    fbxScene = KFbxScene::Create(sdkManager,"");
}


// -- Actions ----------------------------------------------------------------
void Scene::setAnimation(int index)
{
	console() << "Scene::setAnimation("<< index <<")"<< std::endl;

	int numAnimStacks = animationNames.GetCount();
	if (!numAnimStacks || index >= numAnimStacks) {
		return;
	}

	// select the base layer from the animation stack
	KFbxAnimStack* currentAnimationStack = fbxScene->FindMember(FBX_TYPE(KFbxAnimStack), animationNames[index]->Buffer());
	if (currentAnimationStack == NULL) {
		// this is a problem. The anim stack should be found in the scene!
		console() << "The anim stack should be found in the scene!\n";
		return;
	}

	// we assume that the first animation layer connected to the animation stack is the base layer
	// (this is the assumption made in the FBXSDK)
	currentAnimationLayer = currentAnimationStack->GetMember(FBX_TYPE(KFbxAnimLayer), 0);
	fbxScene->GetEvaluator()->SetContext(currentAnimationStack);

	KFbxTakeInfo* lCurrentTakeInfo = fbxScene->GetTakeInfo(*(animationNames[index]));
	if (lCurrentTakeInfo) {
		start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();

		printf("Scene::initAnimStack $1 start %f stop %f \n", start.GetSecondDouble(), stop.GetSecondDouble());

	} else {
		// Take the time line value
		KTimeSpan lTimeLineTimeSpan;
		fbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

		start = lTimeLineTimeSpan.GetStart();
		stop  = lTimeLineTimeSpan.GetStop();

		printf("Scene::initAnimStack $2 start %f stop %f \n", start.GetSecondDouble(), stop.GetSecondDouble());
	}

	currentTime = start;
}

int Scene::getCurrentAnimationIndex() 
{
	int index = 0;
	for(int i = 0; i < animationNames.GetCount(); i++) {
		KString* currentName = animationNames[i];
		KString activeName = KFbxGet<KString>(fbxScene->ActiveAnimStackName);

		if(currentName->Compare(activeName) == 0) {
			index = i;
		}
	}
	return index;
}


// -- Accessors ----------------------------------------------------------------

// Get the global position.
// Do not take in account the geometric transform.
KFbxXMatrix Scene::getGlobalPosition(KFbxNode* pNode, KTime& pTime, KFbxXMatrix* pParentGlobalPosition) {

    // Ideally this function would use parent global position and local position to
    // compute the global position.
    // Unfortunately the equation 
    //    lGlobalPosition = pParentGlobalPosition * lLocalPosition
    // does not hold when inheritance type is other than "Parent" (RSrs). To compute
    // the parent rotation and scaling is tricky in the RrSs and Rrs cases.
    // This is why GetNodeGlobalTransform() is used: it always computes the right
    // global position.

	return pNode->GetScene()->GetEvaluator()->GetNodeGlobalTransform(pNode, pTime);
}

KFbxXMatrix Scene::getGlobalPosition(KFbxNode* node, KTime& time, KFbxPose* pose, KFbxXMatrix* parentGlobalPosition)
{
	KFbxXMatrix lGlobalPosition;
	bool        lPositionFound = false;

	if (pose) {
		int lNodeIndex = pose->Find(node);

		if (lNodeIndex > -1) {
			// The bind pose is always a global matrix.
			// If we have a rest pose, we need to check if it is
			// stored in global or local space.
			if (pose->IsBindPose() || !pose->IsLocalMatrix(lNodeIndex)) {
				lGlobalPosition = getPoseMatrix(pose, lNodeIndex);
			} else {
				// We have a local matrix, we need to convert it to
				// a global space matrix.
				KFbxXMatrix lParentGlobalPosition;

				if (parentGlobalPosition) {
					lParentGlobalPosition = *parentGlobalPosition;
				} else {
					if (node->GetParent()) {
						lParentGlobalPosition = getGlobalPosition(node->GetParent(), time, pose);
					}
				}

				KFbxXMatrix lLocalPosition = getPoseMatrix(pose, lNodeIndex);
				lGlobalPosition = lParentGlobalPosition * lLocalPosition;
			}

			lPositionFound = true;
		}
	}

	if (!lPositionFound) {
		// There is no pose entry for that node, get the current global position instead
		lGlobalPosition = getGlobalPosition(node, time, parentGlobalPosition);
	}

	return lGlobalPosition;
}

// Get the matrix of the given pose
KFbxXMatrix Scene::getPoseMatrix(KFbxPose* pose, int nodeIndex)
{
	KFbxXMatrix lPoseMatrix;
	KFbxMatrix lMatrix = pose->GetMatrix(nodeIndex);

	memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

	return lPoseMatrix;
}

// Get the geometry deformation local to a node. It is never inherited by the
// children.
KFbxXMatrix Scene::getGeometry(KFbxNode* node) {
    KFbxVector4 lT, lR, lS;
    KFbxXMatrix lGeometry;

    lT = node->GetGeometricTranslation(KFbxNode::eSOURCE_SET);
    lR = node->GetGeometricRotation(KFbxNode::eSOURCE_SET);
    lS = node->GetGeometricScaling(KFbxNode::eSOURCE_SET);

    lGeometry.SetT(lT);
    lGeometry.SetR(lR);
    lGeometry.SetS(lS);

    return lGeometry;
}

bool Scene::isProducerCamera(KFbxCamera* camera)
{
	KFbxGlobalCameraSettings& lGlobalCameraSettings = fbxScene->GlobalCameraSettings();
	if (camera == lGlobalCameraSettings.GetCameraProducerPerspective())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerTop())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerBottom())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerFront())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerBack())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerRight())
		return true;
	if (camera == lGlobalCameraSettings.GetCameraProducerLeft())
		return true;

	return false;
}

// -- Cameras ----------------------------------------------------------------------------
KFbxCamera* Scene::getCurrentCamera(KTime time)
{
	KFbxGlobalSettings& lGlobalSettings = fbxScene->GetGlobalSettings();
	KFbxGlobalCameraSettings& lGlobalCameraSettings = fbxScene->GlobalCameraSettings();
	KString lCurrentCameraName = lGlobalSettings.GetDefaultCamera();

	// check if we need to create the Producer cameras!
	if (lGlobalCameraSettings.GetCameraProducerPerspective() == NULL &&
		lGlobalCameraSettings.GetCameraProducerBottom() == NULL &&
		lGlobalCameraSettings.GetCameraProducerTop() == NULL &&
		lGlobalCameraSettings.GetCameraProducerFront() == NULL &&
		lGlobalCameraSettings.GetCameraProducerBack() == NULL &&
		lGlobalCameraSettings.GetCameraProducerRight() == NULL &&
		lGlobalCameraSettings.GetCameraProducerLeft() == NULL)
	{
		lGlobalCameraSettings.CreateProducerCameras();
	}

	if (lCurrentCameraName.Compare(PRODUCER_PERSPECTIVE) == 0) {
		return lGlobalCameraSettings.GetCameraProducerPerspective();

	} else if (lCurrentCameraName.Compare(PRODUCER_BOTTOM) == 0) {
		return lGlobalCameraSettings.GetCameraProducerBottom();

	} else if (lCurrentCameraName.Compare(PRODUCER_TOP) == 0) {
		return lGlobalCameraSettings.GetCameraProducerTop();

	} else if (lCurrentCameraName.Compare(PRODUCER_FRONT) == 0) {
		return lGlobalCameraSettings.GetCameraProducerFront();

	} else if (lCurrentCameraName.Compare(PRODUCER_BACK) == 0) {
		return lGlobalCameraSettings.GetCameraProducerBack();

	} else if (lCurrentCameraName.Compare(PRODUCER_RIGHT) == 0) {
		return lGlobalCameraSettings.GetCameraProducerRight();

	} else if (lCurrentCameraName.Compare(PRODUCER_LEFT) == 0) {
		return lGlobalCameraSettings.GetCameraProducerLeft();

	} else if (lCurrentCameraName.Compare(CAMERA_SWITCHER) == 0) {
		KFbxCameraSwitcher* lCameraSwitcher = fbxScene->GlobalCameraSettings().GetCameraSwitcher();
		KFbxAnimCurve* lCurve = NULL;
		if (lCameraSwitcher) {
			lCurve = lCameraSwitcher->CameraIndex.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
			KFCurve* lFCurve = (lCurve)?lCurve->GetKFCurve():NULL;

			int lCameraIndex = 0;
			if (lFCurve)
				// Get the index of the camera in the camera array.
				lCameraIndex = (int) lFCurve->Evaluate(time) - 1;

			if (lCameraIndex >= 0 && lCameraIndex < cameras.GetCount()) {
				KFbxNode* lNode = cameras[lCameraIndex];

				// Get the animated parameters of the camera.
				getCameraAnimatedParameters(lNode, time);

				return (KFbxCamera*) lNode->GetNodeAttribute();
			}
		}
	} else {
		int i;
		KFbxNode* lNode = NULL;

		// Find the camera in the camera array.
		for (i = 0; i < cameras.GetCount(); i++) {
			if (lCurrentCameraName.Compare(cameras[i]->GetName()) == 0) {
				lNode = cameras[i];
				break;
			}
		}

		if (lNode) {
			// Get the animated parameters of the camera.
			getCameraAnimatedParameters(lNode, time);

			return (KFbxCamera*) lNode->GetNodeAttribute();
		}
	}

	return lGlobalCameraSettings.GetCameraProducerPerspective();
}

// Get the animated parameters of a camera contained in the scene
// and store them in the associated member variables contained in 
// the camera.
void Scene::getCameraAnimatedParameters(KFbxNode* pNode, KTime time)
{
	KFbxCamera* lCamera = (KFbxCamera*) pNode->GetNodeAttribute();
	lCamera->Position.Set(getGlobalPosition(pNode, time).GetT());

	KFbxAnimCurve* fc = lCamera->Roll.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
	if (fc)
		lCamera->Roll.Set(fc->Evaluate(time));

	if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL || 
		lCamera->GetApertureMode() == KFbxCamera::eVERTICAL) 
	{
		fc = lCamera->FieldOfView.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
		if (fc)
			lCamera->FieldOfView.Set(fc->Evaluate(time));
	}
	else if (lCamera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL)
	{
		// In this aperture mode, the squeeze ratio is not relevant 
		// because the fields of view in X and Y are independent.
		// It's value is set to retrieve the proper aspect in function SetCamera().
		//
		// if:
		// 
		// aspect = field of view X / field of view Y = aperture width * squeeze ratio / aperture height
		//
		// then:
		// 
		// squeeze ratio = (field of view X * aperture height) / (field of view Y * aperture width)
		//
		double lFieldOfViewX = lCamera->FieldOfViewX.Get();
		double lFieldOfViewY = lCamera->FieldOfViewY.Get();
		fc = lCamera->FieldOfViewX.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
		if (fc)
			lFieldOfViewX = fc->Evaluate(time);

		fc = lCamera->FieldOfViewY.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
		if (fc)
			lFieldOfViewY = fc->Evaluate(time);

		double lSqueezeRatio = (lFieldOfViewX * lCamera->GetApertureHeight()) / (lFieldOfViewY * lCamera->GetApertureWidth());

		lCamera->FieldOfViewX.Set(lFieldOfViewX);
		lCamera->FieldOfViewY.Set(lFieldOfViewY);
		lCamera->SetSqueezeRatio(lSqueezeRatio);
	}
	else if (lCamera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH)
	{
		double lFocalLength = lCamera->FocalLength.Get();
		fc = lCamera->FocalLength.GetCurve<KFbxAnimCurve>(currentAnimationLayer);
		if (fc && fc ->Evaluate(time))
			lCamera->FocalLength.Set(lFocalLength);
	}
}