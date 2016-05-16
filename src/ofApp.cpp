#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // SCENES
    sceneManager.setup();
    
    // MASKS
    CLMFT.setup();
//    ofSetWindowShape(CLMFT.grabber.getWidth(), CLMFT.grabber.getHeight());
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
        
    windowCenter = ofPoint(ofGetScreenWidth()*.5, ofGetScreenHeight()*.5);
    
    ofSetFullscreen(true);
    
    gui.setup(); // most of the time you don't need a name
    gui.add(showFace.setup("show face", true));
    gui.add(enableMasterScale.setup("scale window", false));
    gui.add(masterScale.setup("  - scale", 1, .1, 2));
    gui.setPosition(windowCenter);

    edgeImage.load("faded-edge.png");
    
    // resolution fitting math
    float sw = (RESOLUTION_SCREEN_WIDTH/(float)RESOLUTION_CAMERA_WIDTH);
    float sh = (RESOLUTION_SCREEN_HEIGHT/(float)RESOLUTION_CAMERA_HEIGHT);
    if(sw > sh)   minCameraFitScale = sw;
    else          minCameraFitScale = sh;
}

//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();
    CLMFT.update();

    // MASKS

    // build face rotation/scale matrix to put the face in the center of the screen
    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate(-windowCenter);
    faceScaleMatrix.scale(CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth);
    faceScaleMatrix.translate(windowCenter);
    faceScaleMatrix.translate( CLMFT.faceCenterSmooth );
    

    // deliver info to the scene manager
    sceneManager.faceFound = CLMFT.faceFound;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
    sceneManager.faceCenterSmooth = CLMFT.faceCenterSmooth;
    sceneManager.faceScaleSmooth = CLMFT.faceScaleSmooth;
    // face parts
    sceneManager.faceNose = CLMFT.faceNose;// - center;
    sceneManager.faceMouth = CLMFT.faceMouth;// - center;
    sceneManager.faceLeftEye = CLMFT.faceLeftEye;// - center;
    sceneManager.faceRightEye = CLMFT.faceRightEye;// - center;
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofPushMatrix();
    // this sequence happens in reverse, bottom -> up
    ofPoint center = ofPoint(ofGetScreenWidth() * .5, ofGetScreenHeight() * .5);
    ofTranslate(center);
    // scale as needed
    if(enableMasterScale)
        ofScale(masterScale, masterScale);
    // rotate to display sideways
//    ofRotate(90 * SCREEN_ROTATION);
    // move center of fbo to 0, 0
//    ofTranslate(-ofGetScreenWidth()*.5, -ofGetScreenHeight()*.5);
//    ofTranslate(-CLMFT.grabber.getWidth()*.5, -CLMFT.grabber.getHeight()*.5);
    // flip across the x=y diagonal line
//    ofScale(-1, -1, 1);
    // draw it
    ofPushMatrix();
    // scale camera to fit inside of screen
    ofScale(minCameraFitScale, minCameraFitScale);
    CLMFT.drawCameraFeed();
    edgeImage.draw(-RESOLUTION_CAMERA_WIDTH * .5, -RESOLUTION_CAMERA_HEIGHT * .5, RESOLUTION_CAMERA_WIDTH, RESOLUTION_CAMERA_HEIGHT);
    if(showFace)
        CLMFT.drawFacePoints();
    ofPopMatrix();

    
    ofNoFill();
    ofSetLineWidth(3);
    ofSetColor(0, 255);
    ofDrawRectangle(-ofGetScreenWidth() * .5, -ofGetScreenHeight() * .5, ofGetScreenWidth(), ofGetScreenHeight());
    ofSetColor(255);
    
    ofPopMatrix();
    
    if(showGUI){
        gui.draw();
    }

    
//    ofPushMatrix();
////        ofMultMatrix(faceScaleMatrix);
//        // MASKS
//        ofBackground(0);
//        ofSetColor( 255 - CLMFT.faceEnergy * 200);
//        ofSetColor( 255, 255 );
//        CLMFT.grabber.draw(0,0);
//        ofSetColor(255);
//        CLMFT.draw();
//    ofPopMatrix();

    
//    ofPushMatrix();
//    // SCENES
//    ofScale(4.2, 4.2, 4.2);
//    ofTranslate(900, 0);
//    ofRotate(90);
//    ofTranslate(0, 900-800);
//    sceneManager.draw();
//    ofPopMatrix();
    
    
    // DEBUG TEXT
//    ofSetColor(255,255,255,255);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
//    ofDrawBitmapString(ofToString(faceFound), 20, 40);
//    ofDrawBitmapString(ofToString(maskEnergy), 20, 60);
////    ofDrawBitmapString(ofToString(CLMFT.faceEnergy), 20, 60);
//    ofDrawBitmapString(ofToString(faceScaleSmooth), 20, 80);
//    ofDrawBitmapString(ofToString(faceRect.getCenter().x), 20, 100);
//    ofDrawBitmapString(ofToString(ofGetWidth()*.5), 20, 120);
}


ofVec3f ofApp::worldToScreen(ofVec3f WorldXYZ, ofMatrix4x4 additionalTransform) {
    ofRectangle viewport = ofGetCurrentRenderer()->getCurrentViewport();
    //    OF_MATRIX_MODELVIEW, OF_MATRIX_PROJECTION,
    ofMatrix4x4 world = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW);
    ofVec3f CameraXYZ = WorldXYZ * additionalTransform * world;
    ofVec3f ScreenXYZ;
    ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
    ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
    ScreenXYZ.z = CameraXYZ.z;
    return ScreenXYZ;
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
