//
//  SMatrixProvider.h
//  3DCocos
//
//  Created by Yifeng Wu on 29/10/14.
//
//

#ifndef ___DCocos__SMatrixProvider__
#define ___DCocos__SMatrixProvider__

#include <stdio.h>
#include <functional>

#include "base/CCRef.h"
#include "math/Mat4.h"

#include "SMacros.h"

NS_S_BEGIN

class SMatrixProvider : public cocos2d::Ref {
public:
    static SMatrixProvider* getInstance ();
    
    enum class MatrixType {
        PROJECTION,
        VIEW,
        IDENTITY
    };
    
    cocos2d::Mat4 getMatrix (MatrixType type);
    
    std::function<cocos2d::Mat4()> getViewMatrix;
    std::function<cocos2d::Mat4()> getProjectionMatrix;
    
private:
    bool init ();
    
    static SMatrixProvider* s_instance;
};


NS_S_END

#endif /* defined(___DCocos__SMatrixProvider__) */
