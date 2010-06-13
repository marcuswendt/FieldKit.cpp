/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef GESTUREEVENT_DISPATCHER_H
#define GESTUREEVENT_DISPATCHER_H

#include "fieldkit/vision/vision/processors/Gesture.h"
#include "fieldkit/vision/vision/processors/GestureEvent.h"
#include "fieldkit/vision/vision/processors/GestureEventListener.h"
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace fk;
using namespace fk::vision;

namespace fk { namespace vision { 

class GestureEventDispatcher
{
public:
	GestureEventDispatcher(){};
	
	void addListener(GestureEventListener *listener)
	{
		listeners.push_back(listener);
	};

	void dispatchEventActive(Gesture *gesture)
	{
		GestureEvent evt(gesture);
		BOOST_FOREACH(GestureEventListener *listener,listeners)
		{
			listener->onGestureActive(evt);
		}
	};
	void dispatchEventInActive(Gesture *gesture)
	{
		GestureEvent evt(gesture);
		BOOST_FOREACH(GestureEventListener *listener,listeners)
		{
			listener->onGestureInActive(evt);
		}
	};
	void dispatchEventUpdate(Gesture *gesture)
	{
		GestureEvent evt(gesture);
		BOOST_FOREACH(GestureEventListener *listener,listeners)
		{
			listener->onGestureUpdate(evt);
		}
	};

protected:
	vector<GestureEventListener *> listeners;
};

} } // namespace fk::vision
#endif