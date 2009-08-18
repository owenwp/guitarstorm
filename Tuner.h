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

#include "fft/fft.h"
//#include "Audio.h"
#include "Sprite.h"
#include "Node.h"
#include "Options.h"

#include <math.h>

using namespace std;

class Tuner : public Node
{
public:
	Tuner();

	int getCents() { return cents; }
	int getNote() { return note; }

	void Update();

	void visible(bool v) { }

protected:
	void MakeTuner();

	Sprite* sprite;
	Sprite* arm;
	Sprite* flat;
	Sprite* sharp;
	Sprite* absolute;

	int cents;
	int note;
};

/*
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
};*/

#endif
