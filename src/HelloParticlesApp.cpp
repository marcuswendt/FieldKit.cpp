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
#include "cinder/Rand.h"
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
class RandomEmitter : public Behaviour {
public:
	RandomEmitter(Space* space) : Behaviour(space) {};
	
	void apply(Particle* p) {
		p->position.x = Rand::randFloat(space->min.x, space->max.x);
		p->position.y = Rand::randFloat(space->min.y, space->max.y);
		p->position.z = Rand::randFloat(space->min.z, space->max.z);
		p->clearVelocity();
	}		
};

class Gravity : public Behaviour {
	void apply(Particle* p) {
		p->force.y += 0.1;
	}		
};

class Wrap : public Behaviour {
public:
	Wrap(Space* space) : Behaviour(space) {};
	
	void apply(Particle* p) {
		if(p->position.y > space->max.y) {
			p->position.y = 0;
			p->clearVelocity();
		}
		//p->force.y += 0.1;
	}		
};

void HelloParticlesApp::setup() {
	timer = new Timer();
	
	Space* space = new Space(getWindowWidth(), getWindowHeight(), getWindowHeight());
	//printf("Space %c", space->toString());
	
	physics = new Physics(space);
	
	Emitter* emitter = new Emitter(physics);
	physics->emitter = emitter;
	emitter->position = space->center();
	emitter->rate = 10.0;
	emitter->interval = 0.1;
	emitter->max = 10000;
	
	emitter->addBehaviour(new RandomEmitter(space));
	physics->addBehaviour(new Gravity());
	physics->addBehaviour(new Wrap(space));
}

void HelloParticlesApp::update() {
	timer->stop();
	double dt = timer->getSeconds();
	timer->start();
	
	//printf("dt %f \n", dt);
	
	physics->update(dt);
	
	printf("particles %i \n", physics->numParticles());
}

void HelloParticlesApp::draw() {
	ci::gl::clear(Color(0, 0, 0));
	
	glColor3f(1,1,1);
	glPointSize(5);
	glBegin(GL_POINTS);
	BOOST_FOREACH(Particle* p, physics->particles) {
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
