/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#include "fieldkit/fbx/Renderer.h"

using namespace fk::fbx;


void fk::fbx::Renderer::clearDelegates()
{
	markerRenderer = NULL;
	skeletonRenderer = NULL;
	meshRenderer = NULL;
	lightRenderer = NULL;
	nullRenderer = NULL;
	cameraRenderer = NULL;
	perspectiveRenderer = NULL;
}

// -- Perspective ------------------------------------------------------------
void fk::fbx::Renderer::drawPerspective(Scene* scene, double orthoCameraScale)
{
	// check if renderer is set
	if(perspectiveRenderer == NULL) return;

	KTime& time = scene->currentTime;

	// Find the current camera at the given time.
	KFbxCamera* camera = scene->getCurrentCamera(scene->currentTime);
	KFbxNode*   cameraNode = camera ? camera->GetNode() : NULL;

	// Compute the camera position and direction.
	KFbxVector4 eye(0,0,1);
	KFbxVector4 center(0,0,0);
	KFbxVector4 up(0,1,0);
	KFbxVector4 forward, right;

	if(camera) {
		eye = camera->Position.Get();
		up = camera->UpVector.Get();
	}

	if(cameraNode && cameraNode->GetTarget()) {
		center = scene->getGlobalPosition(cameraNode->GetTarget(), time).GetT();

	} else {
		if(!cameraNode || scene->isProducerCamera(camera)) {
			if (camera)
				center = camera->InterestPosition.Get();

		} else {
			// Get the direction
			KFbxXMatrix lGlobalRotation;
			KFbxVector4 lRotationVector(scene->getGlobalPosition(cameraNode, time).GetR());
			lGlobalRotation.SetR(lRotationVector);

			// Get the length
			KFbxVector4 lInterestPosition(camera->InterestPosition.Get());
			KFbxVector4 lCameraGlobalPosition(scene->getGlobalPosition(cameraNode, time).GetT());
			double      lLength = (KFbxVector4(lInterestPosition - lCameraGlobalPosition).Length());

			// Set the center.
			// A camera with rotation = {0,0,0} points to the X direction. So create a
			// vector in the X direction, rotate that vector by the global rotation amount
			// and then position the center by scaling and translating the resulting vector
			lRotationVector = KFbxVector4(1.0,0,0);
			center = lGlobalRotation.MultT(lRotationVector);
			center *= lLength;
			center += eye;

			// Update the default up vector with the camera rotation.
			lRotationVector = KFbxVector4(0,1.0,0);
			up = lGlobalRotation.MultT(lRotationVector);
		}
	}

	// Align the up vector.
	forward = center - eye;
	forward.Normalize();
	right = forward.CrossProduct(up);
	right.Normalize();
	up = right.CrossProduct(forward);
	up.Normalize();

	// Rotate the up vector with the roll value.
	double radians = 0;

	if (camera)
		radians = 3.1416 * camera->Roll.Get() / 180.0;
	up *= cos(radians);
	right *= sin(radians);
	up = up + right;

	double nearPlane = 0.01;
	if (camera)
		nearPlane = camera->GetNearPlane();
	double farPlane = 1000.0;
	if (camera)
		farPlane = camera->GetFarPlane();

	// Get the relevant camera settings for a perspective view.
	if (camera && camera->ProjectionType.Get() == KFbxCamera::ePERSPECTIVE) {
		double fovY=0.0;
		double aspect = camera->GetApertureWidth() * camera->GetSqueezeRatio() / camera->GetApertureHeight();

		if (camera->GetApertureMode() == KFbxCamera::eHORIZONTAL 
			|| camera->GetApertureMode() == KFbxCamera::eVERTICAL) {
				fovY = camera->FieldOfView.Get();
				if (camera->GetApertureMode() == KFbxCamera::eHORIZONTAL)
					fovY /= aspect;
		} else if (camera->GetApertureMode() == KFbxCamera::eFOCAL_LENGTH) {
			fovY = camera->ComputeFieldOfView(camera->FocalLength.Get());
			fovY /= aspect;
		} else if (camera->GetApertureMode() == KFbxCamera::eHORIZONTAL_AND_VERTICAL) {
			fovY = camera->FieldOfViewY.Get();
		}

		perspectiveRenderer->setCameraPerspective(
			fovY, aspect, 
			nearPlane, farPlane, 
			eye, center, up);

		// Get the relevant camera settings for an orthogonal view.
	} else {

		double pixelRatio = 1.0;
		if(camera)
			pixelRatio = camera->GetPixelRatio();  

		int width, height;
		double leftPlane, rightPlane, bottomPlane, topPlane;

		perspectiveRenderer->getWindowSize(width, height);

		if(width < height) {   
			leftPlane   = -orthoCameraScale * pixelRatio;
			rightPlane  =  orthoCameraScale * pixelRatio;
			bottomPlane = -orthoCameraScale * height / width;
			topPlane    =  orthoCameraScale * height / width;               

		} else {
			width *= (int) pixelRatio;
			leftPlane   = -orthoCameraScale * width / height;
			rightPlane  =  orthoCameraScale * width / height;
			bottomPlane = -orthoCameraScale;
			topPlane    =  orthoCameraScale;
		}

		perspectiveRenderer->setCameraOrthogonal(
			leftPlane, rightPlane, 
			bottomPlane, topPlane, 
			nearPlane, farPlane, 
			eye, center, up);
	}
}


// -- Scene --------------------------------------------------------------------
void fk::fbx::Renderer::drawScene(Scene* scene)
{
	KFbxScene* fbxScene = scene->fbxScene;
	KFbxXMatrix lDummyGlobalPosition;

	int i, lCount = fbxScene->GetRootNode()->GetChildCount();
	for (i = 0; i < lCount; i++) {
		drawNodeRecursive(scene, fbxScene->GetRootNode()->GetChild(i), lDummyGlobalPosition);
	}
}

void fk::fbx::Renderer::drawNodeRecursive(Scene* scene, KFbxNode* node, KFbxXMatrix& parentGlobalPosition) 
{
	// Compute the node's global position.
	KFbxXMatrix lGlobalPosition = scene->getGlobalPosition(node, scene->currentTime, &parentGlobalPosition);

	// Geometry offset.
	// it is not inherited by the children.
	KFbxXMatrix geometryOffset = scene->getGeometry(node);
	KFbxXMatrix globalOffPosition = lGlobalPosition * geometryOffset;

	drawNode(scene, node, parentGlobalPosition, globalOffPosition);

	int i, lCount = node->GetChildCount();

	for (i = 0; i < lCount; i++) {
		drawNodeRecursive(scene, node->GetChild(i), lGlobalPosition);
	}
}

void fk::fbx::Renderer::drawNode(Scene* scene, KFbxNode* node, 
								 KFbxXMatrix& parentGlobalPosition, KFbxXMatrix globalOffPosition,
								 KFbxPose* pose)
{
	KFbxNodeAttribute* lNodeAttribute = node->GetNodeAttribute();

	if (lNodeAttribute) {
		if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMARKER) {
			drawMarker(globalOffPosition);

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eSKELETON) {
			drawSkeleton(node, parentGlobalPosition, globalOffPosition);

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eMESH) {
			drawMesh(scene, node, globalOffPosition, pose);

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNURB) {
			// Not supported yet. 
			// Should have been converted into a mesh in function ConvertNurbsAndPatch().

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::ePATCH) {
			// Not supported yet. 
			// Should have been converted into a mesh in function ConvertNurbsAndPatch().

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eCAMERA) {
			drawCamera(scene, node, globalOffPosition);

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eLIGHT) {
			drawLight(scene, node, globalOffPosition);

		} else if (lNodeAttribute->GetAttributeType() == KFbxNodeAttribute::eNULL) {
			drawNull(globalOffPosition);
		}
	} else {
		drawNull(globalOffPosition);
	}
}


// -- Marker --------------------------------------------------------------------
void fk::fbx::Renderer::drawMarker(KFbxXMatrix& globalPosition)
{
	if(markerRenderer == NULL) return;
	markerRenderer->drawMarker(globalPosition);
}


// -- Skeleton -------------------------------------------------------------------
void fk::fbx::Renderer::drawSkeleton(KFbxNode* node, KFbxXMatrix& parentGlobalPosition, KFbxXMatrix& globalPosition)
{
	if(skeletonRenderer == NULL) return;

	KFbxSkeleton* skeleton = (KFbxSkeleton*) node->GetNodeAttribute();

	// Only draw the skeleton if it's a limb node and if 
	// the parent also has an attribute of type skeleton.
	if (skeleton->GetSkeletonType() == KFbxSkeleton::eLIMB_NODE &&
		node->GetParent() &&
		node->GetParent()->GetNodeAttribute() &&
		node->GetParent()->GetNodeAttribute()->GetAttributeType() == KFbxNodeAttribute::eSKELETON)
	{
		skeletonRenderer->drawLimb(parentGlobalPosition, globalPosition);
	}
}

// -- Mesh ----------------------------------------------------------------------
void fk::fbx::Renderer::drawMesh(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition, KFbxPose* pose)
{
	if(meshRenderer == NULL) return;

	KTime& time = scene->currentTime;
	KFbxMesh* mesh = (KFbxMesh*) node->GetNodeAttribute();
	int clusterCount = 0;
	int skinCount= 0;
	int vertexCount = mesh->GetControlPointsCount();

	// No vertex to draw.
	if (vertexCount == 0) {
		return;
	}

	// Create a copy of the vertex array to receive vertex deformations.
	KFbxVector4* vertexArray = new KFbxVector4[vertexCount];
	memcpy(vertexArray, mesh->GetControlPoints(), vertexCount * sizeof(KFbxVector4));

	// Active vertex cache deformer will overwrite any other deformer
	if (mesh->GetDeformerCount(KFbxDeformer::eVERTEX_CACHE) &&
		(static_cast<KFbxVertexCacheDeformer*>(mesh->GetDeformer(0, KFbxDeformer::eVERTEX_CACHE)))->IsActive()) {
			readVertexCacheData(mesh, time, vertexArray);

	} else {
		if (mesh->GetShapeCount()) {
			// Deform the vertex array with the shapes.
			computeShapeDeformation(scene, node, mesh, vertexArray);
		}

		//we need to get the number of clusters
		skinCount = mesh->GetDeformerCount(KFbxDeformer::eSKIN);
		for( int i=0; i< skinCount; i++)
			clusterCount += ((KFbxSkin *)(mesh->GetDeformer(i, KFbxDeformer::eSKIN)))->GetClusterCount();
		
		if(clusterCount) {
			// Deform the vertex array with the links.
			computeClusterDeformation(scene, globalPosition, mesh, vertexArray, pose);
		}
	}

	meshRenderer->drawMesh(scene, globalPosition, mesh, vertexArray);

	delete [] vertexArray;
}

void fk::fbx::Renderer::readVertexCacheData(KFbxMesh* mesh, KTime& time, KFbxVector4* vertexArray)
{
	KFbxVertexCacheDeformer* deformer     = static_cast<KFbxVertexCacheDeformer*>(mesh->GetDeformer(0, KFbxDeformer::eVERTEX_CACHE));
	KFbxCache*               cache        = deformer->GetCache();
	int                      channelIndex = -1;
	unsigned int             vertexCount  = (unsigned int)mesh->GetControlPointsCount();
	bool                     readSucceed  = false;
	double*                  readBuf      = new double[3*vertexCount];

	if (cache->GetCacheFileFormat() == KFbxCache::eMC) {
		if ((channelIndex = cache->GetChannelIndex(deformer->GetCacheChannel())) > -1) {
			readSucceed = cache->Read(channelIndex, time, readBuf, vertexCount);
		}

    // ePC2
	} else {
		readSucceed = cache->Read((unsigned int)time.GetFrame(true), readBuf, vertexCount);
	}

	if (readSucceed) {
		unsigned int readBufIndex = 0;
		while (readBufIndex < 3*vertexCount)
		{
			// In statements like "pVertexArray[lReadBufIndex/3].SetAt(2, lReadBuf[lReadBufIndex++])", 
			// on Mac platform, "lReadBufIndex++" is evaluated before "lReadBufIndex/3". 
			// So separate them.
			vertexArray[readBufIndex/3].SetAt(0, readBuf[readBufIndex]); readBufIndex++;
			vertexArray[readBufIndex/3].SetAt(1, readBuf[readBufIndex]); readBufIndex++;
			vertexArray[readBufIndex/3].SetAt(2, readBuf[readBufIndex]); readBufIndex++;
		}
	}
	delete [] readBuf;
}

void fk::fbx::Renderer::computeClusterDeformation(Scene* scene,
												  KFbxXMatrix& globalPosition, 
												  KFbxMesh* mesh, 
												  KFbxVector4* vertexArray,
												  KFbxPose* pose)
{
	KTime& time = scene->currentTime;

	// All the links must have the same link mode.
	KFbxCluster::ELinkMode clusterMode = ((KFbxSkin*)mesh->GetDeformer(0, KFbxDeformer::eSKIN))->GetCluster(0)->GetLinkMode();

	int i, j;
	int clusterCount=0;

	int vertexCount = mesh->GetControlPointsCount();
	int skinCount = mesh->GetDeformerCount(KFbxDeformer::eSKIN);

	KFbxXMatrix* clusterDeformation = new KFbxXMatrix[vertexCount];
	memset(clusterDeformation, 0, vertexCount * sizeof(KFbxXMatrix));
	double* clusterWeight = new double[vertexCount];
	memset(clusterWeight, 0, vertexCount * sizeof(double));

	if (clusterMode == KFbxCluster::eADDITIVE) {
		for (i = 0; i < vertexCount; i++) {
			clusterDeformation[i].SetIdentity();
		}
	}

	for ( i=0; i<skinCount; ++i) {
		clusterCount =( (KFbxSkin *)mesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetClusterCount();

		for (j=0; j<clusterCount; ++j) {
			KFbxCluster* cluster =((KFbxSkin *) mesh->GetDeformer(i, KFbxDeformer::eSKIN))->GetCluster(j);
			if (!cluster->GetLink())
				continue;
			KFbxXMatrix lReferenceGlobalInitPosition;
			KFbxXMatrix referenceGlobalCurrentPosition;
			KFbxXMatrix lClusterGlobalInitPosition;
			KFbxXMatrix clusterGlobalCurrentPosition;
			KFbxXMatrix referenceGeometry;
			KFbxXMatrix lClusterGeometry;

			KFbxXMatrix clusterRelativeInitPosition;
			KFbxXMatrix clusterRelativeCurrentPositionInverse;
			KFbxXMatrix vertexTransformMatrix;

			if (clusterMode == KFbxLink::eADDITIVE && cluster->GetAssociateModel()) {
				cluster->GetTransformAssociateModelMatrix(lReferenceGlobalInitPosition);
				referenceGlobalCurrentPosition = scene->getGlobalPosition(cluster->GetAssociateModel(), time, pose);
				// Geometric transform of the model
				referenceGeometry = scene->getGeometry(cluster->GetAssociateModel());
				referenceGlobalCurrentPosition *= referenceGeometry;

			} else {
				cluster->GetTransformMatrix(lReferenceGlobalInitPosition);
				referenceGlobalCurrentPosition = globalPosition;
				// Multiply lReferenceGlobalInitPosition by Geometric Transformation
				referenceGeometry = scene->getGeometry(mesh->GetNode());
				lReferenceGlobalInitPosition *= referenceGeometry;
			}
			// Get the link initial global position and the link current global position.
			cluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);
			clusterGlobalCurrentPosition = scene->getGlobalPosition(cluster->GetLink(), time, pose);

			// Compute the initial position of the link relative to the reference.
			clusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition;

			// Compute the current position of the link relative to the reference.
			clusterRelativeCurrentPositionInverse = referenceGlobalCurrentPosition.Inverse() * clusterGlobalCurrentPosition;

			// Compute the shift of the link relative to the reference.
			vertexTransformMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;

			int k;
			int lVertexIndexCount = cluster->GetControlPointIndicesCount();

			for (k = 0; k < lVertexIndexCount; ++k) {            
				int index = cluster->GetControlPointIndices()[k];

				// Sometimes, the mesh can have less points than at the time of the skinning
				// because a smooth operator was active when skinning but has been deactivated during export.
				if (index >= vertexCount)
					continue;

				double weight = cluster->GetControlPointWeights()[k];

				if (weight == 0.0) {
					continue;
				}

				// Compute the influence of the link on the vertex.
				KFbxXMatrix influence = vertexTransformMatrix;
				MatrixScale(influence, weight);

				if (clusterMode == KFbxCluster::eADDITIVE) {   
					// Multiply with to the product of the deformations on the vertex.
					MatrixAddToDiagonal(influence, 1.0 - weight);
					clusterDeformation[index] = influence * clusterDeformation[index];

					// Set the link to 1.0 just to know this vertex is influenced by a link.
					clusterWeight[index] = 1.0;

				// lLinkMode == KFbxLink::eNORMALIZE || lLinkMode == KFbxLink::eTOTAL1
				} else {
					// Add to the sum of the deformations on the vertex.
					MatrixAdd(clusterDeformation[index], influence);

					// Add to the sum of weights to either normalize or complete the vertex.
					clusterWeight[index] += weight;
				}

			}
		}
	}

	for (i = 0; i < vertexCount; i++) {
		KFbxVector4 srcVertex = vertexArray[i];
		KFbxVector4& dstVertex = vertexArray[i];
		double weight = clusterWeight[i];

		// Deform the vertex if there was at least a link with an influence on the vertex,
		if (weight != 0.0) 
		{
			dstVertex = clusterDeformation[i].MultT(srcVertex);

			if (clusterMode == KFbxCluster::eNORMALIZE) {
				// In the normalized link mode, a vertex is always totally influenced by the links. 
				dstVertex /= weight;

			} else if (clusterMode == KFbxCluster::eTOTAL1) {
				// In the total 1 link mode, a vertex can be partially influenced by the links. 
				srcVertex *= (1.0 - weight);
				dstVertex += srcVertex;
			}
		} 
	}

	delete [] clusterDeformation;
	delete [] clusterWeight;
}

void fk::fbx::Renderer::computeShapeDeformation(Scene* scene, KFbxNode* node, KFbxMesh* mesh, KFbxVector4* vertexArray)
{
	int i, j;
	int shapeCount = mesh->GetShapeCount();
	int vertexCount = mesh->GetControlPointsCount();

	KFbxVector4* srcVertexArray = vertexArray;
	KFbxVector4* dstVertexArray = new KFbxVector4[vertexCount];
	memcpy(dstVertexArray, vertexArray, vertexCount * sizeof(KFbxVector4));

	for (i = 0; i < shapeCount; i++) {
		KFbxShape* shape = mesh->GetShape(i);

		// Get the percentage of influence of the shape.
		KFbxAnimCurve* fCurve = mesh->GetShapeChannel(i, scene->currentAnimationLayer);
		if (!fCurve) continue;
		double weight = fCurve->Evaluate(scene->currentTime) / 100.0;

		for (j = 0; j < vertexCount; j++) {
			// Add the influence of the shape vertex to the mesh vertex.
			KFbxVector4 influence = (shape->GetControlPoints()[j] - srcVertexArray[j]) * weight;
			dstVertexArray[j] += influence;
		}
	}

	memcpy(vertexArray, dstVertexArray, vertexCount * sizeof(KFbxVector4));
	delete [] dstVertexArray;
}


// -- Light ---------------------------------------------------------------------
void fk::fbx::Renderer::drawLight(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition)
{
	if(lightRenderer == NULL) return;
	
	KFbxAnimLayer* animLayer = scene->currentAnimationLayer;
	KTime& time = scene->currentTime;

	KFbxLight* light = (KFbxLight*) node->GetNodeAttribute();
	KFbxXMatrix lightRotation, lightGlobalPosition;
	KFbxColor color(1,1,1);
	double coneAngle = 0.0;

	// Must rotate the light's global position because 
	// FBX lights point towards the Y negative axis.
	KFbxVector4 lYNegativeAxis(-90.0, 0.0, 0.0);
	lightRotation.SetR(lYNegativeAxis);
	lightGlobalPosition = globalPosition * lightRotation;

	// Get the light color.
	KFbxAnimCurve* fc;

	fc = light->Color.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_COLOR_RED);
	if (fc) color.mRed = fc->Evaluate(time);
	fc = light->Color.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_COLOR_GREEN);
	if (fc) color.mGreen = fc->Evaluate(time);
	fc = light->Color.GetCurve<KFbxAnimCurve>(animLayer, KFCURVENODE_COLOR_BLUE);
	if (fc) color.mBlue = fc->Evaluate(time);

	// The cone angle is only relevant if the light is a spot.
	if (light->LightType.Get() == KFbxLight::eSPOT) {
		fc = light->ConeAngle.GetCurve<KFbxAnimCurve>(animLayer);
		if (fc) coneAngle = fc->Evaluate(time);
	}

	// Draw the light cone
	if (light->LightType.Get() == KFbxLight::eSPOT) {
		lightRenderer->drawSpotLight(color, lightGlobalPosition, coneAngle);
	} else {
		lightRenderer->drawLight(color, lightGlobalPosition);
	}
}


// -- Camera ---------------------------------------------------------------------
void fk::fbx::Renderer::drawCamera(Scene* scene, KFbxNode* node, KFbxXMatrix& globalPosition)
{
	if(cameraRenderer == NULL) return;

	KFbxAnimLayer* animLayer = scene->currentAnimationLayer;
	KTime& time = scene->currentTime;

	KFbxXMatrix cameraGlobalPosition;
	KFbxVector4 cameraPosition, cameraDefaultDirection, cameraInterestPosition;

	cameraPosition = globalPosition.GetT();

	// By default, FBX cameras point towards the X positive axis.
	KFbxVector4 xPositiveAxis(1.0, 0.0, 0.0);
	cameraDefaultDirection = cameraPosition + xPositiveAxis;

	cameraGlobalPosition = globalPosition;

	// If the camera is linked to an interest, get the interest position.
	if (node->GetTarget()) {
		cameraInterestPosition = scene->getGlobalPosition(node->GetTarget(), time).GetT();

		// Compute the required rotation to make the camera point to it's interest.
		KFbxVector4 cameraDirection;
		KFbxVector4::AxisAlignmentInEulerAngle(cameraPosition, 
			cameraDefaultDirection, 
			cameraInterestPosition, 
			cameraDirection);

		// Must override the camera rotation 
		// to make it point to it's interest.
		cameraGlobalPosition.SetR(cameraDirection);
	}

	// Get the camera roll.
	KFbxCamera* cam = node->GetCamera();
	double roll = 0;

	if (cam) {
		roll = cam->Roll.Get();
		KFbxAnimCurve* fc = cam->Roll.GetCurve<KFbxAnimCurve>(animLayer);
		if (fc) fc->Evaluate(time);
	}

	cameraRenderer->drawCamera(cameraGlobalPosition, roll);
}


// -- NULL ---------------------------------------------------------------------
void fk::fbx::Renderer::drawNull(KFbxXMatrix& globalPosition)
{
	if(nullRenderer == NULL) return;
	nullRenderer->drawNull(globalPosition);
}
