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
#include "fieldkit/physics/behaviour/Random.h"
#include "fieldkit/physics/behaviour/Attractor.h"
#include "fieldkit/physics/behaviour/Boundary.h"
#include "fieldkit/physics/behaviour/Force.h"
#include "fieldkit/physics/behaviour/Flocking.h"
#include "fieldkit/physics/space/BasicSpace.h"
#include "fieldkit/physics/Emitter.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fieldkit::physics;

class FlockingParticlesApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void update();
	void draw();
	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	
	void testParticleCreation();
	
	Timer* timer;
	Physics* physics;
	AttractorPoint* attractor;
	
	gl::VboMesh vboParticles;
};

void FlockingParticlesApp::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
	
};

// -- Init ---------------------------------------------------------------------
void FlockingParticlesApp::setup() {
	timer = new Timer();
	
	// init physics
	BasicSpace* space = new BasicSpace(Vec3f::zero(), Vec3f(getWindowWidth(), getWindowHeight(), 0));
	printf("init space %f %f %f\n", space->getWidth(), space->getHeight(), space->getDepth());
	
	physics = new Physics(space);
	
	Emitter* emitter = new Emitter(physics);
	physics->emitter = emitter;
	emitter->setPosition(space->getCenter());
	emitter->setInterval(0.01);
	emitter->setRate(1000);
	emitter->setMax(1 * 1000);
	
	emitter->addBehaviour(new BoxRandom(*space));
	physics->addBehaviour(new Gravity(true));
	
	// wrap
	BoxWrap* wrap = new BoxWrap(*space);
	wrap->preserveMomentum = false;
	physics->addBehaviour(wrap);
	
	// attractor 
	attractor = new AttractorPoint(space);
	attractor->setRange(0.25);
	attractor->setWeight(0);
	physics->addBehaviour(attractor);
	
	// flocking
	FlockAlign* align = new FlockAlign(space);
	align->setRange(0.25);
	physics->addBehaviour(align);
	
	FlockAttract* attract = new FlockAttract(space);
	attract->setRange(0.25);
	physics->addBehaviour(attract);
	
	FlockRepel* repel = new FlockRepel(space);
	repel->setRange(0.25);
	physics->addBehaviour(repel);
	
	// init graphics
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setDynamicPositions();
	layout.setStaticTexCoords2d();
	
	vboParticles = gl::VboMesh(emitter->getMax(), 0, layout, GL_POINTS);
	
	//testParticleCreation();
}

void FlockingParticlesApp::testParticleCreation() {
	
	printf("running testParticleCreation\n");
	timer->stop();
	timer->start();
	
	BasicSpace* space = new BasicSpace(Vec3f::zero(), Vec3f(getWindowWidth(), getWindowHeight(), 0));
	Physics* physics = new Physics(space);
	
	Emitter* emitter = new Emitter(physics);
	physics->emitter = emitter;
	emitter->setPosition(space->getCenter());
	emitter->setInterval(0.0001);
	emitter->setRate(50000);
	emitter->setMax(50 * 1000);
	
	int numIterations = 10;
	for(int i=0; i<numIterations; i++) {
		physics->update(0.016f);
		//physics->createParticle();
	}
	
	timer->stop();
	printf("%f s \n", timer->getSeconds());
}

void FlockingParticlesApp::update() {
	
	// update physics
	timer->stop();
	double dt = timer->getSeconds();
	timer->start();
	
	float mouseX = (float)getMousePos().x;
	float mouseY = (float)getMousePos().y;
	attractor->setPosition(Vec3f(mouseX, mouseY, 0));
	
	physics->update(dt);
	
	// copy particle positions into vbo
	gl::VboMesh::VertexIter iter = vboParticles.mapVertexBuffer();
	for(vector<Particle*>::iterator p = physics->particles.begin(); p != physics->particles.end(); p++) {
		if(!(*p)->isAlive) continue;
		iter.setPosition((*p)->position.x, (*p)->position.y, (*p)->position.z);
		++iter;
	}
}

void FlockingParticlesApp::draw() {
	ci::gl::clear(Color(0, 0, 0));
	
	glColor3f(1,1,1);
	glPointSize(3);
	
	// draw all particles at once using vbo
	drawArrays(vboParticles, 0, physics->getNumParticles());
}



// -- Events -------------------------------------------------------------------
void FlockingParticlesApp::mouseDown(MouseEvent event) {
	attractor->setWeight(1);
}

void FlockingParticlesApp::mouseUp(MouseEvent event) {
	attractor->setWeight(0);
}

void FlockingParticlesApp::mouseDrag(MouseEvent event) {
}

void FlockingParticlesApp::keyDown(KeyEvent event) {
}

CINDER_APP_BASIC(FlockingParticlesApp, RendererGl)
