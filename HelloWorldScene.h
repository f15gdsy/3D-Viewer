#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SMacros.h"

NS_S_BEGIN
class SCameraController;
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
};

#endif // __HELLOWORLD_SCENE_H__