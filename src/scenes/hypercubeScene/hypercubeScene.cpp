#include "hypercubeScene.h"

//--------------------------------------------------------------
void HypercubeScene::setup(){
    
    float MASTER_SCALE = 80;
    
    for(int i = 0; i < NUM_POLY; i++){
        polychron[i].loadVefFile("8cell.ascii.txt");
        highlighted[i].clear();
    }
    hotSpots.push_back( ofPoint(ofGetWidth()*.3, ofGetHeight()*.2) );
    hotSpots.push_back( ofPoint(ofGetWidth()*.7, ofGetHeight()*.2) );
    hotSpots.push_back( ofPoint(ofGetWidth()*.5, ofGetHeight()*.75) );
    hotSpotRadius = 60;

    myShader.load("gradient");
    
    for(int i = 0; i < NUM_POLY; i++){
        rotations[i].x = ofRandom(.01, 0.3);
        rotations[i].y = ofRandom(.01, 0.3);
        rotations[i].z = ofRandom(.01, 0.3);
        
        brightnesses[i] = 0;
    }
    
    for(int i = 0; i < NUM_POLY; i++){
        polyMatrix[i] = ofMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        polyMatrix[i].scale(MASTER_SCALE, MASTER_SCALE, MASTER_SCALE);
//        polyMatrix[i].rotate(ofRandom(90), 1,0,0);
//        polyMatrix[i].rotate(ofRandom(90), 0,0,1);
        // add some rotations and scales between polychrons and increasingly scale them up
        polyMatrix[i].translate(i*0.1, i*0, 0);
        polyMatrix[i].rotate(i*45, 1, 0, 0);
        polyMatrix[i].rotate((i%2)*90, 0, 1, 0);
        polyMatrix[i].rotate( (floor(i/2)*45), 0, 0, 1);
        polyMatrix[i].scale(1+i*0.5, 1+i*0.5, 1+i*0.5);
    }
    
    numPoly = NUM_POLY;
    
    sceneWindowMatrix.makeIdentityMatrix();
    sceneWindowMatrix.scale(RESOLUTION_SCENE_WIDTH / (float)RESOLUTION_WINDOW_WIDTH,
                            RESOLUTION_SCENE_HEIGHT / (float)RESOLUTION_WINDOW_HEIGHT,
                            1.0);
}

void HypercubeScene::reset(){
    resetMoment = ofGetElapsedTimef();
    numPoly = 0;
    for(int i = 0; i < NUM_POLY; i++){
        brightnesses[i] = 0;
    }
}

//--------------------------------------------------------------
void HypercubeScene::update(){
    if (ofGetFrameNum() % 60 == 0){
        myShader.load("gradient");
    }

    
    if(numPoly < NUM_POLY){
        int newNumPoly = powf((ofGetElapsedTimef() - (float)resetMoment),2) * 2.0;
        if(newNumPoly > NUM_POLY)
            newNumPoly = NUM_POLY;
        
        if(numPoly != newNumPoly){
            for(int i = numPoly; i < newNumPoly; i++){
                brightnesses[i] = 1;
            }
            numPoly = newNumPoly;
        }
    }
    else
        numPoly = NUM_POLY;
    
    
    for(int i = 0; i < NUM_POLY; i++){
        brightnesses[i] *= 0.8;
    }

    
    float SCALE = .002;
    for(int i = 0; i < NUM_POLY; i++){
        polychron[i].rotate4DOnly(SCALE * sinf(ofGetElapsedTimef() * rotations[i].x),
                                  SCALE * sinf(ofGetElapsedTimef() * rotations[i].y),
                                  SCALE * sinf(ofGetElapsedTimef() * rotations[i].z) );
    }
    
    for(int i = 0; i < NUM_POLY; i++) {
        // CALCULATE the highlighted vertices
        highlighted[i].clear();
        for(int v = 0; v < polychron[i].getNumVertices(); v++){
            Point4D vertex = polychron[i].vertices[v];
            ofVec3f screenLocation = worldToScreen( ofVec3f(vertex.x, vertex.y, vertex.z), polyMatrix[i] );
            if(pointInHotspot(hotSpots[0], screenLocation) ||
               pointInHotspot(hotSpots[1], screenLocation) ||
               pointInHotspot(hotSpots[2], screenLocation)){
                highlighted[i].push_back(v);
                // vertex energy
                polychron[i].vertexEnergy[v] += 0.05;
                if(polychron[i].vertexEnergy[ v ] > 1)
                    polychron[i].vertexEnergy[ v ] = 1;
                // edge energy
                vector<unsigned int> adjEdg = polychron[i].allEdgesAdjacentTo(v);
                for(int q = 0; q < adjEdg.size(); q++){
                    polychron[i].edgeEnergy[ adjEdg[q] ] += .05;
                    if(polychron[i].edgeEnergy[ adjEdg[q] ] > 1)
                        polychron[i].edgeEnergy[ adjEdg[q] ] = 1;
                }
            }
        }
    }
        
    lastFaceNose = faceNose;
    
    float closeness = 100;
    float xRot = faceCenterSmooth.x * .5;
    float yRot = faceCenterSmooth.y * .5;

    headTiltMatrix.makeIdentityMatrix();
    headTiltMatrix.rotate(xRot, 0, 1, 0);
    headTiltMatrix.rotate(-yRot, 1, 0, 0);
    headTiltMatrix.translate(0, 0, closeness);

}

ofVec3f HypercubeScene::worldToScreen(ofVec3f WorldXYZ, ofMatrix4x4 additionalTransform) {
    ofRectangle viewport = ofGetCurrentRenderer()->getCurrentViewport();
    ofVec3f CameraXYZ = WorldXYZ * additionalTransform * cam.getModelViewProjectionMatrix(viewport);
    ofVec3f ScreenXYZ;
    ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x;
    ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y;
    ScreenXYZ.z = CameraXYZ.z;
    return ScreenXYZ;
}

bool HypercubeScene::pointInHotspot(ofPoint hotSpot, ofPoint point){
    return ( sqrt( powf(point.x - hotSpot.x, 2) + powf(point.y - hotSpot.y, 2)) < hotSpotRadius );
}

//--------------------------------------------------------------
void HypercubeScene::draw(){
    ofVec3f center = ofVec3f(RESOLUTION_SCENE_WIDTH * .5, RESOLUTION_SCENE_HEIGHT * .5, 0);
    hotSpots[0] = faceLeftEye  * faceScaleMatrix * sceneWindowMatrix + center;
    hotSpots[1] = faceRightEye * faceScaleMatrix * sceneWindowMatrix + center;
    hotSpots[2] = faceMouth    * faceScaleMatrix * sceneWindowMatrix + center;
   
//    ofPushMatrix();
//    ofSetColor(255, 0, 128);
//    ofDrawCircle(hotSpots[0], 10);
//    ofDrawCircle(hotSpots[1], 10);
//    ofDrawCircle(hotSpots[2], 10);
//    ofPopMatrix();

    ofTranslate(RESOLUTION_SCENE_WIDTH * .5, RESOLUTION_SCENE_HEIGHT * .5);
    ofScale(RESOLUTION_SCENE_WIDTH / (float)RESOLUTION_WINDOW_WIDTH,
            RESOLUTION_SCENE_HEIGHT / (float)RESOLUTION_WINDOW_HEIGHT);

//    ofPushMatrix();
//    ofSetColor(0, 128, 255);
//    ofDrawCircle(faceLeftEye  * faceScaleMatrix, 10);
//    ofDrawCircle(faceRightEye * faceScaleMatrix, 10);
//    ofDrawCircle(faceNose     * faceScaleMatrix, 10);
//    ofDrawCircle(faceMouth    * faceScaleMatrix, 10);
//    ofPopMatrix();

    ofPushMatrix();
    
    ofMultMatrix(headTiltMatrix);

    ofSetColor(255, 255);

    for(int i = 0; i < numPoly; i++){
        ofPushMatrix();
//        ofSetColor(255, 255 - i * 20);

        myShader.begin();
        myShader.setUniform2f("ptToFadeFrom1", ofVec2f(hotSpots[0].x, hotSpots[0].y));
        myShader.setUniform2f("ptToFadeFrom2", ofVec2f(hotSpots[1].x, hotSpots[1].y));
        myShader.setUniform2f("ptToFadeFrom3", ofVec2f(hotSpots[2].x, hotSpots[2].y));
        ofMultMatrix(polyMatrix[i]);
//
//        ofSetColor(255, brightnesses[i]);
        polychron[i].drawWireframe();
        myShader.end();

        ofSetColor(255, 125 + 125*brightnesses[i]);
        polychron[i].drawWireframe();

//        for(int e = 0; e < polychron[i].edges.size() * .5; e++){
//            ofMesh myMesh;
//            myMesh.setMode(OF_PRIMITIVE_LINES);
//            myMesh.addVertex( polychron[i].vertices[ polychron[i].edges[e*2+0] ].threeD() );
//            myMesh.addColor(ofColor(255,  255* polychron[i].vertexEnergy[ polychron[i].edges[e*2+0] ] ));
//            myMesh.addVertex( polychron[i].vertices[ polychron[i].edges[e*2+1] ].threeD() );
//            myMesh.addColor(ofColor(255,  255* polychron[i].vertexEnergy[ polychron[i].edges[e*2+1] ] ));
//            ofEnableBlendMode(OF_BLENDMODE_ADD);
//            myMesh.draw();
//        }
        ofPopMatrix();
    }
//    cam.end();
    ofPopMatrix();
}