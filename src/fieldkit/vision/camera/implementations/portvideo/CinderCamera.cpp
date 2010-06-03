#include "fieldkit/vision/camera/implementations/portvideo/CinderCamera.h"

namespace fk { namespace vision{

CinderCamera::CinderCamera()
{
	cameraID = -1;
}

CinderCamera::~CinderCamera()
{
}

bool CinderCamera::findCamera() 
{
	int num_components = mCapture.getDevices().size();  
	
	if(num_components==0) {
		// console() << "[CinderCamera::findCamera] no camera found \n";
		return false;
	} else {
		cameraID = 0;
		return true;
	}
}

bool CinderCamera::initCamera(int width, int height, bool colour) 
{
	if(cameraID < 0) return false;
	this->colour = colour;
	mCapture = ci::Capture( width, height );
	return true;
}

unsigned char* CinderCamera::getFrame()
{
	return mCapture.getSurface().getData();
}

bool CinderCamera::startCamera()
{
	mCapture.start();
	return true;
}

bool CinderCamera::stopCamera()
{
	mCapture.stop();
	return true;
}

bool CinderCamera::stillRunning() 
{
	return mCapture.isCapturing();
}

bool CinderCamera::resetCamera()
{
  return (stopCamera() && startCamera());
}

bool CinderCamera::closeCamera()
{
	return stopCamera();
}

void CinderCamera::showSettingsDialog() 
{
	//printf("not yet implemented\n");
}

} } // namespace fk::vision