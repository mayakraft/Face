
#include "clmFaceTracker.h"

#include "ofxCv.h"
#include "CLM_core.h"

#include "appConstants.h"

#include <fstream>
#include <sstream>

#include <opencv2/videoio/videoio.hpp>  // Video write
#include <opencv2/videoio/videoio_c.h>  // Video write

using namespace std;
using namespace cv;

CLMTracker::CLMParameters * clm_parameters;
CLMTracker::CLM * clm_model;
int device = 0;
float fx = 500, fy = 500, cx = 0, cy = 0;
bool use_camera_plane_pose;
Mat_<uchar> grayscale_image;
Mat_<float> depth_image;

void DrawBox(ofVideoGrabber grabber, Vec6d pose, Scalar color, int thickness, float fx, float fy, float cx, float cy)
{
    double boxVerts[] = {-1, 1, -1,
        1, 1, -1,
        1, 1, 1,
        -1, 1, 1,
        1, -1, 1,
        1, -1, -1,
        -1, -1, -1,
        -1, -1, 1};
    
    for (int i = 0; i < 8; i++){
        boxVerts[i*3 + 2] *= 0.2;
    }
    
    
    vector<std::pair<int,int>> edges;
    edges.push_back(pair<int,int>(0,1));
    edges.push_back(pair<int,int>(1,2));
    edges.push_back(pair<int,int>(2,3));
    edges.push_back(pair<int,int>(0,3));
    edges.push_back(pair<int,int>(2,4));
    edges.push_back(pair<int,int>(1,5));
    edges.push_back(pair<int,int>(0,6));
    edges.push_back(pair<int,int>(3,7));
    edges.push_back(pair<int,int>(6,5));
    edges.push_back(pair<int,int>(5,4));
    edges.push_back(pair<int,int>(4,7));
    edges.push_back(pair<int,int>(7,6));
    
    
    
    
    
    
    // The size of the head is roughly 200mm x 200mm x 200mm
    Mat_<double> box = Mat(8, 3, CV_64F, boxVerts).clone() * 60;
    
    
    
    Matx33d rot = CLMTracker::Euler2RotationMatrix(Vec3d(pose[3], pose[4], pose[5]));
    Mat_<double> rotBox;
    
    // Rotate the box
    rotBox = Mat(rot) * box.t();
    rotBox = rotBox.t();
    
    // Move the bounding box to head position
    rotBox.col(0) = rotBox.col(0) + pose[0];
    rotBox.col(1) = rotBox.col(1) + pose[1];
    rotBox.col(2) = rotBox.col(2) + pose[2];
    
    // draw the lines
    Mat_<double> rotBoxProj;
    CLMTracker::Project(rotBoxProj, rotBox, fx, fy, cx, cy);
    
    cv::Rect image_rect(0,0,grabber.getWidth(), grabber.getHeight());
    
    for (size_t i = 0; i < edges.size(); ++i)
    {
        Mat_<double> begin;
        Mat_<double> end;
        
        rotBoxProj.row(edges[i].first).copyTo(begin);
        rotBoxProj.row(edges[i].second).copyTo(end);
        
        cv::Point p1((int)begin.at<double>(0), (int)begin.at<double>(1));
        cv::Point p2((int)end.at<double>(0), (int)end.at<double>(1));
        
        // Only draw the line if one of the points is inside the image
        if(p1.inside(image_rect) || p2.inside(image_rect))
        {
            ofDrawLine(p1.x, p1.y, p2.x, p2.y);
            //cv::line(image, p1, p2, color, thickness);
        }
        
    }
    
}



void Draw(const Mat_<double>& shape2D, Mat_<int>& visibilities)
{
    int n = shape2D.rows/2;
    
    // Drawing feature points
    if(n >= 66)
    {
        for( int i = 0; i < n; ++i)
        {
            if(visibilities.at<int>(i))
            {
                cv::Point featurePoint((int)shape2D.at<double>(i), (int)shape2D.at<double>(i +n));
                
                // A rough heuristic for drawn cv::Point size
                //                int thickness = (int)std::ceil(3.0* ((double)img.cols) / 640.0);
                //                int thickness_2 = (int)std::ceil(1.0* ((double)img.cols) / 640.0);
                //
                //                cv::circle(img, featurePoint, 1, Scalar(0,0,255), thickness);
                //                cv::circle(img, featurePoint, 1, Scalar(255,0,0), thickness_2);
                
                ofDrawCircle(featurePoint.x, featurePoint.y, 3);
                
            }
        }
    }
    else if(n == 28) // drawing eyes
    {
        for( int i = 0; i < n; ++i)
        {
            cv::Point featurePoint((int)shape2D.at<double>(i), (int)shape2D.at<double>(i +n));
            
            // A rough heuristic for drawn cv::Point size
            int thickness = 1.0;
            int thickness_2 = 1.0;
            
            int next_point = i + 1;
            if(i == 7)
                next_point = 0;
            if(i == 19)
                next_point = 8;
            if(i == 27)
                next_point = 20;
            
            cv::Point nextFeaturePoint((int)shape2D.at<double>(next_point), (int)shape2D.at<double>(next_point+n));
            if( i < 8 || i > 19)
                ;
            //cv::line(img, featurePoint, nextFeaturePoint, Scalar(255, 0, 0), thickness_2);
            else
                ;
            //cv::line(img, featurePoint, nextFeaturePoint, Scalar(0, 0, 255), thickness_2);
            
            ofDrawCircle(featurePoint.x, featurePoint.y, 3);
            
            //cv::circle(img, featurePoint, 1, Scalar(0,255,0), thickness);
            //cv::circle(img, featurePoint, 1, Scalar(0,0,255), thickness_2);
            
            
        }
    }
    else if(n == 6)
    {
        for( int i = 0; i < n; ++i)
        {
            cv::Point featurePoint((int)shape2D.at<double>(i), (int)shape2D.at<double>(i +n));
            
            // A rough heuristic for drawn cv::Point size
            int thickness = 1.0;
            int thickness_2 = 1.0;
            
            //cv::circle(img, featurePoint, 1, Scalar(0,255,0), thickness);
            //cv::circle(img, featurePoint, 1, Scalar(0,0,255), thickness_2);
            
            ofDrawCircle(featurePoint.x, featurePoint.y, 3);
            
            
            int next_point = i + 1;
            if(i == 5)
                next_point = 0;
            
            //cv::Point nextFeaturePoint((int)shape2D.at<double>(next_point), (int)shape2D.at<double>(next_point+n));
            //cv::line(img, featurePoint, nextFeaturePoint, Scalar(255, 0, 0), thickness_2);
        }
    }
}


ofQuaternion eulerToQuat(const ofVec3f & rotationEuler) {
    ofQuaternion rotation;
    float c1 = cos(rotationEuler[2] * 0.5);
    float c2 = cos(rotationEuler[1] * 0.5);
    float c3 = cos(rotationEuler[0] * 0.5);
    float s1 = sin(rotationEuler[2] * 0.5);
    float s2 = sin(rotationEuler[1] * 0.5);
    float s3 = sin(rotationEuler[0] * 0.5);
    
    rotation[0] = c1*c2*s3 - s1*s2*c3;
    rotation[1] = c1*s2*c3 + s1*c2*s3;
    rotation[2] = s1*c2*c3 - c1*s2*s3;
    rotation[3] = c1*c2*c3 + s1*s2*s3;
    
    return rotation;
}







//----------------------------------------------------------------
void clmFaceTracker::setup(){
    
    camWidth = RESOLUTION_CAMERA_WIDTH;
    camHeight = RESOLUTION_CAMERA_HEIGHT;
    
    fbo.allocate(RESOLUTION_SCREEN_WIDTH, RESOLUTION_SCREEN_HEIGHT);
    
    grabber.setup(camWidth, camHeight);
    videoRotatedPixels.allocate(camHeight, camWidth, OF_PIXELS_RGB);

    vector < string > tempStrings;
    clm_parameters = new CLMTracker::CLMParameters();
    system("pwd");
    clm_model = new CLMTracker::CLM(clm_parameters->model_location);
    //clm_parameters->validation_boundary = -0.2;
    
    cout << clm_parameters->curr_face_detector << endl;
    cout << clm_parameters->sigma << endl;
    cout << clm_parameters->reinit_video_every << endl;

    bDetectionSuccess = false;
    
    faceScaleSmooth = 1.0;
    faceCenterSmooth = ofPoint(camWidth * .5, camHeight*.5);

    faceEnergy = 0;
    faceFound = false;
    
//    center = ofPoint(camWidth * .5, camHeight * .5);
    
}

//----------------------------------------------------------------
void clmFaceTracker::update(){
    
    grabber.update();
    if (grabber.isFrameNew()){

        ofPixels pixels = grabber.getPixels();
        // rotate pixels, switch widths and heights
        pixels.rotate90To(videoRotatedPixels, SCREEN_ROTATION);
        videoRotatedPixels.resize(RESOLUTION_FACE_DETECTOR_HEIGHT, RESOLUTION_FACE_DETECTOR_WIDTH);
        //        videoRotatedPixels.resize(RESOLUTION_FACE_DETECTOR_WIDTH, RESOLUTION_FACE_DETECTOR_HEIGHT);
        // also mirror it
//        videoRotatedPixels.mirror(1, 0);
        
        cvtColor( ofxCv::toCv(videoRotatedPixels), grayscale_image, CV_BGR2GRAY);
        bool detection_success = CLMTracker::DetectLandmarksInVideo(grayscale_image, depth_image, *clm_model, *clm_parameters);
        
        if (detection_success == true){
            bDetectionSuccess = true;
            //cout << pts.size()  << endl;
        } else {
            bDetectionSuccess = false;
        }
        
        int n = clm_model->detected_landmarks.rows/2;
        
        pts.clear();
        
        for( int i = 0; i < (int)(clm_model->detected_landmarks.rows/2); ++i){
            cv::Point featurePoint((int)clm_model->detected_landmarks.at<double>(i),
                                   (int)clm_model->detected_landmarks.at<double>(i +n));
            pts.push_back( ofPoint(featurePoint.x, featurePoint.y));
        }
        
        detection_certainty = clm_model->detection_certainty;
        
//        if (detection_certainty < 0.2){
        if (detection_certainty < -0.2){
            faceEnergy = faceEnergy * 0.9 + 0.1 * 1.0;
        } else {
            faceEnergy = faceEnergy * 0.9 + 0.1 * 0.0;
        }
        //cout << clm_model->detection_certainty << " cert " << endl;
        
        if(faceEnergy > 0.1)
            faceFound = true;
        else
            faceFound = false;

        
        ofPoint avg;
        if (pts.size() > 0){
            faceMouth.set(0,0);
            faceRightEye.set(0,0);
            faceLeftEye.set(0,0);
            faceNose.set(0, 0);
            
            for (int i = 0; i < pts.size(); i++){
                if (i >= 36 && i <= 41){
                    faceLeftEye += pts[i];
                }
                else if (i >= 42 && i <= 47){
                    faceRightEye += pts[i];
                }
                else if (i >= 48 && i <= 60){
                    faceMouth += pts[i];
                }
                else if (i >= 29 && i <= 35){
                    faceNose += pts[i];
                }
                avg += pts[i];
                //ofDrawBitmapString(ofToString(i), pts[i].x, pts[i].y);
            }
            faceLeftEye /= 6.0;
            faceRightEye /= 6.0;
            faceMouth /= 13.0;
            faceNose /= 7.0;
            
            avg /= (float) pts.size();
            //ofCircle(pts[ mouseX % 68 ], 2);
            //        ofPoint diff = pts[0] - avg;
        }
        
        
        
        
        // get face bouding box
        // to be replaced by smallest enclosing circle
        ofPolyline poly;
        for(int i = 0; i < pts.size(); i++)
            poly.addVertex(pts[i]);
        faceRect = poly.getBoundingBox();
                
        // smooth face zooming scale
        float targetScale = 1.0;
        if(faceFound && faceRect.getHeight() > 0)
            targetScale = ofGetHeight()*.1 / faceRect.getHeight();
        faceScaleSmooth = faceScaleSmooth * .95 + targetScale * 0.05;
        
        // smooth face x y tracking
        ofPoint faceNoseCenter = ofPoint(0, 0);
        if(faceFound)
            faceNoseCenter = faceNose;//-(faceNose - center);
        faceCenterSmooth = faceCenterSmooth * .95 + faceNoseCenter * .05;
    }
}

void clmFaceTracker::drawCameraFeed(){
    ofPushMatrix();
    ofTranslate(-grabber.getWidth()*.5,
                -grabber.getHeight()*.5);
    grabber.draw(0, 0);
    ofPopMatrix();
}

//----------------------------------------------------------------
void clmFaceTracker::drawFacePoints(){
    ofPushMatrix();
    // scale face detection plane up to camera (face detection screen can be smaller for efficiency)
    ofScale( (RESOLUTION_CAMERA_WIDTH/(float)RESOLUTION_FACE_DETECTOR_WIDTH),
             (RESOLUTION_CAMERA_HEIGHT/(float)RESOLUTION_FACE_DETECTOR_HEIGHT));
    // reverse the rotation that happened to the openCV camera pixels
    ofRotate(-90 * SCREEN_ROTATION);
//    ofTranslate(-RESOLUTION_CAMERA_HEIGHT * .5, -RESOLUTION_CAMERA_WIDTH * .5);
    ofTranslate(-RESOLUTION_FACE_DETECTOR_HEIGHT * .5, -RESOLUTION_FACE_DETECTOR_WIDTH * .5);
    
    // draw an optional filled boundary plane
    ofFill();
    ofSetColor(0, 255, 128, 100);
    ofDrawRectangle(0, 0, RESOLUTION_FACE_DETECTOR_HEIGHT, RESOLUTION_FACE_DETECTOR_WIDTH);
    ofSetColor(255, 255);
    ofNoFill();
    
    // draw face points
    int idx = clm_model->patch_experts.GetViewIdx(clm_model->params_global, 0);
    Draw(clm_model->detected_landmarks, clm_model->patch_experts.visibilities[0][idx]);
    ofPopMatrix();
}
//void clmFaceTracker::draw(){
//
//    fbo.begin();
//    grabber.draw(0,0);
//   
//    ofCamera cam;
//    cam.setupPerspective();
//
//    int idx = clm_model->patch_experts.GetViewIdx(clm_model->params_global, 0);
//    
//     Because we only draw visible points, need to find which points patch experts consider visible at a certain orientation
//    Draw(clm_model->detected_landmarks, clm_model->patch_experts.visibilities[0][idx]);
    
//    Vec6d pose_estimate_to_draw = CLMTracker::GetCorrectedPoseCameraPlane(*clm_model, cam.getPosition().z, cam.getPosition().z, grabber.getWidth()/2, grabber.getHeight()/2);
//    
//    ofDrawBitmapStringHighlight("certainty: " + ofToString(detection_certainty), 20,40);
//    ofDrawBitmapStringHighlight("faceEnergy: " + ofToString(faceEnergy), 20,60);
//    
//    fbo.end();
//    fbo.draw(0,0);
//    
//}


