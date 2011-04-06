/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/03/2011.
 */

#include "fieldkit/script/module/Random.h"
#include "fieldkit/script/ObjectWrap.h"
#include "cinder/Rand.h"

using namespace fieldkit::script;

class RandomWrap : ObjectWrap {
public:
    static void Initialize(Handle<Object> target)
    {
        Local<FunctionTemplate> t = FunctionTemplate::New(RandomWrap::New);
        
        classTemplate = Persistent<FunctionTemplate>::New(t);
        classTemplate->InstanceTemplate()->SetInternalFieldCount(1);
        classTemplate->SetClassName(String::NewSymbol("Random"));
        
        // static class methods
        SET_METHOD(classTemplate, "float", RandomWrap::Float);
        SET_METHOD(classTemplate, "int", RandomWrap::Int);
        SET_METHOD(classTemplate, "bool", RandomWrap::Bool);
        
        // instance methods (e.g. working with an instance of var r = new Random; r.X())
        SET_PROTOTYPE_METHOD(classTemplate, "seed", RandomWrap::ISeed);
        SET_PROTOTYPE_METHOD(classTemplate, "randomize", RandomWrap::IRandomize);
        SET_PROTOTYPE_METHOD(classTemplate, "float", RandomWrap::IFloat);
        SET_PROTOTYPE_METHOD(classTemplate, "int", RandomWrap::IInt);
        SET_PROTOTYPE_METHOD(classTemplate, "bool", RandomWrap::IBool);
        
        target->Set(String::NewSymbol("Random"), classTemplate->GetFunction());
    }
    
    static Handle<Value> New(const Arguments& args)
    {
        HandleScope scope;
        RandomWrap *t = new RandomWrap();
        t->Wrap(args.This());
        return args.This();
    }
    
private:
    //! the classTemplate template
    static Persistent<FunctionTemplate> classTemplate;
    
    // -- static class methods --
    static Handle<Value> Float(Arguments const& args) 
    {
        float a = 0.0;
        float b = 1.0;
        
        if(args.Length() >= 1)
            a = args[0]->NumberValue();
        
        if(args.Length() >= 2)
            b = args[1]->NumberValue();
        
        return Number::New(ci::Rand::randFloat(a,b));
    }
    
    static Handle<Value> Int(Arguments const& args) 
    {
        int a = 0;
        int b = 1;
        
        if(args.Length() >= 1)
            a = args[0]->NumberValue();
        
        if(args.Length() >= 2)
            b = args[1]->NumberValue();
        
        return Number::New(ci::Rand::randInt(a,b));
    }
    
    static Handle<Value> Bool(Arguments const& args) 
    {
        return Number::New(ci::Rand::randBool());
    }
    
    
    // -- instance methods --
    static Handle<Value> ISeed(Arguments const& args)
    {
        if(args.Length() == 1) {
            HandleScope scope;
            RandomWrap* wrap = ObjectWrap::Unwrap<RandomWrap>(args.This());
            uint32_t seed = args[0]->Int32Value();
            wrap->rand.seed(seed);
        }
        return Undefined();
    }
    
    static Handle<Value> IRandomize(Arguments const& args)
    {
        HandleScope scope;
        RandomWrap* wrap = ObjectWrap::Unwrap<RandomWrap>(args.This());
        wrap->rand.randomize();
        return Undefined();
    }

    static Handle<Value> IFloat(Arguments const& args)
    {
        float a = 0.0;
        float b = 1.0;
        
        if(args.Length() >= 1)
            a = args[0]->NumberValue();
        
        if(args.Length() >= 2)
            b = args[1]->NumberValue();
        
        HandleScope scope;
        RandomWrap* wrap = ObjectWrap::Unwrap<RandomWrap>(args.This());
        float value = wrap->rand.nextFloat(a,b);
        return scope.Close(Number::New(value));
    }
    
    static Handle<Value> IInt(Arguments const& args)
    {
        int a = 0;
        int b = 1;
        
        if(args.Length() >= 1)
            a = args[0]->NumberValue();
        
        if(args.Length() >= 2)
            b = args[1]->NumberValue();
        
        HandleScope scope;
        int value = a;
        if(a != b) {
            RandomWrap* wrap = ObjectWrap::Unwrap<RandomWrap>(args.This());
            value = wrap->rand.nextInt(a,b);
        }
        return scope.Close(Number::New(value));
    }
    
    static Handle<Value> IBool(Arguments const& args)
    {
        HandleScope scope;
        RandomWrap* wrap = ObjectWrap::Unwrap<RandomWrap>(args.This());
        bool value = wrap->rand.nextBool();
        return scope.Close(Number::New(value));
    }
    
    //! Random number generator used by Random class instances
    ci::Rand rand;
    
    RandomWrap()
    {
        rand.randomize();
    }
    
    ~RandomWrap() {}
    
};

Persistent<FunctionTemplate> RandomWrap::classTemplate;

void Random::Initialize(Handle<Object> target) 
{
    RandomWrap::Initialize(target);
}
