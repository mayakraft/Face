#include "ConicsScene.h"

//--------------------------------------------------------------

void ConicsScene::setup(){
    for(int i = 0; i < NUM_CONES; i++){
        conics[i].setPosition( ofPoint(0, 100, -10 ) );
        conics[i].setLookAt( ofPoint(0, 0, 0) );

        conics[i].setRadius(20 + i*5);
    }
    numCones = 1;
}

//--------------------------------------------------------------
void ConicsScene::update(){
    float SPEED = .2;
    float diff = .3 * sinf(ofGetElapsedTimef()*.2);
    for(int i = 0 ;i < numCones; i++){

        conics[i].setPosition( ofPoint(faceCenter.x-300 + i * 10,
                                       0,
                                       200 + 190 * sin(ofGetElapsedTimef() + i/100.0)));
        conics[i].setLookAt( ofPoint(-(faceCenter.x-300) * .2,
                                     0,
                                     0) );
//        conics[i].setPosition( ofPoint(i * 10,0, 200 + 190 * sin(ofGetElapsedTimef() + i/100.0)));
        //conics[i].setHeight(300 + 250 * sinf(2*i+ofGetElapsedTimef()));
//        conics[i].setRadius(ofGetMouseX() + (ofGetMouseX()*0.9) * ofSignedNoise(ofGetElapsedTimef()*.3, i/10.0));
        
//        conics[i].setLookAt( ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0), 0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0)));
//        conics[i].setLookAt( ofPoint(200 + 100 * sin(ofGetElapsedTimef() + i/100.0), 0 + 50 * ofNoise(ofGetElapsedTimef()*0.1, i/200.0)));
    }
    if(numCones < NUM_CONES)
        numCones++;
}

void ConicsScene::reset(){
    resetMoment = ofGetElapsedTimef();
    numCones = 1;
}

//--------------------------------------------------------------
void ConicsScene::draw(){
    
    ofClear(0);
    
    ofSetLineWidth(1);
    
    cam.begin();
    ofScale(2, 2, 2);
    
//    ofDrawSphere(ofVec3f(0, 0, 0), 10);
    
    //    ofDrawAxis(20);
    
    //    ofSetColor(255, 10);
    //    for(int i = 0 ;i < NUM_CONES; i++)
    //        conics[i].draw();
    
    plane = ofVec3f(0, 0, 0);
    planeNormal = ofVec3f(0, 0 + 2, 1);
    for(int i = 0 ;i < numCones; i++)
        conics[i].drawIntersectionsWithPlane(plane, planeNormal);
    
    //    ofSetColor(255, 0, 0, 255);
    //    for(int i = 0 ;i < NUM_CONES; i++)
    //        conics[i].drawIntersectionsWithPlane(movingPlane, movingPlaneNormal);
    
//    for(int i = 0 ;i < numCones; i++){
//        if(coneAnimate){
//            conics[i].setLookAt( ofVec3f(cosf(ofGetElapsedTimef()*coneLookPeriod + i*coneLookPhase),
//                                         sinf(ofGetElapsedTimef()*coneLookPeriod + i*coneLookPhase),
//                                         1) );
//            conics[i].setPosition( ofVec3f(coneMagnitude * cosf(ofGetElapsedTimef()*conePeriod + i*conePhase),
//                                           coneMagnitude * sinf(ofGetElapsedTimef()*conePeriod + i*conePhase),
//                                           coneZMagnitude * sinf(ofGetElapsedTimef()*coneZPeriod + i*coneZPhase)) );
//        }
//        plane = ofVec3f(planeXY*cosf(ofGetElapsedTimef()*planeXYPeriod + i*planeXYPhase),
//                        planeXY*sinf(ofGetElapsedTimef()*planeXYPeriod + i*planeXYPhase),
//                        planeZ + (planeZ*.9) * sinf(ofGetElapsedTimef()*planeZPeriod + i*planeZPhase) );
//        
//        planeNormal = ofVec3f(planeXYTilt * cosf(ofGetElapsedTimef()*planeXYTiltPeriod+i*planeXYTiltPhase),
//                              planeXYTilt * sinf(ofGetElapsedTimef()*planeXYTiltPeriod+i*planeXYTiltPhase),
//                              planeZTilt+ (planeZTilt*.9) * sinf(ofGetElapsedTimef()*planeZTiltPeriod + i*planeZTiltPhase) );
//        
//        conics[i].drawIntersectionsWithPlane(plane, planeNormal);
//    }
    
    cam.end();
}