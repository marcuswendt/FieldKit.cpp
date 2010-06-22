/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef BLOBEVENT_DISPATCHER_H
#define BLOBEVENT_DISPATCHER_H

#include "fieldkit/vision/vision/processors/Blob.h"
#include "fieldkit/vision/vision/processors/BlobEvent.h"
#include "fieldkit/vision/vision/processors/BlobEventListener.h"
#include <vector>
#include <boost/foreach.hpp>

using namespace std;
using namespace fieldkit;
using namespace fieldkit::vision;

namespace fieldkit { namespace vision { 

class BlobEventDispatcher
{
public:
	BlobEventDispatcher(){};
	
	void addListener(BlobEventListener *listener)
	{
		listeners.push_back(listener);
	};

	void dispatchEventActive(Blob *blob)
	{
		BlobEvent evt(blob);
		BOOST_FOREACH(BlobEventListener *listener,listeners)
		{
			listener->onBlobActive(evt);
		}
	};
	void dispatchEventInActive(Blob *blob)
	{
		BlobEvent evt(blob);
		BOOST_FOREACH(BlobEventListener *listener,listeners)
		{
			listener->onBlobInActive(evt);
		}
	};
	void dispatchEventUpdate(Blob *blob)
	{
		BlobEvent evt(blob);
		BOOST_FOREACH(BlobEventListener *listener,listeners)
		{
			listener->onBlobUpdate(evt);
		}
	};

protected:
	vector<BlobEventListener *> listeners;
};

} } // namespace fieldkit::vision
#endif