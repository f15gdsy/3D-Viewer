//
//  SGLViewImp.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 10/10/14.
//
//

#include "SGLViewImp.h"

#include "base/CCDirector.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventMouse.h"
#include "base/CCEventTouch.h"

USING_NS_CC;

NS_S_BEGIN

class SGLFWEventHandler {
    
private:
    static SGLViewImp* _view;
public:
    static void onGLFWKeyCallback (GLFWwindow* window, int key, int scancode, int action, int mods) {
        _view->onGLFWKeyCallback(window, key, scancode, action, mods);
    }
    
    static void onGLFWMouseButtonCallback (GLFWwindow* window, int button, int action, int modify) {
        _view->onGLFWMouseButtonCallback(window, button, action, modify);
    }
    
    static void onGLFWMouseMovedCallback(GLFWwindow* window, double x, double y) {
        _view->onGLFWMouseMovedCallback(window, x, y);
    }
    
    static void onGLFWMouseScrollCallback(GLFWwindow* window, double x, double y) {
        _view->onGLFWMouseScrollCallback(window, x, y);
    }
    
    static void setView (SGLViewImp* view) {
        _view = view;
    };
};

SGLViewImp* SGLFWEventHandler::_view = nullptr;


SGLViewImp::SGLViewImp () {
    
}

SGLViewImp::~SGLViewImp() {
    glfwTerminate();
}

SGLViewImp* SGLViewImp::create(const std::string &viewName) {
    auto ret = new SGLViewImp;
    if (ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640))) {
        ret->autorelease();
        return ret;
    }
    
    return nullptr;
}

SGLViewImp* SGLViewImp::createWithRect(const std::string &viewName, cocos2d::Rect rect) {
    auto ret = new SGLViewImp;
    if (ret && ret->initWithRect(viewName, rect)) {
        ret->autorelease();
        return ret;
    }

    return nullptr;
}

bool SGLViewImp::initWithRect(const std::string &viewName, cocos2d::Rect rect) {
    setViewName(viewName);
    
    SGLFWEventHandler::setView(this);
    
    // GLFW Setup
    glfwSetErrorCallback(SGLViewImp::onGLFWErrorCallback);
    
    glfwInit();
    
    _mainWindow = glfwCreateWindow(rect.size.width, rect.size.height, viewName.c_str(), _monitor, nullptr);
    glfwMakeContextCurrent(_mainWindow);
    
    // set callbacks
    glfwSetKeyCallback(_mainWindow, SGLFWEventHandler::onGLFWKeyCallback);
    glfwSetMouseButtonCallback(_mainWindow, SGLFWEventHandler::onGLFWMouseButtonCallback);
    glfwSetCursorPosCallback(_mainWindow, SGLFWEventHandler::onGLFWMouseMovedCallback);
    glfwSetScrollCallback(_mainWindow, SGLFWEventHandler::onGLFWMouseScrollCallback);
    
    setFrameSize(rect.size.width, rect.size.height);
    
    return true;
}

bool SGLViewImp::windowShouldClose() {
    if (_mainWindow) {
        return glfwWindowShouldClose(_mainWindow) ? true : false;
    }
    else {
        return false;
    }
}

void SGLViewImp::pollEvents() {
    glfwPollEvents();
}

void SGLViewImp::end() {
    if (_mainWindow) {
        glfwSetWindowShouldClose(_mainWindow, 1);
        _mainWindow = nullptr;
    }
    release();
}

bool SGLViewImp::isOpenGLReady() {
    return nullptr != _mainWindow;
}

void SGLViewImp::swapBuffers() {
    glfwSwapBuffers(_mainWindow);
}

void SGLViewImp::setIMEKeyboardState(bool open) {
    
}

// -------------GLFW callbacks-------------

void SGLViewImp::onGLFWKeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS)
        CCLOG("Key %d is pressed", key);
}

void SGLViewImp::onGLFWMouseButtonCallback(GLFWwindow *window, int button, int action, int modify) {
    EventMouse::MouseEventType type;
    
    // Mouse
    if (action == GLFW_PRESS) {
        type = EventMouse::MouseEventType::MOUSE_DOWN;
    }
    else if (action == GLFW_RELEASE) {
        type = EventMouse::MouseEventType::MOUSE_UP;
    }
    
    EventMouse event(type);
    
    event.setMouseButton(button);
    event.setCursorPosition(_cursorPosX, _cursorPosY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
    // Touch
    if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT) {
        intptr_t id = 0;
        this->handleTouchesBegin(1, &id, &_touchX, &_touchY);
    }
    else if (action == GLFW_RELEASE && button == GLFW_MOUSE_BUTTON_LEFT) {
        intptr_t id = 0;
        this->handleTouchesEnd(1, &id, &_touchX, &_touchY);
    }
}

void SGLViewImp::onGLFWMouseMovedCallback(GLFWwindow *window, double x, double y) {
    _touchX = (float) x;
    _touchY = (float) y;
    
    _cursorPosX = (float) x;
    _cursorPosY = (float) y;
    
    _cursorPosX = _cursorPosX - _viewPortRect.origin.x;
    _cursorPosY = _viewPortRect.origin.y + _viewPortRect.size.height - _cursorPosY;
    
    int button = -1;
    if (glfwGetMouseButton(_mainWindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
        button = GLFW_MOUSE_BUTTON_1;
    }
    else if (glfwGetMouseButton(_mainWindow, GLFW_MOUSE_BUTTON_2)) {
        button = GLFW_MOUSE_BUTTON_2;
    }
    else if (glfwGetMouseButton(_mainWindow, GLFW_MOUSE_BUTTON_3)) {
        button = GLFW_MOUSE_BUTTON_3;
    }
    
    if (button != -1) {
        EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
        event.setCursorPosition(_cursorPosX, _cursorPosY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void SGLViewImp::onGLFWMouseScrollCallback(GLFWwindow *window, double x, double y) {
    EventMouse event(EventMouse::MouseEventType::MOUSE_SCROLL);
    event.setScrollData(x, -y);
    event.setCursorPosition(_cursorPosX, _cursorPosY);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
// ------End-------




NS_S_END