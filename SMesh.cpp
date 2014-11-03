//
//  SMesh.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 17/10/14.
//
//

#include "SMesh.h"

#include "cocos2d.h"

USING_NS_CC;

NS_S_BEGIN

SMesh::~SMesh() {
    cleanBuffer();
}

SMesh* SMesh::create(const std::string &meshPath) {
    auto ret = new SMesh();
    if (ret && ret->init(meshPath)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

SMesh* SMesh::create(std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode) {
    auto ret = new SMesh();
    if (ret && ret->init(vertices, indices, drawMode)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SMesh::init(const std::string &meshPath) {
    setDrawMode(GL_TRIANGLES);
    
    SMFileParser parser;
    parser.setDelegate(this);
    
    parser.parse(meshPath);
    
    updateBound();
    
    buildBuffer();
    
    return true;
}

bool SMesh::init(std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode) {
    setDrawMode(drawMode);
    
    for (int i=0; i<vertices.size(); i++) {
        this->vertices.push_back(vertices[i]);
    }
    for (int i=0; i<indices.size(); i++) {
        this->indices.push_back(indices[i]);
    }
    
    buildBuffer();
    
    return true;
}

void SMesh::updateBound() {
    for (int i=0; i<verticesCount(); i++) {
        float x = vertices[i * 3];
        float y = vertices[i * 3 + 1];
        float z = vertices[i * 3 + 2];
        
        _bound.compareAndSet(Vec3(x, y, z));
    }
}

void SMesh::add(dataType type, float d1, float d2, float d3) {
    if (type == TYPE_VERTEX) {
        vertices.push_back(d1);
        vertices.push_back(d2);
        vertices.push_back(d3);
    }
    else if (type == TYPE_NORMAL) {
        normals.push_back(d1);
        normals.push_back(d2);
        normals.push_back(d3);
    }
    else if (type == TYPE_FACE) {
        indices.push_back(d1 - 1);
        indices.push_back(d2 - 1);
        indices.push_back(d3 - 1);
    }
    else {
        CCLOGERROR("SMesh: Invalid data type: %d", type);
    }
}

void SMesh::buildBuffer() {
    cleanBuffer();
    
    verticesRenderData.clear();
    
    bool hasNormal = normals.size() > 0;
    
    for (int i=0; i<vertices.size() / 3; i++) {
        verticesRenderData.push_back(vertices[i * 3]);
        verticesRenderData.push_back(vertices[i * 3 + 1]);
        verticesRenderData.push_back(vertices[i * 3 + 2]);
        
        if (hasNormal) {
            verticesRenderData.push_back(normals[i * 3]);
            verticesRenderData.push_back(normals[i * 3 + 1]);
            verticesRenderData.push_back(normals[i * 3 + 2]);
        }
    }

    // TODO: Should be more generic
    int numberOfValuesBetweenElement = hasNormal ? 6 : 3;
    _vertexRenderDataSize = numberOfValuesBetweenElement * sizeof(GL_FLOAT);
    
    if (hasNormal) {
        _pointerToNormal = &verticesRenderData[3];
    }
    
    // vertex buffer
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    
    glBufferData(GL_ARRAY_BUFFER, verticesRenderData.size() * sizeof(GLfloat), &verticesRenderData[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // index buffer
    glGenBuffers(1, &_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SMesh::cleanBuffer() {
    if (glIsBuffer(_vertexBuffer)) {
        glDeleteBuffers(1, &_vertexBuffer);
        _vertexBuffer = 0;
    }
    
    if (glIsBuffer(_indexBuffer)) {
        glDeleteBuffers(1, &_indexBuffer);
        _indexBuffer = 0;
    }
}

void SMesh::setDrawMode (GLenum drawMode) {
    if (drawMode == GL_TRIANGLES || drawMode == GL_LINES) {
        _drawMode = drawMode;
    }
    else {
        CCLOG("Invalid Drawmode: %d", drawMode);
    }
}

int SMesh::getNumberOfValuesPerElement() {
    if (_drawMode == GL_TRIANGLES) {
        return 3;
    }
    else {
        return 2;
    }
}


void SMesh::addVertex(float x, float y, float z, int index) {
    add(TYPE_VERTEX, x, y, z);
}

void SMesh::addNormal(float x, float y, float z, int index) {
    add(TYPE_NORMAL, x, y, z);
}

void SMesh::addFace(int v1, int v2, int v3, int index) {
    add(TYPE_FACE, v1, v2, v3);
}

int SMesh::verticesCount() {
    return vertices.size() / 3;
}

int SMesh::normalsCount() {
    return normals.size() / 3;
}

int SMesh::facesCount() {
    return indices.size() / 3;
}

NS_S_END