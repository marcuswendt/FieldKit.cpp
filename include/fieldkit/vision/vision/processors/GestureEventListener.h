/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef GESTUREEVENT_LISTENER_H
#define GESTUREEVENT_LISTENER_H

#include "fieldkit/vision/vision/processors/Gesture.h"
#include "fieldkit/vision/vision/processors/GestureEvent.h"
using namespace fk;
using namespace fk::vision;

namespace fk { namespace vision { 

class GestureEventListener
{
public:
	GestureEventListener(){};
	virtual void onGestureActive(GestureEvent evt){};
	virtual void onGestureInActive(GestureEvent evt){};
	virtual void onGestureUpdate(GestureEvent evt){};
};

} } // namespace fk::vision
#endif