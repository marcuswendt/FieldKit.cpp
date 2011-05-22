/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 23/03/2011.
 */

#include "fieldkit/script/module/Graphics2D.h"

#include "cinder/Color.h"
#include "cinder/gl/GL.h"
#include "cinder/Font.h"

#include "fieldkit/Logger.h"
#include "fieldkit/math/MathKit.h"
#include "fieldkit/script/ObjectWrap.h"

namespace fieldkit { namespace script {
    
    using ci::ColorA;
    
    class Graphics2DImpl {
    public:
        bool strokeEnabled_;
        ColorA stroke_;
        
        bool fillEnabled_;
        ColorA fill_;

        ci::Font font_;
        
        Graphics2DImpl() 
        {
            strokeEnabled_ = false;
            stroke_ = ColorA(1,1,1,1);
            
            fillEnabled_ = true;
            fill_ = ColorA(1,1,1,1);
            
            font_ = ci::Font("Arial", 12);
        }
        
        // -- Color ------------------------------------------------------------
        void Background(ColorA color)
        {
            glClearColor(color.r, color.g, color.b, color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        
        void Stroke(ColorA color)
        {
            strokeEnabled_ = true;
            stroke_ = color;
        }
        
        void NoStroke()
        {
            strokeEnabled_ = false;
        }
        
        void Fill(ColorA color)
        {
            fillEnabled_ = true;
            fill_ = color;
        }

        void NoFill()
        {
            fillEnabled_ = false;
        }

        void StrokeWeight(float weight)
        {
            glLineWidth(weight);
        }

        // -- Shapes -----------------------------------------------------------
        void Ellipse(float x, float y, float w, float h)
        {
            int numSamples = 6 + (int)(abs(w)*0.45); // abs in case w is negative
            if(numSamples > 64) numSamples = 64;
            
            int numVertices = numSamples + 1;
            
            GLfloat* textureCoords = new GLfloat[numVertices * 2];
            GLfloat* vertices = new GLfloat[numVertices * 2];
            
            // center point
            textureCoords[0] = 0.5f;
            textureCoords[1] = 0.5f;
            vertices[0] = x;
            vertices[1] = y;
            
            int index=2;
            for(int i=0; i < numSamples; i++) {
                float t = i / static_cast<float>(numSamples-1);
                float s = sin(t * 2 * M_PI);
                float c = cos(t * 2 * M_PI);
                
                textureCoords[index] = (s + 1.0) * 0.5;
                vertices[index++] = x + s * w;
                
                textureCoords[index] = (c + 1.0) * 0.5;
                vertices[index++] = y + c * h;        
            }
            
            //    polygon(GL_TRIANGLE_FAN, numVertices, textureCoords, vertices);
            
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY); 
            glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            
            if(fillEnabled_) {
                glColor4f(fill_.r, fill_.g, fill_.b, fill_.a);
                glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
            }
            
            if(strokeEnabled_) {
                glColor4f(stroke_.r, stroke_.g, stroke_.b, stroke_.a);
                glDrawArrays(GL_LINE_LOOP, 1, numVertices-1);
            }
            
            // cleanup
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);    

			delete[] textureCoords;
			delete[] vertices;
        }
        
        void Line(float x1, float y1, float z1, float x2, float y2, float z2)
        {
            if(!strokeEnabled_) return;
            
            glColor4f(stroke_.r, stroke_.g, stroke_.b, stroke_.a);
            glBegin(GL_LINES);
            glVertex3f(x1, y1, z1);
            glVertex3f(x2, y2, z2);
            glEnd();
        }
        
        void Point(float x, float y, float z)
        {
            if(!strokeEnabled_) return;
            
            glColor4f(stroke_.r, stroke_.g, stroke_.b, stroke_.a);
            glBegin(GL_POINTS);
            glVertex3f(x, y, z);
            glEnd();
        }
        
        void Rect(float x1, float y1, float w, float h)
        {
            float x2 = x1 + w;
            float y2 = y1 + h;
            
            GLfloat textureCoords[] = {
                0, 0,
                0, 1,
                1, 0,
                1, 1
            };
            
            GLfloat vertices[] = {
                x1, y1, // lower left
                x2, y1, // lower right
                x2, y2,  // upper right
                x1, y2 // upper left
            };
            
            Polygon(GL_QUADS, 4, textureCoords, vertices);
        }
        
        void Polygon(GLenum mode, int numVertices, float* textureCoords, float* vertices)
        {
            // upload data
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glEnableClientState(GL_VERTEX_ARRAY); 
            glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
            glVertexPointer(2, GL_FLOAT, 0, vertices);
            
            if(fillEnabled_) {
                glColor4f(fill_.r, fill_.g, fill_.b, fill_.a);
                glDrawArrays(mode, 0, numVertices);
            }
            
            if(strokeEnabled_) {
                glColor4f(stroke_.r, stroke_.g, stroke_.b, stroke_.a);
                glDrawArrays(GL_LINE_LOOP, 0, numVertices);
            }
            
            // cleanup
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);    
        }
        
        void Box(float w, float h, float d)
        {
            Vec3f center(0,0,0);
            Vec3f size(w,h,d);
            
            if(fillEnabled_) {
                glColor4f(fill_.r, fill_.g, fill_.b, fill_.a);
                ci::gl::drawCube(center, size);
            }
            
            if(strokeEnabled_) {
                glColor4f(stroke_.r, stroke_.g, stroke_.b, stroke_.a);
                ci::gl::drawStrokedCube(center, size);
            }
        }

        void Sphere(float radius)
        {
            ci::gl::drawSphere(Vec3f(0,0,0), radius);
        }
        
        // -- Text -------------------------------------------------------------
        void TextFont(std::string const& name, float size)
        {
            font_ = ci::Font(name, size);
        }
        
        void Text(std::string const& text, float x, float y)
        {
            ci::gl::drawString(text, Vec2f(x,y), fill_, font_);
        }
        
        
        // -- Transform --------------------------------------------------------
        void PopMatrix()
        {
            glPopMatrix();
        }
        
        void PushMatrix()
        {
            glPushMatrix();
        }
        
        void Rotate(float angle, float x, float y, float z)
        {
            glRotatef(angle, x, y, z);
        }
        
        void Scale(float x, float y, float z)
        {
            glScalef(x,y,z);
        }
        
        void Translate(float x, float y, float z)
        {
            glTranslatef(x,y,z);
        }
        
        
        // -- Camera -----------------------------------------------------------
        void Camera(Vec3f const& eye, Vec3f const& center, Vec3f const& up)
        {
            // TODO needs implementing
        }        
        
        
        // -- OpenGL -----------------------------------------------------------
        void EnableDepthTest()
        {
            glEnable(GL_DEPTH_TEST);
        }
        
        void DisableDepthTest()
        {
            glDisable(GL_DEPTH_TEST);
        }
        
        void EnableAlphaBlending(bool premultiplied=false)
        {
            glEnable(GL_BLEND);
            if(!premultiplied)
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            else
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }
        
        void DisableAlphaBlending()
        {
            glDisable(GL_BLEND);
        }
        
        void PointSize(float size)
        {
            glPointSize(size);
        }
    };
    
    
    // -- C++ to V8 wrapper ----------------------------------------------------
    class Graphics2DWrap : ObjectWrap {
    public:
        static void Initialize(Handle<Object> target)
        {
            const char* className = "Graphics2D";
            
            Local<FunctionTemplate> t = FunctionTemplate::New(Graphics2DWrap::New);
            classTemplate = Persistent<FunctionTemplate>::New(t);
            classTemplate->InstanceTemplate()->SetInternalFieldCount(1);
            classTemplate->SetClassName(String::NewSymbol(className));
            
//            // static class methods
//            SET_METHOD(classTemplate, "float", RandomWrap::Float);

            // Color
            SET_PROTOTYPE_METHOD(classTemplate, "background", Graphics2DWrap::Background);
            SET_PROTOTYPE_METHOD(classTemplate, "stroke", Graphics2DWrap::Stroke);
            SET_PROTOTYPE_METHOD(classTemplate, "noStroke", Graphics2DWrap::NoStroke);
            SET_PROTOTYPE_METHOD(classTemplate, "fill", Graphics2DWrap::Fill);
            SET_PROTOTYPE_METHOD(classTemplate, "noFill", Graphics2DWrap::NoFill);

            SET_PROTOTYPE_METHOD(classTemplate, "strokeWeight", Graphics2DWrap::StrokeWeight);
            
            // 2D Primitives
            SET_PROTOTYPE_METHOD(classTemplate, "ellipse", Graphics2DWrap::Ellipse);
            SET_PROTOTYPE_METHOD(classTemplate, "line", Graphics2DWrap::Line);
            SET_PROTOTYPE_METHOD(classTemplate, "point", Graphics2DWrap::Point);
            SET_PROTOTYPE_METHOD(classTemplate, "rect", Graphics2DWrap::Rect);
            
            // 3D Primitives
            SET_PROTOTYPE_METHOD(classTemplate, "box", Graphics2DWrap::Box);
            SET_PROTOTYPE_METHOD(classTemplate, "sphere", Graphics2DWrap::Sphere);
            
            // Text
            SET_PROTOTYPE_METHOD(classTemplate, "textFont", Graphics2DWrap::TextFont);
            SET_PROTOTYPE_METHOD(classTemplate, "text", Graphics2DWrap::Text);
            
            // Transform
            SET_PROTOTYPE_METHOD(classTemplate, "popMatrix", Graphics2DWrap::PopMatrix);
            SET_PROTOTYPE_METHOD(classTemplate, "pushMatrix", Graphics2DWrap::PushMatrix);
            SET_PROTOTYPE_METHOD(classTemplate, "scale", Graphics2DWrap::Scale);
            SET_PROTOTYPE_METHOD(classTemplate, "rotate", Graphics2DWrap::Rotate);
            SET_PROTOTYPE_METHOD(classTemplate, "translate", Graphics2DWrap::Translate);
            
            // Camera
            SET_PROTOTYPE_METHOD(classTemplate, "camera", Graphics2DWrap::Camera);
            
            // GL
            SET_PROTOTYPE_METHOD(classTemplate, "enableAlphaBlending", Graphics2DWrap::EnableAlphaBlending);
            SET_PROTOTYPE_METHOD(classTemplate, "disableAlphaBlending", Graphics2DWrap::DisableAlphaBlending);
            SET_PROTOTYPE_METHOD(classTemplate, "enableDepthTest", Graphics2DWrap::EnableDepthTest);
            SET_PROTOTYPE_METHOD(classTemplate, "disableDepthTest", Graphics2DWrap::DisableDepthTest);
            SET_PROTOTYPE_METHOD(classTemplate, "pointSize", Graphics2DWrap::PointSize);
            
            target->Set(String::NewSymbol(className), classTemplate->GetFunction());
        }
        
        static Handle<Value> New(const Arguments& args)
        {
            HandleScope scope;
            Graphics2DWrap *t = new Graphics2DWrap();
            t->Wrap(args.This());
            return args.This();
        }
        
    private:
        //! the classTemplate template
        static Persistent<FunctionTemplate> classTemplate;
        
        Graphics2DImpl impl_;
        
        Graphics2DWrap() {}
        ~Graphics2DWrap() {}
        
        // -- static class methods --
        static Graphics2DImpl& Impl(Arguments const& args)
        {
            Graphics2DWrap* wrap = ObjectWrap::Unwrap<Graphics2DWrap>(args.This());
            return wrap->impl_;
        }
        
        static ColorA ColorFromArgs(Arguments const& args)
        {
            float r=0, g=0, b=0, a=0;
            float grey;
            
            switch(args.Length()) {
                case 1:
                    grey = args[0]->NumberValue();
                    r = grey;
                    g = grey;
                    b = grey;
                    a = 1.0;
                    break;
                    
                case 2:
                    grey = args[0]->NumberValue();
                    r = grey;
                    g = grey;
                    b = grey;
                    a = args[1]->NumberValue();
                    break;
                    
                case 3:
                    r = args[0]->NumberValue();
                    g = args[1]->NumberValue();
                    b = args[2]->NumberValue();
                    a = 1.0;
                    break;
                    
                case 4:
                    r = args[0]->NumberValue();
                    g = args[1]->NumberValue();
                    b = args[2]->NumberValue();
                    a = args[3]->NumberValue();
                    break;
            }
            
            return ColorA(r,g,b,a);
        }
        
        static Handle<Value> Background(Arguments const& args) 
        {
            Impl(args).Background(ColorFromArgs(args));
            return Undefined();
        }
        
        static Handle<Value> Stroke(Arguments const& args) 
        {
            Impl(args).Stroke(ColorFromArgs(args));
            return Undefined();
        }
        
        static Handle<Value> NoStroke(Arguments const& args) 
        {
            Impl(args).NoStroke();
            return Undefined();
        }
        
        static Handle<Value> Fill(Arguments const& args) 
        {
            Impl(args).Fill(ColorFromArgs(args));
            return Undefined();
        }
        
        static Handle<Value> NoFill(Arguments const& args) 
        {
            Impl(args).NoFill();
            return Undefined();
        }
        
        static Handle<Value> StrokeWeight(Arguments const& args) 
        {
            if(args.Length() == 1) {
                Impl(args).StrokeWeight(args[0]->NumberValue());
            }
            return Undefined();
        }
        
        
        // -- Shape Wrappers ---------------------------------------------------
        static Handle<Value> Ellipse(Arguments const& args) 
        {
            if(args.Length() == 4) {            
                float x = args[0]->NumberValue();
                float y = args[1]->NumberValue();
                float w = args[2]->NumberValue();
                float h = args[3]->NumberValue();
                Impl(args).Ellipse(x, y, w, h);
            }
            return Undefined();
        }
        
        static Handle<Value> Point(Arguments const& args) 
        {
            if(args.Length() == 2) {            
                float x = args[0]->NumberValue();
                float y = args[1]->NumberValue();
                Impl(args).Point(x, y, 0);
                
            } else if(args.Length() == 3) {            
                float x = args[0]->NumberValue();
                float y = args[1]->NumberValue();
                float z = args[2]->NumberValue();
                Impl(args).Point(x, y, z);
            }
            return Undefined();
        }
        
        static Handle<Value> Line(Arguments const& args) 
        {
            if(args.Length() == 4) {            
                float x1 = args[0]->NumberValue();
                float y1 = args[1]->NumberValue();
                float x2 = args[2]->NumberValue();
                float y2 = args[3]->NumberValue();
                Impl(args).Line(x1, y1, 0, x2, y2, 0);
                
            } else if(args.Length() == 6) {            
                float x1 = args[0]->NumberValue();
                float y1 = args[1]->NumberValue();
                float z1 = args[2]->NumberValue();
                float x2 = args[3]->NumberValue();
                float y2 = args[4]->NumberValue();
                float z2 = args[5]->NumberValue();
                Impl(args).Line(x1, y1, z1, x2, y2, z2);
            }
            return Undefined();
        }
        
        static Handle<Value> Rect(Arguments const& args) 
        {
            if(args.Length() == 4) {            
                float x = args[0]->NumberValue();
                float y = args[1]->NumberValue();
                float w = args[2]->NumberValue();
                float h = args[3]->NumberValue();
                Impl(args).Rect(x, y, w, h);
            }
            return Undefined();
        }
        
        static Handle<Value> Box(Arguments const& args) 
        {
            if(args.Length() == 1) {
                float s = args[0]->NumberValue();
                Impl(args).Box(s,s,s);
                
            } else if(args.Length() == 3) {
                float w = args[0]->NumberValue();
                float h = args[1]->NumberValue();
                float d = args[2]->NumberValue();
                Impl(args).Box(w, h, d);
            }
            return Undefined();
        }
        
        static Handle<Value> Sphere(Arguments const& args) 
        {
            if(args.Length() == 1) {
                Impl(args).Sphere(args[0]->NumberValue());
            }
            return Undefined();
        }
        
        
        // -- Text Wrappers ----------------------------------------------------
        static Handle<Value> TextFont(Arguments const& args) 
        {
            if(args.Length() == 2) {
                std::string name = *String::AsciiValue(args[0]);
                float size = args[1]->NumberValue();
                Impl(args).TextFont(name, size);
            }
            return Undefined();
        }
        
        static Handle<Value> Text(Arguments const& args) 
        {
            if(args.Length() == 3) {
                std::string text = *String::AsciiValue(args[0]);
                float x = args[1]->NumberValue();
                float y = args[2]->NumberValue();
                Impl(args).Text(text, x, y);
            }
            return Undefined();
        }
        
        // -- Transform --------------------------------------------------------
        static Handle<Value> PopMatrix(Arguments const& args) 
        {
            Impl(args).PopMatrix();
            return Undefined();
        }
        
        static Handle<Value> PushMatrix(Arguments const& args) 
        {
            Impl(args).PushMatrix();
            return Undefined();
        }
        
        static Handle<Value> Scale(Arguments const& args) 
        {
            float tmp;
            switch(args.Length()) {
                // scale x,y,z
                case 1:                          
                    tmp = args[0]->NumberValue();
                    Impl(args).Scale(tmp, tmp, tmp);
                    break;
                    
                // scale x,y
                case 2:
                    Impl(args).Scale(args[0]->NumberValue(), 
                                     args[1]->NumberValue(), 
                                     1.0);
                    break;
                    
                case 3:
                    Impl(args).Scale(args[0]->NumberValue(), 
                                     args[1]->NumberValue(), 
                                     args[2]->NumberValue());
                    break;
            }
            return Undefined();
        }
        
        static Handle<Value> Rotate(Arguments const& args) 
        {
            switch(args.Length()) {
                // rotate around z
                case 1:                    
                    Impl(args).Rotate(args[0]->NumberValue(), 0, 0, 1);    
                    break;
            
                // rotate around x,y,z
                case 3:
                    Impl(args).Rotate(args[0]->NumberValue(), 1, 0, 0);    
                    Impl(args).Rotate(args[1]->NumberValue(), 0, 1, 0);
                    Impl(args).Rotate(args[2]->NumberValue(), 0, 0, 1);
                    break;
                    
                // rotate around custom axis
                case 4:
                    Impl(args).Rotate(args[0]->NumberValue(),
                                      args[1]->NumberValue(),
                                      args[2]->NumberValue(),
                                      args[3]->NumberValue());
                    break;
            }
            return Undefined();
        }
        
        static Handle<Value> Translate(Arguments const& args) 
        {
            switch(args.Length()) {
                // translate x,y
                case 2:
                    Impl(args).Translate(args[0]->NumberValue(), 
                                         args[1]->NumberValue(), 
                                         0.0);
                    break;

                // translate x,y,z
                case 3:
                    Impl(args).Translate(args[0]->NumberValue(), 
                                         args[1]->NumberValue(), 
                                         args[2]->NumberValue());
                    break;
            }
            return Undefined();
        }
        
        
        // -- Camera -----------------------------------------------------------
        static Handle<Value> Camera(Arguments const& args) 
        {
            if(args.Length() == 9) {
                Vec3f eye, center, up;
                
                eye.x = args[0]->NumberValue();
                eye.y = args[1]->NumberValue();
                eye.z = args[2]->NumberValue();
                
                center.x = args[3]->NumberValue();
                center.y = args[4]->NumberValue();
                center.z = args[5]->NumberValue();
                
                up.x = args[6]->NumberValue();
                up.y = args[7]->NumberValue();
                up.z = args[8]->NumberValue();

                Impl(args).Camera(eye, center, up);
            }
            return Undefined();
        }
        
        
        // -- GL Wrappers ------------------------------------------------------
        static Handle<Value> EnableAlphaBlending(Arguments const& args) 
        {
            bool premultiplied=false;
            if(args.Length() == 1) premultiplied = args[0]->BooleanValue();
            Impl(args).EnableAlphaBlending(premultiplied);
            return Undefined();
        }
        
        static Handle<Value> DisableAlphaBlending(Arguments const& args) 
        {
            Impl(args).DisableAlphaBlending();
            return Undefined();
        }

        static Handle<Value> EnableDepthTest(Arguments const& args) 
        {
            Impl(args).EnableDepthTest();
            return Undefined();
        }

        static Handle<Value> DisableDepthTest(Arguments const& args) 
        {
            Impl(args).DisableDepthTest();
            return Undefined();
        }
        
        static Handle<Value> PointSize(Arguments const& args) 
        {
            if(args.Length() == 1) {            
                Impl(args).PointSize(args[0]->NumberValue());
            }
            return Undefined();
        }
    };
    
    Persistent<FunctionTemplate> Graphics2DWrap::classTemplate;

    // -- Bindings -------------------------------------------------------------
    void Graphics2D::Initialize(v8::Handle<v8::Object> target) 
    {
        Graphics2DWrap::Initialize(target);
    }
    
} } // namespace fieldkit::script
