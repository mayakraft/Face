#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "clmFaceTracker.h"
#include "SceneManager.h"

#include "attractScreen.h"

#include "appConstants.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofPoint windowCenter;

    
    // press spacebar to bring up the GUI
    bool showGUI;
    ofxPanel gui;
    ofxFloatSlider screenBrightness;
    ofxFloatSlider faceDarkeningScale;
    ofxToggle flipScreenOrientation;
//    ofxToggle cameraRotationToggle;
//    void cameraRotationToggleListener(bool & cameraRotationToggle);
    // attract screen
    ofxFloatSlider attractScreenWaitTime;
    ofxFloatSlider attractScreenScale;
    void attractScreenScaleListener(float & attractScreenScale);
    ofxToggle showFace;
    ofxToggle enableMasterScale;
    ofxFloatSlider masterScale;
    ofxFloatSlider lineThicknessSlider;
    void lineThicknessSliderListener(float & lineThicknessSlider);
    ofxFloatSlider faceFoundZoomScale;
    void faceFoundZoomScaleListener(float & faceFoundZoomScale);
    ofxFloatSlider sceneDurationSlider;
    void sceneDurationSliderListener(float & sceneDurationSlider);
    
    
    // scales and transformations
    float minCameraFitScale;
    ofPoint cameraToWindowScale;
    float minWindowFitScale;
    ofPoint windowToScreenScale;
    
    // SCENES
    SceneManager sceneManager;
    
    // MASKS
    clmFaceTracker CLMFT;
    ofMatrix4x4 faceScaleMatrix;
    ofImage edgeImage; // faded border around camera
    
    AttractScreen attractScreen;
    unsigned long lastFaceDetection;
    unsigned long attractBeginTime;
    float attractScreenBrightness;
};
