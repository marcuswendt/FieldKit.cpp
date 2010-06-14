/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef CINDER_CAMERA_H
#define CINDER_CAMERA_H

#include "fieldkit/vision/camera/Camera.h"
#include "fieldkit/vision/opencv.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace fk { namespace vision 
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
} } // namespace fk::vision

#endif