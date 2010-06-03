#ifndef CinderCamera_H
#define CinderCamera_H

#include "cinder/Capture.h"
#include "fieldkit/vision/camera/implementations/portvideo/cameraEngine.h"

namespace fk { namespace vision{

class CinderCamera : public cameraEngine
{
public:
	CinderCamera();
	~CinderCamera();
	
	bool findCamera();
	bool initCamera(int width, int height, bool colour);
	bool startCamera();
	unsigned char* getFrame();
	bool stopCamera();
	bool stillRunning();
	bool resetCamera();
	bool closeCamera();
	
	void showSettingsDialog();

protected:
	ci::Capture	mCapture;

};

} } // namespace fk::vision

#endif
