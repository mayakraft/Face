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


#define NUM_SCENES 2


//-----------------------------------------------------------------------------------
SceneManager::~SceneManager(){
//    ofRemoveListener(sync.ffwKeyPressed, this, &sceneManager::setAdvanceCurrentScene);
}
//-----------------------------------------------------------------------------------
void SceneManager::setup(){
 
    // add all possible scenes to the scenes array
//    scenes.push_back(new CirclesScene());
//    scenes.push_back(new ArcsScene());
    scenes.push_back(new ConicsScene());
    scenes.push_back(new HypercubeScene());

    sceneFbo.allocate(VISUALS_WIDTH, VISUALS_HEIGHT, GL_RGBA, 4);
    lastSceneFbo.allocate(VISUALS_WIDTH, VISUALS_HEIGHT, GL_RGBA, 4);
    
    dimmerShader.load("dimmer");
    
    
//    ofPixels lastFrame, currFrame;
//    lastFrame.allocate(VISUALS_WIDTH, VISUALS_HEIGHT, OF_PIXELS_RGBA);
//    currFrame.allocate(VISUALS_WIDTH, VISUALS_HEIGHT, OF_PIXELS_RGBA);
    

    // setup GUI
    gui.setDefaultWidth(300);
    gui.setup("Control Panel");
    gui.add(enableParameterSounds.set("Enable parameter sounds", true));
    gui.add(drawFacePoints.set("draw face points", true));
//    gui.add(bAutoPlay.set("Auto Play on scene change", false));
//    gui.add(autoadvanceDelay.set("Autoadvance", 0, 0, 60));
//    gui.add(bSceneWaitForCode.set("Scene wait for code", true));
//    gui.add(bFadeOut.set("Scene fade out", true));
//    gui.add(bAutoAdvance.set("Auto Advance Scene", false));
    gui.setPosition(ofGetWidth() - gui.getShape().width-20,  ofGetHeight() - gui.getShape().getHeight() - 100);
    

    
    for (auto scene : scenes){
        scene->dimensions.set(0,0,VISUALS_WIDTH, VISUALS_HEIGHT);
        scene->setup();
    }
    
    
    SCENE_INTERVAL = 20;
    FADE_DURATION = 3.0;

    currentScene = 0;
    startScene(currentScene);
    
    masterFade = 0.0;
    
    faceLeftEye = ofPoint(0, 0);
    faceRightEye = ofPoint(0, 0);
    faceMouth = ofPoint(0, 0);
    faceNose = ofPoint(0, 0);
}
//-----------------------------------------------------------------------------------

static float fadeAmnt;

void SceneManager::startScene(int whichScene){
    scenes[currentScene]->reset();
}

void SceneManager::advanceScene(){
    
}

void SceneManager::nextScene(bool forward){
    
}
//-----------------------------------------------------------------------------------
void SceneManager::update(){
    if(ofGetElapsedTimef() > fadeStartTime + SCENE_INTERVAL){
        // begin fade transition
        fadeStartTime = ofGetElapsedTimef();
        lastScene = currentScene;
        currentScene = (currentScene + 1)%NUM_SCENES;
        isFading = true;
        scenes[currentScene]->reset();
    }
    if(isFading){
        fadeAmnt = (ofGetElapsedTimef() - fadeStartTime) / FADE_DURATION;
        if(fadeAmnt > 1.0)
            fadeAmnt = 1.0;
        if(fadeAmnt < 0.0)
            fadeAmnt = 0.0;
        if(ofGetElapsedTimef() > fadeStartTime + FADE_DURATION)
            isFading = false;
    }
    
    if(isFading){
        scenes[lastScene]->faceLeftEye = faceLeftEye;
        scenes[lastScene]->faceRightEye = faceRightEye;
        scenes[lastScene]->faceMouth = faceMouth;
        scenes[lastScene]->faceNose = faceNose;
        scenes[lastScene]->update();

        scenes[lastScene]->faceScaleMatrix = faceScaleMatrix;

    }
    scenes[currentScene]->faceLeftEye = faceLeftEye;
    scenes[currentScene]->faceRightEye = faceRightEye;
    scenes[currentScene]->faceMouth = faceMouth;
    scenes[currentScene]->faceNose = faceNose;
    scenes[currentScene]->update();
    
    scenes[currentScene]->faceScaleMatrix = faceScaleMatrix;
}

//-----------------------------------------------------------------------------------
void SceneManager::draw(){

//    ofClear(0,0,0,255);
    ofSetColor(255);

    
    // FILL BUFFERS
    if (true){//shouldDrawScene) {
        sceneFbo.begin();
        ofClear(0,0,0,255);
        ofPushStyle();
        scenes[currentScene]->draw();
        ofPopStyle();
        sceneFbo.end();
        
        if(isFading){
            lastSceneFbo.begin();
            ofClear(0,0,0,255);
            ofPushStyle();
            scenes[lastScene]->draw();
            ofPopStyle();
            lastSceneFbo.end();
        }
        
//        dimmedSceneFbo.begin();
//        dimmerShader.begin();
//        dimmerShader.setUniformTexture("texture0", sceneFbo.getTexture(), 0);
//        dimmerShader.setUniform1f("dimAmt", fadeAmnt);
//        ofSetColor(255);
//        ofClearAlpha();
//        ofDrawRectangle(0, 0, VISUALS_WIDTH, VISUALS_HEIGHT);
//        dimmerShader.end();
//        dimmedSceneFbo.end();
//        dimmedSceneFbo.draw(1,0,VISUALS_WIDTH, VISUALS_HEIGHT);
//        dimmedSceneFbo.draw(0,0,VISUALS_WIDTH, VISUALS_HEIGHT);
        
    } else {
        ofSetColor(0);
        ofFill();
        ofClearAlpha();
        ofDrawRectangle(0, 0, VISUALS_WIDTH+1, VISUALS_HEIGHT);
    }
 
    
    // DRAW THE SCENES
    ofSetColor(255, 255 * masterFade);

    if(isFading){
        // draw previous scene fading out
        ofSetColor(255, masterFade * 255*(1-fadeAmnt) );
        lastSceneFbo.draw(0, 0);
        // set current screen fade in color
        ofSetColor(255, masterFade * 255*fadeAmnt);
    }
    
    sceneFbo.draw(0, 0);
    
    ofSetColor(255, 255);
//    ofDrawSphere(faceCenter, 10);
    
    gui.draw();
}

