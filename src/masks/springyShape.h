
#pragma once

#include "ofMain.h"
#include "particle.h"
#include "spring.h"


class springyShape {


public:
    
    
    void setup();
    void update();
    void draw();
    
    void dragTo(ofPoint pt);
    
    
    // let's make a vector of them
    vector <particle> particles;
    vector <spring> springs;
    
    float targetAngle;
};