#include "Conics.h"

Conics::Conics(){
    radius = 50;
    height = -100;
    focus = ofVec3f(0, 0, -200);;
    for(int i = 0; i < RESOLUTION; i++){
        float angle = ofMap(i, 0, RESOLUTION, 0, TWO_PI);
        basePoints[i] = ofVec3f( radius *cos(angle), radius *sin(angle), height);
    }
    cone.setPosition( apex );
    cone.lookAt( focus );
}

// setters

void Conics::setHeight(float h){
    height = -h;
    for(int i = 0; i < RESOLUTION; i++){
        basePoints[i] = ofVec3f(basePoints[i].x, basePoints[i].y, height);
    }
}

void Conics::setRadius(float r){
    radius = r;
    for(int i = 0; i < RESOLUTION; i++){
        float angle = ofMap(i, 0, RESOLUTION, 0, TWO_PI);
        basePoints[i] = ofVec3f( radius *cos(angle), radius *sin(angle), height);
    }
}

void Conics::setPosition(ofVec3f pos){
    cone.setPosition(pos);
    cone.lookAt( focus );
}

void Conics::setLookAt(ofVec3f look){
    focus = look;
    cone.lookAt( focus );
}

void Conics::drawIntersectionsWithPlane(ofVec3f planePt, ofVec3f planeNormal){
    ofVec3f intersect;
    ofVec3f ap = apex * cone.getLocalTransformMatrix();

    bool lastRound = false;
    int lastDraw = -1;
    ofPolyline polyline;
    int i;
    float thisU, lastU;
    ofPushMatrix();
    ofTranslate(apex * cone.getLocalTransformMatrix());
    for(i = 0; i < RESOLUTION; i++){
        ofVec3f bp = basePoints[i] * cone.getLocalTransformMatrix();
        bool thisRound = linePlaneIntersect(ap, bp, planePt, planeNormal, &intersect, &thisU);
        if(thisRound && lastRound){
            // continue the line segment, only if on same side of cone
            if( ( thisU >= 0 && lastU >= 0) || (thisU < 0 && lastU < 0) )
                polyline.addVertex(intersect);
            else{
                polyline.draw();
                lastDraw = i;
                polyline.clear();
                polyline.addVertex(intersect);
            }
        }
        else if(thisRound){ // new beginning to the polyline
            polyline.clear();
            polyline.addVertex(intersect);
        }
        else if(lastRound){ // an ending to the current polyline
            polyline.draw();
            lastDraw = i;
        }
        lastRound = thisRound;
        lastU = thisU;
    }

    // if first and last are both draw, connect them, close the polyline loop
    ofVec3f firstPoint = basePoints[0] * cone.getLocalTransformMatrix();
    bool firstIndex = linePlaneIntersect(ap, firstPoint, planePt, planeNormal, &intersect, &thisU);
    if(lastRound && firstIndex && ((thisU >= 0 && lastU >= 0) || (thisU < 0 && lastU < 0)) )
        polyline.addVertex(intersect);

    if(lastDraw != i-1)
        polyline.draw();
    ofPopMatrix();
}

void Conics::fillIntersectionsWithPlane(ofVec3f planePt, ofVec3f planeNormal){
    ofVec3f intersect;
    ofVec3f ap = apex * cone.getLocalTransformMatrix();
    bool lastRound = false;
    int lastDraw = -1;
    int i;
    float thisU, lastU;
    ofPushMatrix();
    ofTranslate(apex * cone.getLocalTransformMatrix());
    ofBeginShape(); // added for fills that are only on one side of the cone
    for(i = 0; i < RESOLUTION; i++){
        ofVec3f bp = basePoints[i] * cone.getLocalTransformMatrix();
        bool thisRound = linePlaneIntersect(ap, bp, planePt, planeNormal, &intersect, &thisU);
        if(thisRound && lastRound){
            // continue the line segment, only if on same side of cone
            if( ( thisU >= 0 && lastU >= 0) || (thisU < 0 && lastU < 0) )
                ofVertex(intersect);
            else{
                ofEndShape();
                lastDraw = i;
                ofBeginShape();
                ofVertex(intersect);
            }
        }
        else if(thisRound){ // new beginning to the polyline
//            ofBeginShape();
            ofVertex(intersect);
        }
        else if(lastRound){ // an ending to the current polyline
//            ofEndShape();
            lastDraw = i;
        }
        lastRound = thisRound;
        lastU = thisU;
    }
    
    // if first and last are both draw, connect them, close the polyline loop
    ofVec3f firstPoint = basePoints[0] * cone.getLocalTransformMatrix();
    bool firstIndex = linePlaneIntersect(ap, firstPoint, planePt, planeNormal, &intersect, &thisU);
    if(lastRound && firstIndex && ((thisU >= 0 && lastU >= 0) || (thisU < 0 && lastU < 0)) )
        ofVertex(intersect);
//        polyline.addVertex(intersect);
    
//    if(lastDraw != i-1)
        ofEndShape();
//    polyline.draw();
    ofPopMatrix();
}


void Conics::draw(){
    cone.transformGL();
    for(int i = 0; i < RESOLUTION; i++)
        ofDrawLine(apex, basePoints[i]);
    cone.restoreTransformGL();
}


float Conics::dotProduct(ofVec3f a, ofVec3f b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

ofVec3f Conics::crossProduct(ofVec3f a, ofVec3f b){
    float x = a[1] * b[2] - a[2] * b[1];
    float y = a[2] * b[0] - a[0] * b[2];
    float z = a[0] * b[1] - a[1] * b[0];
    return(ofVec3f(x, y, z));
}

bool Conics::linePlaneIntersect(ofVec3f linePt1, ofVec3f linePt2, ofVec3f planePt, ofVec3f planeN, ofVec3f *intersection, float *u){
    // line is defined by 2 points on a line
    // plane is defined by planeP point in the plane with normal planeN
    
    ofVec3f l = (linePt2 - linePt1);
    
    float numerator = dotProduct(planeN, (planePt - linePt1) );
    float denominator = dotProduct(planeN, l);
    if(denominator != 0){
        *u = numerator / denominator;
        if(SINGLE_SIDED && *u < 0)
            return false;
        // BONUS : if u is between 0 and 1, the intersection point is between P1 and P2
        *intersection = l * *u;
        return true;
    }
    return false;
}

