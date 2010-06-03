/*
 *  PortVideoCamera.h
 *  FieldVision
 *
 *  Created by Marcus Wendt on 07/11/2009.
 *  Copyright 2009 FIELD. All rights reserved.
 *
 */

#ifndef PORT_VIDEO_CAMERA_H
#define PORT_VIDEO_CAMERA_H

#include "fieldkit/vision/camera/Camera.h"
#include "fieldkit/vision/camera/implementations/portvideo/cameraEngine.h"
#include "fieldkit/vision/opencv.h"

namespace fk { namespace vision 
{
	//
	// integrates the camera capture components from the port video project
	// http://portvideo.sourceforge.net
	//
	class PortVideoCamera : public Camera
	{
	public:
		PortVideoCamera() {};
		~PortVideoCamera() {};
		
		int init();
		int update();
		int close();
		IplImage* getImage(int channel=0);
		
	private:
		typedef Camera super;
		cameraEngine* capture;
		IplImage* image;
	};
} } // namespace fk::vision

#endif