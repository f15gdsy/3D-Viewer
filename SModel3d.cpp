//
//  SModel3d.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 03/11/14.
//
//

#include <functional>

#include "renderer/CCRenderer.h"

#include "SModel3d.h"
#include "SShaderProgram.h"
#include "SMesh.h"
#include "SMatrixProvider.h"
#include "SMat3.h"

USING_NS_CC;

NS_S_BEGIN


int SModel3d::_totalSModel3dToRender = 0;
int SModel3d::_totalSModel3dRendered = 0;

SModel3d::~SModel3d() {
    clearVAO();
    
    CC_SAFE_RELEASE(_shaderProgram);
    CC_SAFE_RELEASE(mesh);
}

SModel3d* SModel3d::create(const std::string &modelPath) {
    auto ret = new SModel3d();
    if (ret && ret->init(modelPath)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

SModel3d* SModel3d::create(const std::string &modelPath, Samurai::SShaderProgram *shaderProgram) {
    auto ret = new SModel3d();
    if (ret && ret->init(modelPath, shaderProgram)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool SModel3d::init(const std::string &modelPath) {
    SShaderProgram* shaderProgram = SShaderProgram::create("lighting.vertexshader", "lighting.fragmentshader");
//    SShaderProgram* shaderProgram = SShaderProgram::create("mvp.vertexshader", "mvp.fragmentshader");
//    SShaderProgram* shaderProgram = SShaderProgram::create("normal.vertexshader", "normal.fragmentshader");
//    SShaderProgram* shaderProgram = SShaderProgram::create("normal_flat.vertexshader", "normal_flat.fragmentshader");
//     SShaderProgram* shaderProgram = SShaderProgram::create("lighting_flat.vertexshader", "lighting_flat.fragmentshader");
    
    return init(modelPath, shaderProgram);
}

bool SModel3d::init(const std::string &modelPath, Samurai::SShaderProgram *shaderProgram) {
    mesh = SMesh::create(modelPath);
    mesh->retain();
    
    setShader(shaderProgram);
    
    _lightPosition = Vec3(100, 100, 100);
    _color = Vec3(1, 1, 0);
    
    return true;
}

void SModel3d::setShader(Samurai::SShaderProgram *shaderProgram) {
    if (_shaderProgram) {
        CC_SAFE_RELEASE(_shaderProgram);
    }
    _shaderProgram = shaderProgram;
    _shaderProgram->retain();
}

void SModel3d::draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) {
    _renderCommand.init(-10);
    _renderCommand.func = std::bind(&SModel3d::renderFunc, this);
    
    if (strcmp(getName().c_str(), "Model") == 0) {
//        CCLOG("Drawing model");
    }
    _totalSModel3dToRender++;
    
    renderer->addCommand(&_renderCommand);
}

void SModel3d::renderFunc() {
    if (_vao == 0) {
        buildVAO();
    }
    
    GL::bindVAO(_vao);

    GL::useProgram(_shaderProgram->getProgramId());
    
    updateUniforms();
    updateDrawStates();
    
    glDrawElements(mesh->getDrawMode(), mesh->indices.size(), GL_UNSIGNED_INT, 0);
    
    _totalSModel3dRendered++;
    
    clearDrawStates();
    
    GL::bindVAO(0);
}

void SModel3d::testRenderFunc() {}

void SModel3d::buildVAO() {
    if (_vao) {
        clearVAO();
    }
    
    glGenVertexArrays(1, &_vao);
    GL::bindVAO(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());

    // vertex attribute position
    SAttribute* positionAttribute = _shaderProgram->getAttribute(S_ATTRIBUTE_NAME_POSITION);
    if (positionAttribute) {
        glEnableVertexAttribArray(positionAttribute->index);
        glVertexAttribPointer(positionAttribute->index, 3, GL_FLOAT, GL_FALSE, mesh->getVertexRenderDataSize(), 0);
    }
    
    SAttribute* normalAttribute = _shaderProgram->getAttribute(S_ATTRIBUTE_NAME_NORMAL);
    if (mesh->getHasNormal() && normalAttribute) {
        glEnableVertexAttribArray(normalAttribute->index);
        glVertexAttribPointer(normalAttribute->index, 3, GL_FLOAT, GL_FALSE, mesh->getVertexRenderDataSize(), (void *) (3 * sizeof(GLfloat)));
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
    
    GL::bindVAO(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SModel3d::clearVAO() {
    if (_vao) {
        glDeleteVertexArrays(1, &_vao);
        _vao = 0;
        GL::bindVAO(0);
    }
}

void SModel3d::updateUniforms() {
    Mat4 model = _modelViewTransform;
    Mat4 view = SMatrixProvider::getInstance()->getMatrix(SMatrixProvider::MatrixType::VIEW);
    Mat4 projection = SMatrixProvider::getInstance()->getMatrix(SMatrixProvider::MatrixType::PROJECTION);
    
    _shaderProgram->updateBuiltInUniforms(model, view, projection);
    
    SUniform* uniform = _shaderProgram->getUniform("LightPosition_world");
    if (uniform) {
        glUniform3f(uniform->index, _lightPosition.x, _lightPosition.y, _lightPosition.z);
    }
    
    uniform = _shaderProgram->getUniform("InversedTransposedModelMatrix");
    if (uniform) {
        SMat3 inverseTransposed(model);
        inverseTransposed.inverse();
        inverseTransposed.transpose();
        
        glUniformMatrix3fv(uniform->index, 1, GL_FALSE, inverseTransposed.m);
    }
    
    uniform = _shaderProgram->getUniform("S_Color");
    if (uniform) {
        glUniform3f(uniform->index, _color.x, _color.y, _color.z);
    }
}

void SModel3d::updateDrawStates() {
    if (_wireFrameEnabled) {
        glPolygonMode(GL_FRONT, GL_LINE);
        glPolygonMode(GL_BACK, GL_LINE);
    }
    
    glShadeModel(GL_FLAT);
    
    if (_depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }
}

void SModel3d::clearDrawStates() {
    if (_wireFrameEnabled) {
        glPolygonMode(GL_FRONT, GL_FILL);
        glPolygonMode(GL_BACK, GL_FILL);
    }
    
//    glShadeModel(GL_SMOOTH);
    
    if (_depthTestEnabled) {
        // Temporary solution to split 2d and 3d
        if (_totalSModel3dRendered >= _totalSModel3dToRender) {
            glClear(GL_DEPTH_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);
            _totalSModel3dRendered = 0;
            _totalSModel3dToRender = 0;
        }
    }
}



NS_S_END