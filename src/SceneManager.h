//
//  SceneManager.hpp
//  sceneSwitcher
//
//  Created by Robby on 5/6/16.
//
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include "ofMain.h"
#include "BaseScene.h"
#include "ofxGui.h"
#include <stdio.h>

class SceneManager {
    
private:
    void nextScene(bool forward);
    
public:
    
    ~SceneManager();
    vector<BaseScene*> scenes;
    int currentScene, lastScene;
    
    void setup();
    void update();
    void draw();
    
    void advanceScene();
//    void regressScene();
    void startScene(int whichScene);
    
    ofFbo sceneFbo;
    ofFbo lastSceneFbo;

    ofShader dimmerShader;
    
    float SCENE_INTERVAL; // seconds
    float FADE_DURATION;  // shorter than SCENE_INTERVAL

    bool isFading;
    unsigned long fadeStartTime;
    
    ofxPanel gui;
    ofParameter<bool>drawFacePoints, enableParameterSounds;
    ofParameter<float> autoadvanceDelay, ofSmoothing;
    float lastAutoadvanceTime;
    
    float masterFade; // 0 to 1
    
    ofPoint faceLeftEye;
    ofPoint faceRightEye;
    ofPoint faceMouth;
    ofPoint faceNose;
    
    ofMatrix4x4 faceScaleMatrix;

};

#endif /* SceneManager_hpp */