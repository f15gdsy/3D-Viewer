//
//  SBound.h
//  3DCocos
//
//  Created by Yifeng Wu on 31/10/14.
//
//

#ifndef ___DCocos__SBound__
#define ___DCocos__SBound__

#include "math/Vec3.h"
#include "math/CCGeometry.h"
#include "base/CCPlatformMacros.h"


#include "SMacros.h"

NS_S_BEGIN

class SBound {
public:
    SBound ();
    SBound (cocos2d::Vec3 minXYZ, cocos2d::Vec3 maxXYZ);
    
    cocos2d::Vec3 getSize ();
    bool compareAndSet (cocos2d::Vec3 xyz);
    
    cocos2d::Vec3 maxXYZ;
    cocos2d::Vec3 minXYZ;
};

NS_S_END

#endif /* defined(___DCocos__SBound__) */
