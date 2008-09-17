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
#ifndef TUNER
#define TUNER

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <osgText/Text>

#include "fft/fft.h"
#include "Audio.h"
#include "Sprite.h"

#include <math.h>

using namespace std;
using namespace osg;

class Tuner : public Referenced
{
public:
	Tuner();

	Sprite* getSprite() {return sprite;}
	Group* getScene() { return scene.get(); }

	int getCents() { return cents; }
	int getNote() { return note; }

	void Update();

	void visible(bool v) { scene->setNodeMask(v); }

protected:
	void MakeTuner();

	Sprite* sprite;
	Sprite* arm;
	Sprite* flat;
	Sprite* sharp;
	Sprite* absolute;
	ref_ptr<osgText::Text> ntext;
	ref_ptr<osgText::Text> stext;
	ref_ptr<Group>    scene;

	int cents;
	int note;
};


class tunerCallback : public NodeCallback 
{
public:
   virtual void operator()(Node* node, NodeVisitor* nv)
   {
      ref_ptr<Tuner> data = 
         dynamic_cast<Tuner*> (node->getUserData() );
      if(data != NULL)
      {
         data->Update();
      }
      traverse(node, nv); 
   }
};

#endif
