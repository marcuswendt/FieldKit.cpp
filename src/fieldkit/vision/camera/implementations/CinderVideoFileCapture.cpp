/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#include "fieldkit/vision/camera/implementations/CinderVideoFileCapture.h"

namespace fk { namespace vision 
{

CinderVideoFileCapture::CinderVideoFileCapture()
{
	filepathSet = false;
	moviesurfaceCreated = false;
	ready = false;
}

CinderVideoFileCapture::CinderVideoFileCapture(string filepath)
{
	setVideoPath(filepath);
	CinderVideoFileCapture();
}

void CinderVideoFileCapture::setVideoPath(string filepath)
{
	filepathSet = true;
	this->filepath = filepath;
	loadVideo();
}

void CinderVideoFileCapture::setVideoPathFromFileWindow()
{
	filepath = getOpenFilePath();
	if(! filepath.empty() ) {
		filepathSet = true;
		loadVideo();
	}
}

void CinderVideoFileCapture::loadVideo()
{
	if(filepathSet){
		moviesurfaceCreated = true;
		moviesurface = qtime::MovieSurface(filepath);
		this->width = moviesurface.getWidth();
		this->height = moviesurface.getHeight();
		image = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
	}
}

int CinderVideoFileCapture::init()
{	
	try {
		
	}
	catch( ... ) {
	}
	return Camera::init();
}

int CinderVideoFileCapture::start()
{
	if(moviesurfaceCreated) moviesurface.play();
	return Camera::start();
}

int CinderVideoFileCapture::stop()
{
	if(moviesurfaceCreated) moviesurface.stop();
	return Camera::stop();
}

int CinderVideoFileCapture::close()
{
	return Camera::close();
}
	
int CinderVideoFileCapture::update()
{
	if(!ready)
	{
		// check if moviesurface is ready
		if(moviesurfaceCreated && moviesurface.checkPlayable() && moviesurface.getCurrentTime() > 0 )
		{
			// surface is now accessible so set the rowwidth
			image->widthStep = moviesurface.getSurface().getRowBytes();
			ready = true;
		}
	}
	if(ready)
	{
		image->imageData = (char *)moviesurface.getSurface().getData();
	}
	return (ready) ? 1:0;
}

IplImage* CinderVideoFileCapture::getImage(int channel)
{
	return image;
}

} } // namespace fk::vision