/*
 *  GLSLMain.cpp
 *  guitarstorm
 *
 *  Created by Owen Pedrotti on 8/9/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <GLUT/GLUT.h>
#include <ft2build.h>
#include FT_FREETYPE_H

GLint v, f, p;

void drawQuad()
{
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

void loadShaders()
{
	char *vs,*fs;
	
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);	
	
	//vs = fopen("Sprite.vert");
	//fs = fopen("Sprite.frag");
	
	const char * vv = "void main(){gl_FrontColor = gl_Color; gl_Position = ftransform();}";
	const char * ff = "void main(){gl_FragColor = gl_Color;}";
	
	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);
	
	//free(vs);free(fs);
	
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
	
	loadShaders();
	
	glutDisplayFunc(renderScene);
	glutMainLoop();
	
	unloadShaders();
	
	return 0;
}
