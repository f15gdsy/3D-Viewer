//
//  SMatrixProvider.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 29/10/14.
//
//

#include "SMatrixProvider.h"
#include "base/CCDirector.h"

USING_NS_CC;

NS_S_BEGIN

SMatrixProvider* SMatrixProvider::s_instance = nullptr;

SMatrixProvider* SMatrixProvider::getInstance() {
    if (s_instance == nullptr) {
        s_instance = new SMatrixProvider();
        s_instance->init();
    }
    return s_instance;
}

bool SMatrixProvider::init() {
    // Use cocos2d-x's default matrices by default
    getProjectionMatrix = std::bind(&Director::getMatrix, Director::getInstance(), MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    // Cocos2d-x does not have a view matrix, it was multipiled with projection matrix already.
    getViewMatrix = std::bind(&SMatrixProvider::getMatrix, this, MatrixType::IDENTITY);
    
    return true;
}

Mat4 SMatrixProvider::getMatrix(MatrixType type) {
    if (type == MatrixType::PROJECTION) {
        if (getProjectionMatrix != nullptr) {
            return getProjectionMatrix();
        }
    }
    else if (type == MatrixType::VIEW) {
        if (getViewMatrix != nullptr) {
            return getViewMatrix();
        }
    }
    else if (type == MatrixType::IDENTITY) {
        return Mat4();
    }
    else {
        CCASSERT(false, "unkown matrix type");
    }
    
    return Mat4();
}



NS_S_END