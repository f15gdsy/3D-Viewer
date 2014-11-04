//
//  SModel3d.h
//  3DCocos
//
//  Created by Yifeng Wu on 16/10/14.
//
//

#ifndef ___DCocos__SModel3d__
#define ___DCocos__SModel3d__

#include <vector>

#include "2d/CCNode.h"
#include "renderer/CCCustomCommand.h"

#include "SMFileParser.h"
#include "SMacros.h"

NS_S_BEGIN

class SMesh;
class SShaderProgram;

class SModel3d : public cocos2d::Node {
public:    
    static SModel3d* create(const std::string &modelPath);
    static SModel3d* create(const std::string &modelPath, SShaderProgram* shaderProgram);
    
    void setShader (SShaderProgram* shaderProgram);
    SShaderProgram* getShaderProgram () {
        return _shaderProgram;
    }
    
    void setWireFrameEnabled (bool enabled) {
        _wireFrameEnabled = enabled;
    }
    
    void setColor (cocos2d::Vec3 color) {
        _color = color;
    }

    SMesh* mesh;
    
protected:
    ~SModel3d ();
    
    bool init(const std::string &modelPath);
    bool init(const std::string &modelPath, SShaderProgram* shaderProgram);
    
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    
    void renderFunc ();
    void testRenderFunc ();
    void buildVAO ();
    void clearVAO ();
    void updateUniforms ();
    void updateDrawStates ();
    void clearDrawStates ();
    
    cocos2d::CustomCommand _renderCommand;
    GLuint _vao;

    SShaderProgram* _shaderProgram;
  
    bool _wireFrameEnabled;
    bool _depthTestEnabled = true;
    
    cocos2d::Vec3 _color;
    
    static int _totalSModel3dToRender;
    static int _totalSModel3dRendered;
};

NS_S_END

#endif /* defined(___DCocos__SModel3d__) */
