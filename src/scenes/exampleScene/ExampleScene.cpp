#include "ExampleScene.h"

void ExampleScene::setup(){
    
    angle.set("angle", 0, -180, 180);
    width.set("width", 100, 20, 400);
    parameters.add(angle);
    parameters.add(width);
    
}

void ExampleScene::update(){
    
    angle = angle.getMin() + ofMap(sin(ofGetElapsedTimef()*0.5), -1, 1, 0, 1) * (angle.getMax() - angle.getMin());
    
//    width = width.getMin() + ofMap(sin(ofGetElapsedTimef()*0.7), -1, 1, 0, 1) * (width.getMax() - width.getMin());
}

void ExampleScene::draw(){
    
    ofBackground(ofRandom(255), ofRandom(255), ofRandom(255));

    ofSetRectMode(OF_RECTMODE_CENTER);
    ofPushMatrix();
    
    ofTranslate(dimensions.width/2, dimensions.height/2);
    ofRotate(angle);
    ofDrawRectangle(0, 0, width, 40);
    
    ofPopMatrix();
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    ofDrawLine(0, 0, dimensions.width, dimensions.height);
    ofDrawLine(0,  dimensions.height, dimensions.width, 0);
}
