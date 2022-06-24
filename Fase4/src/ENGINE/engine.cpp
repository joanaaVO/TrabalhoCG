#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

//#define GLEW_STATIC
#ifdef __LINUX__
#include <GL/glew.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include <string>
#include <GL/glut.h>
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
#include <IL/il.h>
#include "headers/Group.hpp"
#include "headers/Transformation.hpp"
#include "headers/catmull.hpp"

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

// Lights Structure
struct Light {
    string type;
    vector<float> p;
    vector<float> d;
    float cutoff;
};
vector<Light> all_lights; //It can contain up to 8 "light" items

bool axis = true;
bool curve = true;
int drawMode = 0;
float fov, near, far;
double posx, posy, posz, lookx, looky, lookz;
float upx, upy, upz;

float camAlpha = M_PI/12, camBeta = M_PI/12;
float currCamAlpha = camAlpha, currCamBeta = camBeta;
float r;
float all_size = 1.0f;

vector<string> s_array;
vector<Group> groupArray;
vector<float> v;
vector<float> n;
vector<float> t;
int ptr = 0;
GLuint buffs;
GLuint normals;
GLuint textures;
GLuint verticeCount = 0;
bool first = true;
string backgroundName;
GLuint backgroundTexture;
int BackGroundMode = 0;

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
* Function that loads given texture filepath.
*/
int loadTexture(string s){
    unsigned int t, tw, th;
    unsigned char* texData;
    GLuint teximage;
    // for each image �

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)s.c_str());

    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    // create a texture slot
    glGenTextures(1, &teximage);
    // bind the slot
    glBindTexture(GL_TEXTURE_2D, teximage);

    // define texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // send texture data to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return teximage;
}


/*
* Function that reads all .3d files.
*/
void parse3D(std::string files, Group g) {
    char tmp[256];

    // Get the directory path
    getcwd(tmp, 256);
    string path(tmp);

    int found = path.find("build");
    replace(path.begin(), path.end(), '\\', '/');
    path.erase(path.begin() + found, path.end());

    // Get all the points in a global vector
    string newPath = path + "src/MODELS/" + files;
        
    ifstream indata;
    indata.open(newPath);
        
    int figure_total_points, lineCounter = 1;
    indata >> figure_total_points;

    string line("");
    vector<float> figure;
    while (lineCounter <= figure_total_points) {

        // Parse one line of the .3d file (Draw Points)        
        indata >> line;

        int j = 0;
        float vertices[3];
        size_t last = 0, next = 0;
        while ((next = line.find(',', last)) != string::npos) {
            vertices[j++] = std::stof(line.substr(last, next - last));
            last = next + 1;
        }
        vertices[j] = std::stof(line.substr(last));

        // Parse one line of the .3d file (Normal Points)
        indata >> line;
        int n_j = 0;
        float n_vertices[3];
        size_t n_last = 0, n_next = 0;
        while ((n_next = line.find(',', n_last)) != string::npos) {
            n_vertices[n_j++] = std::stof(line.substr(n_last, n_next - n_last));
            n_last = n_next + 1;
        }
        n_vertices[n_j] = std::stof(line.substr(n_last));


        // Parse one line of the .3d file (texture Points)
        indata >> line;
        int t_j = 0;
        float t_vertices[3];
        size_t t_last = 0, t_next = 0;
        while ((t_next = line.find(',', t_last)) != string::npos) {
            t_vertices[t_j++] = std::stof(line.substr(t_last, t_next - t_last));
            t_last = t_next + 1;
        }
        t_vertices[t_j] = std::stof(line.substr(t_last));


        figure.push_back(vertices[0]);
        figure.push_back(vertices[1]);
        figure.push_back(vertices[2]);

        //Fill Draw Vector (VBO)
        v.push_back(vertices[0]);
        v.push_back(vertices[1]);
        v.push_back(vertices[2]);

        //Fill Normal Vector (VBO)
        n.push_back(n_vertices[0]);
        n.push_back(n_vertices[1]);
        n.push_back(n_vertices[2]);

        //Fill Texture Vector (VBO)
        t.push_back(t_vertices[0]);
        t.push_back(t_vertices[1]);
        //t.push_back(t_vertices[2]);

        lineCounter++;
    }
    

    g.addTotalFigures(figure);

    indata.close();
}

void drawBackground(void)
{
    float w = 0.0f;  // Width
    float h = 0.0f;  // Height

    // No transformations or Lights to be applied
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    // Calculate width and height
    w = (float)glutGet(GLUT_WINDOW_WIDTH);
    h = (float)glutGet(GLUT_WINDOW_HEIGHT);

    gluOrtho2D(0.0, (GLdouble)w, (GLdouble)h, 0.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);

    // Draw Plane
    glBegin(GL_QUADS);

    glColor3f(1.0f, 1.0f, 1.0f);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(0.0f, h, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(w, h, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(w, 0.0f, 0.0f);

    glEnd();


    // Return to default
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glBindTexture(GL_TEXTURE_2D, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
}

void loadBackground(void) {
    unsigned int t = 0;
    unsigned int tw = 0;
    unsigned int th = 0;
    unsigned char* texData;

    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

    ilGenImages(1, &t);
    ilBindImage(t);
    ilLoadImage((ILstring)backgroundName.c_str());

    tw = ilGetInteger(IL_IMAGE_WIDTH);
    th = ilGetInteger(IL_IMAGE_HEIGHT);

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    texData = ilGetData();

    glGenTextures(1, &backgroundTexture);

    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

/**
* Function that draws all figures represented in a group with all the right characteristics (color, rotate, translate and scale).
* @param g (Group)
*/
void drawFigures(Group g) {
    std::vector<std::string> files = g.getFile();
    std::vector<Transformation> transformation = g.getTransf();
    std::vector<Group> subgroup = g.getSubGroup();

    bool hasColor = false;

    glPushMatrix();
    glPushAttrib(GL_LIGHTING_BIT);

    for (Transformation transf : transformation){
        std::vector<float> coords = transf.getCoordinates();

        //Translate
        if (transf.getName().compare("translate") == 0) {
            //Normal
            if (transf.getTime() == -1) {
                glTranslatef(coords[0], coords[1], coords[2]);
            }
            //Dynamic
            else {
                glPushMatrix();

                if (curve)
                    renderCatmullRomCurve(coords);
                
                glPopMatrix();
                
                float pos[3];
                float deriv[3];

                float t = ((float) glutGet(GLUT_ELAPSED_TIME) / 1000) / (float)(transf.getTime());
                getGlobalCatmullRomPoint(coords, t, (float*)pos, (float*) deriv);

                glTranslatef(pos[0], pos[1], pos[2]);
                
                if (transf.getAlign()){
                    float x[3] = {deriv[0], deriv[1], deriv[2]};
                    normalize(x);
                    float y0[3] = {0,1,0};
                    float z[3];
                    cross(x,y0,z);
                    normalize(z);
                    float y[3];
                    cross(z,x,y);
                    normalize(y);

                    float matrix[16];
                    buildRotMatrix(x,y,z, matrix);

                    glMultMatrixf(matrix);
                }
            }
        }

        //Scale
        else if (transf.getName().compare("scale") == 0) {
            glScalef(coords[0], coords[1], coords[2]);
        }

        //Rotate
        else if (transf.getName().compare("rotate") == 0) {
            //Normal
            if (transf.getAngle() != 0 && transf.getTime() == -1) {
                glRotatef(transf.getAngle(), coords[0], coords[1], coords[2]);
            }
            //Dynamic
            else {
                float angle = (((float)glutGet(GLUT_ELAPSED_TIME) / 1000) * 360) / (float)(transf.getTime());
                glRotatef(angle, coords[0], coords[1], coords[2]); 
            }
        }

        //Color
        else if (transf.getName().compare("color") == 0 && coords.capacity() != 0) {
            hasColor = true;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glColor3f(coords[0], coords[1], coords[2]);
        }

    }
 
    // Draw Figures
    if (files.capacity() != 0) {
        vector<vector<float>> figures = g.getTotalFigures();
        vector<float> light_color = g.getLightColor();
        GLfloat diffuse[4] = { light_color[0] /(float)255,  light_color[1] /(float)255, light_color[2] /(float)255, 1.0f };
        GLfloat ambient[4] = { light_color[3] /(float)255,  light_color[4] /(float)255, light_color[5] /(float)255, 1.0f };
        GLfloat specular[4] = { light_color[6] /(float)255,  light_color[7] /(float)255, light_color[8] /(float)255, 1.0f };
        GLfloat emissive[4] = { light_color[9] /(float)255,  light_color[10] /(float)255, light_color[11] /(float)255 , 1.0f };
        float shininess =(float) light_color[12];
    
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
        
        string texture_file = g.getTextureFile();
        
        int nVertices = figures[0].size() / 3;
        
        //VBOs 
        glBindBuffer(GL_ARRAY_BUFFER, buffs);
        glVertexPointer(3, GL_FLOAT, 0, 0);
        glBindBuffer(GL_ARRAY_BUFFER, normals);
        glNormalPointer(GL_FLOAT, 0, 0);

        //Textures
        if (texture_file.compare("") != 0) {
            glEnable(GL_TEXTURE_2D);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, textures); // PONTOS TEXTURA
            if (first) {
                GLuint texture = loadTexture(texture_file);
                g.setTexImage(texture);
            }
            glTexCoordPointer(2, GL_FLOAT, 0, 0);
            glBindTexture(GL_TEXTURE_2D, g.getTexImage()); // IMAGEM TEXTURA
        }

        else {
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisable(GL_TEXTURE_2D);
        }
        glDrawArrays(GL_TRIANGLES, ptr, nVertices);
        ptr += nVertices;
    }

    // Draw subGroups
    for (Group g : subgroup){
        drawFigures(g);
    }    

    glPopMatrix();
    glPopAttrib();
}

void renderScene(void) {
    int i = 0;
    GLfloat white[4] = { 0.8,0.8,0.8,1.0 };
    GLfloat dark[4] = { 0.2,0.2,0.2,1.0 };
     
    // Clear buffers
    glClearColor(0.0f,0.0f,0.0f,0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw Background
    if (BackGroundMode) {
        //glEnable(GL_TEXTURE_2D);
        drawBackground();
    }

    // Set the camera
    glLoadIdentity();
    gluLookAt(posx, posy, posz, lookx, looky, lookz, upx, upy, upz);    

    for(Light light : all_lights) {
       
        if (light.type == "point"){
            float pos[4] = {light.p[0], light.p[1], light.p[2], 1.0f};
            glLightfv(GL_LIGHT0+i, GL_POSITION, pos);
        }
        else if (light.type == "directional"){
            float dir[4] = {light.d[0], light.d[1], light.d[2], 0.0f};
            glLightfv(GL_LIGHT0+i, GL_POSITION, dir);
        }
        else {
            float pos[4] = {light.p[0], light.p[1], light.p[2], 1.0f};
            float dir[3] = {light.d[0], light.d[1], light.d[2] };
            glLightfv(GL_LIGHT0+i, GL_POSITION, pos);
            glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, dir);
            glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, light.cutoff);
            glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, 0.0); // Não sei se é preciso
        }
        glLightfv(GL_LIGHT0+i, GL_AMBIENT, white);
        glLightfv(GL_LIGHT0+i, GL_DIFFUSE, white);
        glLightfv(GL_LIGHT0+1, GL_SPECULAR, white);
        glEnable(GL_LIGHT0+i);
        i++;
    }
    
    // Zoom in and Zoom out
    glScalef(all_size, all_size, all_size);

    // Draw the axis
    if (axis)
        drawAxis();
    
    for(Group g : groupArray)
        drawFigures(g);   

    first = false;
       
    ptr = 0; 

    // End of frame
    glutSwapBuffers();
}

void keysFunction(unsigned char key, int x, int y) {
    switch (key) {
        // Move the camera UP
        case 'W':
        case 'w':
            if (currCamBeta > 1.3) break;
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

         // Enable/Disable the curve
        case 'H':
        case 'h':
            curve ? curve = false : curve = true;
            break;

        // Enable/Disable the three axis
        case 'Z':
        case 'z':
            axis ? axis = false : axis = true;
            break;
        
        // Turn off the background (0 - OFF, 1 - ON)
        case 'X':
        case 'x':
            BackGroundMode == 0 ? BackGroundMode = 1 : BackGroundMode = 0;
            break;
        

        default:
            break;
    }

    glutPostRedisplay();
}

void specialKeys(int key_code, int x, int y){
	switch(key_code) {
        // Zoom in
		case GLUT_KEY_UP:
			if (all_size < 5)
                all_size += 0.1f;
			break;
        // Zoom out
		case GLUT_KEY_DOWN:
            if (all_size > 0)
			    all_size += -0.1f;
			break;
	}

	glutPostRedisplay();
}

/*
* Function that parses lights
*/
void parseLights(XMLElement* lights){
    Light li;
    XMLElement* element = lights->FirstChildElement();
    const char* type;

    while(element){
        type = element->Attribute("type");
        if(strcmp(type, "point") == 0){
            li.type = type;
            float x, y, z;
            element->QueryFloatAttribute("posx", &x);
            element->QueryFloatAttribute("posy", &y);
            element->QueryFloatAttribute("posz", &z);
            li.p.push_back(x);
            li.p.push_back(y);
            li.p.push_back(z);
        }
        else if(strcmp(type, "directional") == 0){
            li.type = type;
            float x, y, z;
            element->QueryFloatAttribute("dirx", &x);
            element->QueryFloatAttribute("diry", &y);
            element->QueryFloatAttribute("dirz", &z);
            li.d.push_back(x);
            li.d.push_back(y);
            li.d.push_back(z);
        }
        else if(strcmp(type, "spot") == 0){
            li.type = type;
            float x, y, z, dx, dy, dz;
            float cutoff;
            element->QueryFloatAttribute("posx", &x);
            element->QueryFloatAttribute("posy", &y);
            element->QueryFloatAttribute("posz", &z);
            li.p.push_back(x);
            li.p.push_back(y);
            li.p.push_back(z);

            element->QueryFloatAttribute("dirx", &dx);
            element->QueryFloatAttribute("diry", &dy);
            element->QueryFloatAttribute("dirz", &dz);
            li.d.push_back(dx);
            li.d.push_back(dy);
            li.d.push_back(dz);

            element->QueryFloatAttribute("cutoff", &cutoff);
            li.cutoff = cutoff;
        }
        all_lights.push_back(li);
        li.p.clear();
        li.d.clear();

        element = element->NextSiblingElement();
    }
}

/*
* Function that parses all groups.
*/
Group parseGroup(XMLElement* group) {
    Group gr;
    float x, y, z, angle, time = -1;
    const char* align; //usar const char* se nao funcionar
    XMLElement* element = group->FirstChildElement();
    XMLElement* element_aux;

    while (element) {
        if (strcmp(element->Name(), "transform") == 0) {
            XMLElement* transform = element->FirstChildElement();
            while(transform){
                Transformation t;
                if (strcmp(transform->Name(), "translate") == 0) {
                    t.setName("translate");
                    transform->QueryFloatAttribute("time", &time);
                    if (time != -1) { // Se o time existir temos de usar bezier, entao temos de ler varios pontos
                        t.setTime(time);
                        align = transform->Attribute("align");
                        if(strcmp(align, "True") == 0)
                            t.setAlign(true);
                        else
                            t.setAlign(false); 
                        element_aux = transform->FirstChildElement();
                        string a = element_aux->Name();
                        while (element_aux) {
                            element_aux->QueryFloatAttribute("x", &x);
                            element_aux->QueryFloatAttribute("y", &y);
                            element_aux->QueryFloatAttribute("z", &z);
                            std::vector<float> translate;
                            translate.push_back(x);
                            translate.push_back(y);
                            translate.push_back(z);
                            t.addCoordinates(translate);
                            element_aux = element_aux->NextSiblingElement();
                        }
                        gr.addTransformation(t);
                    }
                    else {
                        transform->QueryFloatAttribute("x", &x);
                        transform->QueryFloatAttribute("y", &y);
                        transform->QueryFloatAttribute("z", &z);
                        std::vector<float> translate;
                        translate.push_back(x);
                        translate.push_back(y);
                        translate.push_back(z);
                        t.addCoordinates(translate);
                        gr.addTransformation(t);
                    }
                }
                else if (strcmp(transform->Name(), "color") == 0) {
                    t.setName("color");
                    transform->QueryFloatAttribute("r", &x);
                    transform->QueryFloatAttribute("g", &y);
                    transform->QueryFloatAttribute("b", &z);
                    std::vector<float> color;
                    color.push_back(x);
                    color.push_back(y);
                    color.push_back(z);
                    t.addCoordinates(color);
                    gr.addTransformation(t);
                }
                else if (strcmp(transform->Name(), "scale") == 0) {
                    t.setName("scale");
                    transform->QueryFloatAttribute("x", &x);
                    transform->QueryFloatAttribute("y", &y);
                    transform->QueryFloatAttribute("z", &z);
                    std::vector<float> scale;
                    scale.push_back(x);
                    scale.push_back(y);
                    scale.push_back(z);
                    t.addCoordinates(scale);
                    gr.addTransformation(t);
                }
                else if (strcmp(transform->Name(), "rotate") == 0) {
                    t.setName("rotate");
                    transform->QueryFloatAttribute("time", &time);
                    transform->QueryFloatAttribute("angle", &angle);
                    transform->QueryFloatAttribute("x", &x);
                    transform->QueryFloatAttribute("y", &y);
                    transform->QueryFloatAttribute("z", &z);
                    std::vector<float> rotate;
                    rotate.push_back(x);
                    rotate.push_back(y);
                    rotate.push_back(z);
                    t.addCoordinates(rotate);
                    t.setAngle(angle);
                    t.setTime(time);
                    gr.addTransformation(t);
                }
                transform = transform->NextSiblingElement();
                time = -1;
            }
        }
        else if (strcmp(element->Name(), "models") == 0) {
            XMLElement* file = element->FirstChildElement("model");
            //while (file != NULL) {
            if(file != NULL){
                const XMLAttribute* fileName = file->FindAttribute("file");
                string fileN = fileName->Value();
                parse3D(fileN, gr);
                gr.addFile(fileN);
                //file = file->NextSiblingElement();
            }

            XMLElement* texture = file->FirstChildElement("texture");
            if (texture) {
                const XMLAttribute* textureFile = texture->FindAttribute("file");
                string texture_file = textureFile->Value();
                char tmp[256];
                getcwd(tmp, 256);
                string path(tmp);
                int found = path.find("build");
                replace(path.begin(), path.end(), '\\', '/');
                path.erase(path.begin() + found, path.end());
                path = path + "src/MODELS/" + texture_file;
                gr.setTextureFile(path);
            }
            
            vector<float> light_colors;
            float r, g, b, shininess = 0;
            XMLElement* colors = file->FirstChildElement("color");
            if (colors != NULL) {
                XMLElement* color_child = colors->FirstChildElement();
                while (color_child != NULL) {
                    if (strcmp(color_child->Name(), "shininess") == 0) {
                        color_child->QueryFloatAttribute("value", &shininess);
                        light_colors.push_back(shininess);
                    }
                    else {
                        color_child->QueryFloatAttribute("R", &r);
                        color_child->QueryFloatAttribute("G", &g);
                        color_child->QueryFloatAttribute("B", &b);
                        light_colors.push_back(r);
                        light_colors.push_back(g);
                        light_colors.push_back(b);
                    }
                    color_child = color_child->NextSiblingElement();
                }
            }
            else {
                light_colors = { 200, 200, 200, 50, 50, 50, 0, 0, 0, 0, 0, 0, 0 };
            }
            gr.setLightColor(light_colors);

        }
        else if (strcmp(element->Name(), "group") == 0) {
            gr.addGroup(parseGroup(element));
        }
        element = element->NextSiblingElement();
    }
    return gr;
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
    string original_path = path;
    path = path + "src/MODELS/" + fileName;

    cout << path << endl;
    XMLDocument xmlDoc;
    xmlDoc.LoadFile(path.c_str());

    XMLNode* world = xmlDoc.FirstChild();
    if (world == nullptr) {
        cout << "Invalid XML File!" << endl;
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

    XMLElement* lights = world->FirstChildElement("lights");
    parseLights(lights);
    
    XMLElement* group = world->FirstChildElement("group");
    while (group) {
        string a = group->Name();
        groupArray.push_back(parseGroup(group));
        group = group->NextSiblingElement();
    }
    backgroundName = original_path + "src/MODELS/" + "background.jpg";
    cout << "XML Done...\n" << endl;
    return 0;
}

void prepareData(){
     // VBOs
    glGenBuffers(1, &buffs);
    glBindBuffer(GL_ARRAY_BUFFER, buffs);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * v.size(), &v[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normals);
    glBindBuffer(GL_ARRAY_BUFFER, normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * n.size(), &n[0], GL_STATIC_DRAW);

    glGenBuffers(1, &textures);
    glBindBuffer(GL_ARRAY_BUFFER, textures);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * t.size(), &t[0], GL_STATIC_DRAW);
}

void initGL() {

// OpenGL settings 
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	// enable light
    glEnable(GL_LIGHTING);
    glEnable(GL_RESCALE_NORMAL);
    glEnable(GL_TEXTURE_2D);
    
// init
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    ilInit();
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_UPPER_LEFT);

    loadBackground();

    float amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);

    prepareData();
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
    glutCreateWindow("ProjetoCG-Fase3");

    // Required callback registry 
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    // Callback registration for keyboard processing
    glutKeyboardFunc(keysFunction);
    glutSpecialFunc(specialKeys);

    #ifndef __APPLE__	
    // init GLEW
        glewInit();
    #endif

    initGL();   
    
    glutMainLoop();

    return 0;
}
