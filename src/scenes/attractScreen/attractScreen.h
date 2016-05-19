#pragma once

#include "ofMain.h"
#include "BaseScene.h"
#include "Polychron.h"

static const unsigned int AT_COLS = 3;
static const unsigned int AT_ROWS = 5;

class AttractScreen : public BaseScene{

public:
    void setup();
    void update();
    void draw();
    
    float scaleAmnt = 60;

private:
    
    ofEasyCam cam;
    
    
    Polychron polychron[AT_COLS*AT_ROWS];
    ofVec3f rotations[AT_COLS*AT_ROWS];

    bool grid = true;
    float gridGap = 3.5;
    bool indexed = true;
    float radiusScale = 0;
    ofVec3f angleOffset = ofVec3f(0, 0, 0);
    
    bool fourDAnimated = true;
    ofVec3f fourD = ofVec3f(30, 25, 80);
    ofVec3f compression = ofVec3f(0, 0, 0);
    
    bool rotAnimations = true;
    float rotAnimSpeed = 10;
    float rotAnimMag = 1;
    bool posAnimations = true;
    float posAnimSpeed = 1;
    float posAnimMag = .2;

};
