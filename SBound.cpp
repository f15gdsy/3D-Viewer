//
//  SBound.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 31/10/14.
//
//

#include <limits>

#include "SBound.h"


USING_NS_CC;

NS_S_BEGIN

SBound::SBound() {
    float minFloat = std::numeric_limits<float>::min();
    float maxFloat = std::numeric_limits<float>::max();

    maxXYZ = Vec3(minFloat, minFloat, minFloat);
    minXYZ = Vec3(maxFloat, maxFloat, maxFloat);
}

SBound::SBound(Vec3 minXYZ, Vec3 maxXYZ) {
    this->minXYZ = minXYZ;
    this->maxXYZ = maxXYZ;
}

Vec3 SBound::getSize() {
    Vec3 size(maxXYZ.x - minXYZ.x, maxXYZ.y - minXYZ.y, maxXYZ.z - minXYZ.z);
    return size;
}

bool SBound::compareAndSet(cocos2d::Vec3 xyz) {
    float x = xyz.x;
    float y = xyz.y;
    float z = xyz.z;
    
    Vec3 originMin = Vec3(minXYZ);
    Vec3 originMax = Vec3(maxXYZ);
    
    if (x < minXYZ.x) {
        minXYZ.x = x;
    }
    if (y < minXYZ.y) {
        minXYZ.y = y;
    }
    if (z < minXYZ.z) {
        minXYZ.z = z;
    }
    if (x > maxXYZ.x) {
        maxXYZ.x = x;
    }
    if (y > maxXYZ.y) {
        maxXYZ.y = y;
    }
    if (z > maxXYZ.z) {
        maxXYZ.z = z;
    }
    
    return minXYZ != originMin || maxXYZ != originMax;
}

NS_S_END