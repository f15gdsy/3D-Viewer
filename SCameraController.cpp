//
//  SCameraController.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 28/10/14.
//
//

#include <math.h>

#include "SCameraController.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerMouse.h"

USING_NS_CC;

NS_S_BEGIN


SCameraController* SCameraController::create() {
    return SCameraController::create(nullptr);
}

SCameraController* SCameraController::create(Node* target) {
    auto ret = new SCameraController();
    if (ret && ret->init(target)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SCameraController::init(Node* target) {
    _target = target;
    
    // Subscribe mouse events
    _mouseEventHandler = EventListenerMouse::create();

    _mouseEventHandler->onMouseScroll = CC_CALLBACK_1(SCameraController::onMouseScrolled, this);
    _mouseEventHandler->onMouseDown = CC_CALLBACK_1(SCameraController::onMousePressed, this);
    _mouseEventHandler->onMouseMove = CC_CALLBACK_1(SCameraController::onMouseHeld, this);
    _mouseEventHandler->onMouseUp = CC_CALLBACK_1(SCameraController::onMouseReleased, this);
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_mouseEventHandler, 1);
    
    // Camera position initialization
    Size winSize = Director::getInstance()->getWinSize();
    float zEye = Director::getInstance()->getZEye();
    _position.x = winSize.width / 2;
    _position.y = winSize.height / 2;
    _position.z = 50;
    
    _center = _position;
    _center.z = 0;
    
    _up = Vec3(0, 1, 0);
    
    updateViewMatrix();
    updateProjectionMatrix();
    
    return true;
}

void SCameraController::onMouseScrolled(cocos2d::Event *event) {
    EventMouse* mouseEvent = (EventMouse*) event;
    float scollY = mouseEvent->getScrollY();
    
    // Scroll
    _position.z = _position.z + scollY;
    updateViewMatrix();
}

void SCameraController::onMousePressed(cocos2d::Event *event) {
    EventMouse* mouseEvent = (EventMouse *) event;
    
    _previousCursorX = mouseEvent->getCursorX();
    _previousCursorY = mouseEvent->getCursorY();
}

void SCameraController::onMouseHeld(cocos2d::Event *event) {
    EventMouse* mouseEvent = (EventMouse *) event;
    
    float currentCursorX = mouseEvent->getCursorX();
    float currentCursorY = mouseEvent->getCursorY();
    
    float deltaX = currentCursorX - _previousCursorX;
    float deltaY = currentCursorY - _previousCursorY;
    
    if (mouseEvent->getMouseButton() == MOUSE_BUTTON_LEFT) {
        // Rotate
        Vec3 rotation = _target->getRotation3D();
        
        rotation.y += deltaX / 10;
        rotation.x += deltaY / 10;
        
        _target->setRotation3D(rotation);

        updateViewMatrix();
    }
    else if (mouseEvent->getMouseButton() == MOUSE_BUTTON_RIGHT) {
        
        // Translate
        _position.x += deltaX / 20;
        _position.y += deltaY / 20;
        
        _center.x = _position.x;
        _center.y = _position.y;
        _center.z = 0;
        
        updateViewMatrix();
    }
    else if (mouseEvent->getMouseButton() == MOUSE_BUTTON_MIDDLE) {
    }
    _previousCursorX = mouseEvent->getCursorX();
    _previousCursorY = mouseEvent->getCursorY();
}

void SCameraController::onMouseReleased(cocos2d::Event *event) {
//    EventMouse* mouseEvent = (EventMouse *) event;
}

void SCameraController::updateViewMatrix() {
    Mat4::createLookAt(_position,  _center, _up, &_viewMatrix);
}

void SCameraController::updateProjectionMatrix() {
    Size winSize = Director::getInstance()->getWinSize();
    float zEye = Director::getInstance()->getZEye();
    
    Mat4::createPerspective(60, winSize.width / winSize.height, 10, zEye + winSize.height / 2, &_projectionMatrix);
    
    Mat4::createOrthographicOffCenter(-winSize.width/8, winSize.width/8, -winSize.height/8, winSize.height/8, 0, 1024, &_orthographicMatrix);
}

void SCameraController::setPerspective(bool perspective) {
    if (_usePerspective != perspective) {
        _usePerspective = perspective;
        updateProjectionMatrix();
    }
}


NS_S_END