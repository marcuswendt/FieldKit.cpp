/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2010.
 */

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Vbo.h"
#include <list>

#include "fieldkit/physics/Physics.h"
#include "fieldkit/physics/behaviours/Attractor.h"
#include "fieldkit/physics/behaviours/Boundary.h"
#include "fieldkit/physics/behaviours/Force.h"
#include "fieldkit/physics/behaviours/Random.h"

#include "fieldkit/tweak/Model.h"
#include "fieldkit/tweak/Tweak.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fk::physics;
using namespace fk::tweak;

class HelloTweakApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void initPhysics();
	void initTweaks();
	void initGraphics();
	
	void update();
	void draw();
	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	
	// tweaks
	//TweakManager* tm;
	
	// physics
	Timer* timer;
	Physics* physics;
	
	BoxRandom* boxRandom;
	AttractorPoint* attractor;
	
	gl::VboMesh vboParticles;
};

void HelloTweakApp::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
	
};

// -- Init ---------------------------------------------------------------------
void HelloTweakApp::setup() {
	initPhysics();
	initTweaks();
	initGraphics();
}

void HelloTweakApp::initPhysics() {
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
	emitter->setMax(1000);
	
	boxRandom = new BoxRandom(*space);
	emitter->addBehaviour(boxRandom);
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
}

// -- Tweaks -------------------------------------------------------------------
class EmitterModel : public Model {
	void setup() {
		setName("Emitter");
		add(new Vec3fTweak("position"));
		add(new FloatTweak("rate"));
		add(new IntTweak("rate"));
		add(new IntTweak("max"));
	}
};

class BehaviourModel : public Model {
	void setup() {
		setName("Behaviours");
	}
};

class PhysicsModel : public Model {
	void setup() {
		setName("Physics");
		add(new EmitterModel());
		add(new BehaviourModel());
	}	
};

class HelloTweakAppRoot : public Model {
public:
	void setup() {
		setName("HelloTweakApp");
		add(new PhysicsModel());
	}
};

void HelloTweakApp::initTweaks() {
	
	HelloTweakAppRoot* root = new HelloTweakAppRoot();
	root->init();
	
	//tm = new TweakManager();
	
//	tweak = new TweakManager();
//	
//	// Physics
//	Group* gPhysics = tweak->add("Physics");
//	
//	// Emitter
//	Emitter* emitter = physics->emitter;
//	Group* gEmitter = gPhysics->add("Emitter");
//	
//	gEmitter->add("position", &emitter->position);
//	gEmitter->add("interval", &emitter->interval);
//	gEmitter->add("rate", &emitter->rate);
//	gEmitter->add("max", &emitter->max);
//	
//	// Behaviours
//	Group* gBehaviours = gPhysics->add("Behaviours");
//	
//	// Random
//	Group* gBoxRandom = gBehaviours->add("BoxRandom");
//	gBoxRandom->add("min", &boxRandom->min);
//	gBoxRandom->add("max", &boxRandom->max);
//
//	// Attractor
//	Group* gAttractor = gBehaviours->add("Attractor");
//	gAttractor->add("position", &attractor->position);
//	gAttractor->add("range", &attractor->range);
}

void HelloTweakApp::initGraphics() {
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setDynamicPositions();
	layout.setStaticTexCoords2d();
	
	vboParticles = gl::VboMesh(physics->emitter->getMax(), 0, layout, GL_POINTS);	
}


// -- Update -------------------------------------------------------------------
void HelloTweakApp::update() {
	
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

// -- Draw ---------------------------------------------------------------------
void HelloTweakApp::draw() {
	ci::gl::clear(Color(0, 0, 0));
	
	glColor3f(1,1,1);
	glPointSize(3);
	
	// draw all particles at once using vbo
	drawArrays(vboParticles, 0, physics->getNumParticles());
}



// -- Events -------------------------------------------------------------------
void HelloTweakApp::mouseDown(MouseEvent event) {
	attractor->setWeight(1);
}

void HelloTweakApp::mouseUp(MouseEvent event) {
	attractor->setWeight(0);
}

void HelloTweakApp::mouseDrag(MouseEvent event) {
}

void HelloTweakApp::keyDown(KeyEvent event) {
}

CINDER_APP_BASIC(HelloTweakApp, RendererGl)
