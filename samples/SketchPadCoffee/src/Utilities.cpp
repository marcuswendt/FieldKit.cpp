/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit - SketchPadCoffee
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2012, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 17/01/2012.
 */

#include "Utilities.h"

#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/app/App.h"

#include "fieldkit/gl/ImageCompressor.h"
#include <boost/thread.hpp>

namespace field  {
    
    using namespace v8;

    v8::Handle<v8::Value> GetAverageFps(v8::Arguments const& args) 
    {	
        using namespace v8;
        return Number::New(ci::app::App::get()->getAverageFps());
    }
    
    
    fieldkit::gl::ImageCompressor compressor;
    
    v8::Handle<v8::Value> SaveFrame(v8::Arguments const& args) 
    {	
        using namespace v8;
        if(args.Length() == 1) {
            HandleScope handleScope;
            std::string file = fieldkit::script::ToStdString( Handle<String>::Cast(args[0]) );
            std::string path = ci::getHomeDirectory() + ci::getPathSeparator() + file;
            
            compressor.Compress(path, ci::app::copyWindowSurface());
        }
        return Undefined();
    }
    
    v8::Handle<v8::Value> Sleep(v8::Arguments const& args) 
    {	
        using namespace v8;
        if(args.Length() == 1) {
            HandleScope handleScope;
            double milliseconds = v8::Handle<v8::Number>::Cast(args[0])->Value();
            boost::this_thread::sleep(boost::posix_time::milliseconds(milliseconds)); 
        }
        return Undefined();
    }
    
    void Utilities::Initialize(v8::Handle<v8::Object> target) 
    {
        SET_METHOD(target, "GetAverageFps", GetAverageFps);
        SET_METHOD(target, "SaveFrame", SaveFrame);
        SET_METHOD(target, "Sleep", Sleep);
    }    
}

