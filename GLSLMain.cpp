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
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

#include "Texture.h"
#include "Node.h"
#include "Sprite.h"

bool useShaders = true;

GLint v, f, p; 
FT_Library ft;
FT_Face font;
GLuint circle;
GLuint text[128];
float kerning[128][128];
float wid[128];
float hgt[128];
float xpad[128];
float ypad[128];
int point = 200;

Node* body;

using namespace std;

inline int next_p2 (int a )
{
	int rval=1;
	// rval<<=1 Is A Prettier Way Of Writing rval*=2; 
	while(rval<a) rval<<=1;
	return rval;
}

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

void drawText(string str, float r=1, float g=1, float b=1)
{	
	glPushMatrix();
	
	const char* cstr = str.c_str();
	int count = str.length();
	
	GLint loc = glGetUniformLocation(p,"edgeSize");
	glUniform1f(loc, 0.5);
	loc = glGetUniformLocation(p,"alphaOnly");
	glUniform1i(loc, 1);
	
	for(int i=0; i<count; i++)
	{
		glBindTexture(GL_TEXTURE_2D, text[cstr[i]]);
		
		glBegin(GL_TRIANGLE_STRIP);
		
		glColor3f(r, g, b);
		
		glTexCoord2f(0.0, ypad[cstr[i]]);
		glVertex3f(0, 0, 0.0);
		
		glTexCoord2f(xpad[cstr[i]], ypad[cstr[i]]);
		glVertex3f(wid[cstr[i]], 0, 0.0);
		
		glTexCoord2f(0.0, 0.0);
		glVertex3f(0, hgt[cstr[i]], 0.0);
		
		glTexCoord2f(xpad[cstr[i]], 0.0);
		glVertex3f(wid[cstr[i]], hgt[cstr[i]], 0.0);
		
		glEnd();
		
		if(i+1 < count)
			glTranslatef(kerning[cstr[i]][cstr[i+1]], 0, 0);
	}
	
	glPopMatrix();
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
	glUniform1f(loc, 0.5);
	loc = glGetUniformLocation(p,"shadowPosition");
	glUniform3f(loc, 1, -0.1, 1);
	body->update(1.0f);
	body->render();
	
	glPushMatrix();
	{
		GLint loc = glGetUniformLocation(p,"shadowAlpha");
		glUniform1f(loc, 0);
		
		glTranslatef(-4, -2, 0);
		glScalef(2, 2, 1);
		drawText("Vector", 0, 0, 1);
	}
	glPopMatrix();
	
	glutSwapBuffers();
}

GLint makeVectorTexture(unsigned char* buffer, int bWidth, int bHeight, int vWidth, int vHeight)
{
	return 0;
}

void makeText()
{
	bool use_kerning = FT_HAS_KERNING( font );
	
	for(unsigned char c=0; c<128; c++)
	{
		// The First Thing We Do Is Get FreeType To Render Our Character
		// Into A Bitmap.  This Actually Requires A Couple Of FreeType Commands:
		
		// Load The Glyph For Our Character.
		if(FT_Load_Glyph( font, FT_Get_Char_Index( font, c ), FT_LOAD_DEFAULT ))
			return;
		
		// Move The Face's Glyph Into A Glyph Object.
		FT_Glyph glyph;
		if(FT_Get_Glyph( font->glyph, &glyph ))
			return;
		
		// Convert The Glyph To A Bitmap.
		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
		
		// This Reference Will Make Accessing The Bitmap Easier.
		FT_Bitmap& bitmap=bitmap_glyph->bitmap;
		
		// Use Our Helper Function To Get The Widths Of
		// The Bitmap Data That We Will Need In Order To Create
		// Our Texture.
		int downsample = 8;
		int width = next_p2( bitmap.width + 4 * downsample );
		int height = next_p2( bitmap.rows + 4 * downsample );
		
		float xs = (float)bitmap.width / width;
		float ys = (float)bitmap.rows / height;
		
		width /= downsample;
		height /= downsample;
		
		// Allocate Memory For The Texture Data.
		GLubyte* tex = (GLubyte*)malloc(width * height);
		memset(tex, 0, width*height);
		
		for(int i=0; i < width; i++)
		for(int j=0; j < height; j++)
		{
			tex[i + j*width]= edgeDistance(bitmap.buffer, bitmap.width, bitmap.rows, (i)/xs/width, (j)/ys/height, 1.0);
		}
		
		glGenTextures(1, &text[c]);
		glBindTexture(GL_TEXTURE_2D, text[c]);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tex);
		
		for(int k=0; k<128; k++)
		{
			if(use_kerning)
			{
				FT_Vector kern;
				FT_Get_Kerning( font,         
								FT_Get_Char_Index( font, c ),    
								FT_Get_Char_Index( font, k ),        
								FT_KERNING_DEFAULT, 
								&kern );   
				
				kerning[c][k] = (font->glyph->advance.x + kern.x) / (point * 64.0f);
			}
			else
			{
				kerning[c][k] = font->glyph->advance.x / (point * 64.0f);
			}
		}
		
		wid[c] = bitmap.width / (float)point;
		hgt[c] = bitmap.rows / (float)point;
		
		xpad[c] = xs;
		ypad[c] = ys;
		
		free(tex);
	}
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

void loadFont()
{
	// Create And Initilize A FreeType Font Library.
	FT_Init_FreeType( &ft );
	
	// This Is Where We Load In The Font Information From The File.
	// Of All The Places Where The Code Might Die, This Is The Most Likely,
	// As FT_New_Face Will Fail If The Font File Does Not Exist Or Is Somehow Broken.
	if (FT_New_Face( ft, "/library/fonts/arial.ttf", 0, &font )) 
		return;
	
	// For Some Twisted Reason, FreeType Measures Font Size
	// In Terms Of 1/64ths Of Pixels.  Thus, To Make A Font
	// h Pixels High, We Need To Request A Size Of h*64.
	// (h << 6 Is Just A Prettier Way Of Writing h*64)
	FT_Set_Char_Size( font, point * 64, point * 64, 96, 96);
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

void unloadFont()
{
	FT_Done_Face(font);
	FT_Done_FreeType(ft);
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
	for(unsigned char c=0; c<128; c++)
	{
		glDeleteTextures(1, &text[c]);
	}
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
	loadFont();
	
	body = new Node();
	body->addChild(new Sprite(p, "body"));
	body->setSpin(0.1f);
	body->setPosition(vec(10, 0));
	body->setScale(vec(30, 30));
	
	makeCircle();
	makeText();
	
	unloadFont();

	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutMainLoop();
	
	unloadShaders();
	unloadTextures();
	
	delete body;
	
	return 0;
}
