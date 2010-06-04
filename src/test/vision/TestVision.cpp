#include "cinder/app/AppBasic.h"
#include <list>

#include "opencv/cv.h"
#include "fieldkit/vision/camera/implementations/CinderCamera.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"
#include "fieldkit/vision/vision/Vision.h"
#include "fieldkit/vision/FieldVision.h"
#include "fieldkit/vision/vision/processors/CVBlobDetector.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fk;
using namespace fk::vision;

// We'll create a new Cinder Application by deriving from the AppBasic class
class TestVision : public AppBasic {
 public:
	static const int SCREEN_WIDTH = 1400;
	
	void mouseDrag( MouseEvent event );
	void draw();
	void setup();
	void update();
	void keyDown(KeyEvent e);
	
	gl::Texture	mTexture;
	CVBlobDetector *proc;
	Vision *v;
	char *windows[CVBlobDetector::STAGE_MAX];
	bool updateImages;
	bool resetBackground;
};

void TestVision::setup()
{
	updateImages =false;
	
	proc = new CVBlobDetector();
//	proc->setSize(320, 240);
	proc->setSize(640, 480);

	proc->setROI(180, 90, 320, 240);
//	proc->setROI(320, 240, 320, 240); // works
//	proc->setROI(120, 80, 400, 260); // works
	
	proc->setStageEnabled(true);
	
	//LOG("** init vision **");	
	v = new Vision();
	v->setCamera(new CinderCamera());
	v->setProcessor(proc);
	v->init();
	v->getCamera()->setFramerate(24);
	
	//LOG("** set sliders **");
	// sliders must be set after Vision was initialized
	
	proc->set(CVBlobDetector::SLIDER_BACKGROUND, 0.25f);
	proc->set(CVBlobDetector::SLIDER_THRESHOLD, 0.175f);
	proc->set(CVBlobDetector::SLIDER_DILATE, 0.15f);
	proc->set(CVBlobDetector::SLIDER_ERODE, 0.025f);
	proc->set(CVBlobDetector::SLIDER_CONTOUR_MIN, 0.001f);
	proc->set(CVBlobDetector::SLIDER_CONTOUR_MAX, 1.0f);
	proc->set(CVBlobDetector::SLIDER_CONTOUR_REDUCE, 0.25f);
	proc->set(CVBlobDetector::SLIDER_TRACK_RANGE, 0.75f);

	/*
	proc->setWarp(0, 0,
				  320, 0,
				  320, 239,
				  0, 240);	
	*/
	
	//LOG("** init gui **");
	CvSize windowSize = cvSize(320,240);/// cvSize(v->getProcessor()->getROI().width, v->getProcessor()->getROI().height);
	if(windowSize.height > 240) windowSize.height = 240;
	int tmpX = 0;
	int tmpY = 45;
	
	
	for(int i=0; i<CVBlobDetector::STAGE_MAX; i++) {
		char *windowName = (char *) malloc(50);
		sprintf(windowName, "STAGE %i", i);
		windows[i] = windowName;
		
		cvNamedWindow(windowName, NULL); //CV_WINDOW_AUTOSIZE);
		cvResizeWindow(windowName, windowSize.width, windowSize.height);
		
		if(tmpX + windowSize.width > SCREEN_WIDTH) {
			tmpY += windowSize.height + 23;
			tmpX = 0;
		}
		
		//console() << "moving window " << tmpX <<" " << tmpY  << std::endl;
		//printf("moving window %i to %i | %i \n", i, tmpX, tmpY);
		cvMoveWindow(windowName, tmpX, tmpY);		
		tmpX += windowSize.width;		
	}
	
	// start vision
	//LOG("** starting ** ");	
	v->start();
	
	proc->setSize(640, 480);	
	updateImages = true;
	resetBackground = true;

	/*
	//LOG("** stopping **");	
	for(int i=0; i<CVBlobDetector::STAGE_MAX; i++) {
		cvDestroyWindow(windows[i]);
	}*/
	
	//v->stop();	
	//delete v;
	
}

void TestVision::keyDown( KeyEvent event )
{
	if( event.getChar() == ' ' )
		fvStop();
		//camera.stop();
}

void TestVision::mouseDrag( MouseEvent event )
{
	
}


void TestVision::update()
{
	v->update();
		
	if(updateImages) {
		for(int i=0; i<CVBlobDetector::STAGE_MAX; i++) {
			IplImage *image = v->getProcessor()->getImage(i);
			if(image!=NULL) {
				console() << "[TestVision::update]" << i << std::endl;
				cvShowImage(windows[i], image);
			}
		}
	}
		/*
		// pause thread
        int key = cvWaitKey(10);
        if(key == 'q' || key == 'Q')
            break;
		
		if(key == ' ' || key == 'u')
			updateImages = !updateImages;

		if(key == 'r') {
			proc->setROI(120, 80, 400, 260);
		}
		
		if(key == 'b') {
			float bgVal = resetBackground ? 1.0f : 0.0f;
			resetBackground = !resetBackground;
			proc->set(CVBlobDetector::SLIDER_BACKGROUND, bgVal);
		}
*/
	
}

void TestVision::draw()
{
	if( mTexture )
		gl::draw( mTexture, getWindowBounds() );

}

// This line tells Cinder to actually create the application
//CINDER_APP_BASIC( TestVision, RendererGl )
