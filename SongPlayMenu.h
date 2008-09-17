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
#ifndef SONGPLAYMENU
#define SONGPLAYMENU

#include "Menu.h"
#include "Song.h"
#include "Options.h"
#include "Input.h"
#include "Guitar.h"
#include "utilities/Directory.h"

class SongPlayItem : public Item
{
public:
	SongPlayItem(string n, Difficulty *t, bool a, float x, float y, string pic = ""):Item(n,x,y),Track(t),game(NULL),autoplay(a),picture(pic){}
	void Select();

private:
	bool autoplay;
	string picture;
	Difficulty *Track;
	Guitar *game;
};

class SongPlayMenu : public Menu
{
public:
	SongPlayMenu(string n, Song* s):Menu(n),song(s){}

	void OnOpen();

private:
	osg::ref_ptr<osgText::Text> itemText;
	Song* song;
};

#endif
