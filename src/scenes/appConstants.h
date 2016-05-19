
#pragma once

// (1) or (-1)
//#define SCREEN_ROTATION (-1)
//int SCREEN_ROTATION = -1;
//#define CAMERA_ROTATION (-1)

// to make it easy, everything here inputted as width = longer, height = shorter
// rotations handled in code
#define RESOLUTION_WINDOW_WIDTH 800  //ofGetScreenWidth()
#define RESOLUTION_WINDOW_HEIGHT 450  //ofGetScreenHeight()
#define RESOLUTION_CAMERA_WIDTH 1920  // 1024
#define RESOLUTION_CAMERA_HEIGHT 1080  // 768
#define RESOLUTION_FACE_DETECTOR_WIDTH 800 // 640  //1024  //RESOLUTION_CAMERA_WIDTH  //1024
#define RESOLUTION_FACE_DETECTOR_HEIGHT 450 // 360  //768 //RESOLUTION_CAMERA_HEIGHT  //768
