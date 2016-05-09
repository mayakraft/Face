#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // SCENES
    sceneManager.setup();
    
    // MASKS
    CLMFT.setup();
    ofSetWindowShape(CLMFT.grabber.getWidth(), CLMFT.grabber.getHeight());
    ofSetFrameRate(60);
    ofSetVerticalSync(false);
    maskEnergy = 0;
}


ofPoint lEye;
ofPoint rEye;
ofPoint mouth;


//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();
   
    // MASKS
    
//    ofPoint avg;
//    if (CLMFT.pts.size() > 0){
//        
//        
//        rEye.set(0,0);
//        lEye.set(0,0);
//        mouth.set(0,0);
//        
//        for (int i = 0; i < CLMFT.pts.size(); i++){
//            if (i >= 36 && i <= 41){
//                lEye += CLMFT.pts[i];
//            }
//            
//            if (i >= 42 && i <= 47){
//                rEye += CLMFT.pts[i];
//            }
//            
//            if (i >= 48 && i <= 60){
//                mouth += CLMFT.pts[i];
//            }
//            
//            
//            avg += CLMFT.pts[i];
//            //ofDrawBitmapString(ofToString(i), CLMFT.pts[i].x, CLMFT.pts[i].y);
//        }
//        
//        rEye /= 6.0;
//        lEye /= 6.0;
//        
//        mouth /= 13.0;
//        
//        avg /= (float) CLMFT.pts.size();
//        //ofCircle(CLMFT.pts[ mouseX % 68 ], 2);
//        ofPoint diff = CLMFT.pts[0] - avg;

    
    CLMFT.update();
    
    if (CLMFT.detection_certainty < 0.2){
        maskEnergy = 0.9f * maskEnergy + 0.1 * 1.0;
    } else {
        maskEnergy = 0.9f * maskEnergy + 0.1 * 0.0;
    }
    
    
//    for (int i = 0; i < shapes.size(); i++){
//        shapes[i].update();
//    }
    
    if(findFaceZoom < 1.0 && CLMFT.faceEnergy > 0.1){
        findFaceZoomVelocity = (1.0-findFaceZoom) * .05;
        findFaceZoom += findFaceZoomVelocity;
        if(findFaceZoom > 1.0)
            findFaceZoom = 1.0;
    }
    if(findFaceZoom > 0.0 && CLMFT.faceEnergy < 0.1){
        findFaceZoomVelocity = (findFaceZoom) * .02;
        findFaceZoom -= findFaceZoomVelocity;
        if(findFaceZoom < 0.0)
            findFaceZoom = 0.0;
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPolyline poly;
    for(int i = 0; i < CLMFT.pts.size(); i++)
        poly.addVertex(CLMFT.pts[i]);
    ofRectangle rect = poly.getBoundingBox();

    
    ofPushMatrix();
    
    if(CLMFT.faceEnergy > 0.1){

        ofTranslate(findFaceZoom * (ofGetWidth()*.5),
                    findFaceZoom * (ofGetHeight()*.5) );

        float scaleAmnt = 1.0;
        if(rect.getHeight() != 0)
            scaleAmnt = ofGetHeight()*.7 / rect.getHeight();
        scaleAmnt = (scaleAmnt)*(findFaceZoom) + 1.0*(1-findFaceZoom);
        ofScale(scaleAmnt, scaleAmnt, scaleAmnt);
        
        ofTranslate(-(findFaceZoom * rect.getCenter()) );
    }
    
    sceneManager.faceCenter = rect.getCenter();
    
    // MASKS
//    ofBackground(0);
    ofSetColor( 255 - maskEnergy * 200);
    CLMFT.grabber.draw(0,0);
    ofSetColor(255);
//    CLMFT.draw();
    ofSetColor(255);
    ofSetColor(255,255,255,255 * maskEnergy);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    
//    ofNoFill();
//    if(rect.isEmpty()){
//        ofSetColor(255, 0, 0);
//    }
//    else
//        ofSetColor(0, 255, 0);
//    ofDrawRectangle(rect);
//    ofFill();
    
    ofPopMatrix();
    ofSetColor(0, 100 * findFaceZoom);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    // SCENES
    sceneManager.masterFade = findFaceZoom;
    sceneManager.draw();
    
    ofSetColor(255,255,255,255);
    ofDrawBitmapString(ofToString(findFaceZoom), 20, 40);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
