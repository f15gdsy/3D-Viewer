//
//  SPrimitiveCylinder.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 03/11/14.
//
//

#include <vector>

#include "SPrimitiveCylinder.h"
#include "SShaderProgram.h"
#include "SMesh.h"

using namespace std;
USING_NS_CC;

NS_S_BEGIN

SPrimitiveCylinder* SPrimitiveCylinder::create(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    auto ret = new SPrimitiveCylinder();
    if (ret && ret->init(bottomCenter, radius, height, detail)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SPrimitiveCylinder::init(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    
    mesh = getCylinderMesh(bottomCenter, radius, height, detail);
    mesh->retain();
    
    SShaderProgram *defaultShader = SShaderProgram::create("mvp.vertexshader", "mvp.fragmentshader");
    
    setShader(defaultShader);
    
    _renderCommand.func = CC_CALLBACK_0(SPrimitiveCylinder::renderFunc, this);
    
    return true;
}

SMesh* SPrimitiveCylinder::getCylinderMesh(cocos2d::Vec3 bottomCenter, float radius, float height, int detail) {
    float degreeOffset = 360 / detail;
    float currentDegree = 0;
    
    vector<GLfloat> vertices;
    vector<GLfloat> topVertices;
    
    vertices.push_back(bottomCenter.x);
    vertices.push_back(0);
    vertices.push_back(bottomCenter.z);
    
    vertices.push_back(bottomCenter.x);
    vertices.push_back(height);
    vertices.push_back(bottomCenter.z);
    
    for (int i=0; i<detail; i++) {
        float x = bottomCenter.x + radius * cos(CC_DEGREES_TO_RADIANS(currentDegree));
        float z = bottomCenter.z + radius * sin(CC_DEGREES_TO_RADIANS(currentDegree));
        float y = 0;
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        
        y = height;
        
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
        
        currentDegree += degreeOffset;
    }
    
    vector<GLuint> indices;
    
    for (int i=2; i<2+detail*2; i++) {
        int currentIndice = i;
        int nextIndice = i+1 >= 2+detail*2 ? i+1-detail*2 : i+1;
        int nextNextIndex = i+2 >= 2+detail*2 ? i+2-detail*2 : i+2;
        indices.push_back(currentIndice);
        indices.push_back(nextIndice);
        indices.push_back(nextNextIndex);
        
        if (currentIndice % 2 == 0) {
            indices.push_back(0);
        }
        else {
            indices.push_back(1);
        }
        indices.push_back(currentIndice);
        indices.push_back(nextNextIndex);
    }
    
    return SMesh::create(vertices, indices, GL_TRIANGLES);
}

NS_S_END