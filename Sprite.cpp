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
#include "Sprite.h"
#include <iostream>

using namespace std;

Sprite::Sprite(Sprite* sprite)
{ 
	tex = sprite->tex;
	program = sprite->program;
	color = sprite->color;
	tints = sprite->tints;
}

Sprite::Sprite(GLint p, string filename, bool absolute)
{
	program = p;
	tex = new Texture(filename);
	color = vec(1,1,1,1);
	tints = 0;
}

void Sprite::update(float timeDelta)
{
	color += (tint - color) * min(1.0f, tints * timeDelta);
}

void Sprite::render()
{
	if(!tex)
		return;
	
	tex->Bind(program);
	
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor4f(color.x, color.y, color.z, color.w);
	
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
