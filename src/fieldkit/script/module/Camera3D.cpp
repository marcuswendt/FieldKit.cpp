/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 20/05/2011.
 */

#include "fieldkit/script/module/Camera3D.h"

#include "cinder/app/App.h"
#include "cinder/gl/GL.h"
#include "cinder/Camera.h"

#include "fieldkit/Logger.h"
#include "fieldkit/math/MathKit.h"
#include "fieldkit/script/ObjectWrap.h"

namespace fieldkit { namespace script {
    
    using ci::Camera;
    
    // -- C++ to V8 wrapper ----------------------------------------------------
    class Camera3DWrap : ObjectWrap {
    public:
        static void Initialize(Handle<Object> target)
        {
            const char* className = "Camera3D";
            
            Local<FunctionTemplate> t = FunctionTemplate::New(Camera3DWrap::New);
            classTemplate = Persistent<FunctionTemplate>::New(t);
            classTemplate->InstanceTemplate()->SetInternalFieldCount(1);
            classTemplate->SetClassName(String::NewSymbol(className));
            
            // Camera
            SET_PROTOTYPE_METHOD(classTemplate, "getPointer", Camera3DWrap::GetPointer);
            
            SET_PROTOTYPE_METHOD(classTemplate, "setPerspective", Camera3DWrap::SetPerspective);
            
            SET_PROTOTYPE_METHOD(classTemplate, "apply", Camera3DWrap::SetMatrices);
            SET_PROTOTYPE_METHOD(classTemplate, "setMatrices", Camera3DWrap::SetMatrices);
            SET_PROTOTYPE_METHOD(classTemplate, "setMatricesWindow", Camera3DWrap::SetMatricesWindow);
            SET_PROTOTYPE_METHOD(classTemplate, "lookAt", Camera3DWrap::LookAt);
            
            SET_PROTOTYPE_METHOD(classTemplate, "getBillboardVectors", Camera3DWrap::GetBillboardVectors);
            
            SET_PROTOTYPE_METHOD(classTemplate, "getFov", Camera3DWrap::GetFov);
            SET_PROTOTYPE_METHOD(classTemplate, "setFov", Camera3DWrap::SetFov);
            
            SET_PROTOTYPE_METHOD(classTemplate, "getNearClip", Camera3DWrap::GetNearClip);
            SET_PROTOTYPE_METHOD(classTemplate, "setNearClip", Camera3DWrap::SetNearClip);
            SET_PROTOTYPE_METHOD(classTemplate, "getFarClip", Camera3DWrap::GetFarClip);
            SET_PROTOTYPE_METHOD(classTemplate, "setFarClip", Camera3DWrap::SetFarClip);
            
            target->Set(String::NewSymbol(className), classTemplate->GetFunction());
        }
        
        static Handle<Value> New(const Arguments& args)
        {
            HandleScope scope;
            
            bool isPersp = true;
            if(args.Length() == 1)
                args[0]->BooleanValue();
            
            Camera3DWrap *t = new Camera3DWrap(isPersp);
            t->Wrap(args.This());
            return args.This();
        }
        
    private:
        //! the classTemplate template
        static Persistent<FunctionTemplate> classTemplate;
        
        ci::Camera* impl_;
        
        Camera3DWrap(bool isPersp) 
        {
            if(isPersp)
                impl_ = new ci::CameraPersp();
            else
                impl_ = new ci::CameraOrtho();
        }
        ~Camera3DWrap() {}
        
        
        // -- Helper functions --
        static ci::Camera* Impl(Arguments const& args)
        {
            Camera3DWrap* wrap = ObjectWrap::Unwrap<Camera3DWrap>(args.This());
            return wrap->impl_;
        }
        
        static Vec3f UnwrapVec3(Handle<Value> arg)
        {
            Handle<Object> obj = Handle<Object>::Cast(arg);
            return Vec3f(
                obj->Get(String::New("x"))->NumberValue(),
                obj->Get(String::New("y"))->NumberValue(),
                obj->Get(String::New("z"))->NumberValue()
            );
        }
        
        static void SetVec3(Handle<Value> arg, Vec3f const& v)
        {
            Handle<Object> obj = Handle<Object>::Cast(arg); 
            obj->Set(String::New("x"), Number::New(v.x));
            obj->Set(String::New("y"), Number::New(v.y));
            obj->Set(String::New("z"), Number::New(v.z));
        }
        
        
        // -- Camera control functions --
        
        // returns: int address
        static Handle<Value> GetPointer(Arguments const& args) 
        {
            Camera* cam = Impl(args);
            int address = (int)(cam);
            return Integer::New(address);
        }
        
        // args: horizFovDegrees, aspectRatio, nearPlane, farPlane 
        static Handle<Value> SetPerspective(Arguments const& args) 
        {
            Camera* cam = Impl(args);
            if(args.Length() != 4 || !cam->isPersp()) return Undefined();

            HandleScope handleScope;
            static_cast<ci::CameraPersp*>(cam)->setPerspective( 
                args[0]->NumberValue(),
                args[1]->NumberValue(),
                args[2]->NumberValue(),
                args[3]->NumberValue()
            );
            
            return Undefined();
        }
        
        // args: none
        static Handle<Value> SetMatrices(Arguments const& args) 
        {
            HandleScope handleScope;
            ci::gl::setMatrices(*Impl(args));
            return Undefined();
        }

        // args: bool isPersp
        static Handle<Value> SetMatricesWindow(Arguments const& args) 
        {
            HandleScope handleScope;
            
            bool isPersp = true;
            if(args.Length() == 1)
                isPersp = args[0]->BooleanValue();
            
            if(isPersp) {
                ci::gl::setMatricesWindowPersp(ci::app::getWindowSize());
                
            } else {
                ci::gl::setMatricesWindow(ci::app::getWindowSize());
            }
            
            return Undefined();
        }
        
        // args: Vec3 eye, Vec3 target, Vec3 up
        static Handle<Value> LookAt(Arguments const& args) 
        {            
            HandleScope handleScope;
            
            Camera* c = Impl(args);            
            switch(args.Length()) {
                case 1:
                    c->lookAt( UnwrapVec3(args[0]) );
                    break;
                    
                case 2:
                    c->lookAt( UnwrapVec3(args[0]), UnwrapVec3(args[1]) );
                    break;
                    
                case 3:
                    c->lookAt( UnwrapVec3(args[0]), UnwrapVec3(args[1]), UnwrapVec3(args[2]) );
                    break;
                    
            }
            
            return Undefined();
        }
        
        // args: Vec3 right, Vec3 up
        static Handle<Value> GetBillboardVectors(Arguments const& args) 
        {            
            if(args.Length() != 2) return Undefined();
            
            HandleScope handleScope;
        
            Vec3f right, up;
            Impl(args)->getBillboardVectors(&right, &up);
            
            SetVec3(args[0], right);
            SetVec3(args[1], up);
            
            return Undefined();
        }
        
        // args: none returns: float
        static Handle<Value> GetFov(Arguments const& args) 
        {            
            HandleScope handleScope;
            float value = Impl(args)->getFov();
            return Number::New(value);
        }

        // args: value
        static Handle<Value> SetFov(Arguments const& args) 
        {            
            if(args.Length() != 1) return Undefined();
            HandleScope handleScope;
            float value = args[0]->NumberValue();
            Impl(args)->setFov(value);
            return Undefined();
        }

        // args: none returns: float
        static Handle<Value> GetNearClip(Arguments const& args) 
        {            
            HandleScope handleScope;
            float value = Impl(args)->getNearClip();
            return Number::New(value);
        }
        
        // args: value
        static Handle<Value> SetNearClip(Arguments const& args) 
        {            
            if(args.Length() != 1) return Undefined();
            HandleScope handleScope;
            float value = args[0]->NumberValue();
            Impl(args)->setNearClip(value);
            return Undefined();
        }

        // args: none returns: float
        static Handle<Value> GetFarClip(Arguments const& args) 
        {            
            HandleScope handleScope;
            float value = Impl(args)->getFarClip();
            return Number::New(value);
        }
        
        // args: value
        static Handle<Value> SetFarClip(Arguments const& args) 
        {            
            if(args.Length() != 1) return Undefined();
            HandleScope handleScope;
            float value = args[0]->NumberValue();
            Impl(args)->setFarClip(value);
            return Undefined();
        }

    };
    
    Persistent<FunctionTemplate> Camera3DWrap::classTemplate;
    
    // -- Bindings -------------------------------------------------------------
    void Camera3D::Initialize(v8::Handle<v8::Object> target) 
    {
        Camera3DWrap::Initialize(target);
    }
    
} } // namespace fieldkit::script
