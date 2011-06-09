/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - RubberBand
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 09/05/2011.
 */

#include <stdexcept>
#include <vector>

#include "cinder/Timer.h"
#include "cinder/BSpline.h"
#include "cinder/app/AppBasic.h"

#include "fieldkit/FieldKit.h"
#include "fieldkit/Logger.h"
#include "fieldkit/ConfigDict.h"
#include "fieldkit/physics/PhysicsKit.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace fk;
using namespace fk::physics;

class RubberBandApp : public AppBasic {
public:
    RubberBandApp() {};
    ~RubberBandApp() {};
    
    void prepareSettings(Settings *settings);
    void setup();
    void update();
    void draw();
    
private:
    fk::ConfigDict config;
    
	Timer timer;
    Physics* physics;
    AttractorPoint* attractor;

    void initPhysics();
    void initSprings();
    
    void keyDown(KeyEvent event);
    void mouseDown(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseDrag(MouseEvent event);
};

CINDER_APP_BASIC(RubberBandApp, RendererGl(4)); 


void RubberBandApp::prepareSettings(Settings *settings)
{
	// load config
    try {
        config.loadXML(loadResource(RES_CONFIG));
    } catch(std::exception& e) {
        LOG_ERROR("Couldnt read configuration file");
        _exit(0);
    }
    
	config.overrideWith(getArgs());
	config.print();
    
	// setup screen
	settings->setFrameRate(config.getf("framerate", 60.0f));
	settings->setFullScreen(config.getb("fullscreen"));
    
	Vec2i windowSize = config.get2i("window", Vec2i(640,480));
	settings->setWindowSize(windowSize.x, windowSize.y);
}


void RubberBandApp::setup() 
{
    physics = NULL;
    initPhysics();
}


void RubberBandApp::initPhysics()
{
    // 1. create space
    Space* space = new BasicSpace(Vec3f(0,0,0), Vec3f(getWindowWidth(), getWindowHeight(), 100));

    // 2. (re)create physics
    if(physics != NULL)
        delete physics;
    
    physics = new Physics(space);

    // 3. physics behaviours run permanently
//    BoxWrap* wrap = new BoxWrap(space);
//    wrap->preserveMomentum = false;
//    physics->addBehaviour(wrap);

//	physics->addBehaviour(new Gravity());

//    attractor = new AttractorPoint(space);
//    attractor->setRange(0.25); // relative to space dimensions
//    attractor->setWeight(0); // toggled on/off on mouse event
//    physics->addBehaviour(attractor);

//    FlockAttract* attr = new FlockAttract(space);
//    attr->setRange(0.1);
//    attr->setWeight(0.01);
//    physics->addBehaviour(attr);    
//
//    FlockAlign* align = new FlockAlign(space);
//    align->setRange(0.025);
//    align->setWeight(0.1);
//    physics->addBehaviour(align);    
//
//    FlockRepel* repel = new FlockRepel(space);
//    repel->setRange(0.01);
//    repel->setWeight(1.0);
//    physics->addBehaviour(repel);    
    
    initSprings();
}

void RubberBandApp::initSprings()
{   
    // create particles
    float width = 100;
    float sw = physics->space->getWidth();
    float sh = physics->space->getHeight();
    
    std::vector<Vec3f> points;
    points.push_back(Vec3f(sw*0.2, sh*0.5, 0.0));
    points.push_back(Vec3f(sw*0.8, sh*0.5, 0.0));
    BSpline3f spline(points, 1, false, false);
    
    int res = 2;
    for(int i=0; i<res; i++) {
        Vec3f pos = spline.getPosition( (float)i / (float)res );
        
        Particle* a = new Particle();
        a->lifeTime = Particle::LIFETIME_PERPETUAL;
        a->setPosition(pos + Vec3f(0,-width*0.5,0));
        a->clearVelocity();
        a->size = 5;
        physics->addParticle(a);
        
        Particle* b = new Particle();
        b->lifeTime = Particle::LIFETIME_PERPETUAL;
        b->setPosition(pos + Vec3f(0,width*0.5,0));
        b->clearVelocity();
        b->size = 5;
        physics->addParticle(b);
    }
    
    // connect particles with springs
    for(int i=0; i<res; i++) {
        Particle* a = physics->particles[i*2];
        Particle* b = physics->particles[i*2+1];
        
        Spring* s = new Spring(a, b, a->position.distance(b->position), 0.01);
        physics->addSpring(s);
    }
}


void RubberBandApp::update() 
{
    // update physics
	timer.stop();
	double dt = timer.getSeconds();
	timer.start();
    
    physics->update(dt);
    
//    LOG_INFO("-----------------------------");
    int i=0;
    std::vector<Particle*>::iterator pit = physics->particles.begin();
    for(; pit != physics->particles.end(); pit++) {
        Particle* p = (*pit);
        LOG_INFO("P "<< i <<": "<< p->position);
        i++;
    }
}

void RubberBandApp::draw() 
{
    ci::gl::clear(Color(0.9,0.9,0.9));
    
    // draw springs
    ci::gl::color(Color(0.0,0.1,0.9));
    glLineWidth(2.0);
    
    std::vector<Spring*>::iterator sit = physics->springs.begin();
    for(; sit != physics->springs.end(); sit++) {
        Spring* s = (*sit);
        ci::gl::drawLine(s->a->position, s->b->position);
	}    
    
    // draw particles
    ci::gl::color(Color(0.9,0.1,0.0));
    std::vector<Particle*>::iterator pit = physics->particles.begin();
    for(; pit != physics->particles.end(); pit++) {
        Particle* p = (*pit);
        
        glPointSize(p->size);
        glBegin(GL_POINTS);
		glVertex3f(p->position.x, p->position.y, p->position.z);
        glEnd();
	}
}


void RubberBandApp::keyDown(KeyEvent event)
{
    if(event.getChar() == ' ') {
        initPhysics();
    }
}

void RubberBandApp::mouseDown(MouseEvent event)
{
    attractor->setWeight(1);
}

void RubberBandApp::mouseUp(MouseEvent event)
{
    attractor->setWeight(0);
}

void RubberBandApp::mouseDrag(MouseEvent event)
{
	attractor->setPosition(Vec3f(event.getX(), event.getY(), 0));
}
