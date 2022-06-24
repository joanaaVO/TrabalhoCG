#include <stdlib.h>
#ifdef __LINUX__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
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
#include <tuple>
#include <tgmath.h>

#ifdef __linux__
#include <unistd.h>
#elif _WIN32
#include <direct.h>
#include <io.h>
#else
#endif

using namespace std;
using namespace tinyxml2;

#define _USE_MATH_DEFINES

struct Vertice {
    float x;
    float y;
    float z;
};

float v;
bool axis = true;
int totalFigures = 0, actualFigure = 0, drawMode = 0;
float fov, near, far;
double posx, posy, posz, lookx, looky, lookz;
float upx, upy, upz;

float camAlpha = M_PI/12, camBeta = M_PI/12;
float currCamAlpha = camAlpha, currCamBeta = camBeta;
float r;

vector<vector<Vertice>> figures;
vector<string> s_array;
vector<string> files_array;

/**
* Function that draws all the three axis (X in red, Y in green, Z in blue).
*/
void drawAxis() {
    glBegin(GL_LINES);
        // X axis in red
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(-100.0f, 0.0f, 0.0f);
        glVertex3f(100.0f, 0.0f, 0.0f);

        // Y axis in green
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, -100.0f, 0.0f);
        glVertex3f(0.0f, 100.0f, 0.0f);

        // Z axis in blue
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, -100.0f);
        glVertex3f(0.0f, 0.0f, 100.0f);
    glEnd();
}

/**
* Function that draws a triangle.
* @param VA (vertice)
* @param VB (vertice)
* @param VC (vertice)
*/
void drawTriangle(Vertice vA, Vertice vB, Vertice vC) {
    drawMode == 0 ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    int indice_figure = actualFigure;
    if (actualFigure == totalFigures)
        indice_figure -= 1;

    //If-clause that checks right-hand rule

    if (files_array[indice_figure] == "plane.3d" && posy <= 0){    
        glBegin(GL_TRIANGLES);
            glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vC.x, vC.y, vC.z);
            if (drawMode == 1)
                glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vB.x, vB.y, vB.z);
            if (drawMode == 1)
                glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vA.x, vA.y, vA.z);
        glEnd();         
    }
    else {
        glBegin(GL_TRIANGLES);
            glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vA.x, vA.y, vA.z);
            if (drawMode == 1)
                glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vB.x, vB.y, vB.z);
            if (drawMode == 1)
                glColor3ub(rand() % 255, rand() % 255, rand() % 255);
            glVertex3f(vC.x, vC.y, vC.z);
        glEnd();         
    }    
}

void changeSize(int w, int h) {
    int j = 0, z = 0;
    string temp;
    size_t next = 0, last = 0;

    for (int i = 0; i < s_array.size(); i++) {
        temp = s_array[i];
        while ((next = temp.find(',', last)) != string::npos && z < 2) {
            if (j == 0) {
                posx = std::stod(temp.substr(last, next - last));
            }
            if (j == 1) {
                posy = std::stod(temp.substr(last, next - last));
            }
            if (j == 3) {
                lookx = std::stod(temp.substr(last, next - last));
            }
            if (j == 4) {
                looky = std::stod(temp.substr(last, next - last));
            }
            if (j == 6) {
                upx = std::stof(temp.substr(last, next - last));
            }
            if (j == 7) {
                upy = std::stof(temp.substr(last, next - last));
            }
            if (j == 9) {
                fov = std::stof(temp.substr(last, next - last));
            }
            if (j == 10) {
                near = std::stof(temp.substr(last, next - last));
            }
            j++;
            z++;
            last = next + 1;
        }

        // Distance from camera to origin and its angles
        r = sqrt(pow(posx, 2) + pow(posy, 2) + pow(posz, 2));
        currCamBeta = (float) asin(posy/r);
        currCamAlpha = (float) asin(posx/(r*cosf(currCamBeta)));

        if (j == 2) {
            posz = std::stod(temp.substr(last));
        }
        if (j == 5) {
            lookz = std::stod(temp.substr(last));
        }
        if (j == 8) {
            upz = std::stof(temp.substr(last));
        }
        if (j == 11) {
            far = std::stof(temp.substr(last));
        }
        j++;
        z = 0;
        last = 0;
        next = 0;
    }

    // Prevent a divide by zero, when window is too short                   
    // (you cant make a window with zero width).                            
    if (h == 0)
        h = 1;

    // Compute window's aspect ratio                                        
    float ratio = w * 1.0 / h;

    // Set the projection matrix as current                                 
    glMatrixMode(GL_PROJECTION);

    // Load Identity Matrix                                                 
    glLoadIdentity();

    // Set the viewport to be the entire window                             
    glViewport(0, 0, w, h);

    // Set perspective                                                      
    gluPerspective(fov, ratio, near, far);
    //gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

    // Return to the model view matrix mode                                 
    glMatrixMode(GL_MODELVIEW);
}

void renderScene(void) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //if (posy < 0) v = -1.0f; else v = 1.0f;

    // Set the camera
    glLoadIdentity();
    gluLookAt(posx,posy,posz,lookx,looky,lookz,upx,upy,upz);

    // Draw the axis
    if (axis)
        drawAxis();

    // Draw the figure
    if (actualFigure != totalFigures)
        for (int i = 0; i + 2 <= figures[actualFigure].size(); i += 3)
            drawTriangle(figures[actualFigure][i], figures[actualFigure][i + 1], figures[actualFigure][i + 2]);
    else {
        int countFigures = 0;
        while (countFigures < totalFigures) {
            for (int i = 0; i + 2 <= figures[countFigures].size(); i += 3)
                drawTriangle(figures[countFigures][i], figures[countFigures][i + 1], figures[countFigures][i + 2]);
            countFigures++;
        }
    }

    // End of frame
    glutSwapBuffers();
}

void keysFunction(unsigned char key, int x, int y) {
    switch (key) {
        // Move the camera UP
        case 'W':
        case 'w':
            if (currCamBeta > 1.5) break; 
            currCamBeta += camBeta;
            posx = r * cosf(currCamBeta) * sinf(currCamAlpha);
            posy = r * sinf(currCamBeta);
            posz = r * cosf(currCamBeta) * cosf(currCamAlpha);
            break;
        // Move the camera DOWN
        case 'S':
        case 's':
            if (currCamBeta < -1.5) break;
            currCamBeta -= camBeta;
            posx = r * cosf(currCamBeta) * sinf(currCamAlpha);
            posy = r * sinf(currCamBeta);
            posz = r * cosf(currCamBeta) * cosf(currCamAlpha);
            break;
        // Move the camera LEFT
        case 'A':
        case 'a':
            currCamAlpha -= camAlpha;
            posx = r * cosf(currCamBeta) * sinf(currCamAlpha);
            posy = r * sinf(currCamBeta);
            posz = r * cosf(currCamBeta) * cosf(currCamAlpha);
            break;
        // Move the camera RIGHT
        case 'D':
        case 'd':
            currCamAlpha += camAlpha;
            posx = r * cosf(currCamBeta) * sinf(currCamAlpha);
            posy = r * sinf(currCamBeta);
            posz = r * cosf(currCamBeta) * cosf(currCamAlpha);
            break;
        // Move to next figure
        case 'E':
        case 'e':
            if (totalFigures != 1 && actualFigure != totalFigures)
                actualFigure += 1;
            break;
        // Move to previous figure
        case 'Q':
        case 'q':
            if (actualFigure != 0)
                actualFigure -= 1;
            break;
        // Active/Deactivate the three axis
        case 'Z':
        case 'z':
            axis ? axis = false : axis = true;
            break;
        // Change the draw mode (0- Skeleton figure, 1- Filled figure)
        case 'X':
        case 'x':
            drawMode == 0 ? drawMode = 1 : drawMode = 0;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

/**
* Function that reads a .xml file.
*/
int parseXML(char* fileName) {
    char tmp[256];

    getcwd(tmp, 256);
    string path(tmp);

    int found = path.find("build");
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());
    path = path + "src/MODELS/" + fileName;

    XMLDocument xmlDoc;
    xmlDoc.LoadFile(path.c_str());

    XMLNode* world = xmlDoc.FirstChild();
    if (world == nullptr) {
        cout << "Invalid XML File!";
        return 1;
    }
    cout << "Reading XML file...\n" << endl;

    XMLElement* camera = world->FirstChildElement("camera");
    XMLElement* element = camera->FirstChildElement("position");
    string x1,y1,z1;
    string positionSettings, lookAtSettings, upSettings, projectionSettings;

    const XMLAttribute* x = element->FindAttribute("x");
    const XMLAttribute* y = element->FindAttribute("y");
    const XMLAttribute* z = element->FindAttribute("z");
    x1 = x->Value();
    y1 = y->Value();
    z1 = z->Value();
    positionSettings = x1 + "," + y1 + "," + z1;

    element = camera->FirstChildElement("lookAt");
    const XMLAttribute* x2 = element->FindAttribute("x");
    const XMLAttribute* y2 = element->FindAttribute("y");
    const XMLAttribute* z2 = element->FindAttribute("z");
    x1 = x2->Value();
    y1 = y2->Value();
    z1 = z2->Value();
    lookAtSettings = x1 + "," + y1 + "," + z1;

    element = camera->FirstChildElement("up");
    const XMLAttribute* x3 = element->FindAttribute("x");
    const XMLAttribute* y3 = element->FindAttribute("y");
    const XMLAttribute* z3 = element->FindAttribute("z");
    x1 = x3->Value();
    y1 = y3->Value();
    z1 = z3->Value();
    upSettings = x1 + "," + y1 + "," + z1;

    element = camera->FirstChildElement("projection");
    const XMLAttribute* x4 = element->FindAttribute("fov");
    const XMLAttribute* y4 = element->FindAttribute("near");
    const XMLAttribute* z4 = element->FindAttribute("far");
    x1 = x4->Value();
    y1 = y4->Value();
    z1 = z4->Value();
    projectionSettings = x1 + "," + y1 + "," + z1;

    s_array.push_back(positionSettings);
    s_array.push_back(lookAtSettings);
    s_array.push_back(upSettings);
    s_array.push_back(projectionSettings);

    XMLElement* group = world->FirstChildElement("group");
    XMLElement* models = group->FirstChildElement("models");
    element = models->FirstChildElement();

    int i = 0;
    while (element != NULL) {
        const XMLAttribute* fileName = element->FindAttribute("file");
        x1 = fileName->Value();
        files_array.push_back(x1);
        element = element->NextSiblingElement();
        i++;
    }

    return 0;
}

/*
* Function that reads all .3d files.
*/
int parse3D() {
    char tmp[256];

    // Get the directory path
    getcwd(tmp, 256);
    string path(tmp);

    int found = path.find("build");
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    // Get all the points in a global vector
    totalFigures = files_array.size();
    for (int i = 0; i < totalFigures; i++) {
        vector<Vertice> vectorV = {};
        string newPath = path + "src/MODELS/" + files_array[i];
        
        ifstream indata;
        indata.open(newPath);
        if (!indata) {
            cout << "Invalid 3D file!";
            return 1;
        }
        cout << "Reading '" << files_array[i] << "'..." << endl;

        int figure_total_points, lineCounter = 1;
        indata >> figure_total_points;
        string line("");
        while (lineCounter <= figure_total_points) {
            indata >> line;

            // Parse one line of the .3d file
            int j = 0;
            float vertices[3];
            size_t last = 0, next = 0;
            while ((next = line.find(',', last)) != string::npos) {
                vertices[j++] = std::stof(line.substr(last, next - last));
                last = next + 1;
            }
            vertices[j] = std::stof(line.substr(last));

            Vertice newV = { vertices[0], vertices[1], vertices[2] };
            vectorV.insert(vectorV.end(), newV);
            lineCounter++;
        }

        // Insert the vector in the global vector
        figures.insert(figures.end(), vectorV);
        indata.close();
    }

    return 0;
}


int main(int argc, char **argv) {
    // Parse the .xml file
    const string cameraSettings, lookAtSettings, upSettings, projectionSettins;
    if (parseXML(argv[1])) {
        return 1;
    }

    // Parse the .3d files
    if (parse3D()) {
        return 1;
    }

    cout << "\nDrawing all the models..." << endl;
    // Init GLUT and the window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("ProjetoCG-Fase1");

    // Required callback registry 
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(keysFunction);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enter GLUT's main cycle
    glutMainLoop();

    return 0;
}
