
#pragma once

#include "ofMain.h"
#include "BaseScene.h"

class ExampleScene : public BaseScene {
    
public:
    
    void setup();
    void update();
    void draw();
    
    ofParameter<float> angle;
    ofParameter<float> width;

};