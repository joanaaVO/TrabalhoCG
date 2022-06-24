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
#include "Group.hpp"

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
float fov, near, far;
double posx, posy, posz, lookx, looky, lookz;
float upx, upy, upz;

float camAlpha = M_PI/12, camBeta = 0.1;
float currCamAlpha = camAlpha, currCamBeta = camBeta;
float r;
float all_size = 1.0f;

vector<string> s_array;
vector<Group> groupArray;

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
    glBegin(GL_TRIANGLES);
        glVertex3f(vA.x, vA.y, vA.z);
        glVertex3f(vB.x, vB.y, vB.z);
        glVertex3f(vC.x, vC.y, vC.z);
    glEnd();         
}

void changeSize(int w, int h) {
    int j = 0, z = 0;
    string temp;
    size_t next = 0, last = 0;

    for (int i = 0; i < s_array.size(); i++) {
        temp = s_array[i];
        while ((next = temp.find(',', last)) != string::npos && z < 2) {
            if (j == 0)
                posx = std::stod(temp.substr(last, next - last));
            if (j == 1)
                posy = std::stod(temp.substr(last, next - last));
            if (j == 3)
                lookx = std::stod(temp.substr(last, next - last));
            if (j == 4)
                looky = std::stod(temp.substr(last, next - last));
            if (j == 6)
                upx = std::stof(temp.substr(last, next - last));
            if (j == 7)
                upy = std::stof(temp.substr(last, next - last));
            if (j == 9)
                fov = std::stof(temp.substr(last, next - last));
            if (j == 10)
                near = std::stof(temp.substr(last, next - last));

            j++;
            z++;
            last = next + 1;
        }

        // Distance from camera to origin and its angles
        r = sqrt(pow(posx, 2) + pow(posy, 2) + pow(posz, 2));
        currCamBeta = (float) asin(posy / r);
        currCamAlpha = (float) asin(posx / (r * cosf(currCamBeta)));

        if (j == 2)
            posz = std::stod(temp.substr(last));
        if (j == 5)
            lookz = std::stod(temp.substr(last));
        if (j == 8)
            upz = std::stof(temp.substr(last));
        if (j == 11)
            far = std::stof(temp.substr(last));

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

/*
* Function that reads all .3d files.
*/
vector<vector<Vertice>> parse3D(vector<std::string> files) {
    vector<vector<Vertice>> figures;
    char tmp[256];

    // Get the directory path
    getcwd(tmp, 256);
    string path(tmp);

    int found = path.find("build");
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    // Get all the points in a global vector
    int totalFigures = files.size();
    for (int i = 0; i < totalFigures; i++) {
        vector<Vertice> vectorV = {};
        string newPath = path + "src/MODELS/" + files[i];
        
        ifstream indata;
        indata.open(newPath);
        
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
    return figures;
}

/**
* Function that draws all figures represented in a group with all the right characteristics (color, rotate, translate and scale).
* @param g (Group)
*/
void drawFigures(Group g) {
    std::vector<std::string> files = g.getFile();
    vector<float> color = g.getColor();
    vector<float> rotate = g.getRotate();
    vector<float> translate = g.getTranslate();
    vector<float> scale = g.getScale();
    std::vector<Group> subgroup = g.getSubGroup();

    glPushMatrix();

    // Color Figure
    if (color.capacity() != 0) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glColor3f(color[0], color[1], color[2]);
    }
    else
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Rotate
    if (rotate.capacity() != 0) 
        glRotatef(rotate[0], rotate[1], rotate[2], rotate[3]);

    // Translate
    if (translate.capacity() != 0) 
        glTranslatef(translate[0], translate[1], translate[2]);
    
    // Scale figure
    if (scale.capacity() != 0)
        glScalef(scale[0], scale[1], scale[2]);
    
    // Draw Figures
    if (files.capacity() != 0) {
        vector<vector<Vertice>> figures = parse3D(files);
        for (int j = 0; j < figures.size(); j++) {
            for (int i = 0; i + 2 <= figures[j].size(); i += 3)
                    drawTriangle(figures[j][i], figures[j][i + 1], figures[j][i + 2]);     
        }
    }

    // Draw subGroups
    for (Group g : subgroup)
        drawFigures(g);

    glPopMatrix();
}

void renderScene(void) {
    // Clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //if (posy < 0) v = -1.0f; else v = 1.0f;

    // Set the camera
    glLoadIdentity();
    gluLookAt(posx, posy, posz, lookx, looky, lookz, upx, upy, upz);

    // Zoom in and Zoom out
    glScalef(all_size, all_size, all_size);

    // Draw the axis
    if (axis)
        drawAxis();

    for(Group g : groupArray)
        drawFigures(g);

    // End of frame
    glutSwapBuffers();
}

void keysFunction(unsigned char key, int x, int y) {
    switch (key) {
        // Move the camera UP
        case 'W':
        case 'w':
            if (currCamBeta > 1.4) break;
            currCamBeta += camBeta;
            posx = r * cosf(currCamBeta) * sinf(currCamAlpha);
            posy = r * sinf(currCamBeta);
            posz = r * cosf(currCamBeta) * cosf(currCamAlpha);
            break;
        // Move the camera DOWN
        case 'S':
        case 's':
            if (currCamBeta < -1.4) break;
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

        // Move to next figure - DISABLED
        /*
        case 'E':
        case 'e':
            if (totalFigures != 1 && actualFigure != totalFigures)
                actualFigure += 1;
            break;
        */
        // Move to previous figure - DISABLED
        /*
        case 'Q':
        case 'q':
            if (actualFigure != 0)
                actualFigure -= 1;
            break;
        */

        // Enable/Disable the three axis
        case 'Z':
        case 'z':
            axis ? axis = false : axis = true;
            break;

        // Change the draw mode (0- Skeleton figure, 1- Filled figure) - DISABLED
        /*
        case 'X':
        case 'x':
            drawMode == 0 ? drawMode = 1 : drawMode = 0;
            break;
        */

        default:
            break;
    }

    glutPostRedisplay();
}

void specialKeys(int key_code, int x, int y){
	switch(key_code) {
        // Zoom in
		case GLUT_KEY_UP:
			all_size += 0.1f;
			break;
        // Zoom out
		case GLUT_KEY_DOWN:
			all_size += -0.1f;
			break;
	}

	glutPostRedisplay();
}

/*
* Function that parses all groups.
*/
Group parseGroup(XMLElement* group) {
    Group g;
    float x, y, z, angle;
    XMLElement* element = group->FirstChildElement();
    while (element) {
        if (strcmp(element->Name(), "transform") == 0) {
            XMLElement* transform = element->FirstChildElement();
            while(transform){
                if (strcmp(transform->Name(), "translate") == 0) {
                    transform->QueryFloatAttribute("x", &x);
                    transform->QueryFloatAttribute("y", &y);
                    transform->QueryFloatAttribute("z", &z);
                    vector<float> translate;
                    translate.push_back(x);
                    translate.push_back(y);
                    translate.push_back(z);
                    g.setTranslate(translate);
                }
                else if (strcmp(transform->Name(), "color") == 0) {
                    transform->QueryFloatAttribute("r", &x);
                    transform->QueryFloatAttribute("g", &y);
                    transform->QueryFloatAttribute("b", &z);
                    vector<float> color;
                    color.push_back(x);
                    color.push_back(y);
                    color.push_back(z);
                    g.setColor(color);
                }
                else if (strcmp(transform->Name(), "scale") == 0) {
                    transform->QueryFloatAttribute("x", &x);
                    transform->QueryFloatAttribute("y", &y);
                    transform->QueryFloatAttribute("z", &z);
                    vector<float> scale;
                    scale.push_back(x);
                    scale.push_back(y);
                    scale.push_back(z);
                    g.setScale(scale);
                }
                else if (strcmp(transform->Name(), "rotate") == 0) {
                    transform->QueryFloatAttribute("angle", &angle);
                    transform->QueryFloatAttribute("x", &x);
                    transform->QueryFloatAttribute("y", &y);
                    transform->QueryFloatAttribute("z", &z);
                    vector<float> rotate;
                    rotate.push_back(angle);
                    rotate.push_back(x);
                    rotate.push_back(y);
                    rotate.push_back(z);
                    g.setRotate(rotate);
                }
                transform = transform->NextSiblingElement();
            }
        }
        else if (strcmp(element->Name(), "models") == 0) {
            XMLElement* file = element->FirstChildElement("model");
            while (file != NULL) {
                const XMLAttribute* fileName = file->FindAttribute("file");
                string fileN = fileName->Value();
                g.addFile(fileN);
                file = file->NextSiblingElement();
            }
        }
        else if (strcmp(element->Name(), "group") == 0) {
            g.addGroup(parseGroup(element));
        }
        element = element->NextSiblingElement();
    }
    return g;
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
    while (group) {
        groupArray.push_back(parseGroup(group));
        group = group->NextSiblingElement();
    }

    return 0;
}

int main(int argc, char **argv) {
    // Parse the .xml file
    const string cameraSettings, lookAtSettings, upSettings, projectionSettins;
    if (parseXML(argv[1])) {
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
    glutSpecialFunc(specialKeys);

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE); 
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enter GLUT's main cycle
    glutMainLoop();

    return 0;
}
