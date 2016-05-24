//
//  SceneManager.cpp
//  sceneSwitcher
//
//  Created by Robby on 5/6/16.
//
//

#include "SceneManager.h"

#include "appConstants.h"

#include "ConicsScene.h"
#include "hypercubeScene.h"
#include "CirclesScene.h"
#include "ArcsScene.h"


#define NUM_SCENES 3

//-----------------------------------------------------------------------------------
void SceneManager::setup(){
    
    SCENE_INTERVAL = 16;
    FADE_DURATION = 3.0;
    
    scenes.push_back(new CirclesScene());
    scenes.push_back(new ConicsScene());
    scenes.push_back(new HypercubeScene());

    sceneFbo.allocate(RESOLUTION_SCENE_WIDTH, RESOLUTION_SCENE_HEIGHT, GL_RGBA, 4);
    
    for (auto scene : scenes){
        scene->setup();
    }
    
    currentScene = 0;
    scenes[currentScene]->reset();
    
    masterFade = 0.0;
    faceFound = false;
    
    faceLeftEye = ofPoint(0, 0);
    faceRightEye = ofPoint(0, 0);
    faceMouth = ofPoint(0, 0);
    faceNose = ofPoint(0, 0);
}

void SceneManager::resetSequence(){
    currentScene = 0;
    masterLoopStartTime = ofGetElapsedTimef();
    sceneTransitionStartTime = 0;
}

//-----------------------------------------------------------------------------------
void SceneManager::update(){
    if(faceFound)
        masterFade = masterFade * .95 + .05;
    else
        masterFade = masterFade * .95;

    if(ofGetElapsedTimef() - masterLoopStartTime > sceneTransitionStartTime + SCENE_INTERVAL - FADE_DURATION){
        // begin fade transition
        sceneTransitionStartTime = ofGetElapsedTimef() - masterLoopStartTime;
        isSceneTransition = true;
    }
    if(isSceneTransition){
        sceneTransitionTween = (ofGetElapsedTimef() - masterLoopStartTime - sceneTransitionStartTime) / FADE_DURATION;
        if(sceneTransitionTween > 1.0)
            sceneTransitionTween = 1.0;
        if(sceneTransitionTween < 0.0)
            sceneTransitionTween = 0.0;
        if(ofGetElapsedTimef() - masterLoopStartTime > sceneTransitionStartTime + FADE_DURATION){
            // increment to the next scene
            currentScene = (currentScene + 1)%NUM_SCENES;
            sceneTransitionStartTime = ofGetElapsedTimef() - masterLoopStartTime;
            isSceneTransition = false;
            scenes[currentScene]->reset();
        }
    }
    
    scenes[currentScene]->faceLeftEye = faceLeftEye;
    scenes[currentScene]->faceRightEye = faceRightEye;
    scenes[currentScene]->faceMouth = faceMouth;
    scenes[currentScene]->faceNose = faceNose;
    scenes[currentScene]->faceCenterSmooth = faceCenterSmooth;
    scenes[currentScene]->faceScaleSmooth = faceScaleSmooth;
    scenes[currentScene]->faceScaleMatrix = faceScaleMatrix;

    scenes[currentScene]->update();
}

//-----------------------------------------------------------------------------------
void SceneManager::draw(){

    // FILL BUFFERS
    sceneFbo.begin();
    ofClear(255,0);
    ofSetColor(255,255);
    ofPushStyle();
    scenes[currentScene]->draw();
    ofPopStyle();
    sceneFbo.end();
    
    // DRAW THE SCENES
    ofSetColor(255, 255 * masterFade);
    if(isSceneTransition){
        // draw scene fading out
        ofSetColor(255, masterFade * 255*(1-sceneTransitionTween) );
    }
    sceneFbo.draw(-RESOLUTION_WINDOW_WIDTH * .5, -RESOLUTION_WINDOW_HEIGHT * .5, RESOLUTION_WINDOW_WIDTH, RESOLUTION_WINDOW_HEIGHT);

}

