/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#pragma once

#include "fieldkit/vision/VisionKit.h"
#include "fieldkit/vision/camera/Camera.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace fieldkit { namespace vision 
{
	
	class CinderCamera : public Camera
	{
	public:
		CinderCamera() {};
		~CinderCamera() {};
		
		int init();
		int update();
		int close();
		int stop();
		IplImage* getImage(int channel=0);
		
		ci::Capture *getCapture(){return &mCapture;};
	private:
		bool initialised;
		typedef Camera super;
		IplImage* image;
		ci::Capture	mCapture;
	};
} } // namespace fieldkit::vision
