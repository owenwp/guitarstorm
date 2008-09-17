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
#include "SongListItem.h"

void SongListItem::Select() 
{
	action.release();
	if(select >= songs.size()) 
		action = new SongEditMenu("Edit Song", NULL);
	else if(play) 
		action = new SongPlayMenu(songs[select].title + "\n" + songs[select].artist, &songs[select]);
	else 
		action = new SongEditMenu("Edit Song", &songs[select]);
	
	parent->Open(action.get());
}

void SongListItem::Mouse(float x) {}

void SongListItem::Key(char k) 
{
	int count = songs.size();
	for(int i=0; i<count; i++)
	{
		if(toupper(songs[i].title[0]) == toupper(k))
		{
			select = i;
			Change();
			return;
		}
	}
}

void SongListItem::Setup()
{
	if(songDir) delete songDir;
	songDir = new Directory(Options::instance->songDir);
	
	icons.clear();
	songs.clear();

	float x = -8;
	float y = 8;
	map<string, Directory>::iterator itr;
	for(itr=songDir->dirs.begin(); itr!=songDir->dirs.end(); itr++)
	{
		Directory sdir = itr->second;
		sdir.Load();

		Song song;
		string dest = sdir.files["song"];
		ifstream in(dest.c_str(), ios_base::binary);
		if(!in.fail())
		{
			in >> song;
		}
		in.close();

		songs.push_back(song);

		Sprite* art;
		if(sdir.files["cover"] != "")
			art = new Sprite(sdir.files["cover"], true);
		else
		{
			art = new Sprite("cd.tga", false);

			osg::Geode* geodeLabel = new osg::Geode;
			{
				osg::ref_ptr<osgText::Text> labelText = new osgText::Text;
				labelText->setFont("fonts/arial.ttf");
				labelText->setColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f));
				labelText->setCharacterSize(min(1.0f, 2.0f/song.artist.length()));
				labelText->setPosition(osg::Vec3(0,-0.05f,-0.5f));
				labelText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
				labelText->setDrawMode(osgText::Text::TEXT);
				labelText->setAlignment(osgText::Text::CENTER_CENTER);
				labelText->setAxisAlignment(osgText::Text::XZ_PLANE);
				labelText->setText(song.artist.c_str());
		        
				geodeLabel->addDrawable(labelText.get());
		        
				art->addChild(geodeLabel);
			}
		}

		osg::Geode* geodeTitle = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> titleText = new osgText::Text;
			titleText->setFont("fonts/arial.ttf");
			titleText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
			titleText->setCharacterSize(0.3f);
			titleText->setPosition(osg::Vec3(0,-0.05f,-1.2f));
			titleText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
			titleText->setDrawMode(osgText::Text::TEXT);
			titleText->setBackdropType(osgText::Text::OUTLINE);
			titleText->setBackdropOffset(-0.05f, 0.0f);
			titleText->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
			titleText->setAlignment(osgText::Text::CENTER_CENTER);
			titleText->setAxisAlignment(osgText::Text::XZ_PLANE);
			titleText->setText(song.title.c_str());
	        
			geodeTitle->addDrawable(titleText.get());
	        
			art->addChild(geodeTitle);
			art->setUserData(titleText.get());
		}

		Sprite* shadow = new Sprite("circle.tga");
		shadow->setScale(Vec2(2,0.2f));
		shadow->setColor(Vec4(0,0,0,0.5f));
		shadow->setPosition(osg::Vec3(0,-0.025f,-1.2f));
		art->addChild(shadow);

		art->setPosition(Vec3(x-2,0.1f,y));
		art->setScale(Vec2(1,1));
		if(song.backing == "none")
			art->setColor(Vec4(1, 0, 0, 1));
		_scene->addChild(art);

		icons.push_back(art);

		y -= 2;
	}
	Sprite* art = new Sprite("cd.tga", false);
	
	osg::Geode* geodeTitle = new osg::Geode;
	{
		osg::ref_ptr<osgText::Text> titleText = new osgText::Text;
		titleText->setFont("fonts/arial.ttf");
		titleText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		titleText->setCharacterSize(0.75f);
		titleText->setPosition(osg::Vec3(0,-0.05f,-0.1f));
		titleText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		titleText->setDrawMode(osgText::Text::TEXT);
		titleText->setBackdropType(osgText::Text::OUTLINE);
		titleText->setBackdropOffset(-0.05f, 0.0f);
		titleText->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		titleText->setAlignment(osgText::Text::CENTER_CENTER);
		titleText->setAxisAlignment(osgText::Text::XZ_PLANE);
		titleText->setText("NEW");
        
		geodeTitle->addDrawable(titleText.get());
        
		art->addChild(geodeTitle);
		art->setUserData(titleText.get());
	}

	art->setPosition(Vec3(x-2,0.1f,y));
	art->setScale(Vec2(1,1));
	art->setColor(Vec4(0, 1, 0, 0.25f));
	_scene->addChild(art);

	icons.push_back(art);
}

void SongListItem::Change() 
{
	int count = icons.size();
	if(select < 0)
		select = 0;
	if(select >= count)
		select = count-1;

	for(int i=0; i<count; i++)
	{
		int delta = i - select;
		
		if(delta == 0)
		{
			icons[i]->setSlide(Vec3(0, 0.5f, 3), 5);
			icons[i]->setGrow(Vec2(4,4), 5);
		}
		else
		{
			icons[i]->setSlide(Vec3(delta / abs(delta) * 5 * logf(1.0f*abs(delta)+4), 0.5f + abs(delta) * 0.1f, 7 - abs(delta)), 5);
			icons[i]->setGrow(Vec2(3,3), 5);
		}
	}
}
