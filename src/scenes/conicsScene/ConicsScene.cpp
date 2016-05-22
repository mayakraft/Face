#include "ConicsScene.h"

//--------------------------------------------------------------

void ConicsScene::setup(){
    for(int i = 0; i < NUM_CONES; i++){
        conics[i].setRadius(5 + i*5);
    }
    numCones = 1;

    plane = ofVec3f(0, 0, 100 * sinf(ofGetElapsedTimef()));
    planeNormal = ofVec3f(-100, 0, 100);
    faceSmoothFast = ofVec3f(0, 0, 0);
    faceSmoothMedium = ofVec3f(0, 0, 0);
    faceSmoothSlow = ofVec3f(0, 0, 0);
    smoothSpeed = 0;
}

//--------------------------------------------------------------
void ConicsScene::update(){
    if(numCones < NUM_CONES){
        numConesFloat *= 1.1;
        numCones = numConesFloat;
        if(numCones > NUM_CONES)
            numCones = NUM_CONES;
    }
    
    // FACE TRACK
    faceSmoothSlow = 0.9 * faceSmoothSlow + 0.1 * faceNose;
    faceSmoothMedium = 0.6 * faceSmoothMedium + 0.4 * faceNose;
    faceSmoothFast = 0.3 * faceSmoothFast + 0.7 * faceNose;
    speed = ofDist(faceSmoothFast.x, faceSmoothFast.y, faceSmoothSlow.x, faceSmoothSlow.y);
    smoothSpeed = .9 * smoothSpeed + .1 * speed;
    
    float speedToAdd = speed;
    if(speedToAdd > 30)
        speedToAdd = 30;
    faceEnergy+=speedToAdd*.2;
    faceEnergy *= .9;
    
    if(speedToAdd > 15){
        faceEnergyHigh += speedToAdd*.2;
    }
    faceEnergyHigh *= .9;
    
    // PROGRAMS
    planePos1 = faceSmoothSlow * faceScaleMatrix;
    planeNorm1 = ofVec3f(-100, 0, 100);
    
    planePos2 = faceSmoothSlow * faceScaleMatrix;
    planeNorm2 = ofVec3f(100, 0, 100);

    planePos3 = faceSmoothSlow * faceScaleMatrix;
    planeNorm3 = ofVec3f(-100, 0, 100);

    for(int i = 0; i < NUM_CONES; i++){
        float pctCones = i / ((float)NUM_CONES-1);

        // PROGRAM 1
        ofVec3f interp = faceSmoothSlow * (pctCones) + faceSmoothMedium * (1-pctCones);
        conePos1[i] = faceSmoothSlow * faceScaleMatrix + ofPoint(0, 0, 150);
        coneLook1[i] = interp * faceScaleMatrix;

        // PROGRAM 2
        conePos2[i] = ofPoint(-200 + i * 10,
                              0,
                              -200 + 200 + 190 * sin(ofGetElapsedTimef() + i/100.0));
        coneLook2[i] = ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0),
                                               0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0),
                                               0 + 300 * cos(ofGetElapsedTimef() + i/10.0));

        // PROGRAM 3
//        ofVec3f interp = faceSmoothSlow * (pctCones) + faceSmoothMedium * (1-pctCones);
        ofVec3f spin = ofVec3f(100 * sinf(i * .1 + .05*smoothSpeed), 100 * sinf(i * .1 + .05*smoothSpeed), 0.0);
        conePos3[i] = faceSmoothSlow * faceScaleMatrix + ofPoint(0, 0, 150) + spin;
        coneLook3[i] = interp * faceScaleMatrix;

    }

//    prgmWeight = (sinf(ofGetElapsedTimef() * .5) + 1) * .5;
    float prgmWeight = ofMap(faceEnergy, 0, 100, 0, 1);
    float prgmWeight2 = ofMap(faceEnergy, 0, 30, 0, 1);
    
//    float w1 = 1.0 - prgmWeight;
//    float w2 = prgmWeight2;
//    float w3 = prgmWeight;
    
//    float w1 = 0.0;
//    float w2 = 1.0;
//    float w3 = 0.0;
    
    float w1 = 0.0;
    float w2 = prgmWeight;
    float w3 = 1.0 - prgmWeight;
    
    // SET PROGRAM
    for(int i = 0; i < numCones; i++){
        conics[i].setPosition( conePos1[i] * w1 + conePos2[i] * w2 + conePos3[i] * w3 );
        conics[i].setLookAt( coneLook1[i] * w1 + coneLook2[i] * w2 + coneLook3[i] * w3 );
    }
    plane = planePos1 * w1 + planePos2 * w2 + planePos3 * w3;
    planeNormal = planeNorm1 * w1 + planeNorm2 * w2 + planeNorm3 * w3;

    
    // breathing motion
    plane += ofVec3f(0, 0, 40 * sinf(ofGetElapsedTimef()));

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
        ofSetColor(255, 128+128.0*i/(float)NUM_CONES);
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