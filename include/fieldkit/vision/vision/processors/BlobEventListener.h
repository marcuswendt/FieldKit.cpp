/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef BLOBEVENT_LISTENER_H
#define BLOBEVENT_LISTENER_H

#include "fieldkit/vision/vision/processors/Blob.h"
#include "fieldkit/vision/vision/processors/BlobEvent.h"
using namespace fieldkit;
using namespace fieldkit::vision;

namespace fieldkit { namespace vision { 

class BlobEventListener
{
public:
	BlobEventListener(){};
	virtual void onBlobActive(BlobEvent &blobevent){};
	virtual void onBlobInActive(BlobEvent &blobevent){};
	virtual void onBlobUpdate(BlobEvent &blobevent){};
};

} } // namespace fieldkit::vision
#endif