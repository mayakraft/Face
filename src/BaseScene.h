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

class BaseScene {
    
public:
    
    virtual void setup(){}
    virtual void update(){}
    virtual void draw(){}
    
    virtual void reset(){}
    int resetMoment;
    
    BaseScene(){};
    ~BaseScene(){}
    
    vector<char> paramTypes;
    
    ofParameterGroup parameters;    // this is the parameters of your sketch...
    vector<ofParameter<bool>> boolParams;
    vector<ofParameter<int>> intParams;
    vector<ofParameter<float>> floatParams;
    ofParameterGroup midiParameters;
    
    ofRectangle dimensions;     // this is the dimensions of
    // the surface you are drawing into.
    
    // face stuff
    ofPoint faceLeftEye;
    ofPoint faceRightEye;
    ofPoint faceMouth;
    ofPoint faceNose;
    
    ofMatrix4x4 faceScaleMatrix;
};

#endif /* BaseScene_hpp */