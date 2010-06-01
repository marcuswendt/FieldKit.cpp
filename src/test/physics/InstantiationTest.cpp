/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 01/06/2010.
 */

#include <list>
#include <vector>
#include <boost/foreach.hpp>
#include "cinder/app/AppBasic.h"

using namespace std;
using namespace ci;
using namespace ci::app;

inline float randomRange(float a, float b) {
	return ((b-a)*((float)rand()/RAND_MAX))+a;
}

class Particle {
public:
	
	Vec3f steer;
	Vec3f velocity;
	Vec3f position;
	
	Particle() {
		steer = Vec3f();
		velocity = Vec3f();
		position = Vec3f();
	}
	
	void update() {
		steer.x += randomRange(-100, 100);
		steer.y += randomRange(-100, 100);
		steer.z += randomRange(-100, 100);
		
		velocity += steer;
		position += velocity;
		velocity *= 0.97f;
		steer.zero();
	}
};

class Test {
public:
	const char* name;
	int numParticles;
	
	virtual void init(int numParticles) = 0;
	virtual void update() = 0;
};


class StaticArrayTest : public Test {
public:
	Particle* particles;
	
	void init(int numParticles) {
		this->name = "StaticArrayTest";
		this->numParticles = numParticles;
	}
	
	void update() {
		particles = new Particle[numParticles];
		
		for(int i=0; i<numParticles; i++) {
			particles[i] = Particle();
			particles[i].update();
		}
	}
};

class STLObjectListTest : public Test {
	list<Particle> particles;
	
	void init(int numParticles) {
		this->name = "STLObjectListTest";
		this->numParticles = numParticles;
	}
	
	void update() {
		particles.clear();
		
		for(int i=0; i<numParticles; i++) {
			particles.push_back(Particle());
		}
		
		for(list<Particle>::iterator p = particles.begin(); p != particles.end(); ++p) {	
			p->update();
		}
	}
};


class STLPointerListTest : public Test {
	list<Particle*> particles;
	
	void init(int numParticles) {
		this->name = "STLPointerListTest";
		this->numParticles = numParticles;
	}
	
	void update() {
		particles.clear();
		
		for(int i=0; i<numParticles; i++) {
			particles.push_back(new Particle());
		}
		
		for(list<Particle*>::iterator p = particles.begin(); p != particles.end(); ++p) {	
			(*p)->update();
		}
	}
};


class STLPointerVectorTest : public Test {
	vector<Particle*> particles;
	
	void init(int numParticles) {
		this->name = "STLPointerVectorTest";
		this->numParticles = numParticles;
	}
	
	void update() {
		particles.clear();
		
		for(int i=0; i<numParticles; i++) {
			particles.push_back(new Particle());
		}
		
		for(vector<Particle*>::iterator p = particles.begin(); p != particles.end(); ++p) {	
			(*p)->update();
		}
	}
};


class STLObjectVectorTest : public Test {
	vector<Particle> particles;
	
	void init(int numParticles) {
		this->name = "STLObjectVectorTest";
		this->numParticles = numParticles;
	}
	
	void update() {
		particles.clear();
		
		for(int i=0; i<numParticles; i++) {
			particles.push_back(Particle());
		}
		
		for(vector<Particle>::iterator p = particles.begin(); p != particles.end(); ++p) {	
			p->update();
		}
	}
};

int main(const char* args) {
	printf("---- Iteration Test ----\n");
	
	// config
	int numParticles = 50 * 1000;
	int numIterations = 1;
	Timer* timer = new Timer();
	
	// create tests
	list<Test*> tests;
	tests.push_back(new StaticArrayTest());
	tests.push_back(new STLObjectListTest());
	tests.push_back(new STLPointerListTest());
	tests.push_back(new STLPointerVectorTest());
	tests.push_back(new STLObjectVectorTest());	
	
	// init tests
	printf("initialising %i particles\n", numParticles);
	BOOST_FOREACH(Test* t, tests) {
		t->init(numParticles);
	}
	
	// run tests
	printf("running tests %i times\n", numIterations);
	BOOST_FOREACH(Test* t, tests) {
		printf("running %s: ", t->name);
		timer->start();
		for(int i=0; i<numIterations; i++) {
			t->update();
		}
		timer->stop();
		printf("%f s \n", timer->getSeconds());
	}
	
	printf("---- Done ----\n");
	return 0;
}