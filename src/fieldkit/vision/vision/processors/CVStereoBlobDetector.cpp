/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "fieldkit/vision/vision/processors/CVStereoBlobDetector.h"

namespace fk { namespace vision 
{
	// ---------------------------------------------------------------------------------
	//#pragma mark -- Lifecycle --
	// ---------------------------------------------------------------------------------
	CVStereoBlobDetector::CVStereoBlobDetector()
	{
		initStages(STAGE_MAX);
		
		// init blobs
		blobNum = VISION_BLOB_COUNT;
		blobs = new Blob*[blobNum];
		for(int i=0; i < blobNum; i++) {
			blobs[i] = new Blob(i);
		}
		
		left = new CVBlobDetector();		
		right = new CVBlobDetector();
		
		left->setCameraSource(0);
		right->setCameraSource(1);
	}
	
	CVStereoBlobDetector::~CVStereoBlobDetector()
	{
		delete left;
		delete right;
	}

	int CVStereoBlobDetector::init()
	{
		left->init();
		right->init();
		
		return SUCCESS;
	}	
	
	int CVStereoBlobDetector::update(Camera *camera)
	{
		// update processors
		left->update(camera);
		right->update(camera);

		findFingerPoints();
		
		drawFingerPoints();
		
		return SUCCESS;
	}

	
	// ------------------------------------------------------------------------------------------------
	//#pragma mark -- Finger Points --
	// ------------------------------------------------------------------------------------------------
	void CVStereoBlobDetector::findFingerPoints()
	{
		CvPoint2D64f pointLeft, pointRight, pointFinger;
		
		for(int i=0; i<blobNum; i++) {
			Blob *blobLeft = left->getBlobs()[i];
			Blob *blobRight = right->getBlobs()[i];
			Blob *blob = blobs[i];
			
			if(!blobLeft->isActive || !blobRight->isActive) {
				blob->isActive = false;
				
			} else {
				blob->isActive = true;
				
				findFurthestPoint(&pointLeft, blobLeft);
				findFurthestPoint(&pointRight, blobRight);
				
				pointFinger.x = pointLeft.x + (pointRight.x - pointLeft.x) / 2.0;
				pointFinger.y = pointLeft.y + (pointRight.y - pointLeft.y) / 2.0;
			
//				pointFinger.x = pointLeft.x;
//				pointFinger.y = pointLeft.y;
				
				blob->position = cvPoint(pointFinger.x, pointFinger.y);
				blob->position64f = pointFinger;
			}
		}
	}
	
	void CVStereoBlobDetector::findFurthestPoint(CvPoint2D64f *result, Blob *blob)
	{
		CvPoint2D64f p;
		CvPoint2D64f center = cvPoint2D64f(roi.width / 2.0, roi.height / 2.0);
		double d, distance = 0;

		for(int j=0; j < blob->contour->total; j++) {
			CvPoint *tmp = ((CvPoint*) cvGetSeqElem(blob->contour, j));
			p.x = (double)tmp->x;
			p.y = (double)tmp->y;
			
			d = pt64fDistanceSquared(p, center);
			
			if(d > distance) {
				distance = d;
				result->x = p.x;
				result->y = p.y;
			}
		}
	}
	
	void CVStereoBlobDetector::drawFingerPoints()
	{
		//IplImage *combinedImage = getImage(CVBlobDetector::STAGE_DIFFERENCE);
		//cvZero(combinedImage);		
		
		IplImage *combinedImage = getImage(CVBlobDetector::STAGE_CONTOUR);
		
		for(int i=0; i<blobNum; i++) {
			Blob *blob = blobs[i];
			if(!blob->isActive) continue;
			cvCircle(combinedImage, blob->position, 3, cWhite, 2, CV_AA);
		}
		
		copyStage(STAGE_COMBINED, combinedImage);
	}
	
	
	// ------------------------------------------------------------------------------------------------
	//#pragma mark -- Overrides --
	// ------------------------------------------------------------------------------------------------
	void CVStereoBlobDetector::set(int slider, float value)
	{
		left->set(slider, value);
		right->set(slider, value);
	}
	
	void CVStereoBlobDetector::setStageEnabled(bool enabled, int key)
	{
		CVFrameProcessor::setStageEnabled(enabled, key);
		
		left->setStageEnabled(enabled, key);
		right->setStageEnabled(enabled, key);
	}
	
	
	// ------------------------------------------------------------------------------------------------
	//#pragma mark -- setters --
	void CVStereoBlobDetector::setSize(int width, int height)
	{
		size = cvSize(width, height);
		left->setSize(width, height);
		right->setSize(width, height);
	}
	
	void CVStereoBlobDetector::setROI(int x, int y, int w, int h)
	{
		CVFrameProcessor::setROI(x,y,w,h);
		
		left->setROI(x,y,w,h);
		right->setROI(x,y,w,h);
	}
	
	void CVStereoBlobDetector::setWarp(float sx1, float sy1,
										 float sx2, float sy2,
										 float sx3, float sy3,
										 float sx4, float sy4)
	{
		left->setWarp(sx1,sy1,sx2,sy2,sx3,sy3,sx4,sy4);
		right->setWarp(sx1,sy1,sx2,sy2,sx3,sy3,sx4,sy4);
	}
	
	
	// ------------------------------------------------------------------------------------------------
	//#pragma mark -- Getters --
	// ------------------------------------------------------------------------------------------------
	IplImage* CVStereoBlobDetector::getImage(int key)
	{
		if(key < STAGE_COMBINED) {
			IplImage *leftImage = left->getImage(key);
			IplImage *rightImage = right->getImage(key);
			
			if(leftImage == NULL || rightImage == NULL) return NULL;

			CvSize stageSize = cvSize(leftImage->width, leftImage->height);
			int keyOffset = (key == 0) ? 0 : 1;
			IplImage *blueImage = cache->getTmp(IMAGE_BLUE + keyOffset, stageSize);
			IplImage *composedImage = cache->getTmp(IMAGE_COMPOSED + keyOffset, stageSize, IPL_DEPTH_8U, 3);

	//		printf("leftImage %i %i \n", leftImage->width, leftImage->height);
	//		printf("rightImage %i %i \n", rightImage->width, rightImage->height);
	//		printf("blueImage %i %i \n", blueImage->width, blueImage->height);
	//		printf("composedImage %i %i \n", composedImage->width, composedImage->height);
			
			cvMerge(blueImage, rightImage, leftImage, NULL, composedImage);
			return composedImage;
			
		} else {
			return CVFrameProcessor::getImage(key);
		}
	}
	
	Blob** CVStereoBlobDetector::getBlobs() 
	{
		return blobs;
	}
	
	int CVStereoBlobDetector::getBlobCount()
	{
		return blobNum;
	}
} } // namespace fk::vision
