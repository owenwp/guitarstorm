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

Node* root;

using namespace std;

void renderScene() 
{	
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int time = glutGet(GLUT_ELAPSED_TIME);
	
	float timeDelta = (time - lastTime) * 0.001f;
	lastTime = time;
	
	if(!root)
		return;
	
	root->update(timeDelta);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0, 0, -11);
	
	root->render(p);
	
	glutSwapBuffers();
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

void deleteScene()
{
	delete root;
	root = NULL;
	
	Texture::UnloadAll();
	
	glDetachShader(p, v);
	glDetachShader(p, f);
	
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
	
	exit(0);
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
	l = new Label("arial", "Big");
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
	body->addChild(new Sprite(new Texture("body")));
	body->setSpin(10.0f);
	body->setPosition(vec(10, 0));
	body->setScale(vec(30, 30));
	root->addChild(body);
	
	Node* circle = new Node();
	circle->addChild(new Sprite(new Texture(shapeCircle), vec(0,1,0)));
	circle->setPosition(vec(-3, -1));
	circle->setScale(vec(8, 8));
	root->addChild(circle);

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutWMCloseFunc(deleteScene);
	
	glutMainLoop();
}
