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
    
    faceScaleSmooth = 1.0;
    faceCenterSmooth = ofPoint(ofGetWidth() * .5, ofGetHeight()*.5);
    
    windowCenter = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);
}

//--------------------------------------------------------------
void ofApp::update(){
    // SCENES
    sceneManager.update();
   
    // MASKS
    ofPoint avg;
    if (CLMFT.pts.size() > 0){
        faceMouth.set(0,0);
        faceRightEye.set(0,0);
        faceLeftEye.set(0,0);
        faceNose.set(0, 0);
        
        for (int i = 0; i < CLMFT.pts.size(); i++){
            if (i >= 36 && i <= 41){
                faceLeftEye += CLMFT.pts[i];
            }
            else if (i >= 42 && i <= 47){
                faceRightEye += CLMFT.pts[i];
            }
            else if (i >= 48 && i <= 60){
                faceMouth += CLMFT.pts[i];
            }
            else if (i >= 29 && i <= 35){
                faceNose += CLMFT.pts[i];
            }
            avg += CLMFT.pts[i];
            //ofDrawBitmapString(ofToString(i), CLMFT.pts[i].x, CLMFT.pts[i].y);
        }
        faceLeftEye /= 6.0;
        faceRightEye /= 6.0;
        faceMouth /= 13.0;
        faceNose /= 7.0;
        
        avg /= (float) CLMFT.pts.size();
        //ofCircle(CLMFT.pts[ mouseX % 68 ], 2);
//        ofPoint diff = CLMFT.pts[0] - avg;
    }
    
    CLMFT.update();
    
    if (CLMFT.detection_certainty < 0.2){
        maskEnergy = 0.9f * maskEnergy + 0.1 * 1.0;
    } else {
        maskEnergy = 0.9f * maskEnergy + 0.1 * 0.0;
    }
    
    if(maskEnergy > 0.1)
        faceFound = true;
    else
        faceFound = false;
    
    
    
    // get face bouding box
    // to be replaced by smallest enclosing circle
    ofPolyline poly;
    for(int i = 0; i < CLMFT.pts.size(); i++)
        poly.addVertex(CLMFT.pts[i]);
    faceRect = poly.getBoundingBox();
    
    
    // build face rotation/scale matrix to put the face in the center of the screen
    
    // smooth face zooming scale
    float targetScale = 1.0;
    if(faceFound && faceRect.getHeight() > 0)
        targetScale = ofGetHeight()*.7 / faceRect.getHeight();
    faceScaleSmooth = faceScaleSmooth * .95 + targetScale * 0.05;
    
    // smooth face x y tracking
    ofPoint faceNoseCenter = ofPoint(0, 0);
    if(faceFound)
        faceNoseCenter = -(faceNose - windowCenter);
    faceCenterSmooth = faceCenterSmooth * .95 + faceNoseCenter * .05;

    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate(-windowCenter);
    faceScaleMatrix.scale(faceScaleSmooth, faceScaleSmooth, faceScaleSmooth);
    faceScaleMatrix.translate(windowCenter);
    faceScaleMatrix.translate( faceCenterSmooth );
    

    // deliver info to the scene manager
    sceneManager.faceFound = faceFound;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
    sceneManager.faceCenterSmooth = faceCenterSmooth;
    sceneManager.faceScaleSmooth = faceScaleSmooth;
    // face parts
    sceneManager.faceNose = faceNose;// - center;
    sceneManager.faceMouth = faceMouth;// - center;
    sceneManager.faceLeftEye = faceLeftEye;// - center;
    sceneManager.faceRightEye = faceRightEye;// - center;
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofPushMatrix();
        ofMultMatrix(faceScaleMatrix);
    
        // MASKS
        ofBackground(0);
        ofSetColor( 255 - maskEnergy * 100);//200);
//        ofSetColor( 255, 255 );
        CLMFT.grabber.draw(0,0);
        ofSetColor(255);
        CLMFT.draw();
    
    
//        ofSetColor(0, 255, 0, 100);
//        ofDrawSphere(sceneManager.faceNose, 10);
//        ofDrawSphere(sceneManager.faceMouth, 10);
//        ofDrawSphere(sceneManager.faceLeftEye, 10);
//        ofDrawSphere(sceneManager.faceRightEye, 10);


    ofPopMatrix();
    
    // SCENES
    sceneManager.draw();
    
    
    // DEBUG TEXT
    ofSetColor(255,255,255,255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
    ofDrawBitmapString(ofToString(faceFound), 20, 40);
    ofDrawBitmapString(ofToString(maskEnergy), 20, 60);
//    ofDrawBitmapString(ofToString(CLMFT.faceEnergy), 20, 60);
    ofDrawBitmapString(ofToString(faceScaleSmooth), 20, 80);
    ofDrawBitmapString(ofToString(faceRect.getCenter().x), 20, 100);
    ofDrawBitmapString(ofToString(ofGetWidth()*.5), 20, 120);
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
