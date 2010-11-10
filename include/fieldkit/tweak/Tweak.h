///*                                                                           
// *      _____  __  _____  __     ____                                   
// *     / ___/ / / /____/ / /    /    \   FieldKit
// *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
// *   /_/        /____/ /____/ /_____/    http://www.field.io           
// *   
// *	 created by Marcus Wendt on 01/06/2010.
// */
//
//#pragma once
//
//#include "fieldkit/FieldKit.h"
////#include "tinyxml/tinyxml.h"
//
//namespace fieldkit { namespace tweak {
//
////	template<typename T>
//	class KeyFrame {
//	public:
//		int frame;
//		void* value;
//	};
//
//	/*
//	template<typename T>
//	class Channel {
//	public:
//		Channel() : defaultValue(NULL) {};
//		~Channel() {};
//		
//		void set(int frame, int value) { this->value = value; }
//		int get(int frame) { return 0; }
//		
//		void setDefault(T value) { this->defaultValue = value; }
//		T getDefault() { return this->defaultValue; }
//		
//	protected:
//		vector< KeyFrame<T> > keyFrames;
//		T defaultValue;
//	};
//	 
//	class Tweak {
//	public:
//		Tweak(const string &name) {
//			this->name = name;
//		}
//		
//		void setName(const string &name) {
//			this->name = name;
//		}
//		
//		string getName() { return this->name; }
//		
//	protected: 
//		string name;
//	};
//	 */
//	
//	class Tweak {
//	public:
//		Tweak(const string &_name, void* _defaultValue) : 
//			name(_name), 
//			defaultValue(_defaultValue) {}
//		~Tweak() {}
//		
//		void set(void* value, int frame=0) { }
//		int get(int frame=0) { return 0; }
//		
//		void setDefault(void* value) { this->defaultValue = value; }
//		void* getDefault() { return this->defaultValue; }
//		
//	protected:
//		string name;
//		void* defaultValue;
//		vector<KeyFrame> keyFrames;
//	};
//
//	
//	// -- Implementations ------------------------------------------------------
////	class IntTweak : public Tweak, public Channel<int> {
////	public:
////		IntTweak(const string &name, int defaultValue_ = 0) :
////		Tweak(name) {
////			setDefault(defaultValue);
////		};
////	};
////	
////	class FloatTweak : public Tweak, public Channel<float> {
////	public:
////		FloatTweak(const string &name, float defaultValue_ = 0) :
////		Tweak(name) {
////			setDefault(defaultValue);
////		};
////	};
//	
//	class Vec3fTweak : public Tweak {
//	public:
//		Vec3fTweak(const string &name, Vec3f _defaultValue = Vec3f(0,0,0)) :
//		Tweak(name, _defaultValue) {
//			
//		};
//	};
//} } // namespace fieldkit::tweak