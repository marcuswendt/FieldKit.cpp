/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#include "FieldVision.h"

#include "CVBlobDetector.h"
#include "OpenCVCamera.h"
#include "PTGreyBumblebee2.h"
#include "PortVideoCamera.h"

#ifdef __cplusplus
extern "C" {
#endif

// -- Globals ------------------------------------------------------------------
struct VisionData blobdata;
VisionData* visionData = &blobdata;
bool useContours = true;
	
// global vision pointer
Vision* vision;
	
// -- Create -------------------------------------------------------------------
int fvCreate() {
	if(vision != NULL) fvDestroy();
	
	// init vision main class
	vision = new Vision();
	vision->setProcessor(new CVBlobDetector());
	
	// init data structure
	visionData->size = VISION_DATA_SIZE;
	visionData->isLittleEndian = isLittleEndian();
	visionData->buffer = new int[visionData->size];
	return SUCCESS;
}
	
// -- Destroy ------------------------------------------------------------------
int fvDestroy() {
	if(!vision) return fvError(ERR_NOT_CREATED);
	delete vision;
	return SUCCESS;
}

// -- Start --------------------------------------------------------------------
int fvStart() {
	if(!vision) return fvError(ERR_NOT_CREATED);
	return vision->start();
}
	
// -- Stop ---------------------------------------------------------------------
int fvStop() {
	if(!vision) return fvError(ERR_NOT_CREATED);
	return vision->stop();
}

// -- Update -------------------------------------------------------------------
int fvUpdate() {
	if(!vision) return fvError(ERR_NOT_CREATED);
	
	// update vision and data
	if(vision->update()) {
		
		// clean up data
		visionData->index = 0;
		for(int i=0; i<visionData->size; i++)
			visionData->buffer[i] = 0;
		
		// update with new values	
		CVBlobDetector* proc = (CVBlobDetector*) vision->getProcessor();
				
		for (int i=0; i < proc->getBlobCount(); i++) {
			Blob *blob = proc->getBlobs()[i];

			// skip if this blob is inactive
			if(!blob->isActive) continue;
			
			fvPushData(VISION_DATA_BLOB);

			// basic blob information
			fvPushData(blob->id);
			fvPushData(blob->position.x);
			fvPushData(blob->position.y);
			
			// bounding box
			fvPushData(VISION_DATA_BLOB_BOUNDS);
			fvPushData(blob->bounds.x);
			fvPushData(blob->bounds.y);
			fvPushData(blob->bounds.width);
			fvPushData(blob->bounds.height);

			// contour points
			if(!useContours) continue;
			
			fvPushData(VISION_DATA_BLOB_CONTOURS);
			int contourPoints = blob->contour->total;
			fvPushData(contourPoints);			
			for(int j=0; j<contourPoints; j++) {
				CvPoint* pt = (CvPoint*)cvGetSeqElem(blob->contour, j);
				fvPushData(pt->x);
				fvPushData(pt->y);
			}
		}

//		for(int i=0; i < visionData->index; i++)
//			printf("c>> %i: %i \n", i, visionData->buffer[i]);
		
		return SUCCESS;
	}
	return ERROR;
}

	
// -- Setters ------------------------------------------------------------------
int fvSet(int property, float value) {
	if(!vision) return fvError(ERR_NOT_CREATED);
	vision->getProcessor()->set(property, value);
	return SUCCESS;
}
	
int fvSetCamera(int name) {
	if(!vision) return fvError(ERR_NOT_CREATED);
	
	Camera* camera;
	
	switch (name) {
		case CAMERA_OPENCV:
			camera = new OpenCVCamera(CV_CAP_ANY);
			break;
			
		case CAMERA_OPENCV_FIRST:
			camera = new OpenCVCamera(0);
			break;
			
		case CAMERA_OPENCV_SECOND:
			camera = new OpenCVCamera(1);
			break;
			
		case CAMERA_OPENCV_THIRD:
			camera = new OpenCVCamera(2);
			break;
			
		case CAMERA_OPENCV_FOURTH:
			camera = new OpenCVCamera(3);
			break;
			
		case CAMERA_PTGREY_BUMBLEBEE:
			camera = new PTGreyBumblebee2();
			break;
			
		case CAMERA_PORT_VIDEO:
			camera = new PortVideoCamera();
			break;
			
		default:
			return fvError(ERR_INVALID_ARGUMENT);
	}
	
	vision->setCamera(camera);
	
	return SUCCESS;
}

int fvSetSize(int width, int height) {
	if(!vision) return fvError(ERR_NOT_CREATED);
	vision->setSize(width, height);
	return SUCCESS;
}

int fvSetFramerate(int fps) {
	if(!vision) return fvError(ERR_NOT_CREATED);	
	vision->setFramerate(fps);
	return SUCCESS;
}

int fvSetStageEnabled(int stage, bool enabled) {
	if(!vision) return fvError(ERR_NOT_CREATED);	
	vision->getProcessor()->setStageEnabled(enabled, stage);
	return SUCCESS;
}

int fvSetWarp(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	if(!vision) return fvError(ERR_NOT_CREATED);	
	vision->getProcessor()->setWarp(x1, y1, x2, y2, x3, y3, x4, y4);
	return SUCCESS;
}
	
int fvSetUseContours(bool isEnabled) {
	useContours = isEnabled;
	return SUCCESS;
}
	
// -- Getters ------------------------------------------------------------------
float fvGet(int property) {
	if(!vision) return fvError(ERR_NOT_CREATED);
	return vision->getProcessor()->getProperty(property)->getValue();
}
	
Vision* fvGetVision() { return vision; }

int fvGetBlobCount() { return VISION_BLOB_COUNT; }
		
int* fvGetBlobData() { return visionData->buffer; }

int fvGetBlobDataLength() { return visionData->index; }

	
int fvGetStageWidth(int stage) {
	IplImage *image = vision->getProcessor()->getImage(stage);
	if(image == NULL) return 0;
	return image->width;
}	

int fvGetStageHeight(int stage) {
	IplImage *image = vision->getProcessor()->getImage(stage);
	if(image == NULL) return 0;
	return image->height;
}	
	
int fvGetStageDepth(int stage) {
	IplImage *image = vision->getProcessor()->getImage(stage);
	if(image == NULL) return 0;
	return image->depth;
}	
	
int fvGetStageSize(int stage) {
	IplImage *image = vision->getProcessor()->getImage(stage);
	if(image == NULL) return 0;
	return image->imageSize;
}	
	
char* fvGetStageImage(int stage) {
	IplImage *image = vision->getProcessor()->getImage(stage);
	if(image == NULL) return NULL;
	return image->imageData;
}
	
// -- Helpers ------------------------------------------------------------------
inline void fvPushData(int value) {
	if(visionData->index >= visionData->size) return;	
	//if(visionData->isLittleEndian) value = little2bigEndianINT(value);
	visionData->buffer[visionData->index++] = value;
}
	
int fvError(int err) {
	const char* msg;
	if(err == ERR_NOT_CREATED) {
		msg = "No vision object created yet";
	} else {
		msg = "Unknown error";
	}
	fprintf(stderr, "FieldVision: %s!\n", msg);
	return ERROR;
}
	
#ifdef __cplusplus
}
#endif
