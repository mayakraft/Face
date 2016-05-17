
#pragma once

// (1) or (-1)
//#define SCREEN_ROTATION (-1)
//int SCREEN_ROTATION = -1;
//#define CAMERA_ROTATION (-1)

// to make it easy, everything here inputted as width = longer, height = shorter
// rotations handled in code
#define RESOLUTION_SCREEN_WIDTH ofGetScreenWidth()
#define RESOLUTION_SCREEN_HEIGHT ofGetScreenHeight()
#define RESOLUTION_CAMERA_WIDTH 1920 // 1024
#define RESOLUTION_CAMERA_HEIGHT 1080 // 768
#define RESOLUTION_FACE_DETECTOR_WIDTH 640//1024  //RESOLUTION_CAMERA_WIDTH  //1024
#define RESOLUTION_FACE_DETECTOR_HEIGHT 480//768 //RESOLUTION_CAMERA_HEIGHT  //768
