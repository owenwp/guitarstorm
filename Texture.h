/*
 This file is part of Guitar Storm.
 Copyright 2008 Zombie Process, Owen Pedrotti
 
 Guitar Storm is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 Guitar Storm is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with Guitar Storm.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TEXTURE
#define TEXTURE

#include <GLUT/GLUT.h>
#include <IL/il.h>
#include <string>
#include <map>
#include "VectorMath.h"
#include "utilities/Threads.h"

using namespace std;

enum spriteShape
{
	shapeNone = 0,
	shapeCircle,
	shapeSquare
};

unsigned char edgeDistance(unsigned char* mask, int w, int h, float x, float y, float domain);

class Texture : public runnable
{
private:
	GLuint id;
	float edge;
	float aspect;
	bool alphaOnly;
	bool blend;
	
	thread* loading;
	void run();
	
	Texture(){}
	
	int mWidth;
	int mHeight;
	
	int width;
	int height;
	
	unsigned char* mData;
	unsigned char* vData;
	
public:
	Texture(string name);
	Texture(spriteShape shape);
	~Texture();
	
	static void Cache(string name);
	static void UnloadAll();
	
	float Aspect();
	
	void Bind(GLint p);
};

#endif
