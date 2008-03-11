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
	Notes() { _scene = new osg::Group; }

	osg::Group* getScene() { return _scene.get(); }

	void hitNote(int key);

	void setSong(std::string name);

	void Update();

	Scorer* scorer;

protected:
	~Notes() {}

	void PlaceBeat(double t);
	void PlaceNote(double t, int s, int f);
	
	osg::Timer* time;

	vector<TabColumn>::iterator col;

	float speed;
	float offset;

	double count, count2;

	static const float tolerance;

	typedef std::map<double, osg::ref_ptr<osg::Switch>> NoteChart;

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
