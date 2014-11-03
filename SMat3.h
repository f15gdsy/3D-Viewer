//
//  SMat3.h
//  3DCocos
//
//  Created by Yifeng Wu on 04/11/14.
//
//

#ifndef ___DCocos__SMat3__
#define ___DCocos__SMat3__

#include "SMacros.h"
#include "math/Vec3.h"
#include "math/Mat4.h"

NS_S_BEGIN

class SMat3 {
public:
    float m[9];
    
    SMat3(cocos2d::Mat4 mat4);
    
    bool inverse ();
    void transpose ();
};

NS_S_END

#endif /* defined(___DCocos__SMat3__) */
