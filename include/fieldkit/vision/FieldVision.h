/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef FIELD_VISION_H
#define FIELD_VISION_H

#include "fieldkit/vision/vision/Vision.h"

//
// Defines a C function interface to the C++ vision and camera classes
//

#ifdef __cplusplus
extern "C" {
#endif
	
namespace fk { namespace vision {

#define VISION_DATA_SIZE				10000
	
#define VISION_DATA_BLOB				-1000
#define VISION_DATA_BLOB_BOUNDS			-1001
#define VISION_DATA_BLOB_CONTOURS		-1002
	
// global variables
struct VisionData {
	int size;
	int index;
	int isLittleEndian;
	int *buffer;
};

// functions
int fvCreate();
int fvDestroy();

int fvStart();
int fvStop();
int fvUpdate();
	
// setters
int fvSet(int property, float value);
int fvSetCamera(int name);
int fvSetSize(int width, int height);
int fvSetFramerate(int fps);
int fvSetStageEnabled(int stage, bool enabled);
int fvSetWarp(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
int fvSetUseContours(bool isEnabled);
	
// getters
float fvGet(int property);
	
Vision* fvGetVision();
int fvGetBlobCount();
	
int* fvGetBlobData();
int fvGetBlobDataLength();

char* fvGetStageImage(int stage);
int fvGetStageSize(int stage);
int fvGetStageWidth(int stage);
int fvGetStageHeight(int stage);
int fvGetStageDepth(int stage);
	
// helpers
int fvError(int err);
inline void fvPushData(int value);

} } // namespace fk::vision

#ifdef __cplusplus
}
#endif

#endif