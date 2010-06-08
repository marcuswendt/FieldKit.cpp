/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#ifndef CINDER_VIDEO_FILE_CAPTURE_H
#define CINDER_VIDEO_FILE_CAPTURE_H

#include "fieldkit/vision/camera/Camera.h"
#include "fieldkit/vision/opencv.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;
using namespace ci::qtime;
using namespace std;

namespace fk { namespace vision 
{

	class CinderVideoFileCapture : public Camera
	{
	public:
		CinderVideoFileCapture(string filepath);
		CinderVideoFileCapture();
		~CinderVideoFileCapture() {};
		// set before init
		void setVideoPath(string filepath);
		void setVideoPathFromFileWindow();
		
		int init();
		int start();
		int stop();
		int close();
		int update();
		
		IplImage* getImage(int channel=0);
		
	private:
		void loadVideo();

		IplImage*		image;
		string			filepath;
		bool			ready;
		bool			filepathSet;
		bool			moviesurfaceCreated;
		MovieSurface	moviesurface;
	};
} } // namespace fk::vision

#endif