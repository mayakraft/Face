#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "clmFaceTracker.h"
#include "SceneManager.h"

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

    ofParameterGroup group;
    ofxPanel panel;
    
    ofVec3f worldToScreen(ofVec3f WorldXYZ, ofMatrix4x4 additionalTransform);

    // SCENES
    SceneManager sceneManager;
    
    // MASKS
    clmFaceTracker CLMFT;
    float maskEnergy;
    bool faceFound; // empirical based on feeling from mask energy
    ofRectangle faceRect;
    ofMatrix4x4 faceScaleMatrix;

    ofPoint faceCenterSmooth;  // the smoothed position of the face as it tracks
    float faceScaleSmooth;   //  same, but for zooming
    
    ofPoint faceMouth;
    ofPoint faceRightEye;
    ofPoint faceLeftEye;
    ofPoint faceNose;
};
