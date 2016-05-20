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
    
//private:
//    void nextScene(bool forward);
    
public:
    
    vector<BaseScene*> scenes;
    int currentScene;
    
    void setup();
    void update();
    void draw();
    
    ofFbo sceneFbo;    
    
    // scene transition stuff
    float SCENE_INTERVAL; // seconds
    float FADE_DURATION;  // shorter than SCENE_INTERVAL
    bool isSceneTransition;
    unsigned long sceneTransitionStartTime;
    float sceneTransitionTween;
    
    
    // face orientation stuff
    ofMatrix4x4 faceScaleMatrix;
    ofPoint faceCenterSmooth;
    float faceScaleSmooth;
    bool faceFound;
    
    // face stuff
    ofPoint faceLeftEye;
    ofPoint faceRightEye;
    ofPoint faceMouth;
    ofPoint faceNose;
    
    
    float masterFade; // 0 to 1
    
    unsigned long masterLoopStartTime = 0;

};

#endif /* SceneManager_hpp */