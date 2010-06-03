/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef PTGREY_BUMBLEBEE2_H
#define PTGREY_BUMBLEBEE2_H

#include <dc1394/dc1394.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include "fieldkit/vision/camera/Camera.h"

namespace fk { namespace vision 
{
	const int bb2Width = 640;
	const int bb2Height = 480;
	const int bb2DefaultFPS = 48;
	
	const int bb2ImageSize = bb2Width * bb2Height;
	const int bb2PacketSize = 80;
	
	enum Feature {
		FEATURE_BRIGHTNESS = DC1394_FEATURE_BRIGHTNESS, 
		FEATURE_EXPOSURE = DC1394_FEATURE_EXPOSURE, 
		FEATURE_SHARPNESS = DC1394_FEATURE_SHARPNESS,
		FEATURE_GAMMA = DC1394_FEATURE_GAMMA,
		FEATURE_SHUTTER = DC1394_FEATURE_SHUTTER, 
		FEATURE_GAIN = DC1394_FEATURE_GAIN
	};
	
	enum Mode { 
		MODE_MANUAL = DC1394_FEATURE_MODE_MANUAL,
		MODE_AUTO = DC1394_FEATURE_MODE_AUTO,
		MODE_ONE_PUSH_AUTO = DC1394_FEATURE_MODE_ONE_PUSH_AUTO
	};
	
	//
	// implements a 'Bumblebee2' stereo firewire camera by Point Grey Research
	// www.ptgrey.com
	//
	class PTGreyBumblebee2 : public Camera
	{
	public:
		PTGreyBumblebee2();
		~PTGreyBumblebee2() {};
		
		int init();
		int start();
		int update();
		int stop();
		int close();
		IplImage* getImage(int channel = 0);
		
	private:
		dc1394_t *dc1394;
		dc1394camera_t *camera;
		dc1394video_frame_t *frame;
		dc1394error_t err;
		
		IplImage* bufferLeft;
		IplImage* bufferRight;
		IplImage* bufferDeinterlace;
		
		int setFramerate(int framerate);		
		int setMode(Feature feature, Mode mode);
		int setValue(Feature feature, float normalizedValue);
		float getValue(Feature feature);		
	};
} } // namespace fk::vision
#endif