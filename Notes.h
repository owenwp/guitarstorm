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

	void setSong(std::string name);

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
