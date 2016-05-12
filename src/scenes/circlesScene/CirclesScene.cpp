#include "CirclesScene.h"
//void CirclesScene::setup(){
//}
//void CirclesScene::update(){
//}
//void CirclesScene::draw(){
//}
//void CirclesScene::reset(){
//}

CircleFromThreePoints::CircleFromThreePoints( const ofPoint& inner,
                                             const ofPoint& mid,
                                             const ofPoint& outer,
                                             double precision)
{
    // move to frame where inner.x == inner.y == 0;
    ofPoint b( mid.x-inner.x, mid.y-inner.y);
    ofPoint c( outer.x-inner.x, outer.y-inner.y);
    init( b, c, ofPoint( inner.x, inner.y), precision);
}

void CircleFromThreePoints::init( const ofPoint& b, const ofPoint& c,
                                 const ofPoint& offset, double precision)
{
    double b2 = b.lengthSquared();
    double c2 = c.lengthSquared();
    
    double oX(0), oY(0);
    bool solved = false;
    if (fabs(b.x) > fabs(b.y)) {    // solve for y first
        double k = c.x/b.x;
        double div = 2*(k*b.y - c.y);
        if (fabs(div) < precision) theCurvature = 0;  // if the 3 points lie on a line
        else {
            oY = (k*b2 - c2) / div;
            oX = b2/(2*b.x) - b.y/b.x * oY;
            solved = true;
        }
    }
    else {    // solve for x first
        double k = c.y/b.y;
        double div = 2*(k*b.x-c.x);
        if (fabs(div) < precision) theCurvature = 0;  // if the 3 points lie on a line
        else {
            oX = (k*b2 - c2) / div;
            oY = b2/(2*b.y) - b.x/b.y * oX;
            solved = true;
        }
    }
    if (solved) {
        theCurvature = 1./sqrt(oX*oX + oY*oY);
        double xC = oX + offset.x;
        double yC = oY + offset.y;
        theCenter = ofPoint( xC, yC);
        //    thePhi = acos(xC/sqrt(xC*xC + yC*yC));
        
        //    if (xC<0.) thePhi = thePhi - PI;
        //    cout << setiosflags(ios::showpoint | ios::fixed);
        //
        //    cout << "CircleFromThreePoints::init curv = " << theCurvature << endl;
        //    cout << "CircleFromThreePoints::init center prime = " << oX << " " << oY << endl;
        //    cout << "CircleFromThreePoints::init offset = " << offset.x << " " << offset.y << endl;
        //    cout << "CircleFromThreePoints::init center = " << theCenter.x<< " " << theCenter.y << endl;
        //
        //    float d = sqrt(theCenter.x*theCenter.x+theCenter.y*theCenter.y);
        //    cout << "CircleFromThreePoints::init dfloat = " << setw(10) << setprecision(5) << d << endl;
        //    cout << "CircleFromThreePoints::init radius = " << 1/theCurvature << endl;
    }
}





//--------------------------------------------------------------
void CirclesScene::setup(){

    ofSetCircleResolution(100);
    
    for (int i = 0; i < 3; i++){
        circle temp;
        temp.radius = ofRandom(30,200);
        float angle = ofMap(i,0,10, 0, TWO_PI);
        temp.pos = ofPoint(500,500) + 200 * ofPoint(cos(angle), sin(angle));
       // temp.pos = ofPoint( );
        temp.vel = ofPoint(0,0);
        circles.push_back(temp);
    }
}


void CirclesScene::reset(){
    resetMoment = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void CirclesScene::update(){

    for (int i = 0; i < circles.size(); i++){
        circles[i].updateStart();
    }
    for (int i = 0; i < circles.size(); i++){
        // move to center
        ofPoint diff = ofPoint(500,500) - circles[i].pos;
        circles[i].addForce(diff/300.0);
        
        
        ofPoint diff2 = ofPoint(500,500) - circles[i].pos;
        if (diff2.length() < ofGetMouseX()){
            circles[i].addForce(-diff/600.0);
        }
        
    }

    
    circles[0].radius = 200 + 100 * sin(ofGetElapsedTimef()*0.8+1.0);
    
    circles[1].radius = 100 + 90 * sin(ofGetElapsedTimef());
    circles[2].radius = 100 + 80 * sin(ofGetElapsedTimef()*1.2);
    
    
    for (int i = 0; i < circles.size(); i++){
        circles[i].updateEnd();
    }
    
    
    vector < int > randomOrder;
    for (int i = 0; i < circles.size(); i++){
        randomOrder.push_back(i);
    }
    ofRandomize(randomOrder);
    
    for (int i = 0; i < circles.size(); i++){
        // repel from each other
        for (int j = 0; j < i; j++){
            circles[randomOrder[i]].repelFrom(circles[randomOrder[j]]);
        }
    }

}


int CirclesScene::ClockWise(vector < ofPoint > pts){
    int i,j,k;
    int count = 0;
    double z;
    int n = pts.size();
    
    if (n < 3)
        return(0);
    
    for (i=0;i<n;i++) {
        j = (i + 1) % n;
        k = (i + 2) % n;
        z  = (pts[j].x - pts[i].x) * (pts[k].y - pts[j].y);
        z -= (pts[j].y - pts[i].y) * (pts[k].x - pts[j].x);
        if (z < 0)
            count--;
        else if (z > 0)
            count++;
    }
    if (count > 0)
        return(COUNTERCLOCKWISE);
    else if (count < 0)
        return(CLOCKWISE);
    else
        return(0);
}



void CirclesScene::traceAngleToAngle(circle & circle, ofPoint from, ofPoint to, ofPolyline & line, bool bClockwise, string name){
    
    
    vector < ofPoint > pts;
    pts.push_back(from);
    pts.push_back(circle.pos);
    pts.push_back(to);
    
    int clockwise = ClockWise(pts);
    
    ///ofDrawBitmapStringHighlight(name +  " " + (clockwise == CLOCKWISE ? "clockwise" : "counter") + " want " + (bClockwise ? "clockwise" : "counter"), circle.pos);
    
    
    ofPoint diffA = (from - circle.pos);
    ofPoint diffB = (to - circle.pos);
    
    float angleA = atan2( diffA.y, diffA.x );
    float  angleB = atan2( diffB.y, diffB.x );

    float angleDiff = angleB - angleA;
    if (angleDiff < -PI) angleDiff += TWO_PI;
    if (angleDiff > PI) angleDiff -= TWO_PI;
    
    if ( (clockwise == CLOCKWISE && !bClockwise) ||
        (clockwise == COUNTERCLOCKWISE && bClockwise)){
        
        
        float diff = TWO_PI - fabs(angleDiff);
        if (angleDiff > 0){
            diff *= -1;
        }
        angleDiff = diff;
    }
//    if (angleDiff < -PI) angleDiff += TWO_PI;
//    if (angleDiff > PI) angleDiff -= TWO_PI;
//    
// 
//    if (det > 0 && !bClockwise){
//        
//        if (angleDiff > 0){
//            angleDiff = -TWO_PI + angleDiff;
//        } else {
//             //angleDiff = -TWO_PI + angleDiff;
//        }
//        
//        //angleDiff *= -1;
//    }
//    
//    
//    ofDrawBitmapStringHighlight(name +  " " + (bClockwise ? "clockwise" : "counter"), circle.pos);
//    
    for (int i = 0; i < 100; i++){
        float angle = angleA + (angleDiff/100.0) * i;
        line.addVertex(  circle.pos + circle.radius * ofPoint(cos(angle), sin(angle)));
    }
}

//--------------------------------------------------------------
void CirclesScene::draw(){

//    ofBackground(0);
    ofClear(0);
 
    ofSetColor(255,255,255,90);
    for (int i = 0; i < circles.size(); i++){
        circles[i].draw();
    }
    
    for (int i = 0; i < circles.size(); i++){
        int i_p_1 = (i+1) % circles.size();
        ofDrawLine(circles[i].pos, circles[i_p_1].pos);
    }
//
//    ofSetColor(255,0,0);
//    ofLine(circles[0].pos, circles[1].pos);
//    
//    ofSetColor(255,255,0);
//    ofLine(circles[0].pos, circles[2].pos);
//    
//    
    ofPoint diffA = (circles[1].pos - circles[0].pos);
    ofPoint diffB = (circles[2].pos - circles[0].pos);
    
    float angleA = atan2( diffA.y, diffA.x );
    float angleB = atan2( diffB.y, diffB.x );
    
    float angleDiff = angleB - angleA;
    if (angleDiff < -PI) angleDiff += TWO_PI;
    if (angleDiff > PI) angleDiff -= TWO_PI;
    
    float newAngle = angleA - angleDiff;

    
    ofPolyline tempLine;
    
    
    for (int i = 0; i < circles.size(); i++){
        
        int i_m_1 = i-1;
        int i_p_1 = i+1;
        if (i_m_1 < 0) i_m_1 += circles.size();
        i_p_1 %= circles.size();
        
        traceAngleToAngle(circles[i], circles[i_m_1].pos, circles[i_p_1].pos, tempLine, i %2 == 1 ? ofGetMousePressed() : !ofGetMousePressed() , "0");
    }
    
    ofSetColor(255,255,255,90);
    
    tempLine = tempLine.getSmoothed(3);
    
    
    //tempLine.getVertices().erase(tempLine.getVertices().begin(), tempLine.getVertices().begin()+40);
    //tempLine.getVertices().erase(tempLine.getVertices().end()-40, tempLine.getVertices().end());
    
    ofSetColor(255,255,255);
    tempLine.draw();
//    
//    ofPoint ptEnd = tempLine.getVertices()[tempLine.getVertices().size()-1];
//    
//    ofCircle(ptEnd,  3);
//    
//    
//    ofPoint ptBegin = tempLine.getVertices()[0];
//    
//    ofCircle(ptBegin,  3);
//    
//    
//    ofPoint linePt = circles[0].pos + (circles[2].pos - circles[0].pos)*(0.5+0.5 *sin(ofGetElapsedTimef()));
//    ofCircle(linePt,  3);
//    
//    
//    ofPoint midPt = (circles[1].pos + circles[0].pos) / 2.0;
//    ofCircle(midPt,  3);
//    
//    
//    smoothed = 0.7f * smoothed + 0.3 * linePt + (linePt-midPt)*1;
//    ofLine(linePt, smoothed);
//    
//    
//    CircleFromThreePoints CFT(ptEnd,smoothed, ptBegin);
//    
//    ofPoint center = CFT.center();
//    float radius = 1.0 / CFT.curvature();
//    
//    
//    ofSetColor(255,255,255,90);
//    ofCircle(center, radius);
//    
//    
//    circle c;
//    c.pos = center;
//    c.radius =1.0 / CFT.curvature();
//    traceAngleToAngle(c, ptEnd, ptBegin, tempLine,  false, "0");
//    tempLine.setClosed(true);
//    
//    ofFill();
//    ofSetColor(255, 255, 255, 120);
//    
//    
//    
//    //tempLine.draw();
//    
//    ofBeginShape();
//    for (int i = 0; i < tempLine.size(); i++){
//        ofVertex(tempLine[i].x, tempLine[i].y);
//    }
//    
//    ofEndShape();
    
   // tempLine.getVertices().erase(tempLine.getVertices().begin() + 200, tempLine.getVertices().end());
    
    //tempLine.setClosed(true);
    //traceAngleToAngle(circles[1], circles[0].pos, circles[2].pos, tempLine, !ofGetMousePressed());
    //traceAngleToAngle(circles[2], circles[1].pos, circles[0].pos, tempLine, ofGetMousePressed());
    
//    traceAngleToAngle(circles[1], circles[0].pos, circles[2].pos, tempLine, true);
//    
//    traceAngleToAngle(circles[2], circles[1].pos, circles[0].pos, tempLine, false);
//    
    
    //newAngle = angleA - angleDiff;
    
    
//    for (int i = 0; i < 100; i++){
//        float angle = angleA + (angleDiff/100.0) * i;
//        tempLine.addVertex(   circles[1].pos + circles[1].radius * ofPoint(cos(angle), sin(angle)));
//    }
//    
//    
 
}

//--------------------------------------------------------------
void CirclesScene::keyPressed(int key){

    
    circles.clear();
    
    for (int i = 0; i < 4; i++){
        circle temp;
        temp.radius = ofRandom(30,200);
        float angle = ofMap(i,0,10, 0, TWO_PI);
        temp.pos = ofPoint(500,500) + 200 * ofPoint(cos(angle), sin(angle));
        // temp.pos = ofPoint( );
        temp.vel = ofPoint(0,0);
        circles.push_back(temp);
    }

}


