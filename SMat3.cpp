//
//  SMat3.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 04/11/14.
//
//

#include "SMat3.h"
#include <math.h>

USING_NS_CC_MATH;

NS_S_BEGIN

SMat3::SMat3(Mat4 mat4) {
    m[0] = mat4.m[0];
    m[1] = mat4.m[1];
    m[2] = mat4.m[2];
    
    m[3] = mat4.m[4];
    m[4] = mat4.m[5];
    m[5] = mat4.m[6];
    
    m[6] = mat4.m[8];
    m[7] = mat4.m[9];
    m[8] = mat4.m[10];
}

bool SMat3::inverse() {
    float a0 = m[4] * m[8] - m[7] * m[5];
    float a1 = m[1] * m[8] - m[7] * m[2];
    float a2 = m[1] * m[5] - m[4] * m[2];
    
    float b0 = m[3] * m[8] - m[6] * m[5];
    float b1 = m[0] * m[8] - m[6] * m[2];
    float b2 = m[0] * m[5] - m[3] * m[2];
    
    float c0 = m[3] * m[7] - m[6] * m[4];
    float c1 = m[0] * m[7] - m[6] * m[1];
    float c2 = m[0] * m[4] - m[3] * m[1];
    
    float det = m[0] * a0 - m[1] * b0 + m[2] * c0;
    
    if (fabs(det) <= MATH_TOLERANCE) {
        return false;
    }
    
    m[0] = a0;
    m[1] = -a1;
    m[2] = a2;

    m[3] = -b0;
    m[4] = b1;
    m[5] = -b2;
    
    m[6] = c0;
    m[7] = -c1;
    m[8] = c2;
    
    return true;
}

void SMat3::transpose() {
    float t[9] = {
        m[0], m[3], m[6],
        m[1], m[4], m[7],
        m[2], m[5], m[8]
    };
    memcpy(m, t, sizeof(float) * 9);
}

NS_S_END