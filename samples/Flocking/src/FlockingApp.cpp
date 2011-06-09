/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - Flocking
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 09/05/2011.
 */

#include <stdexcept>
#include <vector>

#include "cinder/Timer.h"
#include "cinder/app/AppBasic.h"
#include "cinder/gl/GlslProg.h"

#include "fieldkit/FieldKit.h"
#include "fieldkit/Logger.h"
#include "fieldkit/ConfigDict.h"

#include "fieldkit/physics/PhysicsKit.h"

#include "fieldkit/gl/PointCloud.h"
#include "Resources.h"

#define MAX_PARTICLES 1000

using namespace ci;
using namespace ci::app;
using namespace ci::gl;
using namespace fk;
using namespace fk::physics;

class FlockingApp : public AppBasic {
public:
    FlockingApp() {};
    ~FlockingApp() {};
    
    void prepareSettings(Settings *settings);
    void setup();
    void update();
    void draw();
    
private:
    fk::ConfigDict config;
    
	Timer timer;
    Physics* physics;
	AttractorPoint* attractor;
    
    fk::gl::PointCloud cloud;

    void initPhysics();
    
    void keyDown(KeyEvent event);
    void mouseDown(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseDrag(MouseEvent event);
};

CINDER_APP_BASIC(FlockingApp, RendererGl(4)); 

void FlockingApp::prepareSettings(Settings *settings)
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

void FlockingApp::setup() 
{
    initPhysics();
    
    fk::gl::PointDataFormat format;
    format.addAttribute("InVertex", 3);
    format.addAttribute("InColor", 4);
    format.addAttribute("InSize", 1);
    cloud.init(format, MAX_PARTICLES, ci::gl::GlslProg(POINTCLOUD_DEFAULT_VS, POINTCLOUD_DEFAULT_FS));
}

void FlockingApp::initPhysics()
{
    // 1. create space
	Vec3f offset = Vec3f::zero();
	Vec3f dimension = Vec3f(getWindowWidth(), getWindowHeight(), 100);
    
	Space* space = new SpatialHash(offset, dimension, dimension.y * 0.1);
	
    // Space* space = new BasicSpace(Vec3f(0,0,0), Vec3f(getWindowWidth(), getWindowHeight(), 100));
    
    
    // 2. create physics
    physics = new Physics(space);
    
    FixedRadiusNeighbourUpdate* nbUpdate = new FixedRadiusNeighbourUpdate();
	nbUpdate->setRadius(dimension.y * 0.25);
	physics->setNeighbourUpdate(nbUpdate);
    
    
    // 3. create emitter
	Emitter* emitter = new Emitter(physics);
	emitter->setPosition(space->getCenter());
	emitter->setMax(MAX_PARTICLES);
    emitter->setRate(50);
    emitter->setInterval(0.01);
	physics->emitter = emitter;
    
    
    // 4. emitter behaviours only run once when particle is created
    Initializer* init = new Initializer();
    init->setSize(5);
    init->setSizeVariance(0.5);
//    init->setPerpetual(true); // never die
    init->setLifeTime(10.0);
    emitter->addBehaviour(init);
    
    emitter->addBehaviour(new BoxRandom(space));
    
    
    // 5. physics behaviours run permanently
    BoxWrap* wrap = new BoxWrap(space);
	wrap->preserveMomentum = false;
	physics->addBehaviour(wrap);
    
//	physics->addBehaviour(new Gravity());
    
    attractor = new AttractorPoint(space);
	attractor->setRange(0.25); // relative to space dimensions
	attractor->setWeight(0); // toggled on/off on mouse event
	physics->addBehaviour(attractor);
    
    FlockAttract* attr = new FlockAttract(space);
    attr->setRange(0.1);
    attr->setWeight(0.01);
	physics->addBehaviour(attr);    
    
    FlockAlign* align = new FlockAlign(space);
    align->setRange(0.025);
    align->setWeight(0.1);
	physics->addBehaviour(align);    
    
    FlockRepel* repel = new FlockRepel(space);
    repel->setRange(0.01);
    repel->setWeight(1.0);
	physics->addBehaviour(repel);    
}

void FlockingApp::update() 
{
    // update physics
	timer.stop();
	double dt = timer.getSeconds();
	timer.start();
    
    float mouseX = getMousePos().x;
	float mouseY = getMousePos().y;
	attractor->setPosition(Vec3f(mouseX, mouseY, 0));
    
    physics->update(dt);
}

void FlockingApp::draw() 
{
    ci::gl::clear(Color(0,0,0));

    // render via point cloud 
    cloud.map();    
    std::vector<fk::physics::Particle*>::iterator it = physics->particles.begin();
    for(; it != physics->particles.end(); it++) {
        Particle* p = (*it);
        Vec3f vel = p->getVelocity();
        
        cloud.put(p->position); // position
        cloud.put(ColorA(vel.x * 2.0, vel.y * 2.0, 1.0, 1.0)); // color
        cloud.put(p->size); // size
        cloud.insert();
    }
    cloud.unmap();
    
    cloud.draw();
}

void FlockingApp::keyDown(KeyEvent event)
{
    if(event.getChar() == ' ') {
        initPhysics();
    }
}

void FlockingApp::mouseDown(MouseEvent event)
{
    attractor->setWeight(1);
}

void FlockingApp::mouseUp(MouseEvent event)
{
    attractor->setWeight(0);
}

void FlockingApp::mouseDrag(MouseEvent event)
{
}
