#pragma once
#include "ofMain.h"

//----------------------------------------------------------------
class clmFaceTracker {
    
public:
    
    void setup();
    void update();
    void drawFacePoints();
    void drawCameraFeed();
    
    // video capture
    ofVideoGrabber grabber;
    int camWidth, camHeight;
    
    
    // face data
    float faceEnergy;
    bool faceFound;
    
    ofRectangle faceRect;
    ofPoint faceMouth;
    ofPoint faceRightEye;
    ofPoint faceLeftEye;
    ofPoint faceNose;
    
    
    // smoothed face data
    ofPoint faceCenterSmooth;  // the smoothed position of the face as it tracks
    float faceScaleSmooth;   //  same, but for zooming

private:
    
    ofFbo fbo;

    vector < ofPoint > pts;
    double detection_certainty;
    bool bDetectionSuccess;
    
    ofPoint center;
    
    ofPixels videoRotatedPixels;

};
//----------------------------------------------------------------
