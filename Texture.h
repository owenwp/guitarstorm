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

using namespace std;

class Texture
{
private:
	GLuint id;
	float edge;
	
public:
	Texture(string name);
	~Texture();
	
	void Bind(GLint p);
};

#endif
