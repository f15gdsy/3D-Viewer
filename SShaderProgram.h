//
//  SShader.h
//  3DCocos
//
//  Created by Yifeng Wu on 20/10/14.
//
//

#ifndef ___DCocos__SShader__
#define ___DCocos__SShader__

#include <string>
#import <OpenGL/gl.h>
#include <unordered_map>

#include "base/CCRef.h"
#include "math/Mat4.h"

#include "SMacros.h"

NS_S_BEGIN
//
#define S_ATTRIBUTE_INDEX_POSITION 0
#define S_ATTRIBUTE_INDEX_NORMAL 1
#define S_ATTRIBUTE_INDEX_COLOR 2
#define S_ATTRIBUTE_NAME_POSITION "a_position"
#define S_ATTRIBUTE_NAME_COLOR "a_color"
#define S_ATTRIBUTE_NAME_NORMAL "a_normal"

#define S_UNIFORM_NAME_M            "S_MMatrix"
#define S_UNIFORM_NAME_V            "S_VMatrix"
#define S_UNIFORM_NAME_P            "S_PMatrix"
#define S_UNIFORM_NAME_MV           "S_MVMatrix"
#define S_UNIFORM_NAME_MVP          "S_MVPMatrix"
#define S_UNIFORM_NAME_TEXTURE_0    "S_Texture0"

struct SAttribute {
    GLint index;
    GLint size;
    GLenum type;
    std::string name;
};

struct SUniform {
    GLint index;
    GLint size;
    GLenum type;
    std::string name;
};

class SShaderProgram : public cocos2d::Ref {
    
public:
    ~SShaderProgram ();
    
    static SShaderProgram* create (std::string vertexShaderPath, std::string fragmentShaderPath);
    
    GLuint inline getProgramId () {return _programId;}
    
    bool checkAttributeActive (std::string attributeName);
    SAttribute* getAttribute (std::string attributeName);
    SUniform* getUniform (std::string uniformName);
    
    void updateBuiltInUniforms (const cocos2d::Mat4& model, const cocos2d::Mat4& view, const cocos2d::Mat4& projection);
    
protected:
    bool init (std::string vertexShaderPath, std::string fragmentShaderPath);
    
    GLuint createShader (std::string shaderPath, GLenum type);
    std::string readShader (std::string shaderPath);
    void bindPredefinedVertexAttributes ();
    void parseShaderAttributes ();
    void parseShaderUniforms ();
    
    GLuint _programId;
    
    std::unordered_map<std::string, SAttribute> _attributes;
    std::unordered_map<std::string, SUniform> _uniforms;
};

NS_S_END

#endif /* defined(___DCocos__SShader__) */
