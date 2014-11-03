#include <vector>

#include <OpenGL/gl.h>

#include "HelloWorldScene.h"
#include "SMesh.h"
#include "SModel3d.h"
#include "SCameraController.h"
#include "SMatrixProvider.h"
#include "SPrimitiveBox.h"
#include "SPrimitiveCylinder.h"
#include "SPrimitiveCone.h"

USING_NS_CC;
USING_NS_S;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

HelloWorld::~HelloWorld() {
    CC_SAFE_RELEASE(_cameraController);
}

bool HelloWorld::init()
{
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
    
    Node* containerModel = Node::create();
    container3D->addChild(containerModel);
    
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

    containerModel->addChild(model);
    
    _cameraController = SCameraController::create(containerModel);
    _cameraController->retain();
    
    SMatrixProvider::getInstance()->getProjectionMatrix = std::bind(&SCameraController::getProjectionMatrix, _cameraController);
    SMatrixProvider::getInstance()->getViewMatrix = std::bind(&SCameraController::getViewMatrix, _cameraController);
    
    
    SPrimitiveBox* boundingBox = SPrimitiveBox::create(bound.minXYZ, bound.maxXYZ);
    boundingBox->setWireFrameEnabled(true);
    boundingBox->setScale(scale);
    containerModel->addChild(boundingBox);
    
    container3D->addChild(createAxisSystem());
    
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

Node* HelloWorld::createAxisSystem() {
    Node* axisSystemContainer = Node::create();
    
    // Axis X
    SPrimitiveCylinder* axisX = SPrimitiveCylinder::create(Vec3(0, 0, 0), 0.5, 30, 50);
    axisX->setColor(Vec3(1, 0, 0));
    SPrimitiveCone* coneX = SPrimitiveCone::create(Vec3(0, 30, 0), 1, 2, 50);
    coneX->setColor(Vec3(1, 0, 0));

    axisX->addChild(coneX);
    axisX->setRotation3D(Vec3(0, 0, 90));
    axisSystemContainer->addChild(axisX);
    
    // Axis Y
    SPrimitiveCylinder* axisY = SPrimitiveCylinder::create(Vec3(0, 0, 0), 0.5, 30, 50);
    axisY->setColor(Vec3(0, 1, 0));
    SPrimitiveCone* coneY = SPrimitiveCone::create(Vec3(0, 30, 0), 1, 2, 50);
    coneY->setColor(Vec3(0, 1, 0));
    
    axisY->addChild(coneY);
    axisSystemContainer->addChild(axisY);
    
    // Axis Z
    SPrimitiveCylinder* axisZ = SPrimitiveCylinder::create(Vec3(0, 0, 0), 0.5, 30, 50);
    axisZ->setColor(Vec3(0, 0, 1));
    SPrimitiveCone* coneZ = SPrimitiveCone::create(Vec3(0, 30, 0), 1, 2, 50);
    coneZ->setColor(Vec3(0, 0, 1));
    
    axisZ->addChild(coneZ);
    axisZ->setRotation3D(Vec3(90, 0, 0));
    axisSystemContainer->addChild(axisZ);
    
    return axisSystemContainer;
}
