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
GLuint t;

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
	unsigned char* tex = (unsigned char*)malloc(32 * 32);
	
	for(int i=0; i<32; i++)
	for(int j=0; j<32; j++)
	{
		int x = i-16;
		int y = j-16;
		int dist2 = x*x + y*y;
		
		if(dist2 < 14*14)
		{
			tex[i+32*j] = 0xff;
		}
		else
		{
			tex[i+32*j] = 0x00;
		}
	}
	
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 32, 32, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
	
	free(tex);
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
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	makeTexture();
	
	//loadShaders();
	
	glutDisplayFunc(renderScene);
	glutMainLoop();
	
	//unloadShaders();
	
	return 0;
}
