/*
 *  PortVideoCamera.cpp
 *  FieldVision
 *
 *  Created by Marcus Wendt on 07/11/2009.
 *  Copyright 2009 FIELD. All rights reserved.
 *
 */

#include "fieldkit/vision/camera/implementations/CinderCamera.h"
#include "cinder/ip/grayscale.h"

namespace fk { namespace vision 
{
	// -------------------------------------------------------------------------
	// INIT
	// -------------------------------------------------------------------------
	int CinderCamera::init()
	{
		if(isStarted) {
			LOG_ERR("PortVideoCamera: Cannot initialize, since camera is already started.");
			return VISION_ERROR;
		}
		
		// TODO black&white mode gives strange artifacts, so force color mode
		color = true;
		
		try {
			mCapture = ci::Capture( width, height );
			mCapture.start();
		}
		catch( ... ) {
			//console() << "[PortVideoCamera::init] Failed to initialize capture" << std::endl;
		}
		int channels = color ? 3 : 1;
		image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, channels);
		
		return super::init();
	}
	

	int CinderCamera::stop()
	{
		mCapture.stop();
		return super::stop();
	}

	// -------------------------------------------------------------------------
	// CLOSE
	// -------------------------------------------------------------------------
	int CinderCamera::close()
	{
		mCapture.stop();
		return super::close();
	}
	
	// -------------------------------------------------------------------------
	// UPDATE
	// -------------------------------------------------------------------------
	int CinderCamera::update()
	{
		int stride = color ? width * 3: width;
		cvSetData(image, mCapture.getSurface().getData(), stride);
		return VISION_SUCCESS;
	}
	
	// -------------------------------------------------------------------------
	// HELPERS
	// -------------------------------------------------------------------------
#pragma mark -- Helpers --
	IplImage* CinderCamera::getImage(int channel)
	{
		return image;
	}
} } // namespace fk::vision