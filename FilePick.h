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
#ifndef FILEPICK
#define FILEPICK

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
#include "Menu.h"

class FilePick : public Menu
{
public:
	FilePick(string n, Directory* d, string* val):Menu(n),dir(d),selected(val) {}
	FilePick(string n, string loc, string* val):Menu(n),selected(val) {dir = new Directory(loc,false);}

protected:
	virtual void OnOpen();
	void OnSelect();
	void OnValueChange();
	
	string *selected;
	int fpos;
	int spos;
	Directory* dir;
	bool done;
};

class DirPick : public FilePick
{
public:
	DirPick(string n, Directory* d, string* val):FilePick(n,d,val){}
	DirPick(string n, string loc, string* val):FilePick(n,loc,val){}

private:
	void OnOpen();
};

#endif
