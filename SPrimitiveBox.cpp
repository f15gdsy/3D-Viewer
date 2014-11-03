//
//  SPrimitiveBox.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 01/11/14.
//
//

#include "SPrimitiveBox.h"
#include "SMesh.h"

USING_NS_CC;
using namespace std;

NS_S_BEGIN

SPrimitiveBox* SPrimitiveBox::create(std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode) {
    auto ret = new SPrimitiveBox();
    if (ret && ret->init(vertices, indices, drawMode)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

SPrimitiveBox* SPrimitiveBox::create(cocos2d::Vec3 minXYZ, cocos2d::Vec3 maxXYZ) {
    auto ret = new SPrimitiveBox();
    if (ret && ret->init(minXYZ, maxXYZ)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SPrimitiveBox::init(cocos2d::Vec3 minXYZ, cocos2d::Vec3 maxXYZ) {
    GLfloat verticesArr[] = {
        minXYZ.x, minXYZ.y, minXYZ.z,
        maxXYZ.x, minXYZ.y, minXYZ.z,
        maxXYZ.x, maxXYZ.y, minXYZ.z,
        minXYZ.x, maxXYZ.y, minXYZ.z,
        
        minXYZ.x, minXYZ.y, maxXYZ.z,
        maxXYZ.x, minXYZ.y, maxXYZ.z,
        maxXYZ.x, maxXYZ.y, maxXYZ.z,
        minXYZ.x, maxXYZ.y, maxXYZ.z,
    };
    
    vector<GLfloat> vertices(verticesArr, verticesArr + sizeof(verticesArr) / sizeof(verticesArr[0]));
    
    GLuint indicesArr[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        
        0, 4,
        4, 7,
        7, 3,
        
        7, 6,
        2, 6,
        
        4, 5,
        5, 6,
        
        1, 5,
    };
    
    vector<GLuint> indices(indicesArr, indicesArr + sizeof(indicesArr) / sizeof(indicesArr[0]));
    
    return init(vertices, indices, GL_LINES);
}

bool SPrimitiveBox::init(std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode) {
    mesh = SMesh::create(vertices, indices, drawMode);
    mesh->retain();
    
    SShaderProgram *defaultShader = SShaderProgram::create("mvp.vertexshader", "mvp.fragmentshader");
    
    setShader(defaultShader);
    
    _renderCommand.func = CC_CALLBACK_0(SPrimitiveBox::renderFunc, this);
    
    return true;
}

NS_S_END