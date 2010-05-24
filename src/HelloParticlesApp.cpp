/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *   HelloParticles.cpp
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "cinder/app/AppBasic.h"
#include <list>
#include "Physics.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fk::physics;

class HelloParticlesApp : public AppBasic {
 public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	
	Timer* timer;
	Physics* physics;
};

void HelloParticlesApp::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
	
};

// -- Loop ---------------------------------------------------------------------
void HelloParticlesApp::setup() {
	timer = new Timer();
	physics = new Physics();
	physics->space->setWidth(getWindowWidth());
	physics->space->setHeight(getWindowHeight());
	physics->space->setDepth(getWindowHeight());
}

void HelloParticlesApp::update() {
	timer->stop();
	double dt = timer->getSeconds();
	timer->start();
	
	//printf("dt %f \n", dt);
	
	physics->update(dt);
}

void HelloParticlesApp::draw() {
	ci::gl::clear(Color(0, 0, 0));	
}



// -- Events -------------------------------------------------------------------
void HelloParticlesApp::mouseDown(MouseEvent event) {
}

void HelloParticlesApp::mouseUp(MouseEvent event) {
}

void HelloParticlesApp::mouseDrag(MouseEvent event) {
}

void HelloParticlesApp::keyDown(KeyEvent event) {
}

CINDER_APP_BASIC(HelloParticlesApp, RendererGl)
