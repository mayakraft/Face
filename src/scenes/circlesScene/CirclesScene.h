#pragma once

#include "ofMain.h"
#include "BaseScene.h"

class CircleFromThreePoints {
public:
    
    /// dummy
    CircleFromThreePoints(){}
    
    //typedef Basic2DVector<float>   Vector2D;
    
    /** Construct from three points (see class description).
     *  The order of points is not essential, but accuracy should be better if
     *  the second point lies between the other two on the circle.
     *  The optional argument "precision" specifies how accurately the
     *  straight line check has to be satisfied for setting the curvature
     *  to zero and the center position to "undefined".
     */
    CircleFromThreePoints( const ofPoint& inner,
                          const ofPoint& mid,
                          const ofPoint& outer,
                          double precision = 1.e-7);
    
    
    /** Returns the curvature (1/radius), in cm^(-1).
     *  The curvature is precomputed, this is just access method (takes no time).
     *  If curvature is zero the center is undefined
     *  (see description of presicion above).
     */
    float curvature() const { return theCurvature;}
    
    /** returns the position of the center of the circle.
     *  If curvature is zero, center() throws an exception to avoid use of
     *  undefined position.
     *  If the curvature is very small the position of the center will
     *  be very inaccurate.
     */
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
        ofCircle(pos,2);
        ofNoFill();
        ofCircle(pos, radius);
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
};
