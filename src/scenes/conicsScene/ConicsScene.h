#pragma once

#include "ofMain.h"
#include "BaseScene.h"
#include "Conics.h"
#include "ofxGui.h"

#define NUM_CONES 64
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
    
    int numCones;
    float numConesFloat;
    
    ofPoint center;
};
