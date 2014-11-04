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

#define BTN_POS_X           850
#define BTN_POS_Y_UNIT      60

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

    // Create UI
    this->addChild(createUI(), 1);

    // Create 3D stuff
    Node* container3D = Node::create();
    container3D->setPosition3D(Vec3(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y, 0));
    this->addChild(container3D);
    
    _containerModel = Node::create();
    container3D->addChild(_containerModel);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    FileUtils::getInstance()->addSearchPath("Models");
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("cap.txt");
    
    _model = SModel3d::create(filePath);
    _shaderProgramNormal = _model->getShaderProgram();
    _shaderProgramNormal->retain();
    
    SBound bound = _model->mesh->getBound();
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
    
    _model->setScale(scale);

    _containerModel->addChild(_model);
    
    _cameraController = SCameraController::create(_containerModel);
    _cameraController->retain();
    
    SMatrixProvider::getInstance()->getProjectionMatrix = std::bind(&SCameraController::getProjectionMatrix, _cameraController);
    SMatrixProvider::getInstance()->getViewMatrix = std::bind(&SCameraController::getViewMatrix, _cameraController);
    
    
    SPrimitiveBox* boundingBox = SPrimitiveBox::create(bound.minXYZ, bound.maxXYZ);
    boundingBox->setWireFrameEnabled(true);
    boundingBox->setScale(scale);
    _containerModel->addChild(boundingBox);
    _boundingBox = boundingBox;
    _boundingBox->retain();
    
    container3D->addChild(createAxisSystem());
    container3D->addChild(createFloor(20, 5));
    _container3D = container3D;
    
    
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

Node* HelloWorld::createFloor(int size, float gridSize) {
    float minX = - size * gridSize / 2;
    float minZ = minX;
    
    Node* floorContainer = Node::create();
    
    for (int iX=0; iX<size; iX++) {
        for (int iZ = 0; iZ<size; iZ++) {
            Vec3 minXYZ = Vec3(iX*gridSize, 0, iZ*gridSize);
            Vec3 maxXYZ = Vec3((iX+1)*gridSize, 0, (iZ+1)*gridSize);
            
            SPrimitiveBox *grid = SPrimitiveBox::create(minXYZ, maxXYZ);
            
            floorContainer->addChild(grid);
        }
    }
    
    floorContainer->setPosition3D(Vec3(minX, 0, minZ));
    
    return floorContainer;
}

Node* HelloWorld::createUI() {
    FileUtils::getInstance()->addSearchPath("fonts");
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto menu = Menu::create();
    
    Size buttonSize;
    Size fontSize;
    
// --------boudning box-------
    auto button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::toggleBoundingBox, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(BTN_POS_X, BTN_POS_Y_UNIT);
    
    menu->addChild(button);

    auto text = Label::createWithTTF("AABB Box", "pixelart.ttf", 18);
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text, -1);
    
// --------Smooth Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::smoothRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(BTN_POS_X, BTN_POS_Y_UNIT * 2);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Smooth Render", "pixelart.ttf", 18);
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text, -1);
    
    // --------Wire Frame Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::wireFrameRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(BTN_POS_X, BTN_POS_Y_UNIT * 3);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Wireframe Render", "pixelart.ttf", 18);
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text, -1);
    
    // --------Flat Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::flatRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(BTN_POS_X, BTN_POS_Y_UNIT * 4);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Flat Render", "pixelart.ttf", 18);
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text, -1);

    
    
    
    menu->setPosition(Vec2::ZERO);

    return menu;
}

void HelloWorld::reset(cocos2d::Ref *sender) {
    CCLOG("Reset...");
}

void HelloWorld::toggleBoundingBox(cocos2d::Ref *sender) {
    CCLOG("Toggle bounding box");
    
    bool on = _boundingBox->getParent() != nullptr;
    if (on) {
        _containerModel->removeChild(_boundingBox);
    }
    else {
        _containerModel->addChild(_boundingBox);
    }
}

void HelloWorld::smoothRender(cocos2d::Ref *sender) {
    CCLOG("Smooth Render");
    
    _model->setWireFrameEnabled(false);
    _model->setShader(_shaderProgramNormal);
}

void HelloWorld::wireFrameRender(cocos2d::Ref *sender) {
    CCLOG("Wire Frame Render");
    
    _model->setWireFrameEnabled(true);
}

void HelloWorld::flatRender(cocos2d::Ref *sender) {
    CCLOG("Flat Render");
    
    if (_shaderProgramFlat == nullptr) {
        _shaderProgramFlat = SShaderProgram::create("lighting_flat.vertexshader", "lighting_flat.fragmentshader");
        _shaderProgramFlat->retain();
    }
    
    _model->setWireFrameEnabled(false);
    _model->setShader(_shaderProgramFlat);
}