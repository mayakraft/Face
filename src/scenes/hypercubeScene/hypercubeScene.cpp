#include "hypercubeScene.h"

//--------------------------------------------------------------
void HypercubeScene::setup(){
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
    }
    
    for(int i = 0 ;i < NUM_POLY; i++){
        polyMatrix[i] = ofMatrix4x4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
        polyMatrix[i].scale(100, 100, 100);
        // add some rotations and scales between polychrons and increasingly scale them up
        polyMatrix[i].translate(i*0.1, i*0, 0);
        polyMatrix[i].rotate(i*45, 1, 0, 0);
        polyMatrix[i].rotate((i%2)*90, 0, 1, 0);
        polyMatrix[i].rotate( (floor(i/2)*45), 0, 0, 1);
        polyMatrix[i].scale(1+i*0.5, 1+i*0.5, 1+i*0.5);
    }
    
    numPoly = NUM_POLY;
}

void HypercubeScene::reset(){
    resetMoment = ofGetElapsedTimef();
    numPoly = 0;
}


//--------------------------------------------------------------
void HypercubeScene::update(){
    
    hotSpots[0] = faceLeftEye;
    hotSpots[1] = faceRightEye;
    hotSpots[2] = faceMouth;
    
    
    
    if (ofGetFrameNum() % 60 == 0){
        myShader.load("gradient");
    }
    
    float SCALE = .002;
    for(int i = 0; i < NUM_POLY; i++){
        polychron[i].rotate4DOnly(SCALE * sinf(ofGetElapsedTimef() * rotations[i].x) + (lastFaceNose.x - faceNose.x)*.004,
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
    ofClear(0);

    ofSetColor(255);
    ofDrawBitmapString(ofToString(ofGetFrameRate()),20,20);
    
    
    ofMultMatrix(faceScaleMatrix);

    float camRadius = 100;
    float camSpeed = .1;
    cam.lookAt(ofPoint(0,0,0));
    cam.setPosition(camRadius * sin(ofGetElapsedTimef()*camSpeed), camRadius * cosf(ofGetElapsedTimef()*camSpeed), 100);
    
    
    ofSetColor(255, 50);
    ofNoFill();
    for(int i = 0 ;i < 3; i++){
        ofDrawCircle(hotSpots[i].x, hotSpots[i].y, hotSpotRadius);
    }
    ofDrawLine(ofGetWidth()*.5, 0, ofGetWidth()*.5, ofGetHeight());
    ofFill();
    ofSetColor(255, 180);
    ofDrawCircle(hotSpots[0], 30);
    ofDrawCircle(hotSpots[1], 30);
    ofDrawCircle(hotSpots[2], 30);
    
    ofSetColor(255);
    
    
    cam.begin();

    ofPushMatrix();

    for(int i = 0; i < numPoly; i++){
        ofPushMatrix();
        
        myShader.begin();
        myShader.setUniform2f("ptToFadeFrom1", ofVec2f(hotSpots[0].x, ofGetHeight() - hotSpots[0].y));
        myShader.setUniform2f("ptToFadeFrom2", ofVec2f(hotSpots[1].x, ofGetHeight() - hotSpots[1].y));
        myShader.setUniform2f("ptToFadeFrom3", ofVec2f(hotSpots[2].x, ofGetHeight() - hotSpots[2].y));
        ofMultMatrix(polyMatrix[i]);

        ofSetColor(255, 50);
        polychron[i].drawWireframe();
        myShader.end();
        
        for(int e = 0; e < polychron[i].edges.size() * .5; e++){
            ofMesh myMesh;
            myMesh.setMode(OF_PRIMITIVE_LINES);
            myMesh.addVertex( polychron[i].vertices[ polychron[i].edges[e*2+0] ].threeD() );
            myMesh.addColor(ofColor( 255* polychron[i].vertexEnergy[ polychron[i].edges[e*2+0] ] ));
            myMesh.addVertex( polychron[i].vertices[ polychron[i].edges[e*2+1] ].threeD() );
            myMesh.addColor(ofColor( 255* polychron[i].vertexEnergy[ polychron[i].edges[e*2+1] ] ));
            ofEnableBlendMode(OF_BLENDMODE_ADD);
            myMesh.draw();
        }
        ofPopMatrix();
    }

    ofPopMatrix();
    cam.end();
}