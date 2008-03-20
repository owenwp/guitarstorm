#ifndef SONGPICK
#define SONGPICK

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

#include "utilities/Directory.h"

class SongPick : public osg::Referenced
{
public:
	SongPick() { CreateList(); }

	osg::Group* getScene() { return _scene.get(); }

	void visible(bool v) { _scene->setNodeMask(v); }

	string get();

	void pick(int p);

	void up();
	void down();

private:
	void CreateList();

	osg::ref_ptr<osg::Group>    _scene;
	osg::ref_ptr<osgText::Text> cursor;
	int select;
	Directory* dir;
};

#endif
