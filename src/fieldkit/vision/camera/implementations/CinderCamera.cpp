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
#include "cinder/app/App.h"
namespace fieldkit { namespace vision 
{
	

	// -------------------------------------------------------------------------
	// INIT
	// -------------------------------------------------------------------------
	int CinderCamera::init()
	{
		if(isStarted) {
			//LOG_ERR("PortVideoCamera: Cannot initialize, since camera is already started.");
			return VISION_ERROR;
		}
		
		// TODO black&white mode gives strange artifacts, so force color mode
		color = true;
		
		try {
			width = 640;
			height = 480;
			mCapture = ci::Capture( width, height );
			mCapture.start();
			isInitialized = true;
		}
		catch( ... ) {
			cinder::app::App::get()->console() << "[CinderCamera::init] Failed to initialize capture" << std::endl;
			isInitialized = false;		
		}

		cinder::app::App::get()->console() << "[CinderCamera::init] success" << std::endl;
		int channels = color ? 3 : 1;
		image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, channels);
		
		return isInitialized;
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
		if(!isInitialized) return 0;
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
} } // namespace fieldkit::vision