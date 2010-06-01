/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 created by Marcus Wendt on 01/06/2010.
 */

#pragma once

#include "fieldkit/FieldKit.h"
//#include "tinyxml/tinyxml.h"

namespace fk { namespace tweak {

	template<typename T>
	class KeyFrame {
	public:
		int frame;
		T value;
	};
	
	template<typename T>
	class Channel {
	public:
		void set(int frame, int value) { this->value = value; }
		int get(int frame) {}
		
		void setDefault(T value) { this->defaultValue = value; }
		T getDefault() { return this->defaultValue; }
		
	protected:
		vector< KeyFrame<T> > keyFrames;
		T defaultValue;
	};
	
	class Tweak {
	public:
		Tweak(const string &name) {
			this->name = name;
		}
		
		void setName(const string &name) {
			this->name = name;
		}
		
		string getName() { return this->name; }
		
	protected: 
		string name;
	};

	// -- Implementations ------------------------------------------------------
	class IntTweak : public Tweak, public Channel<int> {
	public:
		IntTweak(const string &name) : Tweak(name) {
		};
	};
	
	class FloatTweak : public Tweak, public Channel<float> {
	public:
		FloatTweak(const string &name) : Tweak(name) {
		};
	};
	
	class Vec3fTweak : public Tweak, public Channel<Vec3f> {
	public:
		Vec3fTweak(const string &name) : Tweak(name) {
		};
	};
} } // namespace fk::tweak