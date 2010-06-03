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
#include "fieldkit/vision/opencv.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

namespace fk { namespace vision 
{
	//
	// integrates the camera capture components from the port video project
	// http://portvideo.sourceforge.net
	//
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
		typedef Camera super;
		IplImage* image;
		ci::Capture	mCapture;
	};
} } // namespace fk::vision

#endif