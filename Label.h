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

#ifndef LABEL
#define LABEL

#include "Renderable.h"
#include "VectorMath.h"
#include <string>
#include <map>
#include <GLUT/GLUT.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/ftglyph.h>
#include <freetype/ftoutln.h>
#include <freetype/fttrigon.h>

using namespace std;

struct Font
{
	FT_Face face;
	GLuint chars[128];
	float kerning[128][128];
	float wid[128];
	float hgt[128];
	float xpad[128];
	float ypad[128];
	int point;
};

class Label : public Renderable
{
public:
	Label(string f, string t = "");
	void printf(string format, ...);
	
	void setColor(vec c) {color = c; tints = 0;}
	
	vec getColor() {return color;}
	
	void setTint(vec t, float s) {tint = t; tints = s;}
	
	virtual void update(float timeDelta);
	virtual void render(GLint program);
	
protected:
	Font* font;
	
	string text;
	
	vec color;
	vec tint;
	
	float tints;
	
	void makeFont(string f);
};

#endif

