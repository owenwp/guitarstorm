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

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>
#include <osg/BlendEquation>

#include <OpenThreads/Thread>
#include <string>

using namespace osg;
using namespace std;

class Sprite : public MatrixTransform
{
public:
	Sprite(Sprite* sprite);
	Sprite(string filename = "", bool absolute = false);

	void setPosition(Vec3 p) {position = p; slides = 0;}
	void setScale(Vec2 s) {scale = s; grows = 0;}
	void setRotation(float r) {rotation = r; turns = 0;}
	void setCenter(Vec3 c) {center = c;}
	void setScroll(Vec2 s) {scroll = s;}
	void setSpin(float s) {spin = s; turns = 0;}
	void setColor(Vec4 c) {color = c; tints = 0;}
	
	Vec3 getPosition() {return position;}
	Vec2 getScale() {return scale;}
	float getRotation() {return rotation;}
	Vec3 getCenter() {return center;}
	Vec2 getScroll() {return scroll;}
	float getSpin() {return spin;}
	Vec4 getColor() {return color;}

	void setSlide(Vec3 p, float s) {slide = p; slides = s;}
	void setTurn(float t, float s) {turn = t; turns = s;}
	void setGrow(Vec2 g, float s) {grow = g; grows = s;}
	void setTint(Vec4 t, float s) {tint = t; tints = s;}
	
	virtual void Update();

protected:
	virtual void createSquare(Image* image);

	Vec3 position;
	Vec2 scale;
	float rotation;
	float spin;
	Vec3 center;
	Vec2 scroll;
	Vec4 color;
	
	Vec3 slide;
	float turn;
	Vec2 grow;
	Vec4 tint;

	float slides;
	float turns;
	float grows;
	float tints;

	Vec4Array* colora;
	Vec2Array* tcoords;

	Geometry* geom;
	Geode* geode;
	StateSet* stateset;
	float aspect;

	double last;

	static Timer* time;
};

class spriteCallback : public NodeCallback 
{
public:
   virtual void operator()(Node* node, NodeVisitor* nv)
   {
      ref_ptr<Sprite> data = 
         dynamic_cast<Sprite*> (node);
      if(data != NULL)
      {
         data->Update();
      }
      traverse(node, nv); 
   }
};

#endif
