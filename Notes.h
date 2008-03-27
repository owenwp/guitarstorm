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
#ifndef NOTES
#define NOTES

#include <sstream>
#include <fstream>

#include "kguitar/convertgtp.h"
#include "Scorer.h"

using namespace std;

class Notes : public osg::Referenced
{
public:
	Notes();

	osg::Group* getScene() { return _scene.get(); }

	bool setSong(std::string name);

	void setSpeed(int percent);

	void Update();

	Scorer* scorer;

	void visible(bool v) { _scene->setNodeMask(v); }
	
	bool running;
	bool finished;

	osg::Vec3 origin;

protected:
	~Notes() {}

	void PlaceBeat(double t);
	void PlaceNote(double t, int s, int f);

	void addString(osg::Vec3& pos,const std::string& text, float height);
	void createStrings();
	
	osg::Timer* time;

	osg::ref_ptr<osgText::Text> tnote[25];
	osg::ref_ptr<osgText::Text> tbeat;

	vector<TabColumn>::iterator col;
	vector<TabBar>::iterator bar;

	double speed;
	double spacing;
	double offset;

	double last;
	double current;

	double stopped;

	double count, count2, count3;

	static const float tolerance;

	typedef std::map<double, osg::ref_ptr<osg::Switch> > NoteChart;

	TabTrack* tab;

	NoteChart chart;

	osg::ref_ptr<osg::Group>    _scene;
	osg::ref_ptr<osgText::Text> scoreText;

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

class notesCallback : public osg::NodeCallback 
{
public:
   virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
   {
      osg::ref_ptr<Notes> data = 
         dynamic_cast<Notes*> (node->getUserData() );
      if(data != NULL)
      {
         data->Update();
      }
      traverse(node, nv); 
   }
};

#endif
