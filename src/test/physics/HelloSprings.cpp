/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 05/06/2010.
 */

#include "cinder/Rand.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/Vbo.h"
#include "cinder/MayaCamUI.h"
#include <list>

#include "fieldkit/physics/Physics.h"
#include "fieldkit/physics/behaviours/Random.h"
#include "fieldkit/physics/behaviours/Attractor.h"
#include "fieldkit/physics/behaviours/Boundary.h"
#include "fieldkit/physics/behaviours/Force.h"
#include "fieldkit/physics/behaviours/Flocking.h"
#include "fieldkit/physics/behaviours/PlaneConstraint.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace fk::physics;

class HelloSprings : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	void initPhysics();
	
	void update();
	void draw();
	void drawGrid(Space* space, int gridRes);
	

	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	
	Timer* timer;
	Physics* physics;
	AttractorPoint* attractor;
	
	MayaCamUI* camera;
	gl::VboMesh vboParticles;
};

void HelloSprings::prepareSettings(Settings *settings){
    settings->setWindowSize(1280, 720);
    settings->setFrameRate(60.0f);
	
};

// -- Init ---------------------------------------------------------------------
void HelloSprings::setup() {
	
	initPhysics();
	
	// init graphics
	camera = new MayaCamUI();
	
	gl::VboMesh::Layout layout;
	layout.setStaticIndices();
	layout.setDynamicPositions();
	layout.setStaticTexCoords2d();
	
	vboParticles = gl::VboMesh(physics->particles.size(), 0, layout, GL_POINTS);
}

void HelloSprings::initPhysics() {
	timer = new Timer();
	
	// init physics
	float d = 250.0f;
	Space* space = new Space(Vec3f(-d, -d, -d), Vec3f(d, d, d));
	printf("init space %f %f %f\n", space->getWidth(), space->getHeight(), space->getDepth());
	
	physics = new Physics(space);
	physics->emitter = NULL;
	physics->allocParticles(10);
	
//	emitter->addBehaviour(new BoxRandom(*space));
//	physics->addBehaviour(new Gravity());
	
	// wrap
//	BoxWrap* wrap = new BoxWrap(*space);
//	wrap->preserveMomentum = false;
//	physics->addBehaviour(wrap);
	
	// floor constraint
	physics->addConstraint(new FloorConstraint());
	
	// attractor 
	attractor = new AttractorPoint(space);
	attractor->setRange(0.25);
	attractor->setWeight(0);
	physics->addBehaviour(attractor);

	for(ParticlePtr a = physics->particles.begin(); a != physics->particles.end(); a++) {
		Vec3f pos;
		pos.x = Rand::randFloat(space->min.x, space->max.x);
		pos.y = Rand::randFloat(space->min.y, space->max.y);
		pos.z = Rand::randFloat(space->min.z, space->max.z);
		a->init(pos);
	}
	
	for(ParticlePtr a = physics->particles.begin(); a != physics->particles.end(); a++) {

		int numNeighbours = Rand::randInt(1, 25);
		
		if(Rand::randBool()) 
			a->lock();
		
		for(int n=0; n<numNeighbours; n++) {
			ParticlePtr b = physics->particles.begin();
			int bIndex = Rand::randInt(0, physics->particles.size());
			int i = 0;
			while(i < bIndex && b != a) {
				b++;
				i++;
			}
		
			physics->addSpring(new Spring(a, b, 10.0f, 0.01f));
		}
	}
}

void HelloSprings::update() {
	
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

void HelloSprings::draw() {
	gl::clear(Color(0, 0, 0));
	
	gl::setModelView(camera->getCamera());
	
	drawGrid(physics->space, 20);
	
	glColor3f(1,1,1);
	glPointSize(3);
	
	// draw all particles at once using vbo
	drawArrays(vboParticles, 0, physics->getNumParticles());
	
	// draw springs
	gl::color(Color(1.0f, 1.0f, 1.0f));
	glLineWidth(1.0f);
	
	BOOST_FOREACH(Spring* s, physics->springs) {
		gl::drawLine(s->a->position, s->b->position);
	}
}

void HelloSprings::drawGrid(Space* space, int gridRes) {
	
	// xz grid
	gl::color(Color(0.75f, 0.75f, 0.75f));
	
	gl::pushMatrices();
	gl::translate(Vec3f(-space->getWidth() / 2.0f, 0, -space->getDepth() / 2.0f));
	for(int i=0; i<=gridRes; i++) {
		for(int j=0; j<=gridRes; j++) {		
			float x = i/(float)gridRes * space->getWidth();
			gl::drawLine(Vec3f(x, .0f, .0f), Vec3f(x, .0f, space->getDepth()));
			
			float z = j/(float)gridRes * space->getDepth();
			gl::drawLine(Vec3f(.0f, .0f, z), Vec3f(space->getWidth(), .0f, z));
		}
	}
	gl::popMatrices();
	
	
	// space-cube
	gl::color(Color(0.5f, 0.5f, 0.5f));
	
	Vec3f min = space->min;
	Vec3f max = space->max;
	
	gl::drawLine(Vec3f(min.x,min.y,min.z), Vec3f(max.x,min.y,min.z));
	gl::drawLine(Vec3f(min.x,min.y,min.z), Vec3f(min.x,max.y,min.z));
	gl::drawLine(Vec3f(min.x,min.y,min.z), Vec3f(min.x,min.y,max.z));
	
	gl::drawLine(Vec3f(max.x,max.y,max.z), Vec3f(min.x,max.y,max.z));
	gl::drawLine(Vec3f(max.x,max.y,max.z), Vec3f(max.x,min.y,max.z));
	gl::drawLine(Vec3f(max.x,max.y,max.z), Vec3f(max.x,max.y,min.z));
	
	gl::drawLine(Vec3f(max.x,max.y,min.z), Vec3f(max.x,min.y,min.z));
	gl::drawLine(Vec3f(min.x,max.y,max.z), Vec3f(min.x,min.y,max.z));
	
	gl::drawLine(Vec3f(min.x,max.y,min.z), Vec3f(min.x,max.y,max.z));	
	gl::drawLine(Vec3f(min.x,max.y,min.z), Vec3f(max.x,max.y,min.z));
	
	gl::drawLine(Vec3f(min.x,min.y,max.z), Vec3f(max.x,min.y,max.z));
	gl::drawLine(Vec3f(max.x,min.y,max.z), Vec3f(max.x,min.y,min.z));
	
	gl::drawCoordinateFrame(space->extent.length() * 0.5f);
}


// -- Events -------------------------------------------------------------------
void HelloSprings::mouseDown(MouseEvent e) {
	attractor->setWeight(1);
	camera->mouseDown(Vec2i(e.getX(), e.getY()));
}

void HelloSprings::mouseUp(MouseEvent e) {
	attractor->setWeight(0);
}

void HelloSprings::mouseDrag(MouseEvent e) {
	camera->mouseDrag(Vec2i(e.getX(), e.getY()), 
					  e.isLeft(), e.isAltDown(), e.isShiftDown());
}

void HelloSprings::keyDown(KeyEvent e) {
	if(e.getChar() == ' ')
		initPhysics();
}

CINDER_APP_BASIC(HelloSprings, RendererGl)
