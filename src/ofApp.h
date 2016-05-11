#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "clmFaceTracker.h"

//#include "springyShape.h"


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
    
    // SCENES
    
    SceneManager sceneManager;

    
    // MASKS
    
    clmFaceTracker CLMFT;

    ofParameterGroup group;
    ofxPanel panel;
    
    //vector < springyShape > shapes;

    float maskEnergy;
    
    //vector <particle *> myParticles;
    
    float findFaceZoom;  // 0 to 1, 0:not zoomed, 1:fully zoomed
    ofRectangle faceRect;
    ofMatrix4x4 faceScaleMatrix;

    
    ofVec3f worldToScreen(ofVec3f WorldXYZ, ofMatrix4x4 additionalTransform);

    ofPoint faceCenterNow;
    
    ofPoint faceMouth;
    ofPoint faceRightEye;
    ofPoint faceLeftEye;
    ofPoint faceNose;

};
