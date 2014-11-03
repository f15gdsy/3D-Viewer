//
//  SMFileParser.cpp
//  3DCocos
//
//  Created by Yifeng Wu on 10/10/14.
//
//

#include "SMFileParser.h"
#include "platform/CCFileUtils.h"
#include "math/CCMath.h"

#define STATE_VERTEX 1
#define STATE_NORMAL 2
#define STATE_FACE 3

USING_NS_CC;
using namespace std;

NS_S_BEGIN

void SMFileParser::parse(const std::string &fileName) {
    _data = FileUtils::getInstance()->getStringFromFile(fileName);
    
    int count = _data.length();
    _currentPosition = 0;
    int subDataCount = 0;
    int state = STATE_VERTEX;
    
    int verticesIndex = -1;
    int normalsIndex = -1;
    int facesIndex = -1;
    
    float vertex[3] = {0, 0, 0};
    float normal[3] = {0, 0, 0};
    int face[3] = {0, 0, 0};

    while (_currentPosition < count) {
        if (state == STATE_VERTEX) {
            if (subDataCount == 0) {
//                int vertexIndex = getNextInt();
//                CCLOG("Vertex %d", vertexIndex);
                getNextInt();
                subDataCount++;
            }
            else if (subDataCount < 4) {
                float vertexData = getNextFloat();
//                CCLOG("%f", vertexData);
                
                vertex[subDataCount-1] = vertexData;
                
                subDataCount++;
                verticesIndex++;
            }
            else {
//                CCLOG("Normal");
                subDataCount = 0;
                state = STATE_NORMAL;
                
                _delegate->addVertex(vertex[0], vertex[1], vertex[2], verticesIndex);
            }
        }
        else if (state == STATE_NORMAL) {
            if (subDataCount < 3) {
                float normalData = getNextFloat();
//                CCLOG("%f", normalData);
                
                normal[subDataCount] = normalData;
                
                subDataCount++;
                normalsIndex++;
            }
            else {
                subDataCount = 0;
                if (checkNextString("Vertex")) {
                    state = STATE_VERTEX;
                }
                else if (checkNextString("Face")) {
                    state = STATE_FACE;
                }
                else {
                    CCLOGERROR("SMFileParser: no Face defined.");
                }
                
                _delegate->addNormal(normal[0], normal[1], normal[2], normalsIndex);
            }
        }
        else if (state == STATE_FACE) {
            if (subDataCount == 0) {
//                int faceIndex = getNextInt();
//                CCLOG("Face %d", faceIndex);
                getNextInt();
                subDataCount++;
                facesIndex++;
            }
            else if (subDataCount < 4) {
                int vertexIndex = getNextInt();
//                CCLOG("%d", vertexIndex);
                
                face[subDataCount-1] = vertexIndex;
                subDataCount++;
            }
            else {
                subDataCount = 0;
                
                _delegate->addFace(face[0], face[1], face[2], facesIndex);
            }
        }
        else {
            CCLOGERROR("SMFileParser: Invalid state: %d", state);
        }
    }
    
    // The last face is not added
    if (facesIndex == _delegate->facesCount()) {
        _delegate->addFace(face[0], face[1], face[2], facesIndex);
    }
}

int SMFileParser::getNextInt() {
    string temp;

    for (int i=_currentPosition; i<_data.length(); i++) {
        _currentPosition++;
        
        char c = _data[i];
        
        if (isdigit(c)) {
            temp += c;
        }
        else if (temp.length() != 0) {
            break;
        }
    }
    
    return atoi(temp.c_str());
}

enum class ParseFloatState {
    BEGIN,
    INTEGER,
    FLOATING_POINT,
    EXPONENTIAL,
};

float SMFileParser::getNextFloat() {
    ParseFloatState state = ParseFloatState::BEGIN;
    
    string temp;
    string tempE;
    bool negativeE = false;
    
    for (int i=_currentPosition; i<_data.length(); i++) {
        _currentPosition++;
        
        char c = _data[i];
        bool isCValid = false;
        
        if (state == ParseFloatState::BEGIN) {
            if (c == '-' || isdigit(c)) {
                temp += c;
                state = ParseFloatState::INTEGER;
            }
            isCValid = true;
        }
        else if (state == ParseFloatState::INTEGER) {
            if (isdigit(c) || c == '.') {
                if (c == '.') {
                    state = ParseFloatState::FLOATING_POINT;
                }
                temp += c;
                isCValid = true;
            }
        }
        else if (state == ParseFloatState::FLOATING_POINT) {
            if (isdigit(c)) {
                temp += c;
                isCValid = true;
            }
            else if (c == 'e') {
                state = ParseFloatState::EXPONENTIAL;
                isCValid = true;
            }
        }
        else if (state == ParseFloatState::EXPONENTIAL) {
            if (isdigit(c)) {
                tempE += c;
                isCValid = true;
            }
            else if (c == '-') {
                negativeE = true;
                isCValid = true;
            }
        }
        
        if (!isCValid && temp.length() > 0) {
            break;
        }
    }
    
    float floatingPoint = atof(temp.c_str());
    float exponential = negativeE ? 0.1 : 10;
    if (tempE.length() > 0) {
        exponential = pow(exponential, atof(tempE.c_str()));
        floatingPoint *= exponential;
    }
    
    return floatingPoint;
}

bool SMFileParser::checkNextString(const std::string str) {
    int pos = _data.find(str, _currentPosition);
    if (pos == string::npos) {
        return false;
    }
    else {
        _currentPosition += str.length();
        return true;
    }
}

NS_S_END