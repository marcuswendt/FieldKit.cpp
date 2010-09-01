/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#pragma once

#include "fieldkit/vision/VisionKit.h"
#include "fieldkit/vision/camera/Camera.h"

namespace fieldkit { namespace vision 
{
	//
	// integrates the capture components from opencv
	// http://opencv.sourceforge.net
	//
	class OpenCVCamera : public Camera
	{
	public:
		OpenCVCamera(int cameraIndex);
		~OpenCVCamera() {};
		
		int init();
		int update();
		int close();
		IplImage* getImage(int channel=0);
		
	private:
		typedef Camera super;
		
		int cameraIndex;
		CvCapture* capture;
	};
} } // namespace fieldkit::vision
