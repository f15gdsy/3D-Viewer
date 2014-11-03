#include <vector>

#include <OpenGL/gl.h>

#include "HelloWorldScene.h"
#include "SMesh.h"
#include "SModel3d.h"
#include "SCameraController.h"
#include "SMatrixProvider.h"
#include "SPrimitiveBox.h"

USING_NS_CC;
USING_NS_S;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld::~HelloWorld() {
    CC_SAFE_RELEASE(_cameraController);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);



    Node* container3D = Node::create();
    container3D->setPosition3D(Vec3(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y, 0));
    this->addChild(container3D);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    FileUtils::getInstance()->addSearchPath("Models");
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("cap.txt");
    
    SModel3d* model = SModel3d::create(filePath);
    
    SBound bound = model->mesh->getBound();
    float sizeX = bound.getSize().x;
    float scale = 1;
    
    if (sizeX <= 1) {
        for (scale=10; scale<10000000; scale*=10) {
            if (sizeX * scale >= 20) {
                break;
            }
        }
    }
    else if (sizeX >= 20) {
        for (scale = 0.1; scale>0.00000001; scale*=0.1) {
            if (sizeX * scale <= 20) {
                break;
            }
        }
    }
    
    model->setScale(scale);

    container3D->addChild(model);
    
    _cameraController = SCameraController::create(container3D);
    _cameraController->retain();
    
    SMatrixProvider::getInstance()->getProjectionMatrix = std::bind(&SCameraController::getProjectionMatrix, _cameraController);
    SMatrixProvider::getInstance()->getViewMatrix = std::bind(&SCameraController::getViewMatrix, _cameraController);
    
    
    SPrimitiveBox* boundingBox = SPrimitiveBox::create(bound.minXYZ, bound.maxXYZ);
    boundingBox->setWireFrameEnabled(true);
    boundingBox->setScale(scale);
    container3D->addChild(boundingBox);
    
//    int verticesCount = (model->mesh->vertices).size();
//    for (int i=0, j=0; i<verticesCount; j++) {
//        float x = model->mesh->vertices[i++];
//        float y = model->mesh->vertices[i++];
//        float z = model->mesh->vertices[i++];
//        CCLOG("vertex %d: (%f, %f, %f)", j, x, y, z);
//    }
//    
//    int normalsCount = (model->mesh->normals).size();
//    for (int i=0, j=0; i<normalsCount; j++) {
//        float x = model->mesh->normals[i++];
//        float y = model->mesh->normals[i++];
//        float z = model->mesh->normals[i++];
//        CCLOG("normal %d: (%f, %f, %f)", j, x, y, z);
//    }
//    
//    int facesCount = (model->mesh->indices
//).size();
//    for (int i=0, j=0; i<facesCount; j++) {
//        unsigned short v1 = model->mesh->indices[i++];
//        unsigned short v2 = model->mesh->indices[i++];
//        unsigned short v3 = model->mesh->indices[i++];
//        CCLOG("face %d: (%d, %d, %d)", j, v1, v2, v3);
//    }
//    
//    CCLOG("vertices size: %lu", model->mesh->vertices.size());
//    CCLOG("indices size: %lu", model->mesh->indices.size());
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
