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
	if(action) 
		delete action;
	
	if(select >= songs.size()) 
		action = new SongEditMenu("Edit Song", NULL);
	else if(play) 
		action = new SongPlayMenu(songs[select].title + "\n" + songs[select].artist, &songs[select]);
	else 
		action = new SongEditMenu("Edit Song", &songs[select]);
	
	parent->Open(action);
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
	label->setAlignment(alignCenter);
	
	if(songDir) delete songDir;
	songDir = new Directory(Options::instance->songDir);
	
	icons.clear();
	songs.clear();

	float x = -8;
	float y = 8;
	map<string, Directory*>::iterator itr;
	for(itr=songDir->dirs.begin(); itr!=songDir->dirs.end(); itr++)
	{
		Directory* sdir = itr->second;
		sdir->Load();

		Song song;
		string dest = sdir->files["song"];
		ifstream in(dest.c_str(), ios_base::binary);
		if(!in.fail())
		{
			in >> song;
		}
		in.close();

		songs.push_back(song);
		
		Node* node = new Node;
		Sprite* art;
		if(sdir->files["cover"] != "")
		{
			art = new Sprite(new Texture(sdir->files["cover"]));
			node->addChild(art);
		}
		else
		{
			art = new Sprite(new Texture("cd.tga"));
			node->addChild(art);
			
			Node* n = new Node;
			float size = min(0.2f, 1.0f/song.artist.length());
			n->setScale(vec(size, size));
			n->setPosition(vec(0,-0.35,0.1));
			Label* l = new Label("arial", song.artist, alignCenter);
			l->setColor(vec(0,0,0,1));
			n->addChild(l);
			node->addChild(n);
		}
		
		Node* title = new Node;
		title->setScale(vec(0.15, 0.15));
		title->setPosition(vec(0,-0.7,0.1));
		
		Node* shadow = new Node;
		shadow->setScale(vec(5, 1));
		title->addChild(new Sprite(new Texture(shapeCircle), vec(0,0,0,1)));
		
		title->addChild(shadow);
		title->addChild(new Label("arial", song.title, alignCenter));
		node->addChild(title);

		/*
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
		 */
		//icons.push_back(art);
		
		art->setColor(vec(1,1,1,1));
		addChild(node);
		icons.push_back(node);

		y -= 2;
	}
	
	Node* node = new Node;
	Sprite* art = new Sprite(new Texture("cd.tga"));
	art->setColor(vec(0,1,0,1));
	Node* n = new Node;
	n->setScale(vec(0.18, 0.18));
	n->setPosition(vec(0,-0.35,0.1));
	n->addChild(new Label("arial", "NEW", alignCenter));
	node->addChild(art);
	node->addChild(n);
	addChild(node);
	icons.push_back(node);
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
			icons[i]->setSlide(vec(0, 2.5f, 3), 5);
			icons[i]->setGrow(vec(2,2), 5);
		}
		else
		{
			icons[i]->setSlide(vec(delta / abs(delta) * 1.5f * logf(1.0f*abs(delta)+4), 2.5f + abs(delta) * 0.05f, 5 - abs(delta) * 0.01f), 5);
			icons[i]->setGrow(vec(1.5f,1.5f), 5);
		}
	}
}
