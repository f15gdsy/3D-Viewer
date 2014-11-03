//
//  SGLViewImp.h
//  3DCocos
//
//  Created by Yifeng Wu on 10/10/14.
//
//

#ifndef ___3DCocos__SGLViewImp__
#define ___3DCocos__SGLViewImp__

#include "base/CCRef.h"
#include "platform/desktop/CCGLView.h"
#include "glfw3.h"

#include "SMacros.h"

USING_NS_CC;

NS_S_BEGIN

class CC_DLL SGLViewImp : public GLView {
public:
    static SGLViewImp* create(const std::string& viewName);
    static SGLViewImp* createWithRect(const std::string& viewName, Rect rect);
    
    bool windowShouldClose () override;
    void pollEvents () override;

    // override
    void end () override;
    bool isOpenGLReady () override;
    void swapBuffers () override;
    void setIMEKeyboardState (bool open) override;
    
protected:
    SGLViewImp ();
    virtual ~SGLViewImp ();
    
    bool initWithRect (const std::string& viewName, Rect rect);

// glfw callbacks
    
public:
    void onGLFWKeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods);
    void onGLFWMouseButtonCallback (GLFWwindow* window, int button, int action, int modify);
    void onGLFWMouseMovedCallback(GLFWwindow* window, double x, double y);
    void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y);
protected:
    static void onGLFWErrorCallback (int error, const char* description) {
        CCLOGERROR("GLFWError: #%d happened, %s\n", error, description);
    }
    
    float _cursorPosX;
    float _cursorPosY;
    
    float _touchX;
    float _touchY;
    
};

NS_S_END

#endif /* defined(___DCocos__SGLViewImp__) */
