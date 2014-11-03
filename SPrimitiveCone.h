//
//  SPrimitiveCone.h
//  3DCocos
//
//  Created by Yifeng Wu on 03/11/14.
//
//

#ifndef ___DCocos__SPrimitiveCone__
#define ___DCocos__SPrimitiveCone__

#include "SMacros.h"
#include "SModel3d.h"

NS_S_BEGIN

class SPrimitiveCone : public SModel3d {
public:
    static SPrimitiveCone* create(cocos2d::Vec3 bottomCenter, float radius, float height, int detail);
    
private:
    bool init (cocos2d::Vec3 bottomCenter, float radius, float height, int detail);
    
    SMesh* getConeMesh (cocos2d::Vec3 bottomCenter, float radius, float height, int detail);
};

NS_S_END

#endif /* defined(___DCocos__SPrimitiveCone__) */
