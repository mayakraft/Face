#include "attractScreen.h"

//--------------------------------------------------------------
void AttractScreen::setup(){
    
    for(int i = 0; i < 25; i++){
        polychron[i].loadVefFile("8cell.ascii.txt");

        rotations[i].x = ofRandom(.01, 0.3);
        rotations[i].y = ofRandom(.01, 0.3);
        rotations[i].z = ofRandom(.01, 0.3);
    }
}

//--------------------------------------------------------------
void AttractScreen::update(){
    
    if(fourDAnimated){
        float SCALE = 0.0001;
        for(int i = 0; i < 25; i++){
//            if(indexed)
//                polychron[i].rotate4DOnly(SCALE * (i+1) * fourD.x, SCALE * (i+1) * fourD.y, SCALE * (i+1) * fourD.z);
//            else
                polychron[i].rotate4DOnly(SCALE * fourD.x, SCALE * fourD.y, SCALE * fourD.z);
        }
    }
//    else{
//        float SCALE = 0.01;
//        for(int i = 0; i < 25; i++){
//            if(indexed)
//                polychron[i].rotate4DOnlyOnce(SCALE * (i+1) * fourD.x, SCALE * (i+1) * fourD.y, SCALE * (i+1) * fourD.z);
//            else
//                polychron[i].rotate4DOnlyOnce(SCALE * fourD.x, SCALE * fourD.y, SCALE * fourD.z);
//        }
//    }
}

//--------------------------------------------------------------
void AttractScreen::draw(){
    ofPushMatrix();
    
    ofTranslate(ofGetScreenWidth() * .5, ofGetScreenHeight() * .5);
    float scaleAmnt = ofGetScreenHeight() * .04;
    ofScale(scaleAmnt, scaleAmnt, scaleAmnt);
    
    for(int i = 0; i < 25; i++){
        ofPushMatrix();
        if(grid){
            ofTranslate( gridGap * (i%5 - 2), gridGap * ((int)(i/5.0) - 2), 0);
        }
        if(indexed){
            ofScale(1 + radiusScale * i, 1 + radiusScale * i, 1 + radiusScale * i);
            ofScale(1 + compression.x * (i+1), 1 + compression.y * (i+1), 1 + compression.z * (i+1));
            ofRotate(i * angleOffset.x, 1, 0, 0);
            ofRotate(i * angleOffset.y, 0, 1, 0);
            ofRotate(i * angleOffset.z, 0, 0, 1);
        }
        else{
            ofScale(1 + radiusScale, 1 + radiusScale, 1 + radiusScale);
            ofScale(1 + compression.x, 1 + compression.y, 1 + compression.z);
            ofRotate(angleOffset.x, 1, 0, 0);
            ofRotate(angleOffset.y, 0, 1, 0);
            ofRotate(angleOffset.z, 0, 0, 1);
        }
        if(rotAnimations){
            ofRotate(rotAnimMag * sinf(ofGetElapsedTimef()*rotations[i].x * rotAnimSpeed), 1, 0, 0);
            ofRotate(rotAnimMag * sinf(ofGetElapsedTimef()*rotations[i].y * rotAnimSpeed), 0, 1, 0);
            ofRotate(rotAnimMag * sinf(ofGetElapsedTimef()*rotations[i].z * rotAnimSpeed), 0, 0, 1);
        }
        if(posAnimations){
            ofTranslate(posAnimMag * sinf(ofGetElapsedTimef()*rotations[i].x * posAnimSpeed),
                        posAnimMag * sinf(ofGetElapsedTimef()*rotations[i].y * posAnimSpeed),
                        posAnimMag * sinf(ofGetElapsedTimef()*rotations[i].z * posAnimSpeed) );
        }
        polychron[i].drawWireframe();
        
        ofPopMatrix();
    }
    
    ofPopMatrix();
    
}
