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
#include "LineSprite.h"

LineSprite::LineSprite(LineSprite* sprite) : Sprite(sprite)
{ 
	thickness = sprite->thickness;
	setLine(sprite->line);
	makeline = true;
}

LineSprite::LineSprite(Texture* tex, vec col) : Sprite(tex, col)
{
	thickness = tex->Aspect();
	setLine(vec(0, 1));
	makeline = true;
}

void LineSprite::setLine(vec l) 
{
	line = l; 
	len = length(line);
	makeline = true;
	
	vec u = normalize(line % vec(0,0,1)) * (thickness/2);
	
	points[0] = u * -1;
	points[1] = u;
	points[2] = line + (u * -1);
	points[3] = line + u;
}

void LineSprite::render(GLint program)
{
	Renderable::render(program);
	
	if(!texture)
		return;
	
	glPushMatrix();
	
	//glScalef(texture->Aspect(), 1, 1);
	texture->Bind(program);
	
	glBegin(GL_TRIANGLE_STRIP);
	
	glColor4f(color.x, color.y, color.z, 1);
	
	glTexCoord2f(0.0+spos.x, len+spos.y);
	glVertex3f(points[0].x, points[0].y, points[0].z);
	
	glTexCoord2f(1.0+spos.x, len+spos.y);
	glVertex3f(points[1].x, points[1].y, points[1].z);
	
	glTexCoord2f(0.0+spos.x, 0.0+spos.y);
	glVertex3f(points[2].x, points[2].y, points[2].z);
	
	glTexCoord2f(1.0+spos.x, 0.0+spos.y);
	glVertex3f(points[3].x, points[3].y, points[3].z);
	
	glEnd();
	
	glPopMatrix();
}
