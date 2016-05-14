#pragma once



#include "ofMain.h"





//----------------------------------------------------------------
class clmFaceTracker {
    
    
public:
    
    void setup();
    void update();
    void draw();
    
    ofVideoGrabber grabber;
    
    int camWidth, camHeight;
    
    
    //ofNode myNode;
    
    
    vector < ofPoint > pts;
    double detection_certainty;
    bool bDetectionSuccess;
    float faceEnergy;
    
    ofImage face;
    
    ofFbo fbo;
        
};
//----------------------------------------------------------------