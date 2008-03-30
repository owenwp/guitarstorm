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
#include "SongPick.h"

const string location = "tabs";

void SongPick::CreateList()
{
	_scene = new osg::Group;

	// setup interface
	osg::Geode* geodeTrack = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
        trackText->setFont("fonts/arial.ttf");
        trackText->setColor(osg::Vec4(6.0f,1.0f,0.0f,1.0f));
        trackText->setCharacterSize(3.0f);
		trackText->setFontResolution(80,80);
		trackText->setBackdropType(osgText::Text::DROP_SHADOW_BOTTOM_RIGHT);
		trackText->setPosition(osg::Vec3(0.0f,1.0f,11.0f));
		trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		trackText->setDrawMode(osgText::Text::TEXT);
		trackText->setAlignment(osgText::Text::CENTER_TOP);
        trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
		trackText->setText("GUITAR STORM");
        
        geodeTrack->addDrawable(trackText.get());
        
        _scene->addChild(geodeTrack);
    }
	{
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
        trackText->setFont("fonts/arial.ttf");
        trackText->setColor(osg::Vec4(6.0f,1.0f,0.0f,1.0f));
        trackText->setCharacterSize(1.0f);
        trackText->setPosition(osg::Vec3(0.0f,1.0f,8.0f));
		trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        trackText->setDrawMode(osgText::Text::TEXT);
		trackText->setAlignment(osgText::Text::CENTER_TOP);
        trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
		trackText->setText("Prototype 0.1\nCopyright 2008 Zombie Process\nhttp://guitarstormgame.com/");
        
        geodeTrack->addDrawable(trackText.get());
        
        _scene->addChild(geodeTrack);
    }
	{
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
        trackText->setFont("fonts/arial.ttf");
        trackText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        trackText->setCharacterSize(1.5f);
        trackText->setPosition(osg::Vec3(-8.0f,1.0f,4.0f));
		trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        trackText->setDrawMode(osgText::Text::TEXT);
		trackText->setAlignment(osgText::Text::LEFT_TOP);
        trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
		trackText->setText("Choose Song");
        
        geodeTrack->addDrawable(trackText.get());
        
        _scene->addChild(geodeTrack);
    }

	dir = new Directory(location);
	select = 0;

	map<string, string>::iterator itr;
	spos = _scene->getNumChildren();
	for(itr = dir->files.begin(); itr != dir->files.end(); itr++)
	{
		map<string, string> head = ConvertGtp::header(itr->second);
		if(head.size() == 0)
			continue;

		geodeTrack = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
			trackText->setFont("fonts/arial.ttf");
			trackText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
			trackText->setCharacterSize(1.0f);
			trackText->setPosition(osg::Vec3(-8.0f,1.0f,2.0f - select));
			trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
			trackText->setDrawMode(osgText::Text::TEXT);
			trackText->setAlignment(osgText::Text::LEFT_CENTER);
			trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
			trackText->setText(head["TITLE"]+" - "+head["ARTIST"]);
	        
			geodeTrack->addDrawable(trackText.get());
	        
			_scene->addChild(geodeTrack);
		}
		select++;
	}

	geodeTrack = new osg::Geode;
	{
		cursor = new osgText::Text;
		cursor->setFont("fonts/arial.ttf");
		cursor->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
		cursor->setCharacterSize(1.0f);
		cursor->setPosition(osg::Vec3(-9.0f,1.0f,2.0f - select));
		cursor->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		cursor->setDrawMode(osgText::Text::TEXT);
		cursor->setAlignment(osgText::Text::LEFT_CENTER);
		cursor->setAxisAlignment(osgText::Text::XZ_PLANE);
		cursor->setText(">");
        
		geodeTrack->addDrawable(cursor.get());
        
		_scene->addChild(geodeTrack);
	}

	pick(0);
}

string SongPick::get()
{
	map<string, string>::iterator itr;
	int i=0;
	for(itr = dir->files.begin(); itr != dir->files.end(); itr++)
	{
		if(select == i++)
		{
			return itr->second;
		}
	}
	return "";
}

void SongPick::pick(int p)
{
	if(p < 0)
		p = 0;
	if(p >= dir->files.size())
		p = dir->files.size()-1;

	select = p;
	cursor->setPosition(osg::Vec3(-9.0f,1.0f,2.0f - select));
}

void SongPick::error()
{
	_scene->getChild(spos + select)->setNodeMask(0);
	
	osg::Geode* geodeTrack = new osg::Geode;
	{
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
		trackText->setFont("fonts/arial.ttf");
		trackText->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
		trackText->setCharacterSize(1.0f);
		trackText->setPosition(osg::Vec3(-4.0f,1.0f,2.0f - select));
		trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		trackText->setDrawMode(osgText::Text::TEXT);
		trackText->setAlignment(osgText::Text::LEFT_CENTER);
		trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
		trackText->setText("Failed to Load");
        
		geodeTrack->addDrawable(trackText.get());
        
		_scene->addChild(geodeTrack);
	}
}

void SongPick::up()
{
	pick(select-1);
}

void SongPick::down()
{
	pick(select+1);
}
