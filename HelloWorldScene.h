#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SMacros.h"

NS_S_BEGIN
class SCameraController;
class SModel3d;
class SShaderProgram;
NS_S_END

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    ~HelloWorld ();

    virtual bool init();
    
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    Samurai::SCameraController* _cameraController;
    
    Node* createAxisSystem ();
    Node* createFloor (int size, float gridSize);
    Node* createUI ();
    
private:
    // UI
    Node* _container3D;
    Node* _containerModel;
    Samurai::SModel3d* _model;
    Samurai::SShaderProgram* _shaderProgramNormal;
    Samurai::SShaderProgram* _shaderProgramFlat;

    
    Node* _boundingBox;
    
    void reset (cocos2d::Ref* sender);
    void toggleBoundingBox (cocos2d::Ref* sender);
    void smoothRender (cocos2d::Ref* sender);
    void wireFrameRender (cocos2d::Ref* sender);
    void flatRender (cocos2d::Ref* flatRender);
};

#endif // __HELLOWORLD_SCENE_H__
