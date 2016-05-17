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
    
    
    gui.setup();
    
    gui.add(sceneDurationSlider.setup("scene duration (sec)", sceneManager.SCENE_INTERVAL, 5, 30));
    gui.add(screenBrightness.setup("camera brightness", .5, 0, 1));
    gui.add(faceDarkeningScale.setup("face causes dimming", 0, 0, 1));
    gui.add(faceFoundZoomScale.setup("zoom in on face", .3, .02, .6));
    
    gui.add(cameraRotationToggle.setup("flip camera", false));
    gui.add(showFace.setup("show face", false));
    gui.add(enableMasterScale.setup("scale window", false));
    gui.add(masterScale.setup("  - scale", 1, .1, 2));
    
    sceneDurationSlider.addListener(this, &ofApp::sceneDurationSliderListener);
    faceFoundZoomScale.addListener(this, &ofApp::faceFoundZoomScaleListener);
    cameraRotationToggle.addListener(this, &ofApp::cameraRotationToggleListener);

    gui.setPosition(windowCenter);

    
    edgeImage.load("faded-edge.png");
    
    // resolution fitting math
    float sw = (RESOLUTION_SCREEN_WIDTH/(float)RESOLUTION_CAMERA_WIDTH);
    float sh = (RESOLUTION_SCREEN_HEIGHT/(float)RESOLUTION_CAMERA_HEIGHT);
    if(sw > sh)   minCameraFitScale = sw;
    else          minCameraFitScale = sh;
    
    facePointsFrameScale = ofPoint(RESOLUTION_CAMERA_HEIGHT * minCameraFitScale,
                                   RESOLUTION_CAMERA_WIDTH * minCameraFitScale);    
}

//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();
    CLMFT.update();

    // MASKS

    // build face rotation/scale matrix to put the face in the center of the screen
    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate( CLMFT.faceCenterSmooth.y * facePointsFrameScale.y,
                              -CLMFT.faceCenterSmooth.x * facePointsFrameScale.x,
                              0.0);
//    faceScaleMatrix.translate(-windowCenter);
    faceScaleMatrix.scale(CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth, CLMFT.faceScaleSmooth);
//    faceScaleMatrix.translate(windowCenter);
    

    // deliver info to the scene manager
    sceneManager.faceFound = CLMFT.faceFound;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
    sceneManager.faceCenterSmooth = ofPoint(-CLMFT.faceCenterSmooth.y * facePointsFrameScale.y,
                                            CLMFT.faceCenterSmooth.x * facePointsFrameScale.x);
    sceneManager.faceScaleSmooth = CLMFT.faceScaleSmooth;
    // face parts
    sceneManager.faceNose = ofPoint(-CLMFT.faceNose.y * facePointsFrameScale.y,
                                    CLMFT.faceNose.x * facePointsFrameScale.x);// - center;
    sceneManager.faceMouth = ofPoint(-CLMFT.faceMouth.y * facePointsFrameScale.y,
                                     CLMFT.faceMouth.x * facePointsFrameScale.x);// - center;
    sceneManager.faceLeftEye = ofPoint(-CLMFT.faceLeftEye.y * facePointsFrameScale.y,
                                       CLMFT.faceLeftEye.x * facePointsFrameScale.x);// - center;
    sceneManager.faceRightEye = ofPoint(-CLMFT.faceRightEye.y * facePointsFrameScale.y,
                                        CLMFT.faceRightEye.x * facePointsFrameScale.x);// - center;
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    ofPoint center = ofPoint(ofGetScreenWidth() * .5, ofGetScreenHeight() * .5);

    ofPushMatrix();
        ofTranslate(center);
        // scale as needed
        if(enableMasterScale)
            ofScale(masterScale, masterScale);

        ofMultMatrix(faceScaleMatrix);

        ofPushMatrix();
            // scale camera to fit inside of screen
            ofScale(minCameraFitScale, minCameraFitScale);
//            ofSetColor( 255 - CLMFT.faceEnergy * 200, 255);
            ofSetColor(255 * screenBrightness - 255 * screenBrightness * faceDarkeningScale * CLMFT.faceEnergy, 255);
            CLMFT.drawCameraFeed();
            ofSetColor(255, 255);
            edgeImage.draw(-RESOLUTION_CAMERA_WIDTH * .5,
                           -RESOLUTION_CAMERA_HEIGHT * .5,
                           RESOLUTION_CAMERA_WIDTH,
                           RESOLUTION_CAMERA_HEIGHT);
            if(showFace)
                CLMFT.drawFacePoints();
        ofPopMatrix();
    
//        ofPushMatrix();
//            ofRotate(-90 * CLMFT.cameraRotation);
//            ofSetColor(0, 128, 255);
//            ofDrawCircle(CLMFT.faceLeftEye * facePointsFrameScale, 10);
//            ofDrawCircle(CLMFT.faceRightEye * facePointsFrameScale, 10);
//            ofDrawCircle(CLMFT.faceNose * facePointsFrameScale, 10);
//            ofDrawCircle(CLMFT.faceMouth * facePointsFrameScale, 10);
//        ofPopMatrix();
    
    ofPopMatrix();
    
    ofPushMatrix();
        ofTranslate(center);
        // scale as needed
        if(enableMasterScale)
            ofScale(masterScale, masterScale);

        ofNoFill();
        ofSetLineWidth(3);
        ofSetColor(0, 255);
        ofDrawRectangle(-ofGetScreenWidth() * .5, -ofGetScreenHeight() * .5, ofGetScreenWidth(), ofGetScreenHeight());
        ofSetColor(255);
    ofPopMatrix();

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
    
    // SCENES
    ofPushMatrix();
        ofTranslate(center);
        if(enableMasterScale)
            ofScale(masterScale, masterScale);
        sceneManager.draw();
    ofPopMatrix();

    if(showGUI){
        gui.draw();
    }

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

void ofApp::sceneDurationSliderListener(float &sceneDurationSlider){
    sceneManager.SCENE_INTERVAL = sceneDurationSlider;
}
void ofApp::cameraRotationToggleListener(bool &cameraRotationToggle){
    if(cameraRotationToggle == 1)
        CLMFT.cameraRotation = 1;
    else if(cameraRotationToggle == 0)
        CLMFT.cameraRotation = -1;
}
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
