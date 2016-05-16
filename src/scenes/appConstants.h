
#pragma once

// assume that visuals and code are the same here....
// and also assuming our screen is square :)

// 1 or -1
#define SCREEN_ROTATION (-1)

// to make it easy, everything here inputted as width = longer, height = shorter
// rotations handled in code
#define RESOLUTION_SCREEN_WIDTH ofGetScreenWidth()
#define RESOLUTION_SCREEN_HEIGHT ofGetScreenHeight()
#define RESOLUTION_CAMERA_WIDTH 1920 // 1024
#define RESOLUTION_CAMERA_HEIGHT 1080 // 768
#define RESOLUTION_FACE_DETECTOR_WIDTH 1024  //RESOLUTION_CAMERA_WIDTH  //1024
#define RESOLUTION_FACE_DETECTOR_HEIGHT 768 //RESOLUTION_CAMERA_HEIGHT  //768

#define VISUALS_WIDTH 504
#define VISUALS_HEIGHT 1000

#define CODE_WIDTH VISUALS_WIDTH
#define CODE_HEIGHT VISUALS_HEIGHT

#define CODE_X_POS 524      // via the tech staff

// if this is defined, we will wait for type animation
#define TYPE_ANIMATION

#define USE_MIDI_PARAM_SYNC
// this is used to enable recording/playback of midi data.
#define USE_MIDI_RECORDING

#define USE_SCENE_TRANSITIONS

// if this is defined, we will disable all sound playback inside this app, and instead send
// OpenSoundControl messages which can be used to trigger sounds in another program (e.g. Ableton Live)
//#define USE_EXTERNAL_SOUNDS

#define OSC_HOST "localhost" // send to this host
#define OSC_PORT 12345       // send on this port

