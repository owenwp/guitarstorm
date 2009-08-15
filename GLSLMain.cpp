/*
 *  GLSLMain.cpp
 *  guitarstorm
 *
 *  Created by Owen Pedrotti on 8/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <GLUT/GLUT.h>
#include <IL/il.h>

#include "Texture.h"
#include "Node.h"
#include "Sprite.h"
#include "Label.h"

bool useShaders = true;

GLint v, f, p; 
GLuint circle;

Node* root;

using namespace std;

void drawQuad(float r=1, float g=1, float b=1)
{	
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor3f(r, g, b);
	
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-0.5, -0.5, 0.0);
	
	glTexCoord2f(1.0, 1.0);
	glVertex3f(0.5, -0.5, 0.0);
	
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-0.5, 0.5, 0.0);
	
	glTexCoord2f(1.0, 0.0);
	glVertex3f(0.5, 0.5, 0.0);
	
	glEnd();
}

void renderScene() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0, 0, -11);
	
	glPushMatrix();
	{
		glBindTexture(GL_TEXTURE_2D, circle);
		GLint loc = glGetUniformLocation(p,"edgeSize");
		glUniform1f(loc, 1.0);
		loc = glGetUniformLocation(p,"alphaOnly");
		glUniform1i(loc, 1);
		loc = glGetUniformLocation(p,"shadowAlpha");
		glUniform1f(loc, 0);
		
		glTranslatef(-3, 1, 0);
		glScalef(8, 8, 1);
		drawQuad(0, 1, 0);
	}
	glPopMatrix();
	
	GLint loc = glGetUniformLocation(p,"shadowAlpha");
	//glUniform1f(loc, 0.5);
	loc = glGetUniformLocation(p,"shadowPosition");
	//glUniform3f(loc, 1, -0.1, 1);
	root->update(1.0f);
	root->render(p);
	
	glutSwapBuffers();
}

void makeCircle()
{
	unsigned char* tex = (unsigned char*)malloc(16 * 16);
	
	for(int i=0; i<16; i++)
	for(int j=0; j<16; j++)
	{
		int x = i-8;
		int y = j-8;
		float dist2 = sqrt(x*x + y*y);
		
		dist2 -= 7;
		dist2 *= -0.1f;
		dist2 += 0.5f;
		dist2 = min(1.0f, max(0.0f, dist2));
		
		tex[i+16*j] = (unsigned char)(dist2 * 255.0f);
	}
	
	glGenTextures(1, &circle);
	glBindTexture(GL_TEXTURE_2D, circle);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 16, 16, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
	
	free(tex);
}

void openShader(GLint shader, string name)
{
	string folder = "shaders/";
	name = folder + name;
	ifstream in;
	in.open(name.c_str(), ios::in);
	
	char cstr[128];
	
	string str;
	
	do
	{
		in.get(cstr, 128, 0);
		str += string(cstr);
	}
	while(in.gcount() == 127);
	
	cout << endl << "*** " << name << " ***" << endl << str;
	
	const GLchar* c = str.c_str();
	
	glShaderSource(shader, 1, &c, NULL);
}

void loadShaders()
{
	char *vs,*fs;
	
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);	
	
	openShader(v,"Sprite.vert");
	openShader(f,"Sprite.frag");
	
	glCompileShader(v);
	glCompileShader(f);
	
	p = glCreateProgram();
	
	glAttachShader(p,v);
	glAttachShader(p,f);
	
	glLinkProgram(p);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)800 / (float)600, 1, 20);
	
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	
	glEnable(GL_TEXTURE_2D);
	
	if(!useShaders)
	{
		glAlphaFunc(GL_GREATER,0.5f);
		glEnable(GL_ALPHA_TEST);
	}
	else
	{
		glEnable (GL_BLEND);
		glUseProgram(p);
	}
}

void unloadShaders()
{
	glDetachShader(p, v);
	glDetachShader(p, f);
	
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
}

void unloadTextures()
{
	/*for(unsigned char c=0; c<128; c++)
	{
		glDeleteTextures(1, &text[c]);
	}*/
	glDeleteTextures(1, &circle);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	ilInit();
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Guitar Storm GLSL Test");
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	loadShaders();
	
	root = new Node();
	
	Node* text = new Node();
	Label* l = new Label("arial", "Vector");
	l->setColor(vec(0,0,1));
	text->addChild(l);
	text->setPosition(vec(-4, -2));
	text->setScale(vec(2, 2));
	root->addChild(text);
	
	text = new Node();
	l = new Label("arial", "BIG");
	l->setColor(vec(1,0,0));
	text->addChild(l);
	text->setPosition(vec(-5, 1));
	text->setScale(vec(6, 6));
	root->addChild(text);
	
	text = new Node();
	l = new Label("arial", "small");
	l->setColor(vec(1,1,1));
	text->addChild(l);
	text->setPosition(vec(2, -4));
	text->setScale(vec(0.25, 0.25));
	root->addChild(text);
	
	Node* body = new Node();
	body->addChild(new Sprite("body"));
	body->setSpin(0.1f);
	body->setPosition(vec(10, 0));
	body->setScale(vec(30, 30));
	root->addChild(body);
	
	makeCircle();

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMainLoop();
	
	unloadShaders();
	unloadTextures();
	
	delete root;
	
	return 0;
}
