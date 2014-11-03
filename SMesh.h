//
//  SMesh.h
//  3DCocos
//
//  Created by Yifeng Wu on 17/10/14.
//
//

#ifndef ___DCocos__SMesh__
#define ___DCocos__SMesh__

#include <vector>
#include <string>
#include <OpenGl/gl.h>

#include "base/CCRef.h"

#include "SMacros.h"
#include "SMFileParser.h"
#include "SShaderProgram.h"
#include "SBound.h"
#include "SMeshDataDelegate.h"


NS_S_BEGIN

#define TYPE_VERTEX     1
#define TYPE_NORMAL     2
#define TYPE_FACE       3

typedef int dataType;

struct SFace {
public:
    SFace (unsigned short v1, unsigned short v2, unsigned short v3) : v1(v1), v2(v2), v3(v3) {}
    unsigned short v1, v2, v3;
};

struct SVec3 {
public:
    SVec3 (float x, float y, float z) : x(x), y(y), z(z) {}
    float x, y, z;
};


class SMesh : public cocos2d::Ref, public SMeshDataDelegate {
public:
    static SMesh* create (const std::string &meshPath);
    static SMesh* create (std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode);
    
    void add (dataType type, float d1, float d2, float d3);
    
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;

    std::vector<GLfloat> verticesRenderData;
    std::vector<GLuint> indices;
    
    GLuint getVertexBuffer () {return _vertexBuffer;}
    GLuint getIndexBuffer () {return _indexBuffer;}
    int getVertexRenderDataSize () {
        return _vertexRenderDataSize;
    }
    GLvoid* getPointerToNormal () {
        return _pointerToNormal;
    }
    GLenum getDrawMode () {
        return _drawMode;
    }
    void setDrawMode (GLenum drawMode);
    int getNumberOfValuesPerElement ();
    bool getHasNormal () {
        return normals.size() > 0;
    }
    
    SBound getBound () {
        return _bound;
    }
    
    void buildBuffer();
    void cleanBuffer();

    
protected:
    virtual ~SMesh ();
    
    bool init (const std::string &meshPath);
    bool init (std::vector<GLfloat> vertices, std::vector<GLuint> indices, GLenum drawMode);
    
    void updateBound ();
    
    GLuint _vertexBuffer;
    GLuint _indexBuffer;
    int _vertexRenderDataSize;
    GLvoid *_pointerToNormal;
    
    GLenum _drawMode;
    
    SBound _bound;
    
public:
    // SMeshDataDelegate
    virtual void addVertex (float x, float y, float z, int index) override;
    virtual void addNormal (float x, float y, float z, int index) override;
    virtual void addFace (int v1, int v2, int v3, int index) override;
    virtual int verticesCount () override;
    virtual int normalsCount () override;
    virtual int facesCount () override;
};

NS_S_END

#endif /* defined(___DCocos__SMesh__) */
