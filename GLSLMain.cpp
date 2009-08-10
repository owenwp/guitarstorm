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
#include <ft2build.h>
#include FT_FREETYPE_H

GLint v, f, p; 
GLuint t;

using namespace std;

void drawQuad()
{
	glEnable(GL_TEXTURE_2D);
	glEnable (GL_BLEND);
	
	glBindTexture(GL_TEXTURE_2D, t);
	
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor3f(1.0, 0.0, 0.0);
	
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
	
	drawQuad();
	
	glFlush();
}

void makeTexture()
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
	
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 16, 16, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
	
	free(tex);
}

void openShader(GLint shader, const char* name)
{
	ifstream in;
	in.open(name, ios::in);
	
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
	glUseProgram(p);
}

void unloadShaders()
{
	glDetachShader(p, v);
	glDetachShader(p, f);
	
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Guitar Storm GLSL Test");
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	makeTexture();
	
	loadShaders();
	
	glutDisplayFunc(renderScene);
	glutMainLoop();
	
	unloadShaders();
	
	return 0;
}
