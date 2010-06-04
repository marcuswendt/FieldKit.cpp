/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 01/06/2010.
 */

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/gl/Vbo.h"
#include <list>

#include "fieldkit/FieldKit.h"
#include "fieldkit/physics/Physics.h"
#include "fieldkit/physics/behaviours/Attractor.h"
#include "fieldkit/physics/behaviours/Boundary.h"
#include "fieldkit/physics/behaviours/Force.h"
#include "fieldkit/physics/behaviours/Random.h"

#include "fieldkit/tweak/Model.h"
#include "fieldkit/tweak/Tweak.h"

// -- App ----------------------------------------------------------------------
class HelloTweakAppModel;

using namespace ci;
using namespace ci::app;
using namespace fk::physics;

class HelloTweakApp : public AppBasic {
public:
	void prepareSettings(Settings *settings);
	void setup();
	
	void update();
	void draw();
	
	void mouseDown(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void keyDown(KeyEvent event);
	
protected:
	HelloTweakAppModel* model;
	Physics* physics;
	Timer* timer;
	gl::VboMesh vboParticles;
};


// -- Model --------------------------------------------------------------------
using namespace fk::tweak;

class HelloTweakAppModel : public Model {
public:
	void setup();
};

class PhysicsModel : public Model {
public:
	Space* space;
	Physics* physics;
	
	void setup();
	void create();
};

class EmitterModel : public Model {
public:
	Emitter* emitter;
	
	void setup();
	void create();
	
	Physics* getPhysics() { return ((PhysicsModel*)parent)->physics; }
	Space* getSpace() { return ((PhysicsModel*)parent)->space; }
};

class BehaviourModel : public Model {
public:
	AttractorPoint* attractor;
	
	void setup();
	void create();
	
	Physics* getPhysics() { return ((PhysicsModel*)parent)->physics; }
	Space* getSpace() { return ((PhysicsModel*)parent)->space; }
};
