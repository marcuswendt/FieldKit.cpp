/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef GESTUREEVENT_H
#define GESTUREEVENT_H

#include "fieldkit/vision/vision/processors/Gesture.h"
using namespace fieldkit;
using namespace fieldkit::vision;

namespace fieldkit { namespace vision { 

class GestureEvent
{
public:
	enum 
	{
		ON_CREATED,
		ON_ENDED,
		ON_MOVE
	};
	
	GestureEvent(Gesture *gesture)
	{
		this->gesture = gesture;
	};

	Gesture *getGesture()
	{
		return gesture;
	};

	void setGesture(Gesture *gesture)
	{
		this->gesture = gesture;
	};

	GestureEvent &operator=(GestureEvent &gestureEvent )
	{
		this->gesture = gestureEvent.getGesture();
		return *this;
	};

protected:	
	Gesture *gesture;
};

} } // namespace fieldkit::vision
#endif