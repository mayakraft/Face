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
    
    // (1) or (-1) which means 90 or -90 degrees
    // 0 means no rotation, horizontally wide aspect
    int cameraRotation = -1;
    
    // smoothed face data
    ofPoint faceCenterSmooth;  // the smoothed position of the face as it tracks
    float faceScaleSmooth;   //  same, but for zooming
    
    // customizable
    float faceFoundZoomScale = 0.3;

private:
    
    ofFbo fbo;

    vector < ofPoint > pts;
    double detection_certainty;
    bool bDetectionSuccess;
    
    ofPoint center;
    
    ofPixels videoRotatedPixels;

};
//----------------------------------------------------------------
