/*                                                                            *\
**           _____  __  _____  __     ____                                    **
**          / ___/ / / /____/ / /    /    \    FieldKit                       **
**         / ___/ /_/ /____/ / /__  /  /  /    (c) 2009, field.io             **
**        /_/        /____/ /____/ /_____/     http://www.field.io            **
\*                                                                            */
/* created October 27, 2009 */

#ifndef GESTURE_H
#define GESTURE_H

#include <vector>
using namespace std;
#include "cinder/Vector.h"

namespace fk { namespace vision { 

	//
	// represents a single detected element in the scene
	//
	class Gesture
	{
		public:
			static const int MAX_HISTORY_LEN = 10;

			int id;
			bool isActive;
			bool isAssigned;
			bool newpointset;

			cinder::Vec2f position;
			vector<cinder::Vec2f>positionHistory;
			int nhistory;

			Gesture(){};
			Gesture (int id) { 
				this->id = id;
				init();
			};
			~Gesture () {};	
					
			void init() 
			{
				resetHistory();
				isActive = false;
				isAssigned = false;
				newpointset = false;
			}
	
			void pushToHistory(cinder::Vec2f position)
			{
				if(nhistory == MAX_HISTORY_LEN){
					positionHistory.erase(positionHistory.begin());
					nhistory--;
				}
				positionHistory.push_back(position);
				nhistory++;
			}

			void set(Gesture *b) 
			{
				this->position.x = b->position.x;
				this->position.y = b->position.y;
			}

			void pushCurrentPositionToHistory()
			{
				pushToHistory(this->position);
			}

			void resetHistory()
			{ 
				positionHistory.clear();
				nhistory = 0;
			}

			cinder::Vec2f getVelocity()
			{
				cinder::Vec2f vel;
				if(nhistory == 0){
					vel.x = 0;
					vel.y = 0;
					return vel;
				}
				vel.x = position.x - positionHistory[nhistory-1].x;
				vel.y = position.y - positionHistory[nhistory-1].y;
				return vel;
			}
		
		protected:
	};
} } // namespace fk::vision
#endif