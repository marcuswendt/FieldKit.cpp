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
using namespace ci;
using namespace ci::app;

#include <list>
using namespace std;

#include "fieldkit/physics/Physics.h"
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
class Gravity : public Behaviour {
	void apply(Particle* p) {
		p->force.y += 0.1f;
	}		
};

void HelloParticlesApp::setup() {
	timer = new Timer();
	
	Space* space = new Space((float)getWindowWidth(), (float)getWindowHeight(), (float)getWindowHeight());
	//printf("Space %c", space->toString());
	
	physics = new Physics(space);
	
	Emitter* emitter = new Emitter(physics);
	physics->emitter = emitter;
	emitter->position = space->center();
	emitter->rate = 1;
	emitter->interval = 1;
	emitter->max = 10000;
	
	emitter->addBehaviour(new Gravity());
}

void HelloParticlesApp::update() {
	timer->stop();
	float dt = (float)timer->getSeconds();
	timer->start();
	
	//printf("dt %f \n", dt);
	
	physics->update(dt);
	
	printf("particles %i \n", physics->numParticles());
}

void HelloParticlesApp::draw() {
	ci::gl::clear(Color(0, 0, 0));
	
	glColor4f(1,1,1,1);
	glBegin(GL_POINTS);
	BOOST_FOREACH(Particle* p, physics->particles) {
		glPointSize(p->size * 3);
		glVertex3f(p->position.x, p->position.y, p->position.z);
	}
	glEnd();
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
