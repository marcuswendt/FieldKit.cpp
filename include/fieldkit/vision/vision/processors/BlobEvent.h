/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef BLOBEVENT_H
#define BLOBEVENT_H

#include "fieldkit/vision/vision/processors/Blob.h"
using namespace fieldkit;
using namespace fieldkit::vision;

namespace fieldkit { namespace vision { 

class BlobEvent
{
public:
	enum 
	{
		ON_CREATED,
		ON_ENDED,
		ON_MOVE
	};
	
	BlobEvent(Blob *blob)
	{
		this->blob = blob;
	};

	Blob *getBlob()
	{
		return blob;
	};

	void setBlob(Blob *blob)
	{
		this->blob = blob;
	};

	BlobEvent &operator=(BlobEvent &blobevent )
	{
		this->blob = blobevent.getBlob();
		return *this;
	};

protected:	
	Blob *blob;
};

} } // namespace fieldkit::vision
#endif