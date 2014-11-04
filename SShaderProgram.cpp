//
//  SShader.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 20/10/14.
//
//

#include <vector>

#include "SShaderProgram.h"
#include "platform/CCFileUtils.h"

USING_NS_CC;

NS_S_BEGIN

SShaderProgram::~SShaderProgram() {
    if (_programId)
    {
        glDeleteProgram(_programId);
    }
}

SShaderProgram* SShaderProgram::create(std::string vertexShaderPath, std::string fragmentShaderPath) {
    auto ret = new SShaderProgram();
    if (ret && ret->init(vertexShaderPath, fragmentShaderPath)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SShaderProgram::init(std::string vertexShaderPath, std::string fragmentShaderPath) {
    _programId = glCreateProgram();
    
    GLuint vertexShaderId;
    GLuint fragmentShaderId;
    
    if (!vertexShaderPath.empty()) {
        vertexShaderId = createShader(vertexShaderPath, GL_VERTEX_SHADER);
        if (!vertexShaderId) {
            return false;
        }
        glAttachShader(_programId, vertexShaderId);
    }
    if (!fragmentShaderPath.empty()) {
        fragmentShaderId = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);
        if (!fragmentShaderId) {
            return false;
        }
        glAttachShader(_programId, fragmentShaderId);
    }
    
    bindPredefinedVertexAttributes();
    
    // Link program
    glLinkProgram(_programId);
    
    parseShaderAttributes();
    parseShaderUniforms();
    
    // Check program
    GLint result = GL_FALSE;
    GLint infoLength;
    
    glGetProgramiv(_programId, GL_LINK_STATUS, &result);
    glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &infoLength);
    if (!result) {
        std::vector<char> programErrorMessage(infoLength + 1);
        glGetProgramInfoLog(_programId, infoLength, nullptr, &programErrorMessage[0]);
        printf("%s\n", &programErrorMessage[0]);
        return false;
    }
    
    if (vertexShaderId) {
        glDeleteShader(vertexShaderId);
    }
    if (fragmentShaderId) {
        glDeleteShader(fragmentShaderId);
    }
    
    return true;
}

GLuint SShaderProgram::createShader(std::string shaderPath, GLenum type) {
    GLuint shaderId = glCreateShader(type);
    
    GLint result = GL_FALSE;
    GLint infoLength;
    
    // Compile shader
    std::string shaderCode = readShader(shaderPath);
    const char* shaderCodePointer = shaderCode.c_str();
    
    const char* sources[] = {
        "uniform mat4 S_MMatrix;\n"
        "uniform mat4 S_VMatrix;\n"
        "uniform mat4 S_PMatrix;\n"
        "uniform mat4 S_MVMatrix;\n"
        "uniform mat4 S_MVPMatrix;\n"
        "uniform sampler2D S_Texture0;\n"
        "uniform vec3 S_Color;\n\0",
        shaderCodePointer
    };
    
    glShaderSource(shaderId, 2, sources, nullptr);
    glCompileShader(shaderId);
    
    // Check
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLength);
    if (!result) {
        std::vector<char> shaderErrorMessage(infoLength + 1);
        glGetShaderInfoLog(shaderId, infoLength, nullptr, &shaderErrorMessage[0]);
        printf("%s\n", &shaderErrorMessage[0]);
    }
    
    return shaderId;
}


void SShaderProgram::bindPredefinedVertexAttributes() {
    glBindAttribLocation(_programId, S_ATTRIBUTE_INDEX_POSITION, S_ATTRIBUTE_NAME_POSITION);
    glBindAttribLocation(_programId, S_ATTRIBUTE_INDEX_COLOR, S_ATTRIBUTE_NAME_COLOR);
    glBindAttribLocation(_programId, S_ATTRIBUTE_INDEX_NORMAL, S_ATTRIBUTE_NAME_NORMAL);
}

std::string SShaderProgram::readShader(std::string shaderPath) {
    auto fileUtils = FileUtils::getInstance();
    std::string shaderSource = fileUtils->getStringFromFile(shaderPath);
    return shaderSource;
}

void SShaderProgram::parseShaderAttributes() {
    _attributes.clear();
    
    GLint activeAttributeSize;
    
    glGetProgramiv(_programId, GL_ACTIVE_ATTRIBUTES, &activeAttributeSize);
    if (activeAttributeSize > 0) {
        SAttribute attribute;
        GLsizei bufSize;
        
        glGetProgramiv(_programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &bufSize);
        if (bufSize > 0) {
            GLchar* attributeName = (GLchar *) alloca(bufSize + 1);
            
            for (int i=0; i<activeAttributeSize; i++) {
                glGetActiveAttrib(_programId, i, bufSize, nullptr, &attribute.size, &attribute.type, attributeName);
                attributeName[bufSize] = '\0';
                attribute.name = std::string(attributeName);
                attribute.index = glGetAttribLocation(_programId, attributeName);
                _attributes[attribute.name] = attribute;
            }
        }
    }
}

void SShaderProgram::parseShaderUniforms() {
    _uniforms.clear();
    
    GLint activeUniformSize;
    glGetProgramiv(_programId, GL_ACTIVE_UNIFORMS, &activeUniformSize);
    if (activeUniformSize > 0) {
        SUniform uniform;
        GLint bufSize;
        
        glGetProgramiv(_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &bufSize);
        if (bufSize > 0) {
            GLchar* uniformName = (GLchar *) alloca(bufSize + 1);
            
            for (int i=0; i<activeUniformSize; i++) {
                glGetActiveUniform(_programId, i, bufSize, nullptr, &uniform.size, &uniform.type, uniformName);
                uniformName[bufSize] = '\0';
                
                // Only add uniforms that are not built-in.
                // The ones starting with 'S_' are built-ins.
//                if (strncmp("S_", uniformName, 2) != 0) {
                    uniform.name = std::string(uniformName);
                    uniform.index = glGetUniformLocation(_programId, uniformName);
                    _uniforms[uniform.name] = uniform;
//                }
                
            }
        }
    }
}

bool SShaderProgram::checkAttributeActive(std::string attributeName) {
    const auto itr = _attributes.find(attributeName);
    if (itr != _attributes.end()) {
        return true;
    }
    return false;
}

SAttribute* SShaderProgram::getAttribute(std::string attributeName) {
    const auto itr = _attributes.find(attributeName);
    if (itr != _attributes.end()) {
        return &itr->second;
    }
    return nullptr;
}

SUniform* SShaderProgram::getUniform(std::string uniformName) {
    const auto itr = _uniforms.find(uniformName);
    if (itr != _uniforms.end()) {
        return &itr->second;
    }
    return nullptr;
}

void SShaderProgram::updateBuiltInUniforms(const Mat4 &model, const Mat4 &view, const Mat4 &projection) {
    Mat4 mv = view * model;
    Mat4 mvp = projection * mv;
    
    SUniform* mUniform = getUniform(S_UNIFORM_NAME_M);
    if (mUniform) {
        glUniformMatrix4fv(mUniform->index, 1, GL_FALSE, model.m);
    }
    
    SUniform* vUniform = getUniform(S_UNIFORM_NAME_V);
    if (vUniform) {
        glUniformMatrix4fv(vUniform->index, 1, GL_FALSE, view.m);
    }
    
    SUniform* pUniform = getUniform(S_UNIFORM_NAME_P);
    if (pUniform) {
        glUniformMatrix4fv(pUniform->index, 1, GL_FALSE, projection.m);
    }
    
    SUniform* mvUniform = getUniform(S_UNIFORM_NAME_MV);
    if (mvUniform) {
        glUniformMatrix4fv(mvUniform->index, 1, GL_FALSE, mv.m);
    }
    
    SUniform* mvpUniform = getUniform(S_UNIFORM_NAME_MVP);
    if (mvpUniform) {
        glUniformMatrix4fv(mvpUniform->index, 1, GL_FALSE, mvp.m);
    }
}


NS_S_END