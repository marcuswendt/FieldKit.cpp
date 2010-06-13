/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef GESTURETRACKER_H
#define GESTURETRACKER_H
#include "fieldkit/vision/opencv.h"
#include "fieldkit/vision/vision/processors/Gesture.h"
#include "fieldkit/vision/vision/processors/GestureEventDispatcher.h"
#include "fieldkit/vision/constants.h"

using namespace fk;
using namespace fk::vision;

namespace fk { namespace vision { 

class GestureTracker : public GestureEventDispatcher
{
public:
	static const int MAX_GESTURES = 100;
	GestureTracker();
	void init();
	void beginUpdate();  
	Gesture *addFoundPoint(float x, float y);
	void endUpdate();
	void setTrackingRange(float range);
	void setFrameSize(float x, float y, float w, float h);

	Gesture *getTrackedGesture(int i){return &tracked[i];};
	
protected:	
	Gesture *getFreeTrackedGesture();
	
	Gesture found[MAX_GESTURES];
	Gesture tracked[MAX_GESTURES];
	int nfound;
	float trackingRange;
	float framex, framey, framew, frameh;
	float oneOverFrameh, oneOverFramew;
};

} } // namespace fk::vision
#endif