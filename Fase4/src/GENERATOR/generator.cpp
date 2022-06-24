#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../tinyxml2/tinyxml2.h"
#include "headers/bezier.hpp"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <cstring>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

using namespace std;
using namespace tinyxml2;

/**
* Function that creates an .3d file with the model information received.
* @param ModelInformation (string)
* @param FileName (string)
*/
void writeTo3D(string modelInformation, string FileName){
    char tmp[256];

    // Keep the actual directory in the variable 'tmp'
    getcwd(tmp, 256);

    // Find the 'build' location
    string path(tmp);
    int found = path.find("build");

    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    string path3d = path + "src/MODELS/" + FileName;
    ofstream File(path3d);
    File << modelInformation;

    File.close();
}

/**
* Function that creates a plane in all the possible planes.
* @param Length (float)
* @param Divisions (int), number of divisions in each axis
* @param FaceID (int), identifier for faces in a box (0- bottom, 1- plane with number of points, 2- upper, 3- back, 4- front, 5-left, 6-right)
*/
string createPlane(float length, float divisions, int faceID) {
    string planeINFO("");
    float distance = length / divisions;
    float half_length = length/2;
    float px, py, pz;

    float hor = 1.0f / (float) divisions;
    float ver = 1.0f / (float) divisions;

    // BOTTOM face
    int numberPOINTS = 0;
    if (faceID == 0) {
        float h;
        float v;

        py = -length/2;
        for (int i = 0; i < divisions; i++) {
            pz = distance * i - half_length;

            h = 0;
            v = ver * i;

            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;
                h = hor * j;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";


                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

            }
        }
    }
    // PLANE 
    if (faceID == 1) {
        py = 0.0f;

        float v;
        float h;

        for (int i = 0; i < divisions; i++) {
            pz = distance * i - half_length;

            h = 0;
            v = ver * i;

            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;
                h = hor * j;

                // Para cima
                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                //PARA BAIXO
                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                // Second triangle
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                numberPOINTS += 12;
            }
        }
    }
    // UPPER face
    if (faceID == 2) {
        float h;
        float v;

        py = length/2;
        for (int i = 0; i < divisions; i++) {
            h = 0;
            v = ver * i;
        
            pz = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                h = hor * j;
                px = distance * j - half_length;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

            }
        }
    }
    // BACK face
    else if (faceID == 3) {
        pz = -half_length;

        float h;
        float v;

        for (int i = 0; i < divisions; i++) {
            v = ver * i;
            h = 0;
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                h = hor * j;
                px = distance * j - half_length;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(-1) + "\n";   
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";             

            }
        }
    }
    // FRONT face
    else if (faceID == 4) {
        pz = half_length;

        float h;
        float v;

        for (int i = 0; i < divisions; i++) {
            v = ver * i;
            h = 0;
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                h = hor * j;
                px = distance * j - half_length;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";


                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px + distance) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(0) + "," + std::to_string(0) + "," + std::to_string(1) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";


            }

        }
    }
    // LEFT face
    else if (faceID == 5) {
        px = -half_length;

        float h;
        float v;

        for (int i = 0; i < divisions; i++) {
            v = ver * i;
            h = 0;
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                h = hor * j;
                pz = distance * j - half_length;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";


                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(-1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";  
            }
        }
    }
    // RIGHT face
    else if (faceID == 6) {
        px = half_length;

        float h;
        float v;

        for (int i = 0; i < divisions; i++) {
            v = ver * i;
            h = 0;
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                h = hor * j;
                pz = distance * j - half_length;

                // Draw Points, Draw Normals, Draw Textures
                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";


                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v + ver) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h + hor) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(1) + "," + std::to_string(0) + "," + std::to_string(0) + "\n";
                planeINFO += std::to_string(h) + "," + std::to_string(v) + "," + std::to_string(0) + "\n";

            }
        }
    }

    return (faceID == 1) ? std::to_string(numberPOINTS) + "\n" + planeINFO : planeINFO;
}

/**
* Function that creates a box.
* @param Length (float)
* @param Divisions (int), number of divisions in each axis
*/
string createBox(int length, int divisions) {
    string boxINFO("");

    // Calculate the number of points in the box
    int numberPOINTS = pow(6, 2) * pow(divisions, 2);
    boxINFO += std::to_string(numberPOINTS) + "\n";

    // BOTTOM FACE
    boxINFO += createPlane(length, divisions, 0);

    // UPPER FACE
    boxINFO += createPlane(length, divisions, 2);

    // BACK FACE
    boxINFO += createPlane(length, divisions, 3);

    // FRONT FACE
    boxINFO += createPlane(length, divisions, 4);

    // LEFT FACE
    boxINFO += createPlane(length, divisions, 5);

    // RIGHT FACE
    boxINFO += createPlane(length, divisions, 6);

    return boxINFO;
}

/**
* Function that creates a sphere.
* @param Radius (float)
* @param Slices (int)
* @param Stacks (int)
*/
string createSphere(float radius, int slices, int stacks) {
    string sphereINFO("");
    int numberPoints = slices * stacks * 6;
    sphereINFO += std::to_string(numberPoints) + "\n";

    float textH = 1.0f / (float) slices;
    float textV = 1.0f / (float) stacks;

    float angle_alpha = 2 * M_PI / slices;
    float angle_beta = M_PI / stacks;

    for (int i = 0; i < stacks; i++) {
        float beta = -(M_PI / 2) + i * angle_beta;
        float y2 = radius * sinf(angle_beta + beta);
        float y1 = radius * sinf(beta);
        float n_y2 = sinf(angle_beta + beta);
        float n_y1 = sinf(beta);

        for (int j = 0; j < slices; j++) {
            float alpha = angle_alpha * j;
            
            float x1 = radius * cos(beta + angle_beta) * sin(alpha);
            float z1 = radius * cos(beta + angle_beta) * cos(alpha);
            float n_x1 = cos(beta + angle_beta) * sin(alpha);
            float n_z1 = cos(beta + angle_beta) * cos(alpha);
            float t_x1 = j * textH;
            float t_y1 = i * textV + textV;

            float x2 = radius * cosf(beta) * sinf(alpha);
            float z2 = radius * cosf(beta) * cosf(alpha);
            float n_x2 = cosf(beta) * sinf(alpha);
            float n_z2 = cosf(beta) * cosf(alpha);
            float t_x2 = j * textH;
            float t_y2 = i * textH;

            float x3 = radius * cosf(beta) * sinf(alpha + angle_alpha);
            float z3 = radius * cosf(beta) * cosf(alpha + angle_alpha);
            float n_x3 = cosf(beta) * sinf(alpha + angle_alpha);
            float n_z3 = cosf(beta) * cosf(alpha + angle_alpha);
            float t_x3 = j * textH + textH;
            float t_y3 = i * textH;
            
            float x4 = radius * cosf(beta + angle_beta) * sinf(alpha + angle_alpha);
            float z4 = radius * cosf(beta + angle_beta) * cosf(alpha + angle_alpha);
            float n_x4 = cosf(beta + angle_beta) * sinf(alpha + angle_alpha);
            float n_z4 = cosf(beta + angle_beta) * cosf(alpha + angle_alpha);
            float t_x4 = j * textH + textH;
            float t_y4 = i * textH + textV;


            // Draw Points, Draw Normals, Draw Textures
            sphereINFO += std::to_string(x1) + "," + std::to_string(y2) + "," + std::to_string(z1) + "\n";
            sphereINFO += std::to_string(n_x1) + "," + std::to_string(n_y2) + "," + std::to_string(n_z1) + "\n";
            sphereINFO += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

            sphereINFO += std::to_string(x2) + "," + std::to_string(y1) + "," + std::to_string(z2) + "\n";
            sphereINFO += std::to_string(n_x2) + "," + std::to_string(n_y1) + "," + std::to_string(n_z2) + "\n";
            sphereINFO += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";

            sphereINFO += std::to_string(x3) + "," + std::to_string(y1) + "," + std::to_string(z3) + "\n";
            sphereINFO += std::to_string(n_x3) + "," + std::to_string(n_y1) + "," + std::to_string(n_z3) + "\n";
            sphereINFO += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

            sphereINFO += std::to_string(x1) + "," + std::to_string(y2) + "," + std::to_string(z1) + "\n";
            sphereINFO += std::to_string(n_x1) + "," + std::to_string(n_y2) + "," + std::to_string(n_z1) + "\n";
            sphereINFO += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

            sphereINFO += std::to_string(x3) + "," + std::to_string(y1) + "," + std::to_string(z3) + "\n";
            sphereINFO += std::to_string(n_x3) + "," + std::to_string(n_y1) + "," + std::to_string(n_z3) + "\n";
            sphereINFO += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

            sphereINFO += std::to_string(x4) + "," + std::to_string(y2) + "," + std::to_string(z4) + "\n";
            sphereINFO += std::to_string(n_x4) + "," + std::to_string(n_y2) + "," + std::to_string(n_z4) + "\n";
            sphereINFO += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

        }
    }

    return sphereINFO;
}

/**
* Function that creates a cone.
* @param Radius (float)
* @param Height (float)
* @param Slices (int)
* @param Stacks (int)
*/
string createCone(float radius, float height, int slices, int stacks) {
    string coneINFO("");
    float angle_alpha = (2 * M_PI) / slices;
    float px, py, pz;
    int numberPOINTS = 0;

    float textH = 1.0f / slices;
    float textV = 1.0f / stacks;

    float var = (2 * radius * M_PI) / slices;

    // BASE
    for (int i = 0; i < slices; i++) {
        float alpha = angle_alpha * i;

        // Draw Points, Draw Normals, Draw Textures
        coneINFO += std::to_string(radius * sinf(alpha)) + ",0.0," + std::to_string(radius* cosf(alpha)) + "\n";
        coneINFO += "0.0,-1.0,0.0\n";
        coneINFO += std::to_string(i * var) + ",0.0," + "0.0" + "\n";

        coneINFO += "0.0,0.0,0.0\n";
        coneINFO += "0.0,-1.0,0.0\n";
        coneINFO += std::to_string(var / 2) + ",1.0," + "0.0" + "\n";

        coneINFO += std::to_string(radius * sinf(alpha + angle_alpha)) + ",0.0," + std::to_string(radius* cosf(alpha + angle_alpha)) + "\n";
        coneINFO += "0.0,-1.0,0.0\n";
        coneINFO += std::to_string((i+1) * var) + ",0.0," + "0.0" + "\n";

        numberPOINTS += 3;
    }

    // SIDES
    float angle_beta = height / stacks;
    float n_y = cosf(atan(height/radius));

    for (int i = 0; i < stacks; i++) {
        float beta = i * angle_beta;
        float next_beta = (i + 1) * angle_beta;
        float r1 = radius - (i * (radius / stacks));
        float r2 = radius - ((i + 1) * (radius / stacks));

        for (int j = 0; j < slices; j++) {
            float alpha = angle_alpha * j;

            float x1 = r1 * sinf(alpha);
            float z1 = r1 * cosf(alpha);
            float n_x1 = sinf(alpha);
            float n_z1 = cosf(alpha);
            float t_x1 = j * textH;
            float t_y1 = i * textV;

            float x2 = r2 * sinf(alpha + angle_alpha);
            float z2 = r2 * cosf(alpha + angle_alpha);
            float n_x2 = sinf(alpha + angle_alpha);
            float n_z2 = cosf(alpha + angle_alpha);
            float t_x2 = (j + 1) * textH;
            float t_y2 = (i + 1) * textV;

            float x3 = r2 * sinf(alpha);
            float z3 = r2 * cosf(alpha);
            float n_x3 = sinf(alpha);
            float n_z3 = cosf(alpha);
            float t_x3 = j * textH;
            float t_y3 = (i+1) * textV;

            float x4 = r1 * sinf(alpha + angle_alpha);
            float z4 = r1 * cosf(alpha + angle_alpha);
            float n_x4 = sinf(alpha + angle_alpha);
            float n_z4 = cosf(alpha + angle_alpha);
            float t_x4 = (j + 1) * textH;
            float t_y4 = i * textV;

            // Draw Points, Draw Normals, Draw Textures
            coneINFO += std::to_string(x1) + "," + std::to_string(beta) + "," + std::to_string(z1) + "\n";
            coneINFO += std::to_string(n_x1) + "," + std::to_string(n_y) + "," + std::to_string(n_z1) + "\n";
            coneINFO += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

            coneINFO += std::to_string(x2) + "," + std::to_string(next_beta) + "," + std::to_string(z2) + "\n";
            coneINFO += std::to_string(n_x2) + "," + std::to_string(n_y) + "," + std::to_string(n_z2) + "\n";
            coneINFO += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";

            coneINFO += std::to_string(x3) + "," + std::to_string(next_beta) + "," + std::to_string(z3) + "\n";
            coneINFO += std::to_string(n_x3) + "," + std::to_string(n_y) + "," + std::to_string(n_z3) + "\n";
            coneINFO += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

            coneINFO += std::to_string(x1) + "," + std::to_string(beta) + "," + std::to_string(z1) + "\n";
            coneINFO += std::to_string(n_x1) + "," + std::to_string(n_y) + "," + std::to_string(n_z1) + "\n";
            coneINFO += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

            coneINFO += std::to_string(x4) + "," + std::to_string(beta) + "," + std::to_string(z4) + "\n";
            coneINFO += std::to_string(n_x4) + "," + std::to_string(n_y) + "," + std::to_string(n_z4) + "\n";
            coneINFO += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

            coneINFO += std::to_string(x2) + "," + std::to_string(next_beta) + "," + std::to_string(z2) + "\n";
            coneINFO += std::to_string(n_x2) + "," + std::to_string(n_y) + "," + std::to_string(n_z2) + "\n";
            coneINFO += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";
            

            numberPOINTS += 6;
        }
    }

    return std::to_string(numberPOINTS) + "\n" + coneINFO;
}

/**
* Function that creates a torus.
* @param RingRadius (float)
* @param InnerRadius (float)
* @param Slices (int)
* @param Stacks (int)
*/
string createTorus(float radius, float ringRadius, int slices, int stacks) {

    string torusInfo;
    int numberPoints = slices * stacks * 6;
    torusInfo += std::to_string(numberPoints) + "\n";

    float angle_alpha = 2 * M_PI / slices;
    float angle_beta = 2 * M_PI / stacks;
    float beta = 0;
    float alpha = 0;

    float textH = 1.0f / slices;
    float textV = 1.0f / stacks;

    for (int i = 0; i < slices; i++) {
        alpha = i * angle_alpha;
        for (int j = 0; j < stacks; j++) {
            beta = j * angle_beta;

            float x1 = (radius + ringRadius * cosf(alpha)) * cosf(beta);
            float z1 = (radius + ringRadius * cosf(alpha))* sinf(beta);
            float y1 = ringRadius * sinf(alpha);
            float n_x1 = cosf(alpha) * cosf(beta);
            float n_z1 = cosf(alpha) * sinf(beta);
            float n_y1 = sinf(alpha);
            float t_x1 = i * textH;
            float t_y1 = j * textV;

            float x2 = (radius + ringRadius * cosf(alpha + angle_alpha)) * cosf(beta);
            float z2 = (radius + ringRadius * cosf(alpha + angle_alpha)) * sinf(beta);
            float y2 = ringRadius * sinf(alpha + angle_alpha);
            float n_x2 = cosf(alpha + angle_alpha) * cosf(beta);
            float n_z2 = cosf(alpha + angle_alpha) * sinf(beta);
            float n_y2 = sinf(alpha + angle_alpha);
            float t_x2 = (i+1) * textH;
            float t_y2 = j * textV;

            float x3 = (radius + ringRadius * cosf(alpha)) * cosf(beta + angle_beta);
            float z3 = (radius + ringRadius * cosf(alpha)) * sinf(beta + angle_beta);
            float y3 = ringRadius * sinf(alpha);
            float n_x3 = cosf(alpha) * cosf(beta + angle_beta);
            float n_z3 = cosf(alpha) * sinf(beta + angle_beta);
            float n_y3 = sinf(alpha);
            float t_x3 = i * textH;
            float t_y3 = (j+1) * textV;

            float x4 = (radius + ringRadius * cosf(alpha + angle_alpha)) * cosf(beta + angle_beta);
            float z4 = (radius + ringRadius * cosf(alpha + angle_alpha)) * sinf(beta + angle_beta);
            float y4 = ringRadius * sinf(alpha + angle_alpha);
            float n_x4 = cosf(alpha + angle_alpha) * cosf(beta + angle_beta);
            float n_z4 = cosf(alpha + angle_alpha) * sinf(beta + angle_beta);
            float n_y4 = sinf(alpha + angle_alpha);
            float t_x4 = (i + 1) * textH;
            float t_y4 = (j + 1) * textV;

            // Draw Points, Draw Normals, Draw Textures
            torusInfo += std::to_string(x3) + "," + std::to_string(y3) + "," + std::to_string(z3) + "\n";
            torusInfo += std::to_string(n_x3) + "," + std::to_string(n_y3) + "," + std::to_string(n_z3) + "\n";
            torusInfo += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

            torusInfo += std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(z1) + "\n";
            torusInfo += std::to_string(n_x1) + "," + std::to_string(n_y1) + "," + std::to_string(n_z1) + "\n";
            torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

            torusInfo += std::to_string(x4) + "," + std::to_string(y4) + "," + std::to_string(z4) + "\n";
            torusInfo += std::to_string(n_x4) + "," + std::to_string(n_y4) + "," + std::to_string(n_z4) + "\n";
            torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

            torusInfo += std::to_string(x2) + "," + std::to_string(y2) + "," + std::to_string(z2) + "\n";
            torusInfo += std::to_string(n_x2) + "," + std::to_string(n_y2) + "," + std::to_string(n_z2) + "\n";
            torusInfo += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";

            torusInfo += std::to_string(x4) + "," + std::to_string(y4) + "," + std::to_string(z4) + "\n";
            torusInfo += std::to_string(n_x4) + "," + std::to_string(n_y4) + "," + std::to_string(n_z4) + "\n";
            torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

            torusInfo += std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(z1) + "\n";
            torusInfo += std::to_string(n_x1) + "," + std::to_string(n_y1) + "," + std::to_string(n_z1) + "\n";
            torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";
            
        }
    }
    return torusInfo;
}

/**
* Function that creates a flat torus.
* @param InnerRadius (float)
* @param OutterRadius (float)
* @param Slices (int)
*/
string createFlatTorus(float innerRadius, float outterRadius, int slices){
        
        string torusInfo;
        int numberPoints = slices * 12;
        torusInfo += std::to_string(numberPoints) + "\n";
        
        float angle_alpha = 2*M_PI/(float)slices;

        float ver = 1.0f / (float) slices;
        float v;
        for (int i = 0; i < slices; i++){

                float alpha = i * angle_alpha;

                float x1 = innerRadius * sinf(alpha);
                float z1 = innerRadius * cosf(alpha);
                float t_x1 = 0;
                float t_y1 = i * ver;

                float x2 = outterRadius * sinf(alpha);
                float z2 = outterRadius * cosf(alpha);
                float t_x2 = 1;
                float t_y2 = i * ver;

                float x3 = innerRadius * sinf(alpha + angle_alpha);
                float z3 = innerRadius * cosf(alpha + angle_alpha);
                float t_x3 = 0;
                float t_y3 = (i+1) * ver;

                float x4 = outterRadius * sinf(alpha + angle_alpha);
                float z4 = outterRadius * cosf(alpha + angle_alpha);
                float t_x4 = 1;
                float t_y4 = (i+1) * ver;

                //UPPER FACE
                // Draw Points, Draw Normals, Draw Textures
                torusInfo += std::to_string(x2) + "," + std::to_string(0) + "," + std::to_string(z2) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x4) + "," + std::to_string(0) + "," + std::to_string(z4) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x1) + "," + std::to_string(0) + "," + std::to_string(z1) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x4) + "," + std::to_string(0) + "," + std::to_string(z4) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x3) + "," + std::to_string(0) + "," + std::to_string(z3) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x1) + "," + std::to_string(0) + "," + std::to_string(z1) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

                //LOWER FACE
                // Draw Points, Draw Normals, Draw Textures
                torusInfo += std::to_string(x1) + "," + std::to_string(0) + "," + std::to_string(z1) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x3) + "," + std::to_string(0) + "," + std::to_string(z3) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x3) + "," + std::to_string(t_y3) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x4) + "," + std::to_string(0) + "," + std::to_string(z4) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x1) + "," + std::to_string(0) + "," + std::to_string(z1) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x1) + "," + std::to_string(t_y1) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x4) + "," + std::to_string(0) + "," + std::to_string(z4) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x4) + "," + std::to_string(t_y4) + "," + std::to_string(0) + "\n";

                torusInfo += std::to_string(x2) + "," + std::to_string(0) + "," + std::to_string(z2) + "\n";
                torusInfo += std::to_string(0) + "," + std::to_string(-1) + "," + std::to_string(0) + "\n";
                torusInfo += std::to_string(t_x2) + "," + std::to_string(t_y2) + "," + std::to_string(0) + "\n";

        }

        return torusInfo;
}

/**
 * Function that creates Bezier
 * @param inputFile (string)
 * @param tesselation (float)
 */
string createBezier(string inputFile, float tesselation) {
    string bezierInfo;
    int numberPatches = 0;
    int numberControlPoints = 0;
    int i = 0;
    vector<int> patchesIndices;
    string points;
    string line;

    char tmp[256];
    getcwd(tmp, 256);
    string path(tmp);
    int found = path.find("build");
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());
    string filePath = path + "src/MODELS/" + inputFile;

    vector<vector<float>> all_points;

    int counter = 0;

    ifstream PatchFile(filePath);

    while (getline(PatchFile, line)) {
        if (i == 0) {
            numberPatches = stoi(line);
        }
        else if (i == numberPatches + 1) {
            numberControlPoints = stoi(line);
        }
        else if (i <= numberPatches) {
            char* char_array;
            char_array = &line[0];
            char* token = strtok(char_array, ",");

            while (token != NULL) {
                patchesIndices.push_back(atof(token));
                token = strtok(NULL, ",");
            }
        
        }
        else if (i > numberPatches + 1) {
            vector<float> point;
            char* char_array;
            char_array = &line[0];
            char* token = strtok(char_array, ",");

            while (token != NULL) {
                
                point.push_back(atof(token));
                
                token = strtok(NULL, ",");
            }
            all_points.push_back(point);
        }
        i++;
    }
    string res = parseBezierPatches(patchesIndices, all_points, tesselation);
    
    return res;
}

// MAIN
int main(int argc, char** argv) {
    string modelInformation;
    string fileName;

    for (int i = 0; i < argc; i++) {
        printf("%d - %s\n", i, argv[i]);
    }

    if (argc < 5) {
        printf ("Not enough arguments");
        return 1;
    }
    else if (std::string(argv[1]) == "sphere") {
        modelInformation = createSphere(stof(argv[2]), stoi(argv[3]), stoi(argv[4]));
        fileName = argv[5];
    }
    else if (std::string(argv[1]) == "box") {
        modelInformation = createBox(stoi(argv[2]), stoi(argv[3]));
        fileName = argv[4];
    }
    else if (std::string(argv[1]) == "cone") {
        modelInformation = createCone(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]));
        fileName = argv[6];
    }
    else if (std::string(argv[1]) == "plane") {
        modelInformation = createPlane(stof(argv[2]), stof(argv[3]), 1);
        fileName = argv[4];
    }
    else if (std::string(argv[1]) == "torus") {
        modelInformation = createTorus(stof(argv[2]), stof(argv[3]), stoi(argv[4]), stoi(argv[5]));
        fileName = argv[6];
    }
    else if (std::string(argv[1]) == "bezier") { // generator bezier teapot.patch 0.1 bezier.3d
        modelInformation = createBezier(argv[2], stof(argv[3]));
        fileName = argv[4];
    }
    else if(std::string(argv[1]) == "flatTorus"){
        modelInformation = createFlatTorus(stof(argv[2]), stof(argv[3]), stof(argv[4]));
        fileName = argv[5];
    }
    else {
        printf("Please provide the correct arguments");
        return 1;
    }

    if (!fileName.empty() && !modelInformation.empty()) {
        writeTo3D(modelInformation, fileName);
    }
    else {
        printf("Invalid Arguments!");
        return 1;
    }

    return 0;
}

