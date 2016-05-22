#pragma once

#include "ofMain.h"
#include "BaseScene.h"
#include "Conics.h"

#define NUM_CONES 48
#define NUM_PLANES 1

class ConicsScene : public BaseScene{

public:
    void setup();
    void update();
    void draw();
    
    void reset();
		
    Conics conics[NUM_CONES];
    
    ofVec3f plane, planeNormal;
    
    ofVec3f movingPlane, movingPlaneNormal;
    
    ofPoint faceSmoothFast, faceSmoothMedium, faceSmoothSlow;
    float speed;
    float smoothSpeed;
    float faceEnergy;
    float faceEnergyHigh;
        
    int numCones;
    float numConesFloat;
    
    float smoothFaceMotionNoise;
    float smoothFaceVelocity;
    ofPoint lastFacePosition;
    
    float MAX_NOISE_VALUE = 200;
    
    ofVec3f conePos1[NUM_CONES];
    ofVec3f coneLook1[NUM_CONES];
    ofVec3f planePos1;
    ofVec3f planeNorm1;
    
    ofVec3f conePos2[NUM_CONES];
    ofVec3f coneLook2[NUM_CONES];
    ofVec3f planePos2;
    ofVec3f planeNorm2;

    ofVec3f conePos3[NUM_CONES];
    ofVec3f coneLook3[NUM_CONES];
    ofVec3f planePos3;
    ofVec3f planeNorm3;

};
