/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - SketchPad
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 02/05/2011.
 */

#include "SketchPad.h"

#include <stdexcept>

#include <boost/algorithm/string/replace.hpp>
#include "cinder/Rand.h"

#include "Resources.h"
#include "Utilities.h"

using namespace ci;
using namespace ci::gl;
using namespace field;
using namespace fk::script;

CINDER_APP_BASIC(SketchPad, RendererGl(4)); 

#define ENTER_SCOPE \
    if(!isEnabled) return; \
    Context::Scope contextScope(context.getContext()); \
    HandleScope handleScope; \
    try {

#define EXIT_SCOPE \
    } catch(std::exception& e) { \
        LOG_ERROR(e.what()); \
        isEnabled = false; \
        return; \
    }

void SketchPad::prepareSettings(Settings *settings)
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

//	requires changes in field/Cinder that aren't included in the master repository yet
//  Vec2i windowPos = config.get2i("position", Vec2i(-1,-1));
//	settings->setWindowPosition(windowPos.x, windowPos.y);
//  settings->setChromeless(config.getb("chromeless"));
}


void SketchPad::setup() 
{
	LOG_INFO("SketchPad::setup");	
    
    Rand::randomize();
    
    // attach script modules
    // these make new functionality available to V8
    // a few handy modules come already with FieldKit
    context.add(new fk::script::Logger());
    context.add(new fk::script::Random()); 
    context.add(new fk::script::TypedArray());
    context.add(new fk::script::Graphics2D());
    context.add(new fk::script::Perlin());
    
    // this is how you can define your own custom functions
    context.add(new Utilities());
    
    // load script
    // by default points to a file within the application bundle
    // use the -script command line argument or config.xml to point it somewhere else e.g. for live coding
    string script = config.gets("script", "$APP/Contents/Resources/Sketch.js");
    boost::algorithm::replace_all(script, "$APP", cinder::app::getAppPath().c_str());
    reload(script);
    
    // init GL context
    ci::gl::enableAlphaBlending();
}


void SketchPad::reload(string script)
{
    isEnabled = true;
    reloadTimer = 0;
    
    // cleanup
    sketch.Dispose();
    
    // execute script
    try {
        context.execute(script);
    } catch(std::exception& e) {
        LOG_ERROR(e.what());
        isEnabled = false;
        return;
    }
    
    // create new sketch instance
    ENTER_SCOPE;
    
    int argc = 2;
    Handle<Value> args[] = { 
        Integer::New(ci::app::getWindowWidth()), 
        Integer::New(ci::app::getWindowHeight())
    };
    
    Handle<Object> object = context.newInstance(context.getContext()->Global(), String::New("Sketch"), argc, args);
    sketch = Persistent<Object>::New(object);
    EXIT_SCOPE;
}

void SketchPad::shutdown()
{
}


void SketchPad::update() 
{
    // 'Live Coding'
    // periodically check for source file modifications and reload script if necessary    
    reloadTimer++;
    if(reloadTimer > 30) {
        reloadTimer = 0;
        if(context.filesModified()) {
            reload();
            return;
        }
    }

    // call the Sketch JavaScript objects update method once per frame
    ENTER_SCOPE;
    context.call(sketch, "update");
    EXIT_SCOPE;
}


void SketchPad::draw() 
{
    ENTER_SCOPE;
    context.call(sketch, "draw");         
    EXIT_SCOPE;
}


// Keyboard Events
void SendKeyEvent(fk::script::ScriptContext& context, bool isEnabled, v8::Persistent<v8::Object> sketch, 
                  const char* name, KeyEvent const& event)
{
    ENTER_SCOPE;
    Handle<Object> obj = Object::New();
    SET_PROPERTY(obj, "char", String::New(string(1, event.getChar()).c_str()));
    SET_PROPERTY(obj, "code", Integer::New(event.getCode()));
    
    
    SET_PROPERTY(obj, "shift", v8::Boolean::New(event.isShiftDown()));
    SET_PROPERTY(obj, "alt", v8::Boolean::New(event.isAltDown()));
    SET_PROPERTY(obj, "control", v8::Boolean::New(event.isControlDown()));
    //    SET_PROPERTY(obj, "meta", v8::Boolean::New(event.isMetaDown()));
    SET_PROPERTY(obj, "accel", v8::Boolean::New(event.isAccelDown()));
    
    Handle<Value> args[] = { obj };
    context.call(sketch, name, 1, args);
    EXIT_SCOPE;    
}

void SketchPad::keyDown(KeyEvent event)
{
    SendKeyEvent(context, isEnabled, sketch, "keyDown", event);
}


// Mouse Events
void SendMouseEvent(fk::script::ScriptContext& context, bool isEnabled, v8::Persistent<v8::Object> sketch, 
                    const char* name, MouseEvent const& event)
{
    int x = event.getX();
    int y = event.getY();
    
    int button = 0;
    if(event.isRight()) button = 1;
    if(event.isMiddle()) button = 2;
    if(event.isAltDown()) button = 1;
    if(event.isControlDown()) button = 2;
    
    ENTER_SCOPE;
    Handle<Object> obj = Object::New();
    SET_PROPERTY(obj, "x", Integer::New(x));
    SET_PROPERTY(obj, "y", Integer::New(y));
    SET_PROPERTY(obj, "button", Integer::New(button));         
    Handle<Value> args[] = { obj };
    context.call(sketch, name, 1, args);
    EXIT_SCOPE;    
}

void SketchPad::mouseDown(MouseEvent event)
{
    SendMouseEvent(context, isEnabled, sketch, "mouseDown", event);
}

void SketchPad::mouseUp(MouseEvent event)
{
    SendMouseEvent(context, isEnabled, sketch, "mouseUp", event);
}

void SketchPad::mouseMove(MouseEvent event)
{
    SendMouseEvent(context, isEnabled, sketch, "mouseMove", event);
}

void SketchPad::mouseDrag(MouseEvent event)
{
    SendMouseEvent(context, isEnabled, sketch, "mouseDrag", event);
}
