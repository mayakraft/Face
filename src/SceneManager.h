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
    
    vector<BaseScene*> scenes;
    int currentScene;
    
    void setup();
    void update();
    void draw();
    
    void advanceScene();
//    void regressScene();
    void startScene(int whichScene);
    
    ofFbo sceneFbo;

//    ofShader dimmerShader;
    
    
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
    
private:
    
    float masterFade; // 0 to 1



    // nothing here for now
    ofxPanel gui;
    ofParameter<bool>drawFacePoints, enableParameterSounds;
    ofParameter<float> autoadvanceDelay, ofSmoothing;

};

#endif /* SceneManager_hpp */