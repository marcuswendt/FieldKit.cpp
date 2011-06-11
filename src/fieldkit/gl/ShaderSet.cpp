/*                                                                           
 *      _____  __  _____  __     ____                                   
 *     / ___/ / / /____/ / /    /    \   FieldKit
 *    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
 *   /_/        /____/ /____/ /_____/    http://www.field.io           
 *   
 *	 Created by Marcus Wendt on 11/05/2011.
 */

#include "fieldkit/gl/ShaderSet.h"

#include <cstdio>
#include <iostream>
#include <vector>
#include <map>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>

#include "cinder/Buffer.h"
#include "fieldkit/Logger.h"

namespace fieldkit { namespace gl {

    using namespace std;
    namespace fs = boost::filesystem;    
    
    
    // -- Helpers ---------------------------------------------------------------------
    
    // Reads a file into a stl string.
    static string ReadFileContents(string const& path)
    {
        FILE* file = fopen(path.c_str(), "rb");
        if (file == NULL) return "";
        
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        rewind(file);
        
        char* chars = new char[size + 1];
        chars[size] = '\0';
        for (int i = 0; i < size;) {
            int read = fread(&chars[i], 1, size - i, file);
            i += read;
        }
        fclose(file);
        
        string contents = string(chars, size);
        delete chars;
        
        return contents;
    }
    
    std::string GetFileName(std::string filePath)
    {
        return fs::path(filePath).filename().string();
//        return fs::path(filePath).stem().string();
    }

    template<typename _OutputIterator>
    void Split(std::string str, std::string delim, _OutputIterator result) {
        typedef boost::algorithm::split_iterator<std::string::iterator> string_split_iterator;
        for(string_split_iterator iter=boost::algorithm::make_split_iterator(str,boost::algorithm::first_finder(delim, boost::algorithm::is_equal()));
            iter!=string_split_iterator();
            ++iter) {
            std::string component = boost::copy_range<std::string>(*iter);
            boost::trim(component); // trim whitespace
            *result = component;
            ++result;
        }
    }
    
    
    // -- ShaderSet ------------------------------------------------------------------
    ShaderSet::ShaderSet() : delimiter("---")
    {
        addSourceExtension("glsl");
        addSourceExtension("shader");
        addSourceExtension("vs");
        addSourceExtension("fs");
        addSourceExtension("gs");
        addSourceExtension("vert");
        addSourceExtension("frag");
        addSourceExtension("geom");
    }
    
    
    void ShaderSet::load(ci::DataSourceRef const& sourceRef)
    {
        ci::Buffer buffer = sourceRef->getBuffer();
        string sourceStr(static_cast<char*>(buffer.getData()), buffer.getDataSize());
        parseSourceFragments(sourceStr, GetFileName(sourceRef->getFilePathHint()));
    }

    
    void ShaderSet::load(std::string const& sourcePath)
    { 
        // check if given path exists
        fs::path path(sourcePath);
        if(!fs::exists(path)) return;
        
        // recursively load shaders
        if(fs::is_directory(path)) 
        {
            
            fs::directory_iterator end_it;
            for(fs::directory_iterator it(path); it != end_it; ++it) 
            {
                if(fs::is_directory(it->status())) {
                    load(it->path().string());
                    
                } else {
                    if(isSourceFile(it->path().extension().string())) {
                        std::string filePath = it->path().string();
                        parseSourceFragments(ReadFileContents(filePath), GetFileName(filePath));
                    }
                }
            }
            
        } else {
            if(isSourceFile(path.extension().string())) {
                std::string filePath = path.string();
                parseSourceFragments(ReadFileContents(filePath), GetFileName(filePath));
            }
        }
    }
    
    
    void ShaderSet::addSourceExtension(std::string extension)
    {
        sourceFileExtensions.push_back(string(".")+ extension);
    }
    
    
    bool ShaderSet::isSourceFile(std::string extension)
    {
        BOOST_FOREACH(std::string supported, sourceFileExtensions) {
            if(extension == supported) return true;
        }
        return false;
    }
    
    
    void ShaderSet::parseSourceFragments(std::string fragmentSource, std::string nameHint)
    {
        if(fragmentSource == "") return;

        vector<string> splitted;
        Split(fragmentSource, delimiter, insert_iterator<vector<string> >(splitted, splitted.begin()));
        
        if(splitted.size() == 1) {
            fragments.insert( pair<string, string>(nameHint, fragmentSource) );
            
        } else {
            vector<string>::iterator it = splitted.begin();
            ++it; // skip header

            for(; it != splitted.end(); ++it) 
            {
                string fragmentName(*it);
                string key = nameHint +"/"+ fragmentName;
                ++it;
                string value(*it);
                fragments.insert( pair<string, string>(key, value) );
            }
        }
    }
    
    
    std::string ShaderSet::compileSource(std::string fragmentName)
    {
        std::map<string, string>::iterator it = fragments.find(fragmentName);
        if(it == fragments.end()) return "";
        
        std::string source = it->second;
        
        // recursively resolve all #include directives
        string key = "#include";
        size_t match;
        
        while( (match = source.find(key)) != string::npos) 
        {
            size_t start = source.find('"', match);
            size_t end = source.find('"', start+1);
            size_t len = end - start -1;
            
            string includeName = source.substr(start+1, len);
            
            string content = compileSource(includeName);
            source.replace(match, end - match +1, content);
        }
        
        return source;
    }

    
#if defined(CINDER_GLES)
    ci::gl::GlslProg ShaderSet::create(std::string vertexShaderName,
                                       std::string fragmentShaderName)
#else
    ci::gl::GlslProg ShaderSet::create(std::string vertexShaderName,
                                        std::string fragmentShaderName,
                                        std::string geometryShaderName,
                                        GLint geometryInputType, 
                                        GLint geometryOutputType, 
                                        GLint geometryOutputVertices)
#endif
    {
        string vertexSource = compileSource(vertexShaderName);
        if(vertexSource == "")
            LOG_WARN("Couldnt find vertex shader (key: "<< vertexShaderName <<")");
        
        string fragmentSource = compileSource(fragmentShaderName);
        if(fragmentSource == "")
            LOG_WARN("Couldnt find fragment shader (key: "<< fragmentShaderName <<")");

#if !defined(CINDER_GLES)
        if(geometryShaderName != "") {
            string geometrySource = compileSource(geometryShaderName);
            if(geometrySource == "")
                LOG_WARN("Couldnt find vertex shader (key: "<< geometryShaderName <<")");
            
            return ci::gl::GlslProg(vertexSource.c_str(), fragmentSource.c_str(), geometrySource.c_str(),
                                    geometryInputType, geometryOutputType, geometryOutputVertices);
        }
#endif
        
        return ci::gl::GlslProg(vertexSource.c_str(), fragmentSource.c_str());
    }
    
} }