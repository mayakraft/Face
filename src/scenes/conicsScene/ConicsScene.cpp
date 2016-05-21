#include "ConicsScene.h"

//--------------------------------------------------------------

void ConicsScene::setup(){
    for(int i = 0; i < NUM_CONES; i++){
        conics[i].setRadius(20 + i*5);
    }
    numCones = 1;

    plane = ofVec3f(0, 0, 100 * sinf(ofGetElapsedTimef()));
    planeNormal = ofVec3f(-100, 0, 100);
    faceSmoothFast = ofVec3f(0, 0, 0);
    faceSmoothMedium = ofVec3f(0, 0, 0);
    faceSmoothSlow = ofVec3f(0, 0, 0);
}

//--------------------------------------------------------------
void ConicsScene::update(){
    if(numCones < NUM_CONES){
        numConesFloat *= 1.1;
        numCones = numConesFloat;
        if(numCones > NUM_CONES)
            numCones = NUM_CONES;
    }
    
    faceSmoothSlow = 0.9 * faceSmoothSlow + 0.1 * faceNose;
    faceSmoothMedium = 0.6 * faceSmoothMedium + 0.4 * faceNose;
    faceSmoothFast = 0.3 * faceSmoothFast + 0.7 * faceNose;
    
    
    // PROGRAMS
    planePos1 = faceSmoothSlow * faceScaleMatrix;
    planeNorm1 = ofVec3f(-100, 0, 100);
    
    planePos2 = faceSmoothSlow * faceScaleMatrix;
    planeNorm2 = ofVec3f(-100, 0, 100);

    for(int i = 0; i < NUM_CONES; i++){
        float pctCones = i / ((float)NUM_CONES-1);

        // PROGRAM 1
        ofVec3f interp = faceSmoothSlow * (pctCones) + faceSmoothMedium * (1-pctCones);
        conePos1[i] = faceSmoothSlow * faceScaleMatrix + ofPoint(0, 0, 150);
        coneLook1[i] = interp * faceScaleMatrix;

        // PROGRAM 2
        conePos2[i] = ofPoint(i * 10,
                              0,
                              200 + 190 * sin(ofGetElapsedTimef() + i/100.0));
        coneLook2[i] = ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0),
                                               0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0),
                                               0 + 300 * cos(ofGetElapsedTimef() + i/10.0));

    }
    
    float weight = (sinf(ofGetElapsedTimef() * .5) + 1) * .5;
    float w1 = weight;
    float w2 = 1.0 - weight;
    
    // SET PROGRAM
    for(int i = 0; i < numCones; i++){
        conics[i].setPosition( conePos1[i] * w1 + conePos2[i] * w2 );
        conics[i].setLookAt( coneLook1[i] * w1 + coneLook2[i] * w2 );
    }
    plane = planePos1 * w1 + planePos2 * w2;
    planeNormal = planeNorm1 * w1 + planeNorm2 * w2;

    
    
    // breathing motion
//    plane += ofVec3f(0, 0, 40 * sinf(ofGetElapsedTimef()));

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
    faceSmoothFast = faceNose;
    faceSmoothMedium = faceNose;
    faceSmoothSlow = faceNose;
    
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