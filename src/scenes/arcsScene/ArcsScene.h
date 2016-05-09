#pragma once

#include "ofMain.h"
#include "BaseScene.h"
#include "particle.h"
#include "spring.h"

class arcCircle {
    public :
    ofPoint pos;
    float radius;
};


class particleCircle : public particle {

public:
    
    float radius;
    void drawMe(){
        ofDrawCircle(pos, radius);
    }
};

class ArcsScene : public BaseScene{

	public:
		void setup();
		void update();
		void draw();
    
    void reset();
    
        vector < particleCircle > circles;
        vector < spring > springs;
    
    int CLOCKWISE = 1;
    int COUNTERCLOCKWISE = -1;
    
    int ClockWise(vector < ofPoint > pts);
    void traceAngleToAngle(arcCircle & circle, ofPoint from, ofPoint to, ofPolyline & line, bool bClockwise, string name);

    
};
