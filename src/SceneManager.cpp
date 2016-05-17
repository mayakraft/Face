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
    
    SCENE_INTERVAL = 80;
    FADE_DURATION = 3.0;
    
    scenes.push_back(new ConicsScene());
    scenes.push_back(new HypercubeScene());
    scenes.push_back(new CirclesScene());

    sceneFbo.allocate(RESOLUTION_SCREEN_WIDTH, RESOLUTION_SCREEN_HEIGHT, GL_RGBA, 4);
    
    for (auto scene : scenes){
        scene->dimensions.set(0,0,RESOLUTION_SCREEN_WIDTH, RESOLUTION_SCREEN_HEIGHT);
        scene->setup();
    }
    
    currentScene = 0;
    startScene(currentScene);
    
    masterFade = 1.0;
    faceFound = false;
    
    faceLeftEye = ofPoint(0, 0);
    faceRightEye = ofPoint(0, 0);
    faceMouth = ofPoint(0, 0);
    faceNose = ofPoint(0, 0);
}
//-----------------------------------------------------------------------------------

void SceneManager::startScene(int whichScene){
    scenes[currentScene]->reset();
}

void SceneManager::advanceScene(){
    
}

void SceneManager::nextScene(bool forward){
    
}
//-----------------------------------------------------------------------------------
void SceneManager::update(){
    if(faceFound)
        masterFade = masterFade * .95 + .05;
    else
        masterFade = masterFade * .95;

    if(ofGetElapsedTimef() > sceneTransitionStartTime + SCENE_INTERVAL - FADE_DURATION){
        // begin fade transition
        sceneTransitionStartTime = ofGetElapsedTimef();
        isSceneTransition = true;
    }
    if(isSceneTransition){
        sceneTransitionTween = (ofGetElapsedTimef() - sceneTransitionStartTime) / FADE_DURATION;
        if(sceneTransitionTween > 1.0)
            sceneTransitionTween = 1.0;
        if(sceneTransitionTween < 0.0)
            sceneTransitionTween = 0.0;
        if(ofGetElapsedTimef() > sceneTransitionStartTime + FADE_DURATION){
            // increment to the next scene
            currentScene = (currentScene + 1)%NUM_SCENES;
            sceneTransitionStartTime = ofGetElapsedTimef();
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

    ofSetColor(255);

    // FILL BUFFERS
    if (true){//shouldDrawScene) {
        sceneFbo.begin();
        ofClear(0,255);
        ofPushStyle();
        scenes[currentScene]->draw();
        ofPopStyle();
        sceneFbo.end();
    } else {
        ofSetColor(0);
        ofFill();
        ofClearAlpha();
        ofDrawRectangle(0, 0, RESOLUTION_SCREEN_WIDTH, RESOLUTION_SCREEN_HEIGHT);
    }
    
    // DRAW THE SCENES
    ofSetColor(255, 255 * masterFade);

    if(isSceneTransition)
        // draw scene fading out
        ofSetColor(255, masterFade * 255*(1-sceneTransitionTween) );
    
    sceneFbo.draw(-RESOLUTION_SCREEN_WIDTH * .5, -RESOLUTION_SCREEN_HEIGHT * .5);
  
//    ofPushMatrix();
//    ofSetColor(255, 128, 0);
//    ofDrawCircle(faceLeftEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceRightEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceNose * faceScaleMatrix, 10);
//    ofDrawCircle(faceMouth * faceScaleMatrix, 10);
//    ofPopMatrix();

    ofSetColor(255, 255);
}

