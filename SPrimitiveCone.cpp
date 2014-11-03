//
//  SPrimitiveCone.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 03/11/14.
//
//

#include "SPrimitiveCone.h"
#include "SShaderProgram.h"
#include "SMesh.h"

using namespace std;
USING_NS_CC;

NS_S_BEGIN

SPrimitiveCone* SPrimitiveCone::create(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    auto ret = new SPrimitiveCone();
    if (ret && ret->init(bottomCenter, radius, height, detail)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SPrimitiveCone::init(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    mesh = getConeMesh(bottomCenter, radius, height, detail);
    mesh->retain();
    
    SShaderProgram *defaultShader = SShaderProgram::create("mvp.vertexshader", "mvp.fragmentshader");
    
    setShader(defaultShader);
    
    _renderCommand.func = CC_CALLBACK_0(SPrimitiveCone::renderFunc, this);
    
    return true;
}

SMesh* SPrimitiveCone::getConeMesh(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    float degreeOffset = 360 / detail;
    float currentDegree = 0;
    
    vector<GLfloat> vertices;
    
    vertices.push_back(bottomCenter.x);
    vertices.push_back(bottomCenter.y);
    vertices.push_back(bottomCenter.z);
    
    vertices.push_back(bottomCenter.x);
    vertices.push_back(bottomCenter.y + height);
    vertices.push_back(bottomCenter.z);
    
    for (int i=0; i<detail; i++) {
        float x = bottomCenter.x + radius * cos(CC_DEGREES_TO_RADIANS(currentDegree));
        float z = bottomCenter.z + radius * sin(CC_DEGREES_TO_RADIANS(currentDegree));
        float y = bottomCenter.y;
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        
        currentDegree += degreeOffset;
    }
    
    vector<GLuint> indices;
    
    for (int i=2; i<2+detail; i++) {
        int currentIndice = i;
        int nextIndice = i+1 >= 2+detail ? i+1-detail : i+1;
        
        indices.push_back(0);
        indices.push_back(currentIndice);
        indices.push_back(nextIndice);
        
        indices.push_back(1);
        indices.push_back(currentIndice);
        indices.push_back(nextIndice);
    }
    
    return SMesh::create(vertices, indices, GL_TRIANGLES);
}

NS_S_END