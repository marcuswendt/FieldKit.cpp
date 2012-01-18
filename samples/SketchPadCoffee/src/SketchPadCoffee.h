/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - SketchPadCoffee
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 17/01/2012.
 */

#pragma once

#include "cinder/app/AppBasic.h"

#include "fieldkit/FieldKit.h"
#include "fieldkit/Logger.h"
#include "fieldkit/ConfigDict.h"
#include "fieldkit/script/ScriptKit.h"

using namespace fk;
using namespace ci::app;

namespace field {
	
    class SketchPadCoffee : public AppBasic {
    public:
        SketchPadCoffee() {};
        ~SketchPadCoffee() {};
        
        void prepareSettings(Settings *settings);
        void setup();
        void shutdown();
        void update();
        void draw();
        
    private:
        fk::ConfigDict config;
        
        // events
        void keyDown(KeyEvent event);
        void mouseDown(MouseEvent event);
        void mouseUp(MouseEvent event);
        void mouseMove(MouseEvent event);
        void mouseDrag(MouseEvent event);
        
		// scripting
        bool isEnabled;
        int reloadTimer;
	    void reload(string script="");
	    fk::script::ScriptContext context;
	    v8::Persistent<v8::Object> sketch;
    };
	
}
