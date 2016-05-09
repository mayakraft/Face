#pragma once



#include "ofMain.h"





//----------------------------------------------------------------
class clmFaceTracker {
    
    
public:
    
    void setup();
    void update();
    void draw();
    
    
    //ofVideoPlayer player;
    
    ofVideoGrabber grabber;
    
    
    //ofNode myNode;
    
    
    vector < ofPoint > pts;
    double detection_certainty;
    bool bDetectionSuccess;
    float faceEnergy;
    
    ofImage face;
    
    ofFbo fbo;
    
    
};
//----------------------------------------------------------------