/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/05/2011.
 */

#include "cinder/Perlin.h"

#include "fieldkit/script/module/Perlin.h"
#include "fieldkit/script/ObjectWrap.h"
#include "fieldkit/math/MathKit.h"

namespace fieldkit { namespace script {

    class PerlinWrap : ObjectWrap {
    public:
        static void Initialize(Handle<Object> target)
        {
            const char* className = "Perlin";
            
            Local<FunctionTemplate> t = FunctionTemplate::New(PerlinWrap::New);
            classTemplate = Persistent<FunctionTemplate>::New(t);
            classTemplate->InstanceTemplate()->SetInternalFieldCount(1);
            classTemplate->SetClassName(String::NewSymbol(className));
           
            // methods
            SET_PROTOTYPE_METHOD(classTemplate, "setSeed", PerlinWrap::SetSeed);
            SET_PROTOTYPE_METHOD(classTemplate, "setOctaves", PerlinWrap::SetOctaves);
            
            SET_PROTOTYPE_METHOD(classTemplate, "noise", PerlinWrap::Noise);    
            SET_PROTOTYPE_METHOD(classTemplate, "dnoise", PerlinWrap::DNoise);    
            SET_PROTOTYPE_METHOD(classTemplate, "fBm", PerlinWrap::FBm);
            SET_PROTOTYPE_METHOD(classTemplate, "dfBm", PerlinWrap::DfBm);
            
            target->Set(String::NewSymbol(className), classTemplate->GetFunction());
        }
        
        static Handle<Value> New(const Arguments& args)
        {
            HandleScope scope;
            PerlinWrap *t = new PerlinWrap();
            t->Wrap(args.This());
            return args.This();
        }
        
    private:
        //! the classTemplate template
        static Persistent<FunctionTemplate> classTemplate;
        
        ci::Perlin impl_;
        
        PerlinWrap() {}
        ~PerlinWrap() {}
        
        
        // -- Helper functions --
        static ci::Perlin& Impl(Arguments const& args)
        {
            PerlinWrap* wrap = ObjectWrap::Unwrap<PerlinWrap>(args.This());
            return wrap->impl_;
        }
        
        
        // -- Noise control --
        
        // args: seed
        static Handle<Value> SetSeed(Arguments const& args) 
        {
            if(args.Length() != 1) return Undefined(); 
            HandleScope handleScope;
            Impl(args).setSeed(args[0]->Int32Value());
            return Undefined();
        }
        
        // args: seed
        static Handle<Value> SetOctaves(Arguments const& args) 
        {
            if(args.Length() != 1) return Undefined(); 
            HandleScope handleScope;
            Impl(args).setOctaves(args[0]->Int32Value());
            return Undefined();
        }
        
        // args: x | x,y, | x,y,z
        static Handle<Value> Noise(Arguments const& args) 
        {
            HandleScope handleScope;
            
            float result = 0;
            switch(args.Length()) {
                case 1:
                    result = Impl(args).noise(args[0]->NumberValue());
                    break;
                
                case 2:
                    result = Impl(args).noise(args[0]->NumberValue(), args[1]->NumberValue());
                    break;
                    
                case 3:
                    result = Impl(args).noise(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
                    break;
            }
            return Number::New(result);
        }
        
        // args: x | x, y 
        // return: {x,} or {x,y,z}
        static Handle<Value> DNoise(Arguments const& args) 
        {
            HandleScope handleScope;
            
            int argc = args.Length();
            if(argc == 2) {
                    Vec2f result = Impl(args).dnoise(args[0]->NumberValue(), args[1]->NumberValue());
                    
                    Local<Object> obj = Object::New();
                    obj->Set(String::New("x"), Number::New(result.x));
                    obj->Set(String::New("y"), Number::New(result.y));

                    return handleScope.Close(obj);
                    
            } else if(argc == 3) {
                    Vec3f result = Impl(args).dnoise(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
                    
                    Local<Object> obj = Object::New();
                    obj->Set(String::New("x"), Number::New(result.x));
                    obj->Set(String::New("y"), Number::New(result.y));
                    obj->Set(String::New("z"), Number::New(result.z));
                    
                    return handleScope.Close(obj);
            }
            
            return Undefined();
        }
        
        // args: x | x,y, | x,y,z
        static Handle<Value> FBm(Arguments const& args) 
        {
            HandleScope handleScope;
            
            float result = 0;
            switch(args.Length()) {
                case 1:
                    result = Impl(args).fBm(args[0]->NumberValue());
                    break;
                    
                case 2:
                    result = Impl(args).fBm(args[0]->NumberValue(), args[1]->NumberValue());
                    break;
                    
                case 3:
                    result = Impl(args).fBm(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
                    break;
            }
            return Number::New(result);
        }
        
        // args: x | x, y 
        // return: {x,} or {x,y,z}
        static Handle<Value> DfBm(Arguments const& args) 
        {
            HandleScope handleScope;
            
            int argc = args.Length();
            if(argc == 2) {
                Vec2f result = Impl(args).dfBm(args[0]->NumberValue(), args[1]->NumberValue());
                
                Local<Object> obj = Object::New();
                obj->Set(String::New("x"), Number::New(result.x));
                obj->Set(String::New("y"), Number::New(result.y));
                
                return handleScope.Close(obj);
                
            } else if(argc == 3) {
                Vec3f result = Impl(args).dfBm(args[0]->NumberValue(), args[1]->NumberValue(), args[2]->NumberValue());
                
                Local<Object> obj = Object::New();
                obj->Set(String::New("x"), Number::New(result.x));
                obj->Set(String::New("y"), Number::New(result.y));
                obj->Set(String::New("z"), Number::New(result.z));
                
                return handleScope.Close(obj);
            }
            
            return Undefined();
        }

    };
    
    Persistent<FunctionTemplate> PerlinWrap::classTemplate;
    
    void Perlin::Initialize(v8::Handle<v8::Object> target)
    {
         PerlinWrap::Initialize(target);
    }
} }
