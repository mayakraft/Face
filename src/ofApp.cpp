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
            if (i >= 42 && i <= 47){
                faceRightEye += CLMFT.pts[i];
            }
            if (i >= 48 && i <= 60){
                faceMouth += CLMFT.pts[i];
            }
            if (i >= 29 && i <= 35){
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
        ofPoint diff = CLMFT.pts[0] - avg;
    }
    
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
        findFaceZoom += (1.0-findFaceZoom) * .05;
        if(findFaceZoom > 1.0)
            findFaceZoom = 1.0;
    }
    if(findFaceZoom > 0.0 && CLMFT.faceEnergy < 0.1){
        findFaceZoom -= (findFaceZoom) * .02*.3333;
        if(findFaceZoom < 0.0)
            findFaceZoom = 0.0;
    }
    
    // get face bouding box
    // to be replaced by smallest enclosing circle
    ofPolyline poly;
    for(int i = 0; i < CLMFT.pts.size(); i++)
        poly.addVertex(CLMFT.pts[i]);
    faceRect = poly.getBoundingBox();
    
    
    ofPoint center = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);

    // build face rotation/scale matrix to put the face in the center of the screen
    float scaleAmnt = 1.0;
    if(faceRect.getHeight() != 0)
        scaleAmnt = ofGetHeight()*.7 / faceRect.getHeight();
    scaleAmnt = (scaleAmnt)*(findFaceZoom) + 1.0*(1-findFaceZoom);
    
    ofPoint faceNoseCenter;
    if(maskEnergy > .1)
        faceNoseCenter = -(faceNose - center);
    else
        faceNoseCenter = ofPoint(0, 0);


    faceCenterNow = faceCenterNow * .97 + faceNoseCenter * .03;

    faceScaleMatrix.makeIdentityMatrix();
    faceScaleMatrix.translate(-center);
    faceScaleMatrix.scale(scaleAmnt, scaleAmnt, scaleAmnt);
    faceScaleMatrix.translate(center);
    faceScaleMatrix.translate( faceCenterNow );
    

    // deliver info to the scene manager
//    sceneManager.faceCenter = faceRect.getCenter();
    sceneManager.masterFade = findFaceZoom;
    sceneManager.faceScaleMatrix = faceScaleMatrix;
 
    sceneManager.faceNose = faceNose;// - center;
    sceneManager.faceMouth = faceMouth;// - center;
    sceneManager.faceLeftEye = faceLeftEye;// - center;
    sceneManager.faceRightEye = faceRightEye;// - center;
}


//--------------------------------------------------------------
void ofApp::draw(){
    
//    ofPoint center = ofPoint(ofGetWidth()*.5, ofGetHeight()*.5);
//    if(maskEnergy > 0.5){
//        ofTranslate(center);
//        ofScale(1.0 + ofGetMouseX() / (float)ofGetWidth(),
//                1.0 + ofGetMouseX() / (float)ofGetWidth() );
//        ofTranslate(-center);
//    }

    ofPushMatrix();
//    cam.begin();
//    ofTranslate(-center);
        ofMultMatrix(faceScaleMatrix);
    
    
        // MASKS
        ofBackground(0);
        ofSetColor( 255 - maskEnergy * 100);//200);
//        ofSetColor( 255, 255 );
        CLMFT.grabber.draw(0,0);
        ofSetColor(255);
//        CLMFT.draw();
    
    
//        ofSetColor(0, 255, 0, 100);
//        ofDrawSphere(sceneManager.faceNose, 10);
//        ofDrawSphere(sceneManager.faceMouth, 10);
//        ofDrawSphere(sceneManager.faceLeftEye, 10);
//        ofDrawSphere(sceneManager.faceRightEye, 10);


    
        // SCENES
        sceneManager.draw();

    ofPopMatrix();

    
    // DEBUG TEXT
    ofSetColor(255,255,255,255);
//    ofDrawBitmapString(ofToString(pt.x), 20, 40);
//    ofDrawBitmapString(ofToString(pt.y), 20, 60);
//    ofDrawBitmapString(ofToString(ofGetFrameRate()), 20, 20);
//    ofDrawBitmapString(ofToString(maskEnergy), 20, 40);
//    ofDrawBitmapString(ofToString(CLMFT.faceEnergy), 20, 60);
    ofDrawBitmapString(ofToString(findFaceZoom), 20, 80);
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
