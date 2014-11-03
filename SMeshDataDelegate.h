//
//  SMeshDataDelegate.h
//  3DCocos
//
//  Created by Yifeng Wu on 31/10/14.
//
//

#ifndef _DCocos_SMeshDataDelegate_h
#define _DCocos_SMeshDataDelegate_h

#include "SMacros.h"

NS_S_BEGIN

class SMeshDataDelegate {
public:
    virtual void addVertex (float x, float y, float z, int index) {}
    virtual void addNormal (float x, float y, float z, int index) {}
    virtual void addFace (int v1, int v2, int v3, int index) {}
    
    virtual int verticesCount () {return -1;}
    virtual int normalsCount () {return -1;}
    virtual int facesCount () {return -1;}
};

NS_S_END

#endif
