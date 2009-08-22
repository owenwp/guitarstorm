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
#ifndef SONGLISTITEM
#define SONGLISTITEM

#include <map>
#include "Menu.h"
#include "Options.h"
#include "utilities/Directory.h"
#include "SongEditMenu.h"
#include "SongPlayMenu.h"

struct Song;

class SongListItem : public Item
{	
public:
	SongListItem(string n, float x, float y, bool p):songDir(NULL),Item(n,x,y),play(p),select(0){Setup(); Change();}

	void Mouse(float x);
	void Select();
	void Left() {select--; Change();}
	void Right() {select++; Change();}
	void Key(char k);

protected:
	void Setup();

	void Change();

	bool play;
	//osg::ref_ptr<Menu> action;
	Directory *songDir;

	int select;

	vector<Node*> icons;
	vector<Song> songs;
};

#endif
