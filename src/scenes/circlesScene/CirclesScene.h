#pragma once

#include "ofMain.h"
#include "BaseScene.h"

class CircleFromThreePoints {
public:
    
    /// dummy
    CircleFromThreePoints(){}
    
    //typedef Basic2DVector<float>   Vector2D;

    CircleFromThreePoints( const ofPoint& inner,
                          const ofPoint& mid,
                          const ofPoint& outer,
                          double precision = 1.e-7);

    float curvature() const { return theCurvature;}

    ofPoint center() const { return theCenter; }
    
private:
    
    float    theCurvature;
    ofPoint theCenter;
    
    void init( const ofPoint& b, const ofPoint& c,
              const ofPoint& offset, double precision);
};


class circle {
    public :

    ofPoint pos;
    ofPoint vel;
    float radius;
    ofPoint frc;
    
    void draw(){
        ofFill();
        ofDrawCircle(pos,2);
        ofNoFill();
        ofDrawCircle(pos, radius);
    }
    
    void updateStart(){
        frc = ofPoint(0,0);
    }
    
    void repelFrom(circle & c){
        float totalradii = radius + c.radius;
       
        ofPoint diff = (pos - c.pos);
        float dist = diff.length();
        if (dist < totalradii){
            pos += diff.getNormalized() * (totalradii-dist) * 0.5;
            c.pos -= diff.getNormalized() * (totalradii-dist) * 0.5;
        }
    }
    
    void addForce(ofPoint toadd){
        frc += toadd;
    }
    
    
    void updateEnd(){
        vel += frc;
        pos += vel;
        vel *= 0.9;
    }
    
};

class CirclesScene : public BaseScene{

public:
    void setup();
    void update();
    void draw();
    
    void reset();
    void keyPressed(int key);
    
    void traceAngleToAngle(circle & circle, ofPoint from, ofPoint to, ofPolyline & line, bool bClockwise, string name);
    int ClockWise(vector < ofPoint > pts);
    
    
    int CLOCKWISE = 1;
    int COUNTERCLOCKWISE = -1;

    ofPoint smoothed;
    vector < circle > circles;
 
    void ofCircleSlice(float x,float y, float radius, float lowAngle, float highAngle, bool closed, bool radians);

    
    float smoothFaceVelocity;
    ofPoint lastFacePosition;
    
    ofPoint faceSmoothFast, faceSmoothSlow;
    float speed;
    float faceEnergy;

};
