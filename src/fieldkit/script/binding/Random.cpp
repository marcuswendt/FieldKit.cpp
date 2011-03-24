/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/03/2011.
 */

#include "fieldkit/script/binding/Random.h"
#include "cinder/Rand.h"

using namespace fieldkit::script;

v8::Handle<Value> RandFloat(Arguments const& args) 
{
    float a = 0.0;
    float b = 1.0;
    
    if(args.Length() >= 1)
        a = args[0]->NumberValue();
    
    if(args.Length() >= 2)
        b = args[1]->NumberValue();
    
    return v8::Number::New(ci::Rand::randFloat(a,b));
}

v8::Handle<Value> RandInt(Arguments const& args) 
{
    int a = 0;
    int b = 1;

    if(args.Length() >= 1)
    a = args[0]->NumberValue();

    if(args.Length() >= 2)
    b = args[1]->NumberValue();

    return v8::Number::New(ci::Rand::randInt(a,b));
}

v8::Handle<Value> RandBool(Arguments const& args) 
{
	return v8::Number::New(ci::Rand::randBool());
}


void Random::attach(Handle<ObjectTemplate> global) 
{
    // create Rand object template
    Handle<ObjectTemplate> holder = ObjectTemplate::New();
    global->Set(String::New("random"), holder);
    
    // assign methods to object
    holder->Set(String::New("float"), FunctionTemplate::New(RandFloat));
    holder->Set(String::New("int"), FunctionTemplate::New(RandInt));
    holder->Set(String::New("bool"), FunctionTemplate::New(RandBool));
}
