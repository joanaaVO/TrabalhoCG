#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../tinyxml2/tinyxml2.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
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

    // BOTTOM face
    int numberPOINTS = 0;
    if (faceID == 0) {
        py = -length/2;
        for (int i = 0; i < divisions; i++) {
            pz = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
            }
        }
    }
    // PLANE 
    if (faceID == 1) {
        py = 0.0f;
        for (int i = 0; i < divisions; i++) {
            pz = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";

                
                numberPOINTS += 6;
            }
        }
    }
    // UPPER face
    if (faceID == 2) {
        py = length/2;
        for (int i = 0; i < divisions; i++) {
            pz = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
            }
        }
    }
    // BACK face
    else if (faceID == 3) {
        pz = -half_length;

        for (int i = 0; i < divisions; i++) {
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
            }
        }
    }
    // FRONT face
    else if (faceID == 4) {
        pz = half_length;

        for (int i = 0; i < divisions; i++) {
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                px = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px + distance) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
            }
        }
    }
    // LEFT face
    else if (faceID == 5) {
        px = -half_length;

        for (int i = 0; i < divisions; i++) {
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                pz = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
            }
        }
    }
    // RIGHT face
    else if (faceID == 6) {
        px = half_length;

        for (int i = 0; i < divisions; i++) {
            py = distance * i - half_length;
            for (int j = 0; j < divisions; j++) {
                pz = distance * j - half_length;

                // First triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz) + "\n";

                // Second triangle
                planeINFO += std::to_string(px) + "," + std::to_string(py + distance) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz + distance) + "\n";
                planeINFO += std::to_string(px) + "," + std::to_string(py) + "," + std::to_string(pz) + "\n";
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

    float angle_alpha = 2 * M_PI / slices;
    float angle_beta = M_PI / stacks;

    for (int i = 0; i < stacks; i++) {
        float beta = -(M_PI / 2) + i * angle_beta;
        float y2 = radius * sinf(angle_beta + beta);
        float y1 = radius * sinf(beta);

        for (int j = 0; j < slices; j++) {
            float alpha = angle_alpha * j;
            float x1 = radius * cos(beta + angle_beta) * sin(alpha);
            float z1 = radius * cos(beta + angle_beta) * cos(alpha);

            float x2 = radius * cosf(beta) * sinf(alpha);
            float z2 = radius * cosf(beta) * cosf(alpha);

            float x3 = radius * cosf(beta) * sinf(alpha + angle_alpha);
            float z3 = radius * cosf(beta) * cosf(alpha + angle_alpha);
            
            float x4 = radius * cosf(beta + angle_beta) * sinf(alpha + angle_alpha);
            float z4 = radius * cosf(beta + angle_beta) * cosf(alpha + angle_alpha);

            sphereINFO += std::to_string(x1) + "," + std::to_string(y2) + "," + std::to_string(z1) + "\n";
            sphereINFO += std::to_string(x2) + "," + std::to_string(y1) + "," + std::to_string(z2) + "\n";
            sphereINFO += std::to_string(x3) + "," + std::to_string(y1) + "," + std::to_string(z3) + "\n";

            sphereINFO += std::to_string(x1) + "," + std::to_string(y2) + "," + std::to_string(z1) + "\n";
            sphereINFO += std::to_string(x3) + "," + std::to_string(y1) + "," + std::to_string(z3) + "\n";
            sphereINFO += std::to_string(x4) + "," + std::to_string(y2) + "," + std::to_string(z4) + "\n";
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

    // BASE
    for (int i = 0; i < slices; i++) {
        float alpha = angle_alpha * i;

        coneINFO += std::to_string(radius * sinf(alpha)) + ",0.0," + std::to_string(radius* cosf(alpha)) + "\n";
        coneINFO += "0.0,0.0,0.0\n";
        coneINFO += std::to_string(radius * sinf(alpha + angle_alpha)) + ",0.0," + std::to_string(radius* cosf(alpha + angle_alpha)) + "\n";

        numberPOINTS += 3;
    }

    // SIDES
    float angle_beta = height / stacks;
    for (int i = 0; i < stacks; i++) {
        float beta = i * angle_beta;
        float next_beta = (i + 1) * angle_beta;
        float r1 = radius - (i * (radius / stacks));
        float r2 = radius - ((i + 1) * (radius / stacks));

        for (int j = 0; j < slices; j++) {
            float alpha = angle_alpha * j;

            float x1 = r1 * sinf(alpha);
            float z1 = r1 * cosf(alpha);

            float x2 = r2 * sinf(alpha + angle_alpha);
            float z2 = r2 * cosf(alpha + angle_alpha);

            float x3 = r2 * sinf(alpha);
            float z3 = r2 * cosf(alpha);

            float x4 = r1 * sinf(alpha + angle_alpha);
            float z4 = r1 * cosf(alpha + angle_alpha);

            coneINFO += std::to_string(x1) + "," + std::to_string(beta) + "," + std::to_string(z1) + "\n";
            coneINFO += std::to_string(x2) + "," + std::to_string(next_beta) + "," + std::to_string(z2) + "\n";
            coneINFO += std::to_string(x3) + "," + std::to_string(next_beta) + "," + std::to_string(z3) + "\n";

            coneINFO += std::to_string(x1) + "," + std::to_string(beta) + "," + std::to_string(z1) + "\n";
            coneINFO += std::to_string(x4) + "," + std::to_string(beta) + "," + std::to_string(z4) + "\n";
            coneINFO += std::to_string(x2) + "," + std::to_string(next_beta) + "," + std::to_string(z2) + "\n";

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

    for (int i = 0; i < slices; i++) {
        alpha = i * angle_alpha;
        for (int j = 0; j < stacks; j++) {
            beta = j * angle_beta;

            float x1 = (radius + ringRadius * cosf(alpha)) * cosf(beta);
            float z1 = (radius + ringRadius * cosf(alpha))* sinf(beta);
            float y1 = ringRadius * sinf(alpha);

            float x2 = (radius + ringRadius * cosf(alpha + angle_alpha)) * cosf(beta);
            float z2 = (radius + ringRadius * cosf(alpha + angle_alpha)) * sinf(beta);
            float y2 = ringRadius * sinf(alpha + angle_alpha);

            float x3 = (radius + ringRadius * cosf(alpha)) * cosf(beta + angle_beta);
            float z3 = (radius + ringRadius * cosf(alpha)) * sinf(beta + angle_beta);
            float y3 = ringRadius * sinf(alpha);

            float x4 = (radius + ringRadius * cosf(alpha + angle_alpha)) * cosf(beta + angle_beta);
            float z4 = (radius + ringRadius * cosf(alpha + angle_alpha)) * sinf(beta + angle_beta);
            float y4 = ringRadius * sinf(alpha + angle_alpha);

            
            torusInfo += std::to_string(x3) + "," + std::to_string(y3) + "," + std::to_string(z3) + "\n";
            torusInfo += std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(z1) + "\n";
            torusInfo += std::to_string(x4) + "," + std::to_string(y4) + "," + std::to_string(z4) + "\n";

            torusInfo += std::to_string(x2) + "," + std::to_string(y2) + "," + std::to_string(z2) + "\n";
            torusInfo += std::to_string(x4) + "," + std::to_string(y4) + "," + std::to_string(z4) + "\n";
            torusInfo += std::to_string(x1) + "," + std::to_string(y1) + "," + std::to_string(z1) + "\n";

        }
    }
    return torusInfo;
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

