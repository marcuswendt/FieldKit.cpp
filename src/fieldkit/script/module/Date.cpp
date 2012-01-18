/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 18/01/2012.
 */

#include "fieldkit/script/module/Date.h"
#include "fieldkit/script/ObjectWrap.h"
//#include "cinder/Rand.h"
#include <boost/date_time/posix_time/posix_time.hpp>

namespace fieldkit { namespace script {

using namespace boost::posix_time; 

class DateWrap : ObjectWrap {
public:
    static void Initialize(Handle<Object> target)
    {
        Local<FunctionTemplate> t = FunctionTemplate::New(DateWrap::New);
        
        classTemplate = Persistent<FunctionTemplate>::New(t);
        classTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        classTemplate->SetClassName(String::NewSymbol("Date"));
        
        // static class methods
//        SET_METHOD(classTemplate, "float", DateWrap::Float);
        
        // instance methods (e.g. working with an instance of var r = new Date; r.X())
        SET_PROTOTYPE_METHOD(classTemplate, "getTime", DateWrap::ITime);
        
        target->Set(String::NewSymbol("Date"), classTemplate->GetFunction());
    }
    
    static Handle<Value> New(const Arguments& args)
    {
        HandleScope scope;
        DateWrap *t = new DateWrap();
        t->Wrap(args.This());
        return args.This();
    }
    
private:
    //! the classTemplate template
    static Persistent<FunctionTemplate> classTemplate;
    
    // -- static class methods --
//    static Handle<Value> Float(Arguments const& args) 
//    {
//        float a = 0.0;
//        float b = 1.0;
//        
//        if(args.Length() >= 1)
//            a = args[0]->NumberValue();
//        
//        if(args.Length() >= 2)
//            b = args[1]->NumberValue();
//        
//        return Number::New(ci::Rand::randFloat(a,b));
//    }
    
    
    // -- instance methods --
    
    // returns the current time of the ptime object as unixtime
    static Handle<Value> ITime(Arguments const& args)
    {
        HandleScope scope;
        DateWrap* wrap = ObjectWrap::Unwrap<DateWrap>(args.This());
        
        static ptime epoch(boost::gregorian::date(1970, 1, 1));
        time_duration diff(wrap->now - epoch);
        int64_t unixtime = (diff.ticks() / diff.ticks_per_second());
        
        return scope.Close( Number::New(unixtime) );
    }
    
    
    //! Date number generator used by Date class instances
    boost::posix_time::ptime now;
    
    DateWrap()
    {
        now = second_clock::local_time();
    }
    
    ~DateWrap() {}
    
};

Persistent<FunctionTemplate> DateWrap::classTemplate;

void fieldkit::script::Date::Initialize(Handle<Object> target) 
{
    DateWrap::Initialize(target);
}

} } // namespace fieldkit::script
