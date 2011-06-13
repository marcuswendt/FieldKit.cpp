/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 11/05/2011.
 */

#pragma once

#include <map>
#include <string>
#include "cinder/DataSource.h"
#include "cinder/gl/GlslProg.h"

namespace fieldkit { namespace gl {

    class ShaderSet {
    public:
        ShaderSet();
        ~ShaderSet() {}
        
        //! path to a single glsl file or directory of .glsl shader source files
        void load(ci::DataSourceRef const& sourceRef);
        void load(std::string const& sourcePath);
        
        //! registers another source file extension (without the .)
        void addSourceExtension(std::string extension);
        
        //! removes all loaded fragments
        void clear() { fragments.clear(); };
        
#if defined(CINDER_GLES)
        ci::gl::GlslProg create(std::string vertexShaderName,
                                           std::string fragmentShaderName);
#else
        ci::gl::GlslProg create(std::string vertexShaderName,
                                std::string fragmentShaderName,
                                std::string geometryShaderName = "",
                                GLint geometryInputType = GL_POINTS, 
                                GLint geometryOutputType = GL_TRIANGLES, 
                                GLint geometryOutputVertices = 0);
#endif
        
    private:
        std::string delimiter;
        std::map<std::string, std::string> fragments;
        std::vector<std::string> sourceFileExtensions;
        
        bool isSourceFile(std::string extension);
        void parseSourceFragments(std::string source, std::string nameHint);

        //! returns glsl source code associated with the given fragment name and also resolves #include "fragmentPath" directives
        std::string compileSource(std::string fragmentName);
    };
    
    
    // define OpenGL ES specific version without geometry shader support
    #if defined(CINDER_GLES)
    ci::gl::GlslProg loadShaderSet(ci::DataSourceRef source,
                                   std::string vertexShaderName,
                                   std::string fragmentShaderName);
    
    #else 
    ci::gl::GlslProg loadShaderSet(ci::DataSourceRef source,
                                   std::string vertexShaderName,
                                   std::string fragmentShaderName,
                                   std::string geometryShaderName = "",
                                   GLint geometryInputType = GL_POINTS, 
                                   GLint geometryOutputType = GL_TRIANGLES, 
                                   GLint geometryOutputVertices = 0);
    
    #endif
} }