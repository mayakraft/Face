#pragma once

#include "ofMain.h"

class Point4D : public ofBaseApp{
public:
    float x, y, z, w;
    Point4D();
    Point4D(float x, float y, float z, float w);
    
    ofVec3f threeD(); // converts 4D to 3D by flattening the 4th (orthographic)
    
    void rotate(ofMatrix4x4 m);
};

class Polychron : public ofBaseApp {
public:
    Polychron();
    void loadVefFile(string file);
    void drawWireframe();
    
    // proper rotaiton (includes all 4D)
    void rotate(ofMatrix4x4 m); // cumulative.  all 4 cols/rows are rotation, not 3&1 translation like 3D opengl
    void rotate4DOnly(float dwx, float dwy, float dwz);  // non cumulative
    void rotate4DOnly(ofVec3f dAxes);  // non cumulative
    
    unsigned int getNumVertices();
    unsigned int getNumEdges();
//    unsigned int getNumFaces();
    
    
    // BONUS THINGS:
    
    // some extra fun stuff
    vector<float> edgeEnergy;  // size of edge / 2, due to span
    vector<float> vertexEnergy;  // size of vertex
    
    // algorithm stuff
    vector<unsigned int>allEdgesAdjacentTo(unsigned int vertexIndex);
    vector<unsigned int>allVerticesAdjacentTo(unsigned int vertexIndex);
    void drawEdgesTouchingVertex(int vertexIndex);
    
    void decrementEnergy();
    
    void log();  // printf vertices, edges, faces
    
//private:
    float wx, wy, wz;  // RADIANS, angle of rotations (each through the 4th dimension)
    vector<Point4D> vertices;  // these will change when you apply the rotations
    vector<unsigned int> edges; // indices in vertices array
    vector<unsigned int> faces;  // indices in vertices array
    unsigned int verticesPerFace; // TODO
    
private:
    // the coordinates of the shape at 0 rotation
    vector<Point4D> _v;
    vector<unsigned int> _e;
    vector<unsigned int> _f;
};
