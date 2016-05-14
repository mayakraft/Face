
#include "clmFaceTracker.h"


#include "ofxCv.h"
#include "CLM_core.h"

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



//----------------------------------------------------------------
void clmFaceTracker::setup(){
    
    camWidth = 1920;
    camHeight = 1080;
    
    fbo.allocate(camWidth, camHeight);
    
    grabber.setup(camWidth, camHeight);
    
    vector < string > tempStrings;
    clm_parameters = new CLMTracker::CLMParameters();
    system("pwd");
    clm_model = new CLMTracker::CLM(clm_parameters->model_location);
    //clm_parameters->validation_boundary = -0.2;
    
    
    cout << clm_parameters->curr_face_detector << endl;
    cout << clm_parameters->sigma << endl;
    cout << clm_parameters->reinit_video_every << endl;

    bDetectionSuccess = false;
    
    faceEnergy = 0;
    //face.loadImage("face.png");
}

//----------------------------------------------------------------
void clmFaceTracker::update(){
    
    
    grabber.update();
    if (grabber.isFrameNew()){
        cvtColor( ofxCv::toCv(grabber), grayscale_image, CV_BGR2GRAY);
        bool detection_success = CLMTracker::DetectLandmarksInVideo(grayscale_image, depth_image, *clm_model, *clm_parameters);
        //cout << detection_success << " " << endl;
        
        
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
        
        if (detection_certainty < -0.2){
            faceEnergy = faceEnergy * 0.9 + 0.1 * 1.0;
        } else {
            faceEnergy = faceEnergy * 0.9 + 0.1 * 0.0;
        }
        //cout << clm_model->detection_certainty << " cert " << endl;
        
        

    }
    
    
}


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
void clmFaceTracker::draw(){
    
    fbo.begin();
    grabber.draw(0,0);
   
    ofCamera cam;
    cam.setupPerspective();
    
    int idx = clm_model->patch_experts.GetViewIdx(clm_model->params_global, 0);
    
    // Because we only draw visible points, need to find which points patch experts consider visible at a certain orientation
   Draw(clm_model->detected_landmarks, clm_model->patch_experts.visibilities[0][idx]);
    
    
   
    
    
    Vec6d pose_estimate_to_draw = CLMTracker::GetCorrectedPoseCameraPlane(*clm_model, cam.getPosition().z, cam.getPosition().z, grabber.getWidth()/2, grabber.getHeight()/2);
    
//    
//    ofPushMatrix();
//    
//    ofNoFill();
//    ofTranslate(grabber.getWidth()/2, grabber.getHeight()/2);
//    
//    //cam.begin();
//    ofDrawBox(pose_estimate_to_draw[0], pose_estimate_to_draw[1], (cam.getPosition().z - pose_estimate_to_draw[2]), 150,150,150);
//    //cam.end();
//    
//    ofTranslate(pose_estimate_to_draw[0], pose_estimate_to_draw[1], (cam.getPosition().z - pose_estimate_to_draw[2]));
//    
//    ofQuaternion q = eulerToQuat( ofVec3f(   pose_estimate_to_draw[3], pose_estimate_to_draw[4],  pose_estimate_to_draw[5] ));
//    //    cout << mouseX/5.0 << endl;
////    ofRotate(1, pose_estimate_to_draw[3] * RAD_TO_DEG, pose_estimate_to_draw[4] * RAD_TO_DEG, pose_estimate_to_draw[5] * RAD_TO_DEG);
//    
//    ofScale(1,1,-1); //(mouseX/5.0);
//    ofMultMatrix(q);
////
////    
////    
////    ofDrawBox(0,0,0, 150,150,50);
//    
//    
////    ofEnableBlendMode(OF_BLENDMODE_ADD);
////    ofSetRectMode(OF_RECTMODE_CENTER);
////    face.draw(0,0,-50, 190,190);
////    ofSetRectMode(OF_RECTMODE_CORNER);
////    ofEnableAlphaBlending();
////    
//    
//    //cout << pose_estimate_to_draw[2] << " " << cam.getPosition().z <<  endl;
//    
//    
//    
//    ofPopMatrix();
    
//    for (int i = 0; i < pts.size(); i++){
//        if (clm_model->patch_experts.visibilities[0][idx][i]){
//            ofDrawCircle(pts[i].x, pts[i].y, 3);
//        }
//    }
    
    ofDrawBitmapStringHighlight("certainty: " + ofToString(detection_certainty), 20,40);
    ofDrawBitmapStringHighlight("faceEnergy: " + ofToString(faceEnergy), 20,60);
    
    fbo.end();
    fbo.draw(0,0);
    //ofViewport(0,0,ofGetWidth(), ofGetHeight());
    
}

