/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 01/06/2010.
 */

#include "test/tweak/HelloTweakApp.h"

void HelloTweakAppModel::setup() {
	setName("HelloTweakApp");
	addChild(new PhysicsModel());
}

void PhysicsModel::setup() {
	setName("Physics");
	addChild(new EmitterModel());
	addChild(new BehaviourModel());
}

void PhysicsModel::create() {
	
	// physics space
	space = new Space(App::get()->getWindowWidth(), App::get()->getWindowHeight(), 0);
	
	// physics simulation
	physics = new Physics(space);
	
	// wrap
	BoxWrap* wrap = new BoxWrap(*space);
	wrap->preserveMomentum = false;
	physics->addBehaviour(wrap);
	
	//printf("init space %f %f %f\n", space->getWidth(), space->getHeight(), space->getDepth());
}

void EmitterModel::setup() {
	setName("Emitter");
	add(new Vec3fTweak("position", Vec3f(0.5f,0.5f,0.5f)));
//	add(new FloatTweak("rate"));
//	add(new IntTweak("rate"));
//	add(new IntTweak("max"));
}

void EmitterModel::create() {
	emitter = new Emitter(getPhysics());
	getPhysics()->emitter = emitter;
	emitter->setPosition(getSpace()->getCenter());
	emitter->setInterval(0.01);
	emitter->setRate(1000);
	emitter->setMax(1000);
	
	BoxRandom* boxRandom = new BoxRandom(*getSpace());
	emitter->addBehaviour(boxRandom);
}

void BehaviourModel::setup() {
	setName("Behaviours");
//	add(new Vec3fTweak("position", Vec3f(0.5f, 0.5f, 0.5f));
//	add(new FloatTweak("range", 0.25f));
//	add(new FloatTweak("weight", 0.0f));
}

void BehaviourModel::create() {
	attractor = new AttractorPoint(getSpace());
	attractor->setRange(0.25);
	attractor->setWeight(0);
	getPhysics()->addBehaviour(attractor);	
}

// -- Init ---------------------------------------------------------------------
void HelloTweakApp::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);	
};

void HelloTweakApp::setup() {
	// model
	model = new HelloTweakAppModel();
	model->init();
	
	// animation
	physics = ((PhysicsModel*)model->getChild("Physics"))->physics;
	timer = new Timer();
	
	// graphics
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
	
//	float mouseX = getMousePos().x;
//	float mouseY = getMousePos().y;
//	attractor->setPosition(Vec3f(mouseX, mouseY, 0));
	
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
//	model->getChild("Physics")->getChild("Behaviours")->set("weight", 1.0f);
}

void HelloTweakApp::mouseUp(MouseEvent event) {
//	model->getChild("Physics")->getChild("Behaviours")->set("weight", 0.0f);
}

void HelloTweakApp::mouseDrag(MouseEvent event) {
}

void HelloTweakApp::keyDown(KeyEvent event) {
}

CINDER_APP_BASIC(HelloTweakApp, RendererGl)
