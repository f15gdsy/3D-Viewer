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

#define FUNC_BTN_POS_X              850
#define FUNC_BTN_POS_Y_UNIT         60
#define COLOR_BTN_POS_X_UNIT        48
#define MODEL_BTN_POS_X_UNIT        150
#define MODEL_BTN_POS_Y             600
#define FONT_FILE                   "PressStart2P.ttf"
#define FONT_SIZE                   14
#define TUT_FONT_SIZE               12
#define TUT_MSG_POS_X               240
#define TUT_MSG_POS_Y               100
#define TUT_MSG_POS_Y_UNIT          -40

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
    CC_SAFE_RELEASE(_shaderProgramNormal);
    CC_SAFE_RELEASE(_shaderProgramFlat);
    CC_SAFE_RELEASE(_model);
    CC_SAFE_RELEASE(_boundingBox);
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
        
    FileUtils::getInstance()->addSearchPath("Models");
    FileUtils::getInstance()->addSearchPath("fonts");
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Create UI
    this->addChild(createUI(), 1);
    this->addChild(createTut());

    // Create 3D stuff
    _container3D = Node::create();
    _container3D->setPosition3D(Vec3(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y, 0));
    this->addChild(_container3D);
    
    _containerModel = Node::create();
    _container3D->addChild(_containerModel);
    
    _containerModel->setPosition3D(Vec3(9, 9, 8));
    
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    
    // Create Default Model
    SModel3d* model = createModel("knot.txt");
    SModel3d* boundingBox = createBoundingBox(model);
    setCurrentModel(model, boundingBox);
    
    _cameraController = SCameraController::create(_containerModel);
    _cameraController->retain();
    
    SMatrixProvider::getInstance()->getProjectionMatrix = std::bind(&SCameraController::getProjectionMatrix, _cameraController);
    SMatrixProvider::getInstance()->getViewMatrix = std::bind(&SCameraController::getViewMatrix, _cameraController);
    
    
    _container3D->addChild(createAxisSystem());
    _container3D->addChild(createFloor(20, 5));
    
    
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

SModel3d* HelloWorld::createModel(const std::string &filename) {
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(filename);
    
    SModel3d* model = SModel3d::create(filePath);
    
    SBound bound = model->mesh->getBound();
    float sizeX = bound.getSize().x;
    float scale = 1;
    
    if (sizeX <= 1) {
        for (scale=10; scale<10000000; scale*=10) {
            if (sizeX * scale >= 10) {
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
    CCLOG("scale: %f", scale);
    model->setScale(scale);
    model->setPosition3D(Vec3(0, 0, 0));
    
    return model;
}

SModel3d* HelloWorld::createBoundingBox (SModel3d* model) {
    SBound bound = model->mesh->getBound();
    float scale = model->getScaleX();
    SPrimitiveBox* boundingBox = SPrimitiveBox::create(bound.minXYZ, bound.maxXYZ);
    boundingBox->setWireFrameEnabled(true);
    boundingBox->setScale(scale);
    
    return boundingBox;
}

void HelloWorld::setCurrentModel(Samurai::SModel3d* model, Samurai::SModel3d* boundingBox) {
    if (_model) {
        _containerModel->removeChild(_model);
        _model->release();
        _model = nullptr;
    }
    _model = model;
    _model->retain();
    _containerModel->addChild(_model);
    
    if (!_shaderProgramNormal) {
        _shaderProgramNormal = _model->getShaderProgram();
        _shaderProgramNormal->retain();
    }
    
    if (!_shaderProgramFlat) {
        _shaderProgramFlat = SShaderProgram::create("lighting_flat.vertexshader", "lighting_flat.fragmentshader");
        _shaderProgramFlat->retain();
    }
    
    if (_boundingBox) {
        _containerModel->removeChild(_boundingBox);
        _boundingBox->release();
        _boundingBox = nullptr;
    }
    _boundingBox = boundingBox;
    _containerModel->addChild(_boundingBox);
    _boundingBox->retain();
    
    _containerModel->setRotation3D(Vec3(0, 0, 0));
}

Node* HelloWorld::createTut() {
    Node* container = Node::create();
    
    std::string tutMessage = "1. left button DRAG to rotate model.";
    auto text = Label::createWithTTF(tutMessage, FONT_FILE, TUT_FONT_SIZE);
    text->setColor(Color3B(255, 255, 255));
    text->setPosition(TUT_MSG_POS_X, TUT_MSG_POS_Y);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    
    container->addChild(text);
    
    tutMessage = "2. middle button SCROLL to zoom     ";
    text = Label::createWithTTF(tutMessage, FONT_FILE, TUT_FONT_SIZE);
    text->setColor(Color3B(255, 255, 255));
    text->setPosition(TUT_MSG_POS_X, TUT_MSG_POS_Y - TUT_MSG_POS_Y_UNIT);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    
    container->addChild(text);
    
    tutMessage = "3. right button DRAG to move camera.";
    text = Label::createWithTTF(tutMessage, FONT_FILE, TUT_FONT_SIZE);
    text->setColor(Color3B(255, 255, 255));
    text->setPosition(TUT_MSG_POS_X, TUT_MSG_POS_Y - TUT_MSG_POS_Y_UNIT * 2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    
    container->addChild(text);
    
    return container;
}

Node* HelloWorld::createUI() {
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto menu = Menu::create();
    
    Size buttonSize;
    Size fontSize;
    
// --------boudning box-------
    auto button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::toggleBoundingBox, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT);
    
    menu->addChild(button);

    auto text = Label::createWithTTF("AABB Box", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
// --------Smooth Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::smoothRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 2);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Smooth", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Wire Frame Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::wireFrameRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 3);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Wireframe", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Flat Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::flatRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 4);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Flat", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);

    // --------Point Rendering-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::pointRender, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 5);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Point", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Toggle Perspective-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::toggleProjection, this));
    
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 6);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Projection", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Change Color-------
    // Red
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("red");
    button->setColor(Color3B(255, 0, 0));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X - COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 7);
    
    menu->addChild(button);
    
    // Green
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("green");
    button->setColor(Color3B(0, 255, 0));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 7);
    
    menu->addChild(button);

    
    // Blue
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("blue");
    button->setColor(Color3B(0, 0, 255));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X + COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 7);
    
    menu->addChild(button);
    
    // Yello
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("yellow");
    button->setColor(Color3B(255, 255, 0));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X - COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 8);
    
    menu->addChild(button);
    
    // Cyan
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("cyan");
    button->setColor(Color3B(0, 255, 255));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 8);
    
    menu->addChild(button);
    
    
    // Megenta
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeColor, this));
    button->setName("megenta");
    button->setColor(Color3B(255, 0, 255));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X + COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 8);
    
    menu->addChild(button);

    // --------Light Position-------
    // Left
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeLight, this));
    button->setName("left");
//    button->setColor(Color3B(255, 255, 0));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X - COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 9);
    
    text = Label::createWithTTF("L", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    menu->addChild(button);
    
    // Middle
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeLight, this));
    button->setName("middle");
//    button->setColor(Color3B(0, 255, 255));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X, FUNC_BTN_POS_Y_UNIT * 9);
    
    text = Label::createWithTTF("M", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    menu->addChild(button);
    
    
    // Right
    button = MenuItemImage::create("button_2_default.png", "button_2_pressed.png", CC_CALLBACK_1(HelloWorld::changeLight, this));
    button->setName("right");
//    button->setColor(Color3B(255, 0, 255));
    buttonSize = button->getContentSize();
    button->setPosition(FUNC_BTN_POS_X + COLOR_BTN_POS_X_UNIT, FUNC_BTN_POS_Y_UNIT * 9);
    
    text = Label::createWithTTF("R", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    menu->addChild(button);
    
    // --------Bunny Model-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::changeModel, this));
    button->setName("bunny.txt");
    
    buttonSize = button->getContentSize();
    button->setPosition(MODEL_BTN_POS_X_UNIT * 1, MODEL_BTN_POS_Y);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Bunny", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Cap Model-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::changeModel, this));
    button->setName("cap.txt");
    
    buttonSize = button->getContentSize();
    button->setPosition(MODEL_BTN_POS_X_UNIT * 2, MODEL_BTN_POS_Y);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Cap", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Eight Model-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::changeModel, this));
    button->setName("eight.txt");
    
    buttonSize = button->getContentSize();
    button->setPosition(MODEL_BTN_POS_X_UNIT * 3, MODEL_BTN_POS_Y);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Eight", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Knot Model-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::changeModel, this));
    button->setName("knot.txt");
    
    buttonSize = button->getContentSize();
    button->setPosition(MODEL_BTN_POS_X_UNIT * 4, MODEL_BTN_POS_Y);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Knot", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    // --------Gargoyle Model-------
    button = MenuItemImage::create("button_1_default.png", "button_1_pressed.png", CC_CALLBACK_1(HelloWorld::changeModel, this));
    button->setName("gargoyle.txt");
    
    buttonSize = button->getContentSize();
    button->setPosition(MODEL_BTN_POS_X_UNIT * 5, MODEL_BTN_POS_Y);
    
    menu->addChild(button);
    
    text = Label::createWithTTF("Gargoyle", FONT_FILE, FONT_SIZE);
    text->setColor(Color3B(1, 1, 1));
    text->setPosition(buttonSize.width/2, buttonSize.height/2);
    text->setHorizontalAlignment(TextHAlignment::RIGHT);
    text->setVerticalAlignment(TextVAlignment::BOTTOM);
    button->addChild(text);
    
    
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
    _model->mesh->setDrawMode(GL_TRIANGLES);
}

void HelloWorld::wireFrameRender(cocos2d::Ref *sender) {
    CCLOG("Wire Frame Render");
    
    _model->setWireFrameEnabled(true);
    _model->mesh->setDrawMode(GL_TRIANGLES);
}

void HelloWorld::flatRender(cocos2d::Ref *sender) {
    CCLOG("Flat Render");
    
    _model->setWireFrameEnabled(false);
    _model->setShader(_shaderProgramFlat);
    _model->mesh->setDrawMode(GL_TRIANGLES);
}

void HelloWorld::pointRender(cocos2d::Ref *sender) {
    CCLOG("Point Render");
    
    _model->setWireFrameEnabled(false);
    _model->mesh->setDrawMode(GL_POINTS);
}

void HelloWorld::toggleProjection (cocos2d::Ref* sender) {
    CCLOG("Toggle Projection");
    
    _cameraController->setPerspective(!_cameraController->getPerspective());
}

void HelloWorld::changeModel (cocos2d::Ref* sender) {
    Node* senderNode = (Node *) sender;
    SModel3d* model = createModel(senderNode->getName());
    model->setName("Model");
    SModel3d* boundingBox = createBoundingBox(model);
    setCurrentModel(model, boundingBox);
    
    _cameraController->updateMatrices();
    
    smoothRender(nullptr);
}

void HelloWorld::changeColor(cocos2d::Ref *sender) {
    Node* senderNode = (Node *) sender;
    Vec3 color;
    if (strcmp(senderNode->getName().c_str(), "red") == 0) {
        color = Vec3(1, 0, 0);
    }
    else if (strcmp(senderNode->getName().c_str(), "green") == 0) {
        color = Vec3(0, 1, 0);
    }
    else if (strcmp(senderNode->getName().c_str(), "blue") == 0) {
        color = Vec3(0, 0, 1);
    }
    else if (strcmp(senderNode->getName().c_str(), "yellow") == 0) {
        color = Vec3(1, 1, 0);
    }
    else if (strcmp(senderNode->getName().c_str(), "cyan") == 0) {
        color = Vec3(0, 1, 1);
    }
    else if (strcmp(senderNode->getName().c_str(), "megenta") == 0) {
        color = Vec3(1, 0, 1);
    }
    
    _model->setColor(color);
}

void HelloWorld::changeLight(cocos2d::Ref *sender) {
    Node* senderNode = (Node *) sender;
    Vec3 lightPosition;
    if (strcmp(senderNode->getName().c_str(), "left") == 0) {
        lightPosition = Vec3(100, 100, 100);
    }
    else if (strcmp(senderNode->getName().c_str(), "middle") == 0) {
        lightPosition = Vec3(480, 400, 200);
    }
    else if (strcmp(senderNode->getName().c_str(), "right") == 0) {
        lightPosition = Vec3(800, 400, 200);
    }
    
    _model->setLight(lightPosition);
}