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
#ifndef GUITAR
#define GUITAR

#include <sstream>
#include <fstream>

#include "kguitar/convertgtp.h"
#include "Scorer.h"
#include "Audio.h"
#include "Song.h"
#include "Menu.h"
#include "Sprite.h"
#include "LineSprite.h"
#include "Node.h"

using namespace std;

class Guitar : public Node
{
public:
	Guitar();

	bool setSong(Difficulty *t, string pic = "", bool a = true);

	void setSpeed(int percent);

	void setOffset(float o) { current += tabdelay - backdelay - o; tabdelay -= tabdelay - backdelay - o; }

	void update(float timeDelta);

	Scorer* scorer;
	
	bool running;
	bool finished;
	bool playing;

protected:
	void PlaceBeat(double t);
	void PlaceNote(double t, int s, int f);

	void MakeGuitar();
	
	LineSprite* strings[6];
	
	vec nstring[6];
	vec bstring[6];
	
	Node* body;

	Menu *parent;
	Menu *pauseMenu;

	Label* tnote[25];
	Sprite* snote;
	Sprite* sbeat;

	Node* chart;
	Node* beats;

	vector<TabColumn>::iterator col;
	vector<TabBar>::iterator bar;

	vector<TabColumn>::iterator repc;
	vector<TabBar>::iterator repb;
	int repcount;

	double backdelay;
	double tabdelay;

	double speed;
	double spacing;
	double offset;

	double last;
	double current;
	double realtime;

	double stopped;

	double count, count2, count3;

	static const float tolerance;
	static const float nutpos;
	static const float neckscale;
	static const float neckpos;
	static const float bridgepos;
	static const float bridgeheight;
	static const float neckspace;
	static const float bridgespace;

	TabTrack* tab;

	Label* scoreText;

	int score;
	int combo;
	int multiplier;

	// header information
	string version;
	string title;
	string artist;
	string transcription;

	vector<string> lyrics;

	float tempo;
	char key;
	char octave;
};

/*class guitarCallback : public NodeCallback 
{
public:
   virtual void operator()(Node* node, NodeVisitor* nv)
   {
      ref_ptr<Guitar> data = 
         dynamic_cast<Guitar*> (node->getUserData() );
      if(data != NULL)
      {
         data->Update();
      }
      traverse(node, nv); 
   }
};*/

#endif
