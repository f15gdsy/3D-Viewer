//
//  SCameraController.h
//  3DCocos
//
//  Created by Yifeng Wu on 28/10/14.
//
//

#ifndef ___DCocos__SCameraController__
#define ___DCocos__SCameraController__

#include <stdio.h>
#include "SMacros.h"
#include "base/CCRef.h"
#include "2d/CCNode.h"

USING_NS_CC;

NS_CC_BEGIN
class EventListenerMouse;
NS_CC_END

NS_S_BEGIN

class SCameraController : public Ref {
public:
    static SCameraController* create ();
    static SCameraController* create (Node* target);
    
    void setTarget (Node* target) {
        _target = target;
    };
    
    void setPerspective (bool perspective);
    bool getPerspective () {
        return _usePerspective;
    }
    
    void updateMatrices ();
    
    Mat4 getProjectionMatrix () {
        if (_usePerspective) {
            return _projectionMatrix;
        }
        else {
            return _orthographicMatrix;
        }
    }
    Mat4 getViewMatrix () {
        return _viewMatrix;
    }
private:
    bool init (Node* target);
    
    void onMouseScrolled (cocos2d::Event* event);
    void onMousePressed (cocos2d::Event* event);
    void onMouseHeld (cocos2d::Event* event);
    void onMouseReleased (cocos2d::Event* event);
    
    void updateViewMatrix ();
    void updateProjectionMatrix ();
    
    EventListenerMouse* _mouseEventHandler;
    
    bool _usePerspective = true;
    
    Node* _target;
    float _previousCursorX;
    float _previousCursorY;
    Vec3 _position;
    Vec3 _center;
    Vec3 _up;
    float _toTargetAngle;
    
    Mat4 _projectionMatrix;
    Mat4 _orthographicMatrix;
    Mat4 _viewMatrix;
};


NS_S_END

#endif /* defined(___DCocos__SCameraController__) */
