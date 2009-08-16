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
#ifndef SPRITE
#define SPRITE

#include <string>
#include <GLUT/GLUT.h>
#include "VectorMath.h"
#include "Renderable.h"
#include "Texture.h"

using namespace std;

class Sprite : public Renderable
{
public:
	Sprite(Sprite* sprite);
	Sprite(Texture* tex, vec col = vec(1,1,1,1));

	void setColor(vec c) {color = c; tints = 0;}
	
	vec getColor() {return color;}

	void setTint(vec t, float s) {tint = t; tints = s;}
	
	virtual void update(float timeDelta);
	virtual void render(GLint program);

protected:
	Texture* texture;
	
	vec color;
	vec tint;
	
	float tints;
};

#endif
