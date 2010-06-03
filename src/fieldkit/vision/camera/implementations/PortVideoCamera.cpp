/*
 *  PortVideoCamera.cpp
 *  FieldVision
 *
 *  Created by Marcus Wendt on 07/11/2009.
 *  Copyright 2009 FIELD. All rights reserved.
 *
 */

#include "fieldkit/vision/camera/implementations/PortVideoCamera.h"
#include "fieldkit/vision/camera/implementations/portvideo/CinderCamera.h"

namespace fk { namespace vision 
{
	// -------------------------------------------------------------------------
	// INIT
	// -------------------------------------------------------------------------
	int PortVideoCamera::init()
	{
		if(isStarted) {
			LOG_ERR("PortVideoCamera: Cannot initialize, since camera is already started.");
			return ERROR;
		}
		
		// TODO black&white mode gives strange artifacts, so force color mode
		color = true;
		
		capture = new CinderCamera();
		capture->findCamera();
		capture->initCamera(width, height, color);
		capture->startCamera();
		
		int channels = color ? 3 : 1;
		image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, channels);
		
		return super::init();
	}
	
	// -------------------------------------------------------------------------
	// CLOSE
	// -------------------------------------------------------------------------
	int PortVideoCamera::close()
	{
		capture->closeCamera();
		return super::close();
	}
	
	// -------------------------------------------------------------------------
	// UPDATE
	// -------------------------------------------------------------------------
	int PortVideoCamera::update()
	{
		int stride = color ? width * 3: width;
		cvSetData(image, capture->getFrame(), stride);
		return SUCCESS;
	}
	
	// -------------------------------------------------------------------------
	// HELPERS
	// -------------------------------------------------------------------------
#pragma mark -- Helpers --
	IplImage* PortVideoCamera::getImage(int channel)
	{
		return image;
	}
} } // namespace fk::vision