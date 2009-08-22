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

#ifndef NODE
#define NODE

#include "VectorMath.h"
#include "Renderable.h"

class Node : public Renderable
{
public:
	Node();
	~Node();
	
	void setPosition(vec p) {position = p; slides = 0;}
	void setScale(vec s) {scale = s; grows = 0;}
	void setRotation(float r) {rotation = r; turns = 0;}
	void setCenter(vec c) {center = c;}
	void setScroll(vec s) {scroll = s;}
	void setSpin(float s) {spin = s; turns = 0;}
	
	vec getPosition() {return position;}
	vec getScale() {return scale;}
	float getRotation() {return rotation;}
	vec getCenter() {return center;}
	vec getScroll() {return scroll;}
	float getSpin() {return spin;}
	
	void setSlide(vec p, float s) {slide = p; slides = s;}
	void setTurn(float t, float s) {turn = t; turns = s;}
	void setGrow(vec g, float s) {grow = g; grows = s;}
	
	void setHidden(bool h) {hidden = h;}
	
	void update(float timeDelta);
	void render(GLint program);
	
	void addChild(Renderable* c);
	void insertChild(Renderable* c, int i);
	void removeChild(Renderable* c);
	void removeChild(int i);
	
protected:
	vec position;
	vec scale;
	float rotation;
	float spin;
	vec center;
	vec scroll;
	
	vec slide;
	float turn;
	vec grow;
	
	float slides;
	float turns;
	float grows;
	
	bool hidden;
	Renderable* children;
};

#endif
