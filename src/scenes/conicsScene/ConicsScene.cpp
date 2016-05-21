#include "ConicsScene.h"

//--------------------------------------------------------------

void ConicsScene::setup(){
    for(int i = 0; i < NUM_CONES; i++){
        conics[i].setRadius(20 + i*5);
    }
    numCones = 1;

    plane = ofVec3f(0, 0, 100 * sinf(ofGetElapsedTimef()));
    planeNormal = ofVec3f(-100, 0, 100);
    faceNoseSmooth1 = ofVec3f(0, 0, 0);
    faceNoseSmooth2 = ofVec3f(0, 0, 0);
}

//--------------------------------------------------------------
void ConicsScene::update(){
    
    if(numCones < NUM_CONES){
        numConesFloat *= 1.1;
        numCones = numConesFloat;
        if(numCones > NUM_CONES)
            numCones = NUM_CONES;
    }
    
    
    faceNoseSmooth1 = 0.9 * faceNoseSmooth1 + 0.1 * faceNose;
    faceNoseSmooth2 = 0.5 * faceNoseSmooth1 + 0.5 * faceNose;
    
    // breathing motion
    ofVec3f breathing = ofVec3f(0, 0, 40 * sinf(ofGetElapsedTimef()));
    plane = faceNoseSmooth1 * faceScaleMatrix;
    planeNormal = ofVec3f(-100, 0, 100);
    
    for(int i = 0; i < numCones; i++){
        float pctCones = i / ((float)numCones-1);
        ofVec3f interp = faceNoseSmooth1 * (pctCones) + faceNoseSmooth2 * (1-pctCones);
        conics[i].setPosition( faceNoseSmooth1 * faceScaleMatrix + ofPoint(0, 0, 150) );
        conics[i].setLookAt( interp * faceScaleMatrix );
    }

    plane += breathing;
    
//    ofPoint vel = faceNose - lastFacePosition;
//    float d = sqrt( powf(vel.x, 2) + powf(vel.y, 2) );
//    smoothFaceMotionNoise += d;
//    smoothFaceMotionNoise *= .95;
//    if(smoothFaceMotionNoise < 0)
//        smoothFaceMotionNoise = 0;
//    lastFacePosition = faceNose;
//    float mag = smoothFaceMotionNoise / MAX_NOISE_VALUE;
//    float SPEED = .2;
//    float diff = .3 * sinf(ofGetElapsedTimef()*.2);
//    for(int i = 0; i < numCones; i++){
//        conics[i].setPosition( ofPoint(i * 10,
//                                       0,
//                                       200 + mag * 190 * sin(ofGetElapsedTimef() + i/100.0)
//                                        + 190 * sin(ofGetElapsedTimef()*.2 + i/100.0)));
//        
//        conics[i].setLookAt( ofPoint(200 + mag * 100 * sin(ofGetElapsedTimef() + i/100.0)
//                                               + 100 * sin(ofGetElapsedTimef()*.2 + i/100.0),
//                                     0 + mag * 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0),
//                                     0 + mag * 300 * cos(ofGetElapsedTimef() + i/10.0)));
//    }

}

void ConicsScene::reset(){
    resetMoment = ofGetElapsedTimef();
    numCones = 1;
    numConesFloat = 1.0;
    lastFacePosition = faceNose;
    smoothFaceMotionNoise = 0;
}

//--------------------------------------------------------------
void ConicsScene::draw(){
    
//    // SCENE TO WINDOW CONVERSION
    ofTranslate(RESOLUTION_SCENE_WIDTH * .5, RESOLUTION_SCENE_HEIGHT * .5);
    ofScale(RESOLUTION_SCENE_WIDTH / (float)RESOLUTION_WINDOW_WIDTH,
            RESOLUTION_SCENE_HEIGHT / (float)RESOLUTION_WINDOW_HEIGHT);
    
    ofSetColor(255, 255);

    for(int i = 0; i < numCones; i++){
        conics[i].drawIntersectionsWithPlane(plane, planeNormal);
    }
    
//    ofPushMatrix();
//    ofSetColor(0, 128, 255);
//    ofDrawCircle(faceLeftEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceRightEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceNose * faceScaleMatrix, 10);
//    ofDrawCircle(faceMouth * faceScaleMatrix, 10);
//    ofPopMatrix();
//    
}