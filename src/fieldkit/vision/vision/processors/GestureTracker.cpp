/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2010, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */

#include "fieldkit/vision/vision/processors/GestureTracker.h"
using namespace fieldkit::vision;

GestureTracker::GestureTracker()
{
	trackingRange = 0.1f;
	setFrameSize(0,0,320,240);
}

void GestureTracker::init()
{
	for(int i =0; i< MAX_GESTURES;i++)
	{
		found[i].init();
		tracked[i].init();
		tracked[i].id = i;
		found[i] = i;
	}
}

void GestureTracker::beginUpdate()
{
	nfound = 0;
	for(int i =0; i< MAX_GESTURES;i++)
	{
		found[i].init();
	}
}


Gesture *GestureTracker::addFoundPoint(float x, float y)
{
	if(nfound >= MAX_GESTURES) return 0;
	
	Gesture *gesture = &found[nfound];
	gesture->isActive = true;
	gesture->position.x = (x - framex)*oneOverFramew;
	gesture->position.y = (y - framey)*oneOverFrameh;
	nfound ++;
	return gesture;
}

void GestureTracker::endUpdate()
{
	float dist;
	int i,j;
	Gesture *t, *match, *f;
	float distClosest;

	// loop through tracked gesture points
	for(i =0; i< MAX_GESTURES;i++)
	{
		t = &tracked[i];
		if(!t->isActive) continue;
		match = 0;
		distClosest = trackingRange*trackingRange;
		
		// try to find a match among the new found gestures within the range
		for (j=0; j<MAX_GESTURES; j++) 
		{
			f = &found[j];
			if(!f->isActive || f->isAssigned) continue;
			dist = f->position.distanceSquared(t->position);
			if(dist < distClosest) {
				distClosest = dist;
				match = f;
			}
		}
			
		if(match == 0) 
		{  
			// no match found
			t->isActive = false;
			dispatchEventInActive(t);
		} 
		else { 
			// match found, continue updating this
			//printf("found %i => %i \n", tracked->id, match->id);
			t->isActive = true;
			//t->pushCurrentPositionToHistory();
			t->set(match); // update position 
			match->isAssigned = true;
			dispatchEventUpdate(t);
		}			
	}


	Gesture *newtracked;
	// for each new found gesture point not assigned, add the tracking list and dispatch active event
	for(i = 0 ; i < MAX_GESTURES;i++)
	{
		f = &found[i];
		if(f->isActive && !f->isAssigned)
		{
			f->isAssigned  = true;
			newtracked = getFreeTrackedGesture();
			if(newtracked)
			{
				newtracked->set(f);
				newtracked->isAssigned = true;
				newtracked->isActive = true;
				//newtracked->resetHistory();
				
				dispatchEventActive(newtracked);
			}
		}
	}
}

Gesture *GestureTracker::getFreeTrackedGesture()
{
	Gesture *gest;
	for(int i = 0;i<MAX_GESTURES;i++)
	{
		gest = &tracked[i];
		if(!gest->isActive)
		{
			return gest;
		}
	}
	return 0;
}


void GestureTracker::setFrameSize(float x, float y, float w, float h)
{
	this->framex = x;
	this->framey = y;
	this->framew = w;
	this->frameh = h;
	this->oneOverFrameh = 1.0f/frameh;
	this->oneOverFramew = 1.0f/framew;
}

void GestureTracker::setTrackingRange(float trackingRange)
{
	this->trackingRange = trackingRange;
}
