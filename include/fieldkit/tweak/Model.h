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
#include "fieldkit/tweak/Tweak.h"

#include "tinyxml/tinyxml.h"

namespace fk { namespace tweak {

	class Model {
	public:
		Model() {
			setName("undefined");
		};
		~Model() {};
		
		void init() {
			// setup the model in a subclass
			setup();
			
			// initialise all child-models and tweaks
			BOOST_FOREACH(Model* m, children) {
				m->init();
			}	
		}
		
		void addChild(Model* m);
		void add(Tweak* c) { tweaks.push_back(c); }
		
		Model* getChild(const string &name);
//		void set(const string &name, void value) {};
		
		// Getters & Setters
		void setName(const string &name) { this->name = name; }
		string getName() { return this->name; }

		void setParent(Model* parent) { this->parent = parent; }
		Model* getParent() { return parent; }
		
		// Serialization
		virtual void fromXML(TiXmlElement* element);
		virtual TiXmlElement* toXML();
		
	protected:
		string name;
		Model* parent;
		
		vector<Model*> children;
		vector<Tweak*> tweaks;

		virtual void setup() = 0;
	};
} } // namespace fk::tweak