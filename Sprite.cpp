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
	texture = sprite->texture;
	color = sprite->color;
	tints = sprite->tints;
	spos = sprite->spos;
	scroll = sprite->scroll;
}

Sprite::Sprite(Texture* tex, vec col)
{
	texture = tex;
	color = col;
	tints = 0;
	spos = vec(0,0);
	scroll = vec(0,0);
}

void Sprite::update(float timeDelta)
{
	color += (tint - color) * min(1.0f, tints * timeDelta);
	spos += scroll * timeDelta;
}

void Sprite::render(GLint program)
{
	if(!texture)
		return;
	
	glPushMatrix();
	
	glScalef(texture->Aspect(), 1, 1);
	texture->Bind(program);
	
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor4f(color.x, color.y, color.z, 1);
	
	glTexCoord2f(0.0+spos.x, 1.0+spos.y);
	glVertex3f(-0.5, -0.5, 0.0);
	
	glTexCoord2f(1.0+spos.x, 1.0+spos.y);
	glVertex3f(0.5, -0.5, 0.0);
	
	glTexCoord2f(0.0+spos.x, 0.0+spos.y);
	glVertex3f(-0.5, 0.5, 0.0);
	
	glTexCoord2f(1.0+spos.x, 0.0+spos.y);
	glVertex3f(0.5, 0.5, 0.0);
	
	glEnd();
	
	glPopMatrix();
}
