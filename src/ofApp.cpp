#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
        
    ofSetWindowPosition(1500, 10);
    ofSetFullscreen(true);
    
    // MASKS
    CLMFT.setup();
//    ofSetWindowShape(CLMFT.grabber.getWidth(), CLMFT.grabber.getHeight());
    ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    // SCREEN AND RESOLUTION
    windowCenter = ofPoint(ofGetScreenWidth()*.5, ofGetScreenHeight()*.5);
    float cw = (RESOLUTION_WINDOW_WIDTH/(float)RESOLUTION_CAMERA_WIDTH);
    float ch = (RESOLUTION_WINDOW_HEIGHT/(float)RESOLUTION_CAMERA_HEIGHT);
    if(cw > ch)   minCameraFitScale = cw;
    else          minCameraFitScale = ch;
    cameraToWindowScale = ofPoint(RESOLUTION_CAMERA_HEIGHT * minCameraFitScale,
                                   RESOLUTION_CAMERA_WIDTH * minCameraFitScale);
    
    float sw = (ofGetScreenWidth()/(float)RESOLUTION_WINDOW_WIDTH);
    float sh = (ofGetScreenHeight()/(float)RESOLUTION_WINDOW_HEIGHT);
    if(sw > sh)   minWindowFitScale = sw;
    else          minWindowFitScale = sh;
    windowToScreenScale = ofPoint(RESOLUTION_CAMERA_HEIGHT * minWindowFitScale,
                                  RESOLUTION_CAMERA_WIDTH * minWindowFitScale);


    
    // SCENES
    sceneManager.setup();
    attractScreen.setup();
    
    edgeImage.load("faded-edge.png");
    
    
    // GUI
    gui.setup();
    // appearance
    gui.add(screenBrightness.setup("camera brightness", .8, 0, 1));
    gui.add(faceDarkeningScale.setup("face causes dimming", .75, 0, 1));
    lineThicknessSlider.setup("line weight", 1, 0.25, 5);
    faceFoundZoomScale.setup("zoom in on face", .28, .02, .6);
    attractScreenScale.setup("scr.saver scale", 30, 5, 150);
    // animation
    attractScreenWaitTime.setup("scr. saver delay", 8, 5, 120);
    sceneDurationSlider.setup("scene duration", sceneManager.SCENE_INTERVAL, 5, 30);
    // admin and debug
    gui.add(flipScreenOrientation.setup("flip screen", false));
    showFace.setup("show face dots", false);
    enableMasterScale.setup("scale window", false);
    masterScale.setup("  - scale", 1, .1, 2);
    // handlers
    attractScreenScale.addListener(this, &ofApp::attractScreenScaleListener);
    lineThicknessSlider.addListener(this, &ofApp::lineThicknessSliderListener);
    sceneDurationSlider.addListener(this, &ofApp::sceneDurationSliderListener);
    faceFoundZoomScale.addListener(this, &ofApp::faceFoundZoomScaleListener);
//    cameraRotationToggle.addListener(this, &ofApp::cameraRotationToggleListener);
    // gui position
    gui.setPosition(windowCenter);
    
    
    // set program properties from GUI
    attractScreen.scaleAmnt = attractScreenScale;
    CLMFT.faceFoundZoomScale = faceFoundZoomScale;
    ofSetLineWidth(lineThicknessSlider);
}

//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();

    // MASKS
    CLMFT.update();

    // build face rotation/scale matrix to put the face in the center of the screen
    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate( CLMFT.faceCenterSmooth.y * cameraToWindowScale.y,
                              -CLMFT.faceCenterSmooth.x * cameraToWindowScale.x,
                              0.0);
    faceScaleMatrix.scale(CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth);
    
    // deliver info to the scene manager
    sceneManager.faceFound = CLMFT.faceFound;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
    sceneManager.faceCenterSmooth = ofPoint(-CLMFT.faceCenterSmooth.y * cameraToWindowScale.y,
                                            CLMFT.faceCenterSmooth.x * cameraToWindowScale.x);
    sceneManager.faceScaleSmooth = CLMFT.faceScaleSmooth;
    // face parts
    sceneManager.faceNose = ofPoint(-CLMFT.faceNose.y * cameraToWindowScale.y,
                                    CLMFT.faceNose.x * cameraToWindowScale.x);// - center;
    sceneManager.faceMouth = ofPoint(-CLMFT.faceMouth.y * cameraToWindowScale.y,
                                     CLMFT.faceMouth.x * cameraToWindowScale.x);// - center;
    sceneManager.faceLeftEye = ofPoint(-CLMFT.faceLeftEye.y * cameraToWindowScale.y,
                                       CLMFT.faceLeftEye.x * cameraToWindowScale.x);// - center;
    sceneManager.faceRightEye = ofPoint(-CLMFT.faceRightEye.y * cameraToWindowScale.y,
                                        CLMFT.faceRightEye.x * cameraToWindowScale.x);// - center;
    
    if(CLMFT.faceFound){
        lastFaceDetection = ofGetElapsedTimef();
    }
    if(ofGetElapsedTimef() > lastFaceDetection + attractScreenWaitTime){
        attractScreenBrightness = (ofGetElapsedTimef() - (lastFaceDetection + attractScreenWaitTime)) / 3.0;
    }
    else if(attractScreenBrightness > 0.0){
        attractScreenBrightness -= .05;
    }

    if(attractScreenBrightness < 0) attractScreenBrightness = 0;
    if(attractScreenBrightness > 1.0) attractScreenBrightness = 1.0;
    
    if (ofGetFrameNum() % 60 == 0){
        if(attractScreenBrightness == 1.0){
            sceneManager.resetSequence();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    ofPushMatrix(); // window translate to center
        ofTranslate(windowCenter);
        if(enableMasterScale){
            ofScale(masterScale, masterScale);
        }
        if(flipScreenOrientation)
            ofRotate(180);
    // scale window to fit
        ofScale(minWindowFitScale, minWindowFitScale);
    
        ofPushMatrix();  // roving face-found zoom
            ofMultMatrix(faceScaleMatrix);

            ofPushMatrix();  // cameraToWindowScale, aspect fit camera onto screen
                // scale camera to fit inside of screen
                ofScale(minCameraFitScale, minCameraFitScale);
                // (sceneManager.masterFade)  in place of  (CLMFT.faceEnergy)
                ofSetColor(255 * screenBrightness - (sceneManager.masterFade) * 255 * screenBrightness * faceDarkeningScale, 255);
                CLMFT.drawCameraFeed();
                ofSetColor(255, (sceneManager.masterFade) * 255);
                edgeImage.draw(-RESOLUTION_CAMERA_WIDTH * .5,
                               -RESOLUTION_CAMERA_HEIGHT * .5,
                               RESOLUTION_CAMERA_WIDTH,
                               RESOLUTION_CAMERA_HEIGHT);
                if(showFace){
                    ofSetColor(255, 255);
                    CLMFT.drawFacePoints();
                }
            ofPopMatrix(); // cameraToWindowScale, aspect fit camera onto screen
    
        ofPopMatrix();  // roving face-found zoom

    
        // SCENES
        ofSetColor(255, 255);
        ofPushMatrix();
            sceneManager.draw();
        ofPopMatrix();

    
        // ATTRACT SCREEN
        if(attractScreenBrightness != 0.0){
            ofSetLineWidth(lineThicknessSlider * minWindowFitScale);
            ofPushMatrix();
                ofSetColor(255, 100 * attractScreenBrightness);
                attractScreen.update();
                attractScreen.draw();
            ofPopMatrix();
            ofSetLineWidth(lineThicknessSlider);
        }
    
    
        // black border around screen
        if(enableMasterScale){
            ofNoFill();
            ofSetColor(255, 0, 100, 255);
            ofDrawRectangle(-RESOLUTION_WINDOW_WIDTH * .5, -RESOLUTION_WINDOW_HEIGHT * .5, RESOLUTION_WINDOW_WIDTH, RESOLUTION_WINDOW_HEIGHT);
            ofFill();
        }
    
    ofPopMatrix(); // window translate to center
    
    if(showGUI){
        ofPushMatrix();
        ofSetColor(255, 255);
        gui.draw();
        ofPopMatrix();
    }
    
    // DEBUG TEXT
//    ofSetColor(255,255);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
//    ofDrawBitmapString(ofToString(CLMFT.faceFound), 20, 40);
//    ofDrawBitmapString(ofToString(maskEnergy), 20, 60);
//    ofDrawBitmapString(ofToString(CLMFT.faceEnergy), 50, 40);
//    ofDrawBitmapString(ofToString(ofGetMouseX()), 20, 60);
//    ofDrawBitmapString(ofToString(attractScreenBrightness), 20, 80);
//    ofDrawBitmapString(ofToString(sceneManager.faceCenterSmooth.x), 20, 80);
//    ofDrawBitmapString(ofToString(sceneManager.faceCenterSmooth.y), 20, 100);
//    ofDrawBitmapString(ofToString(faceRect.getCenter().x), 20, 100);
//    ofDrawBitmapString(ofToString(minWindowFitScale), 20, 120);
}

void ofApp::attractScreenScaleListener(float &attractScreenScale){
    attractScreen.scaleAmnt = attractScreenScale;
}

void ofApp::lineThicknessSliderListener(float &lineThicknessSlider){
    ofSetLineWidth(lineThicknessSlider);
}

void ofApp::sceneDurationSliderListener(float &sceneDurationSlider){
    sceneManager.SCENE_INTERVAL = sceneDurationSlider;
}
//void ofApp::cameraRotationToggleListener(bool &cameraRotationToggle){
//    if(cameraRotationToggle == 1)
//        CLMFT.cameraRotation = 1;
//    else if(cameraRotationToggle == 0)
//        CLMFT.cameraRotation = -1;
//}
void ofApp::faceFoundZoomScaleListener(float &faceFoundZoomScale){
    CLMFT.faceFoundZoomScale = faceFoundZoomScale;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' '){
        showGUI = !showGUI;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
