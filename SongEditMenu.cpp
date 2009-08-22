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
#include "SongEditMenu.h"

void TrackEditMenu::AddProps()
{
	string *trackList = new string[tab->t.size()+2];
	trackList[0] = "None";
	for(int i=0; i<tab->t.size(); i++)
	{
		trackList[i+1] = tab->t[i].name;
	}
	Add(new NumberItem("Primary Track", &difficulty->track1, trackList, -4, -2));
	Add(new NumberItem("Secondary Track", &difficulty->track2, trackList, -4, -2.5));
	Add(new RealItem("Offset Coarse", &difficulty->offset, 0.5f, true, -4, -3));
	Add(new RealItem("Offset Fine", &difficulty->offset, 0.01f, true, -4, -3.5));
	
	// autoplay guitar
	guitar = new Guitar();
	//guitar->getSprite()->setScale(Vec2(2.0f,2.0f));
	//guitar->getSprite()->setPosition(Vec3(3.0f,0.5f,4.25f));
	//guitar->getSprite()->setRotation(0);
	//guitar->setSong(difficulty, "", true);
	//_mscene->addChild(guitar->getScene());
}

void TrackEditMenu::OnOpen()
{
	if(firstOpen && parent)
	{
		picker = new FilePick("Choose a Guitar Tab", Options::instance->tabDir, &pickpath);
		Add(new MenuItem("Guitar Tab", picker, -4, -1));
		Add(new BackItem("OK", -2, -4.5));
		
		if(difficulty->used.length())
		{
			tab = ConvertGtp::load(difficulty->tab);
			if(tab) 
				AddProps();
			else
				Open(picker);
		}
		else
		{
			Open(picker);
		}
	}
	//else if(guitar) 
	//	guitar->setSong(difficulty, "", true);
}

void TrackEditMenu::OnClose()
{
	if(guitar) 
		guitar->setSong(NULL);
	//Audio::stopMusic();
}

void TrackEditMenu::OnValueChange()
{
	/*if(!tab && pickpath.length())
	{
		tab = ConvertGtp::load(pickpath);
		if(!tab)
			return;

		difficulty->tab = pickpath;
		difficulty->used = "true";

		((SongEditMenu*)parent)->SetTitle(tab->info["TITLE"]);
		((SongEditMenu*)parent)->SetArtist(tab->info["ARTIST"]);

		AddProps();
	}

	if(guitar)
	{
		guitar->setOffset(difficulty->offset);
	}*/
}

void SongEditMenu::Setup()
{
	/*osg::Geode* geodeTitle = new osg::Geode;
    {
		itemText = new osgText::Text;
        itemText->setFont("fonts/arial.ttf");
        itemText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        itemText->setCharacterSize(0.75f);
		itemText->setPosition(osg::Vec3(0.0f,1.0f,9.0f));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setBackdropType(osgText::Text::OUTLINE);
		itemText->setBackdropOffset(-0.05f, 0.0f);
		itemText->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		itemText->setAlignment(osgText::Text::CENTER_TOP);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText("");
        
        geodeTitle->addDrawable(itemText.get());
        
        _mscene->addChild(geodeTitle);
    }*/
}

void SongEditMenu::OnValueChange()
{
	/*itemText->setText(song->title + " by " + song->artist);
	filed = song->title != "" && song->artist != "";
	if(song->backing != "none")
	{
		// play track
		GetArt(song->backing);
		Audio::openMusic(song->backing);
	}
	backitem->Enable(filed);*/
}

void SongEditMenu::OnClose()
{
	//Audio::stopMusic();
}

void SongEditMenu::OnOpen()
{
	if(song == NULL)
	{
		song = new Song;
		song->backing = "none";
	}
	else
		filed = true;

	//Audio::openMusic(song->backing);

	song->difficulty[0].name = "easy";
	song->difficulty[1].name = "medium";
	song->difficulty[2].name = "hard";
	song->difficulty[3].name = "expert";
	
	song->difficulty[0].tab = song->path + "/" + song->difficulty[0].name + ".tab";
	song->difficulty[1].tab = song->path + "/" + song->difficulty[1].name + ".tab";
	song->difficulty[2].tab = song->path + "/" + song->difficulty[2].name + ".tab";
	song->difficulty[3].tab = song->path + "/" + song->difficulty[3].name + ".tab";
	
	Add(backitem = new MenuItem("Backing Track", new FilePick("Choose a Backing Track", Options::instance->backingDir, &song->backing), -2, 2));
	Add(new NullItem("Distortion Effects", -2, 1, false));
	Add(new NullItem("Stompbox Effect", -2, 0, false));
	Add(new MenuItem("Easy", new TrackEditMenu(song->title + " - Easy", &song->difficulty[0]),-2, -1));
	Add(new MenuItem("Medium", new TrackEditMenu(song->title + " - Medium", &song->difficulty[1]),-2, -1.5));
	Add(new MenuItem("Hard", new TrackEditMenu(song->title + " - Hard", &song->difficulty[2]),-2, -2));
	Add(new MenuItem("Expert", new TrackEditMenu(song->title + " - Expert", &song->difficulty[3]),-2, -2.5));

	Add(new BackItem("Save", -2, -4));
	Add(new BackItem("Cancel", -2, -4.5));
	 
	OnValueChange();
}

void SongEditMenu::OnSelect()
{
	if(items[select]->name == "Save")
		Save();
	else if(items[select]->name == "Cancel" && filed)
		Load();
}

void SongEditMenu::GetArt(string f)
{
	/*id3_file* file = id3_file_open(f.c_str(), ID3_FILE_MODE_READONLY);
	id3_tag* tag = id3_file_tag(file);
	id3_frame* frame = id3_tag_findframe(tag, "APIC", 0);

	if(frame)
	{
		id3_length_t len;
		string type = (char*)frame->fields[1].latin1.ptr;
		if(type.find("/"))
			type = type.substr(type.find("/")+1);
		if(type == "jpeg")
			type = "JPG";
		const id3_byte_t* pic = id3_field_getbinarydata(&frame->fields[4], &len);
		string pfile = Options::instance->songDir + "/" + song->title + " - " + song->artist + "/cover." + type;

		Save();

		FileCopy((char*)pic, len, pfile);
		
		Sprite* art = new Sprite(pfile, true);
		art->setPosition(Vec3(-10,0.5f,3));
		art->setScale(Vec2(3,3));
		_mscene->addChild(art);

	}

	id3_tag_delete(tag);
	id3_file_close(file);*/
}

void SongEditMenu::Save()
{
	if(!song->title.length())
		return;

	Directory dir(Options::instance->songDir + "/" + song->title + " - " + song->artist);

	song->path = dir.Path();

	for(int i=0; i<4; i++)
	{
		if(song->difficulty[i].used.length())
		{
			string dest = dir.Path() + "/" + song->difficulty[i].name + ".tab";
			FileCopy(song->difficulty[i].tab, dest); 
		}
	}

	string dest = dir.Path() + "/song.dat";
	ofstream out(dest.c_str(), ios_base::binary);
	out << *song;
	filed = true;
}

void SongEditMenu::Load()
{
	Directory dir(Options::instance->songDir + "/" + song->title + " - " + song->artist);

	string dest = dir.Path() + "/song.dat";
	ifstream in(dest.c_str(), ios_base::binary);
	if(in.fail())
		return;

	in >> *song;
	
	song->difficulty[0].tab = dir.Path() + "/" + song->difficulty[0].name + ".tab";
	song->difficulty[1].tab = dir.Path() + "/" + song->difficulty[1].name + ".tab";
	song->difficulty[2].tab = dir.Path() + "/" + song->difficulty[2].name + ".tab";
	song->difficulty[3].tab = dir.Path() + "/" + song->difficulty[3].name + ".tab";
	
	filed = true;
	OnValueChange();
}
