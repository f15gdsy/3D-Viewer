//
//  SPrimitiveBox.h
//  3DCocos
//
//  Created by Yifeng Wu on 01/11/14.
//
//

#ifndef ___DCocos__SPrimitiveBox__
#define ___DCocos__SPrimitiveBox__

#include <vector>

#include "SMacros.h"
#include "SModel3d.h"

NS_S_BEGIN

class SPrimitiveBox : public SModel3d {
public:
    static SPrimitiveBox* create (std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode);
    static SPrimitiveBox* create (cocos2d::Vec3 minXYZ, cocos2d::Vec3 maxXYZ);
    
private:
    bool init (cocos2d::Vec3 minXYZ, cocos2d::Vec3 maxXYZ);
    bool init (std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode);
};

NS_S_END

#endif /* defined(___DCocos__SPrimitiveBox__) */
