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
#ifndef SONGEDITMENU
#define SONGEDITMENU

#include "id3tag.h"

#include "Menu.h"
#include "Song.h"
#include "Options.h"
#include "Audio.h"
#include "FilePick.h"
#include "Guitar.h"
#include "utilities/FileCopy.h"
#include "kguitar/convertgtp.h"

class TrackEditMenu : public Menu
{
public:
	TrackEditMenu(string n, Difficulty *d):Menu(n),tab(NULL),difficulty(d),picker(NULL),guitar(NULL){}

	void OnOpen();
	void OnClose();
	void OnValueChange();

private:
	void AddProps();

	TabSong* tab;
	string pickpath;
	Difficulty* difficulty;
	FilePick* picker;
	Guitar* guitar;
};

class SongEditMenu : public Menu
{
public:
	SongEditMenu(string n, Song* s):Menu(n),song(s),filed(false){Setup();}

	void OnOpen();
	void OnClose();
	void OnSelect();
	void OnValueChange();

	void SetTitle(string t) {if(!song->title.length()) song->title = t;}
	void SetArtist(string a) {if(!song->artist.length()) {if(a.length()) song->artist = a; else song->artist = "unknown";}}

	void GetArt(string f);

private:
	void Save();
	void Load();

	void Setup();

	bool filed;

	osg::ref_ptr<osgText::Text> itemText;
	Item* backitem;
	Song* song;
};

#endif
