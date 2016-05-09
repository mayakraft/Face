#pragma once

#include "ofMain.h"

#define RESOLUTION 400

// set flag to 1 for only 1 side of cone
#define SINGLE_SIDED 1

class Conics : public ofBaseApp{
public:
    
    Conics();
    
    // setters
    void setRadius(float r); // larger than 0
    void setHeight(float h); // larger than 0
    void setPosition(ofVec3f pos);
    void setLookAt(ofVec3f look);

    // draw
    void draw();
    void drawIntersectionsWithPlane(ofVec3f planePt, ofVec3f planeNormal);
    void fillIntersectionsWithPlane(ofVec3f planePt, ofVec3f planeNormal);

    
private:
    // the parent node - use to move around
    ofNode cone; // lies at the apex
    
    // the structure of the cone
    const ofVec3f apex = ofVec3f(0,0,0);  // the tip of the cone
    float radius; // radius of the base plane
    float height; // distance to the base plane, always along the -Z
    ofVec3f focus; //the direction the (ofNode)cone faces
    
    ofVec3f basePoints[RESOLUTION];
    
    ofVec3f crossProduct(ofVec3f A1, ofVec3f A2);
    float dotProduct(ofVec3f A1, ofVec3f A2);

    bool linePlaneIntersect(ofVec3f linePt1, ofVec3f linePt2, ofVec3f planePt, ofVec3f planeN, ofVec3f *intersection, float *u);
};