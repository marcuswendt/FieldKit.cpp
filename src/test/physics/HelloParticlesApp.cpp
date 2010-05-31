/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Vbo.h"
#include <list>

#include "fieldkit/physics/Physics.h"
#include "fieldkit/physics/behaviours/Attractor.h"
#include "fieldkit/physics/behaviours/Boundary.h"
#include "fieldkit/physics/behaviours/Force.h"

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
	AttractorPoint* attractor;
	gl::VboMesh vboParticles;
};

void HelloParticlesApp::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
	
};

// -- Loop ---------------------------------------------------------------------
class RandomEmitter : public Behaviour {
public:
	RandomEmitter(Space* space) : Behaviour(space) {};
	
	void apply(ParticlePtr p) {
		p->position.x = Rand::randFloat(space->min.x, space->max.x);
		p->position.y = Rand::randFloat(space->min.y, space->max.y);
		p->position.z = Rand::randFloat(space->min.z, space->max.z);
		
		p->lifeTime = Rand::randFloat(10, 100);
		//printf("random emit: %f %f %f\n", p->x, p->y, p->z);
		p->clearVelocity();
	}		
};

void HelloParticlesApp::setup() {
	timer = new Timer();
	
	// init physics
	Space* space = new Space(getWindowWidth(), getWindowHeight(), 0);
	printf("init space %f %f %f\n", space->getWidth(), space->getHeight(), space->getDepth());
	
	physics = new Physics(space);
	
	Emitter* emitter = new Emitter(physics);
	physics->emitter = emitter;
	emitter->setPosition(space->getCenter());
	emitter->setInterval(0.01);
	emitter->setRate(1000);
	emitter->setMax(50 * 1000);
	
	emitter->addBehaviour(new RandomEmitter(space));
	physics->addBehaviour(new Gravity());
	
	// wrap
	BoxWrap* wrap = new BoxWrap(*space);
	wrap->preserveMomentum = false;
	physics->addBehaviour(wrap);
	
	// attractor 
	attractor = new AttractorPoint(space);
	attractor->setRange(0.25);
	attractor->setWeight(0);
	physics->addBehaviour(attractor);
	
	// init graphics
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setDynamicPositions();
	layout.setStaticTexCoords2d();
	
	vboParticles = gl::VboMesh(emitter->getMax(), 0, layout, GL_POINTS);
}

void HelloParticlesApp::update() {
	
	// update physics
	timer->stop();
	double dt = timer->getSeconds();
	timer->start();

	float mouseX = getMousePos().x;
	float mouseY = getMousePos().y;
	attractor->setPosition(Vec3f(mouseX, mouseY, 0));
	
	physics->update(dt);

	// copy particle positions into vbo
	gl::VboMesh::VertexIter iter = vboParticles.mapVertexBuffer();
	for(ParticlePtr p = physics->particles.begin(); p != physics->particles.end(); p++) {
		if(!p->isAlive) continue;
		iter.setPosition(p->position.x, p->position.y, p->position.z);
		++iter;
	}
}

void HelloParticlesApp::draw() {
	ci::gl::clear(Color(0, 0, 0));
	
	glColor3f(1,1,1);
	glPointSize(3);
	
	// draw all particles at once using vbo
	drawArrays(vboParticles, 0, physics->getNumParticles());
}



// -- Events -------------------------------------------------------------------
void HelloParticlesApp::mouseDown(MouseEvent event) {
	attractor->setWeight(1);
}

void HelloParticlesApp::mouseUp(MouseEvent event) {
	attractor->setWeight(0);
}

void HelloParticlesApp::mouseDrag(MouseEvent event) {
}

void HelloParticlesApp::keyDown(KeyEvent event) {
}

CINDER_APP_BASIC(HelloParticlesApp, RendererGl)
