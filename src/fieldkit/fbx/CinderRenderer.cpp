/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 15/06/2010.
 */

#include "fieldkit/fbx/CinderRenderer.h"

#include "cinder/Camera.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace fieldkit::fbx;

CinderRenderer::CinderRenderer()
{
	markerSize = 2.0f;
	markerColor = ColorA(1,1,1,1);
	markerWidth = 1.0;

	skeletonColor = ColorA(1,0,0,1);
	skeletonWidth = 2.0f;

	nullSize = 5.0;
	nullColor = ColorA(1,1,1,1);

	meshDrawMode = DRAW_MODE_WIREFRAME;
	meshWireframeWidth = 1.0f;
	meshColor = ColorA(1,1,1,1);
}


// -- Perspective --------------------------------------------------------------
void CinderRenderer::setCameraPerspective(double fovY, double aspect, 
										  double nearPlane, double farPlane, 
										  KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up)
{
	CameraPersp	cam = CameraPersp(ci::app::getWindowWidth(), ci::app::getWindowHeight(), (float)fovY);

	cam.setNearClip((float)nearPlane);
	cam.setFarClip((float)farPlane);
	cam.setEyePoint(KFbxVector4ToVec3f(eye));
	cam.setCenterOfInterestPoint(KFbxVector4ToVec3f(center));
	cam.setWorldUp(KFbxVector4ToVec3f(up));

	gl::setProjection(cam);
	gl::setModelView(cam);
}

void CinderRenderer::getWindowSize(int& width, int& height)
{
	width = app::getWindowWidth();
	height = app::getWindowHeight();
}

void CinderRenderer::setCameraOrthogonal(double leftPlane, double rightPlane, 
										 double bottomPlane, double topPlane,
										 double nearPlane, double farPlane, 
										 KFbxVector4& eye, KFbxVector4& center, KFbxVector4& up)
{
	CameraOrtho	cam = CameraOrtho((float)leftPlane, (float)rightPlane, 
		(float)bottomPlane, (float)topPlane, 
		(float)nearPlane, (float)farPlane);

	cam.setEyePoint(KFbxVector4ToVec3f(eye));
	cam.setCenterOfInterestPoint(KFbxVector4ToVec3f(center));
	cam.setWorldUp(KFbxVector4ToVec3f(center));

	gl::setProjection(cam);
	gl::setModelView(cam);
}


// -- Marker --------------------------------------------------------------------
void CinderRenderer::drawMarker(KFbxXMatrix& position)
{
	gl::color(markerColor);
	glLineWidth(markerWidth);

	glPushMatrix();
	glMultMatrixd((double*) position);
	gl::drawCube(Vec3f(), Vec3f(markerSize, markerSize, markerSize));
	glPopMatrix();
}


// -- Skeleton -------------------------------------------------------------------
void CinderRenderer::drawLimb(KFbxXMatrix& parentGlobalPosition, KFbxXMatrix& globalPosition)
{
	gl::color(skeletonColor);
	glLineWidth(skeletonWidth);

	glBegin(GL_LINES);
	glVertex3dv((GLdouble *)parentGlobalPosition.GetT());
	glVertex3dv((GLdouble *)globalPosition.GetT());
	glEnd();
}


// -- Lights --------------------------------------------------------------------
void CinderRenderer::drawSpotLight(KFbxColor& color, KFbxXMatrix& position, double coneAngle)
{
	glPushMatrix();
	glMultMatrixd((double*) position);
	gl::color(Color((float)color.mRed, (float)color.mGreen, (float)color.mBlue));
	
	float radians = (float)(3.1416 * coneAngle / 180.0);
	float height = 15.0f;
	float base = height * tan(radians / 2.0f);
	//glutWireCone(base, height, 18, 1);
	gl::drawCube(Vec3f(), Vec3f(base, base, height));

	glPopMatrix();
}

void CinderRenderer::drawLight(KFbxColor& color, KFbxXMatrix& position)
{
	glPushMatrix();
	glMultMatrixd((double*) position);
	gl::color(Color((float)color.mRed, (float)color.mGreen, (float)color.mBlue));
	gl::drawSphere(Vec3f(), 10.0f);
	glPopMatrix();
}

// -- NULL ----------------------------------------------------------------------
void CinderRenderer::drawNull(KFbxXMatrix& position)
{
	float s = nullSize * 0.5f;
	glPushMatrix();
	glMultMatrixd((double*) position);
	gl::color(nullColor);
	gl::drawLine(Vec3f(-s,0,0), Vec3f(s,0,0));
	gl::drawLine(Vec3f(0,-s,0), Vec3f(0,s,0));
	gl::drawLine(Vec3f(0,0,-s), Vec3f(0,0,s));
	glPopMatrix();
}

// -- Camera -------------------------------------------------------------------
void CinderRenderer::drawCamera(KFbxXMatrix& position, double roll)
{
	// TODO
}

void CinderRenderer::drawMesh(Scene* scene, KFbxXMatrix& globalPosition, KFbxMesh* mesh, KFbxVector4* vertexArray)
{
	int drawMode = 
		(meshDrawMode == DRAW_MODE_TEXTURED && 
		 mesh->GetTextureUVCount() == 0 && 
		 mesh->GetLayer(0)) ? DRAW_MODE_WIREFRAME : meshDrawMode;

	KFbxLayerElementArrayTemplate<KFbxVector2>* uvArray = NULL;    
	mesh->GetTextureUV(&uvArray, KFbxLayerElement::eDIFFUSE_TEXTURES); 

	KFbxLayerElement::EMappingMode mappingMode = KFbxLayerElement::eNONE;
	fbx::Texture* texture     = NULL;

	if(mesh->GetLayer(0) && mesh->GetLayer(0)->GetUVs())
		mappingMode = mesh->GetLayer(0)->GetUVs()->GetMappingMode();

	// Find the texture data
	if (drawMode == DRAW_MODE_TEXTURED) {
		KFbxTexture* currentTexture           = NULL;
		KFbxLayerElementTexture* textureLayer = NULL;
		KFbxSurfaceMaterial* surfaceMaterial= KFbxCast <KFbxSurfaceMaterial>(mesh->GetNode()->GetSrcObject(KFbxSurfaceMaterial::ClassId, 0));

		if(surfaceMaterial) {
			KFbxProperty lProperty;
			lProperty = surfaceMaterial->FindProperty(KFbxSurfaceMaterial::sDiffuse);
			if(lProperty.IsValid())
			{   
				currentTexture = KFbxCast <KFbxTexture>(lProperty.GetSrcObject(KFbxTexture::ClassId, 0)); 

				KArrayTemplate<Texture*> textures = scene->getTextures();
				int i, lCount = textures.GetCount();
				for (i=0; i<lCount; i++) {
					if(textures[i]->refTexture == currentTexture) {
						texture = textures[i];
						break;
					}
				}
			}
		}
	}

	drawMode = (drawMode == DRAW_MODE_TEXTURED && texture) ? drawMode : DRAW_MODE_WIREFRAME;

	int glPrimitive = drawMode == DRAW_MODE_WIREFRAME ? GL_LINE_LOOP : GL_POLYGON;

	glColor3f(meshColor);

	glPushMatrix();
	glMultMatrixd((double*) globalPosition);

	int polygonIndex;
	int polygonCount = mesh->GetPolygonCount();

	if (drawMode == DRAW_MODE_TEXTURED) {
		glEnable(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP );
		glTexParameteri( GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP );
		glTexEnvi( GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);

		// TODO
		logger() << "Warning! CinderRenderer::drawMesh DRAW_MODE_TEXTURED not properly implemented yet \n";
//		glTexImage2D(GL_TEXTURE_2D, 0,  3, lTexture->mW, lTexture->mH, 0,  GL_BGR_EXT,  GL_UNSIGNED_BYTE,  lTexture->mImageData);

	} else if(drawMode == DRAW_MODE_WIREFRAME) {
		glLineWidth(meshWireframeWidth);
	}

	for (polygonIndex = 0; polygonIndex < polygonCount; polygonIndex++) {
		int verticeIndex;
		int verticeCount = mesh->GetPolygonSize(polygonIndex);

		glBegin(glPrimitive);

		for (verticeIndex = 0; verticeIndex < verticeCount; verticeIndex++) {
			if (drawMode == DRAW_MODE_TEXTURED) {
				int currentUVIndex;

				if (mappingMode == KFbxLayerElement::eBY_POLYGON_VERTEX) {
					currentUVIndex = mesh->GetTextureUVIndex(polygonIndex, verticeIndex);

				} else {
					// KFbxLayerElement::eBY_CONTROL_POINT 
					currentUVIndex = mesh->GetPolygonVertex(polygonIndex, verticeIndex);
				}
				if(uvArray)
					glTexCoord2dv(uvArray->GetAt(currentUVIndex).mData);
			}

			glVertex3dv((GLdouble *)vertexArray[mesh->GetPolygonVertex(polygonIndex, verticeIndex)]);
		}
		glEnd();
	}

	if (drawMode == DRAW_MODE_TEXTURED)
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}
