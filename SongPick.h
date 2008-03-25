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
