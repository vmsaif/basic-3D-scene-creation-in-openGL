
/*
    Author: Saif Mahmud
    Date : 5 / 3 / 2023

    Purpose: Creating a scene that has 
        1. a textured background;
        2. five 3D composite objects, each made up of at least five geometric shapes;
        3. two light sources with different lighting colours;
        4. an atmospheric attenuation effect - fog.

    Also have used complex display lists for efficient rendering.
*/

#include <iostream>
#include <GL/glut.h>
#include <fstream>
#include "windows.h"
#include <string>
#include "math.h"
#include "vector3.h"

#define SILVER 0
#define GOLD 1
#define BRONZE 2
#define EMERALD 3
#define RUBY 4
#define PEARL 5

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define FRONT 4
#define BACK 5

using namespace std;

// viewer
vector3 viewer(7.0, 7.0, 7.0);

// Light properties
GLfloat light0_position[] = { -30.0, 10.0, 20.0, 0.0 };
GLfloat light1_position[] = { 30.0, 10.0, 20.0, 0.0 };


//light 0
//white light
GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };

//light 1
//sunlight
GLfloat sunlight_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat sunlight_diffuse[] = { 1.0, 0.95, 0.8, 1.0 };
GLfloat sunlight_specular[] = { 1.0, 0.95, 0.8, 1.0 };

GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

// silver color
GLfloat silver_ambient[] = { 0.19225, 0.19225, 0.19225, 1.0 };
GLfloat silver_diffuse[] = { 0.50754, 0.50754, 0.50754, 1.0 };
GLfloat silver_specular[] = { 0.508273, 0.508273, 0.508273, 1.0 };
GLfloat silver_shininess[] = { 51.2 };

// gold color
GLfloat gold_ambient[] = { 0.24725, 0.1995, 0.0745, 1.0 };
GLfloat gold_diffuse[] = { 0.75164, 0.60648, 0.22658, 1.0 };
GLfloat gold_specular[] = { 0.628281, 0.555802, 0.366065, 1.0 };
GLfloat gold_shininess[] = { 51.2 };

// emerald color
GLfloat emerald_ambient[] = { 0.0215, 0.1745, 0.0215, 1.0 };
GLfloat emerald_diffuse[] = { 0.07568, 0.61424, 0.07568, 1.0 };
GLfloat emerald_specular[] = { 0.633, 0.727811, 0.633, 1.0 };
GLfloat emerald_shininess[] = { 76.8 };

// ruby color
GLfloat ruby_ambient[] = { 0.1745, 0.01175, 0.01175, 1.0 };
GLfloat ruby_diffuse[] = { 0.61424, 0.04136, 0.04136, 1.0 };
GLfloat ruby_specular[] = { 0.727811, 0.626959, 0.626959, 1.0 };
GLfloat ruby_shininess[] = { 76.8 };

// pearl color
GLfloat pearl_ambient[] = { 0.25, 0.20725, 0.20725, 1.0 };
GLfloat pearl_diffuse[] = { 1.0, 0.829, 0.829, 1.0 };
GLfloat pearl_specular[] = { 0.296648, 0.296648, 0.296648, 1.0 };
GLfloat pearl_shininess[] = { 11.264 };

// polished bronze
GLfloat bronze_ambient[] = { 0.25, 0.148, 0.06475, 1.0 };
GLfloat bronze_diffuse[] = { 0.4, 0.2368, 0.1036, 1.0 };
GLfloat bronze_specular[] = { 0.774597, 0.458561, 0.200621, 1.0 };
GLfloat bronze_shininess[] = { 76.8 };

typedef GLdouble vertex3[3];

// defining the vertices of the ground
vertex3 back_left = { -8.0, -0.1, -8.0 };
vertex3 back_right = { 8.0, -0.1, -8.0 };
vertex3 front_left = { -8.0, -0.1, 8.0 };
vertex3 front_right = { 8.0, -0.1, 8.0 };

// defining the vertices of the height from the ground for the background texture
vertex3 front_left_height = { -8.0, 10.0, 8.0 };
vertex3 front_right_height = { 8.0, 10.0, 8.0 };
vertex3 back_left_height = { -8.0, 10.0, -8.0 };
vertex3 back_right_height = { 8.0, 10.0, -8.0 };

// image
GLubyte* image;
GLubyte* l_texture;
BITMAPFILEHEADER fileheader;
BITMAPINFOHEADER infoheader;
RGBTRIPLE rgb;
GLuint texName;


// defining the vertices of the cube
vertex3 wall_pt[8] = {
    {0,0,0}, //0
    {0,1,0}, //1
    {1,0,0}, //2
    {1,1,0}, //3
    {0,0,1}, //4
    {0,1,1}, //5
    {1,0,1}, //6
    {1,1,1}  //7
};

// defining the vertices of the floor
vertex3 floor_pt[4] = {
    {-0.75, 0, -0.75}, //back left
    {-0.75, 0, 1.75}, //front left
    {1.75, 0, 1.75}, // front right
    {1.75, 0, -0.75} // back right
};

// defining the vertices of the top floor which will be used to draw the roof triangles
vertex3 top_floor_pt[4] = {
    {-0.2, 1.0, -0.2}, //back left
    {-0.2, 1.0, 1.2}, //front left
    {1.2, 1.0, 1.2}, // front right
    {1.2, 1.0, -0.2} // back right
};

GLuint houseList, carList, treeList, rocketList, benchList;

// This function is responsible for drawing the background texture
void drawBackgroundTexture() {

    glEnable(GL_TEXTURE_2D); // enable texture mapping
    // drawing the landscape texture.
    glBindTexture(GL_TEXTURE_2D, texName);

    // left half of the background
    glBegin(GL_QUADS);

    glTexCoord2d(0.0, 0.0); // bottom left
    glVertex3dv(front_left); // frontLeft of the ground

    glTexCoord2d(0.5, 0.0); // bottom right
    glVertex3dv(back_left); // backLeft of the ground

    glTexCoord2d(0.5, 1.0); // top right
    glVertex3dv(back_left_height);

    glTexCoord2d(0.0, 1.0); // top left
    glVertex3dv(front_left_height);

    glEnd();


    // right half of the background
    glBegin(GL_QUADS);

    glTexCoord2d(0.5, 0.0); // bottom left
    glVertex3dv(back_left); // backLeft of the ground

    glTexCoord2d(1.0, 0.0); // bottom right
    glVertex3dv(back_right); // backRight of the ground

    glTexCoord2d(1.0, 1.0); // top right
    glVertex3dv(back_right_height); // X, Z at backRight of the ground

    glTexCoord2d(0.5, 1.0); // top left
    glVertex3dv(back_left_height); // X, Z at backLeft of the ground

    glEnd();

    glDisable(GL_TEXTURE_2D);
}


// This function is responsible for drawing the main landscape
void drawLand() {
    // draw the town land
    
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3dv(back_left); // backLeft
    glVertex3dv(front_left); // frontLeft
    glVertex3dv(front_right); // frontRight
    glVertex3dv(back_right); // backRight
    glEnd();
    
}

// Loads the texture from a bitmap file
void makeImage(void) {

    int i, j = 0;
    FILE* l_file;
    string fn = "bg.bmp";

    const char* filename = fn.c_str();
    // open image file, return if error

    fopen_s(&l_file, filename, "rb");

    if (l_file == NULL) return;

    // read file header and header info

    fread(&fileheader, sizeof(fileheader), 1, l_file);
    fseek(l_file, sizeof(fileheader), SEEK_SET);
    fread(&infoheader, sizeof(infoheader), 1, l_file);

    // allocate space for the image file

    l_texture = (GLubyte*)malloc(infoheader.biWidth * infoheader.biHeight * 4);
    memset(l_texture, 0, infoheader.biWidth * infoheader.biHeight * 4);

    // read da data

    j = 0;

    for (i = 0; i < infoheader.biWidth * infoheader.biHeight; i++)
    {
        fread(&rgb, sizeof(rgb), 1, l_file);
        l_texture[j + 0] = (GLubyte)rgb.rgbtRed; // Red component
        l_texture[j + 1] = (GLubyte)rgb.rgbtGreen; // Green component
        l_texture[j + 2] = (GLubyte)rgb.rgbtBlue; // Blue component
        l_texture[j + 3] = (GLubyte)255; // Alpha value
        j += 4; // Go to the next position
    }

    fclose(l_file); // Closes the file stream
}

// Setting the light model, light position, and light color
void setLight() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);

    
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, sunlight_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, sunlight_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, sunlight_specular);

    // set light model 
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
}

// This function is responsible for loading the texture and setting the texture parameters
void loadTexture() {
    // texture loading
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, infoheader.biWidth, infoheader.biHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, l_texture);
}

// This function is responsible for setting the fog over the house area on the scene
void initializeFog() {
    GLfloat fogColor[] = { 0.7, 0.7, 0.9, 1.0 }; // pale blue 
    GLfloat fogDensity = 0.04;

    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP2);
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, fogDensity);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    glFogf(GL_FOG_START, 0.0);
    glFogf(GL_FOG_END, 5.0);
}

// This function is responsible for setting the material of the object
void setMaterial(int color) {

    if (color == SILVER) {
        glMaterialfv(GL_FRONT, GL_AMBIENT, silver_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, silver_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, silver_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, silver_shininess);
    }
    else if (color == GOLD)
    {
        glMaterialfv(GL_FRONT, GL_AMBIENT, gold_ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, gold_diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, gold_specular);
        glMaterialfv(GL_FRONT, GL_SHININESS, gold_shininess);
    }
    else if (color == BRONZE) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, bronze_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, bronze_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, bronze_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, bronze_shininess);
	}
    else if (color == EMERALD) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, emerald_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, emerald_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, emerald_shininess);
	}
    else if (color == RUBY) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, ruby_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, ruby_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, ruby_shininess);
	}
    else if (color == PEARL) {
		glMaterialfv(GL_FRONT, GL_AMBIENT, pearl_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, pearl_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, pearl_shininess);
    }
    else {
        glMaterialfv(GL_FRONT, GL_AMBIENT, white_light);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, white_light);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white_light);
        glMaterialfv(GL_FRONT, GL_SHININESS, white_light);
    }
}

// This function is responsible for drawing the top cone of the house
void top_hat() {
    GLdouble hatTopPoint[3] = { 0.5, 1.5, 0.5 }; // the point where all the triangle's tops will meet

    setMaterial(GOLD);

    glBegin(GL_TRIANGLES);

    // triangle 1
    glNormal3f(-1, 0, 0);
    glVertex3dv(top_floor_pt[0]);
    glVertex3dv(top_floor_pt[1]);
    glVertex3dv(hatTopPoint);

    // triangle 2
    glNormal3f(0, 0, -1);
    glVertex3dv(top_floor_pt[1]);
    glVertex3dv(top_floor_pt[2]);
    glVertex3dv(hatTopPoint);

    // triangle 3
    glNormal3f(1, 0, 0);
    glVertex3dv(top_floor_pt[2]);
    glVertex3dv(top_floor_pt[3]);
    glVertex3dv(hatTopPoint);

    // triangle 4
    glNormal3f(0, 0, 1);
    glVertex3dv(top_floor_pt[3]);
    glVertex3dv(top_floor_pt[0]);
    glVertex3dv(hatTopPoint);

    glEnd();

}


/**
Description:This function is responsible for drawing the floor
of the house. It takes four vertices as input and uses them
to draw a floor.
*/
void floor() {
    setMaterial(EMERALD);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glVertex3dv(floor_pt[0]);
    glVertex3dv(floor_pt[1]);
    glVertex3dv(floor_pt[2]);
    glVertex3dv(floor_pt[3]);
    glEnd();
}

/**
Description: This function takes four vertices as input
and draws a wall using these vertices.
It is called by the allWalls() function to draw all the walls of the house.
*/
void wall(GLint n1, GLint n2, GLint n3, GLint n4, int face) {

    glBegin(GL_QUADS);

    if (face == RIGHT) {
        // Right face
        glNormal3f(1, 0, 0);
    }
    else if (face == LEFT) {
        // Left face
        glNormal3f(-1, 0, 0);
    }
    else if (face == TOP) {
        // Top face
        glNormal3f(0, 1, 0);
    }
    else if (face == BOTTOM) {
        // Bottom face
        glNormal3f(0, -1, 0);
    }
    else if (face == BACK) {
		// Back face
		glNormal3f(0, 0, 1);
    }
    else {
		// Front face
		glNormal3f(0, 0, -1);
	}

    glVertex3dv(wall_pt[n1]);
    glVertex3dv(wall_pt[n2]);
    glVertex3dv(wall_pt[n3]);
    glVertex3dv(wall_pt[n4]);
    glEnd();

}

/**
Description:This function calls the wall() function with the
appropriate vertices to draw all the walls of the house.
*/

void allWalls() {
    setMaterial(RUBY);
    wall(6, 2, 3, 7, RIGHT); //rightSideWall
    wall(5, 1, 0, 4, LEFT); //leftSideWall
    wall(7, 3, 1, 5, TOP); //topWall
    wall(4, 0, 2, 6, BOTTOM); //bottomWall
    wall(2, 0, 1, 3, BACK); //backWall
    wall(7, 5, 4, 6, FRONT); //frontWall
}

// draw the house
void drawHouse() {
	allWalls();
	floor();
	top_hat();
}

// draw the rocket
void drawRocket() {

    GLfloat rocketBodyStarts = 1.0;
    GLfloat rocketBodyHeight = 2.0;
    GLfloat tipHeight = 1.5;
    GLfloat rocketRadius = 0.5;
    GLfloat slices = 20;
    GLfloat stacks = 20;

    // Draw the rocket body (cylinder)
    GLUquadric* body = gluNewQuadric();
    setMaterial(RUBY); // Set the material color of the rocket body
    glPushMatrix();
    glTranslatef(0.0, rocketBodyStarts, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(body, rocketRadius, rocketRadius, rocketBodyHeight, slices, stacks);
    glPopMatrix();

    // Draw the rocket cone (tip)
    GLUquadric* cone = gluNewQuadric();
    setMaterial(GOLD); // Set the material color of the rocket cone
    glPushMatrix();
    glTranslatef(0.0, rocketBodyStarts + rocketBodyHeight, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(cone, rocketRadius, 0.0, tipHeight, slices, stacks);
    glPopMatrix();

    // Draw the rocket fins (triangles)
    setMaterial(BRONZE); // Set the material color of the rocket fins
    
    glBegin(GL_TRIANGLES);
        // Fin 1
        glVertex3f(-0.5, rocketBodyStarts, 0.0);
        glVertex3f(-1.5, 0.0, 0.0);
        glVertex3f(-0.5, 0.0, 0.0);

        // Fin 2
        glVertex3f(0.5, rocketBodyStarts, 0.0);
        glVertex3f(1.5, 0.0, 0.0);
        glVertex3f(0.5, 0.0, 0.0);

        // Fin 3
        glVertex3f(0.0, rocketBodyStarts, -0.5);
        glVertex3f(0.0, 0.0, -1.5);
        glVertex3f(0.0, 0.0, -0.5);

        // Fin 4
        glVertex3f(0.0, rocketBodyStarts, 0.5);
        glVertex3f(0.0, 0.0, 1.5);
        glVertex3f(0.0, 0.0, 0.5);
    glEnd();

    gluDeleteQuadric(body);
    gluDeleteQuadric(cone);
}

// draw a cube
void drawCube(GLfloat width, GLfloat height, GLfloat depth) {
    GLfloat w = width / 2;
    GLfloat h = height / 2;
    GLfloat d = depth / 2;

    glPushMatrix();
    glBegin(GL_QUADS);

    // Front face
    glNormal3f(0, 0, -1);
    glVertex3f(-w, -h, d);
    glVertex3f(w, -h, d);
    glVertex3f(w, h, d);
    glVertex3f(-w, h, d);

    // Back face
    glNormal3f(0, 0, 1);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, -h, -d);

    // Left face
    glNormal3f(-1, 0, 0);
    glVertex3f(-w, -h, -d);
    glVertex3f(-w, -h, d);
    glVertex3f(-w, h, d);
    glVertex3f(-w, h, -d);

    // Right face
    glNormal3f(1, 0, 0);
    glVertex3f(w, -h, -d);
    glVertex3f(w, h, -d);
    glVertex3f(w, h, d);
    glVertex3f(w, -h, d);

    // Top face
    glNormal3f(0, 1, 0);
    glVertex3f(-w, h, -d);
    glVertex3f(-w, h, d);
    glVertex3f(w, h, d);
    glVertex3f(w, h, -d);

    // Bottom face
    glNormal3f(0, -1, 0);
    glVertex3f(-w, -h, -d);
    glVertex3f(w, -h, -d);
    glVertex3f(w, -h, d);
    glVertex3f(-w, -h, d);

    glEnd();
    glPopMatrix();
}

// draw the car wheel
void drawWheel() {

    GLfloat wheelRadius = 0.15;
    GLfloat wheelHeight = 0.1;
    GLint slices = 20;
    GLint stacks = 20;
   
    // Draw the wheel
    setMaterial(SILVER);

    glPushMatrix();
    glTranslatef(0.0, 0.0, wheelHeight / 2);
    glutSolidSphere(wheelRadius, slices, stacks);
    glPopMatrix();

}

// draw the car
void drawCar() {
    
    // Car body
    setMaterial(RUBY);
    glPushMatrix();
    glTranslatef(0.0, 0.25, 0.0);
    drawCube(1.75, 0.45, 1.0);
    glPopMatrix();

    // Car roof
    setMaterial(PEARL);
    glColor3f(0.0, 0.0, 0.5);
    glPushMatrix();
    glTranslatef(0.0, 0.6, 0.0);
    drawCube(1.0, 0.3, 0.8);
    glPopMatrix();

    // Wheels
    glPushMatrix();
    glTranslatef(-0.5, 0.1, -0.5);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.5, 0.1, 0.5);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5, 0.1, -0.5);
    drawWheel();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5, 0.1, 0.5);
    drawWheel();
    glPopMatrix();
}

// draw the tree
void drawTree() {
    
    GLdouble bodyHeight = 2.0;
    GLint slices = 20;
    GLint stacks = 20;
    // tree body (cylinder)
    setMaterial(BRONZE);
    GLUquadricObj* body = gluNewQuadric();
    glPushMatrix();
    glTranslatef(0.0, 0.0, 0.0);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(body, 0.2, 0.2, bodyHeight, slices, stacks);
    glPopMatrix();

    gluDeleteQuadric(body);

    // tree foliages (spheres)
    
    setMaterial(EMERALD);

    // Center foliage (spheres)
    glPushMatrix();
    glTranslatef(0.0, bodyHeight, 0.0);
    glutSolidSphere(0.7, slices, stacks);
    glPopMatrix();

    // Second foliage (spheres)
    glPushMatrix();
    glTranslatef(0.0, bodyHeight + 0.5, 0.0);
    glutSolidSphere(0.5, slices, stacks);
    glPopMatrix();

    // Third foliage (sphere)
    glPushMatrix();
    glTranslatef(0.5, bodyHeight + 0.2, 0.0);
    glutSolidSphere(0.5, slices, stacks);
    glPopMatrix();

    // Fourth foliage (sphere)
    glPushMatrix();
    glTranslatef(-0.5, bodyHeight + 0.2, 0.0);
    glutSolidSphere(0.5, slices, stacks);
    glPopMatrix();
}


void drawBench() {
    
    GLfloat legRadius = 0.05;
    GLfloat legHeight = 0.75;
    GLint slices = 20;
    GLint stacks = 20;
    setMaterial(BRONZE);

    // Seat (cube)
    glPushMatrix();
    glTranslatef(0.0, 0.85, 0.0);
    drawCube(2.0, 0.2, 0.5);
    glPopMatrix();

    // Legs (cylinders)
    GLUquadricObj* leg = gluNewQuadric();

    glPushMatrix();
    glTranslatef(-1.0, 0.1, 0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(leg, legRadius, legRadius, legHeight, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.1, 0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(leg, legRadius, legRadius, legHeight, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-1.0, 0.1, -0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(leg, legRadius, legRadius, legHeight, slices, stacks);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.0, 0.1, -0.2);
    glRotatef(-90, 1.0, 0.0, 0.0);
    gluCylinder(leg, legRadius, legRadius, legHeight, slices, stacks);
    glPopMatrix();

}

// this function is responsible for initializing the display lists
void initDisplayLists() {
    
    houseList = glGenLists(1);
    glNewList(houseList, GL_COMPILE);
    drawHouse();
    glEndList();

    carList = glGenLists(1);
    glNewList(carList, GL_COMPILE);
    drawCar();
    glEndList();

    treeList = glGenLists(1);
    glNewList(treeList, GL_COMPILE);
    drawTree();
    glEndList();

    rocketList = glGenLists(1);
    glNewList(rocketList, GL_COMPILE);
    drawRocket();
    glEndList();

    benchList = glGenLists(1);
    glNewList(benchList, GL_COMPILE);
    drawBench();
    glEndList();

}

// renders the scene
void render() {

    // draw the background texture
    drawBackgroundTexture();

    // draw the land
    drawLand();

    // a house with 3 triangles as hat, 4 quads as walls, 1 quad as floors.
    glPushMatrix();
    glTranslatef(-5.0, 0.0, -5.0);
    glScaled(2.0, 2.0, 2.0);
    glCallList(houseList);
    glPopMatrix();

    // a rocket with 1 cylinder as body, 1 cylinder as top cone, 3 triangles as fins.
    glPushMatrix();
    glTranslatef(-5.0, 0.0, 1.0);
    glCallList(rocketList);
    glPopMatrix();

    // a car with 1 cube as body, 1 cube as roof, 4 spheres as wheels.
    glPushMatrix();
    glTranslatef(3.0, 1.0, -2.5);
    glScaled(1.5, 1.5, 1.5);
    glCallList(carList);
    glPopMatrix();

    // a tree with 4 spheres as foliages and a cylinder as body.
    glPushMatrix();
    glTranslatef(2.0, 0.0, 4.0);
    glCallList(treeList);
    glPopMatrix();
    
    // a bench with 1 cube as seat and 4 cylinders as legs.
    glPushMatrix();
    glTranslatef(2.5, 0.0, 5.5);
    glScaled(1.25, 1.25, 1.25);
    glCallList(benchList);
    glPopMatrix();
}


// initializing the program with setting the background color and enabling the depth test and lighting, Also setting the light model, light position, and light color
void initialize() {
    // set background color
    glClearColor(0.0, 0.0, 0.0, 1.0);

    // set the lights
    setLight();

    // get the image
    makeImage();

    // set the texture
    loadTexture();

    // initialize the display lists
    initDisplayLists();

    // set the fog
    initializeFog();
}

// display registry, clears the color and depth buffers, sets camera position and orientation and calls the render function 
void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); // reset the modelview matrix
    gluLookAt(viewer.x, viewer.y, viewer.z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // set the camera position and orientation
    render(); // render the scene
    glutSwapBuffers(); //Swap the front and back buffers
}

// reshape registry
// called when window is resized to change the viewport

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the client window area
    glMatrixMode(GL_PROJECTION); // set the projection matrix mode
    glLoadIdentity(); // reset the projection matri
    glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 200.0); // set the projection frustum
    glMatrixMode(GL_MODELVIEW); // set the modelview matrix mode
}


// main program 
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // set the display mode
    glutInitWindowSize(500, 500); //set display-window width and height
    glutInitWindowPosition(100, 100);

    int windowHandle = glutCreateWindow("Basic 3D Scene in OpenGL"); //create display window
    glutSetWindow(windowHandle);

    glutDisplayFunc(display); //call display function
    glutReshapeFunc(reshape); // call reshape function

    initialize(); // initialize OpenGL
    glutMainLoop(); //display everything and wait

    return EXIT_SUCCESS;

}