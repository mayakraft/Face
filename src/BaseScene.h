//
//  BaseScene.hpp
//  sceneSwitcher
//
//  Created by Robby on 5/6/16.
//
//

#ifndef BaseScene_hpp
#define BaseScene_hpp

#include "ofMain.h"
#include <stdio.h>
#include "appConstants.h"

class BaseScene {
    
public:
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    
    virtual void reset(){}
    int resetMoment;
    
    BaseScene(){};
    ~BaseScene(){}
        
    // face orientation stuff
    ofMatrix4x4 faceScaleMatrix;
    ofPoint faceCenterSmooth;
    float faceScaleSmooth;
    
    // face stuff
    ofPoint faceLeftEye;
    ofPoint faceRightEye;
    ofPoint faceMouth;
    ofPoint faceNose;
    
};

#endif /* BaseScene_hpp */