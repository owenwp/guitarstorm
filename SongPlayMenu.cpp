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
#include "SongPlayMenu.h"

void SongPlayMenu::OnOpen()
{
	if(!song)
	{
		Close();
		return;
	}

	Directory dir(song->path);
	song->difficulty[0].tab = song->path + "/" + song->difficulty[0].name + ".tab";
	song->difficulty[1].tab = song->path + "/" + song->difficulty[1].name + ".tab";
	song->difficulty[2].tab = song->path + "/" + song->difficulty[2].name + ".tab";
	song->difficulty[3].tab = song->path + "/" + song->difficulty[3].name + ".tab";
	//Audio::openMusic(song->backing);

	Node* n = new Node;
	Sprite* art;
	if(dir.files["cover"] != "")
		art = new Sprite(new Texture(dir.files["cover"]));
	else
		art = new Sprite(new Texture("cd.tga"));
	n->addChild(art);
	n->setPosition(vec(0, 0.5f));
	n->setScale(vec(3,3));
	if(song->backing == "none")
		art->setColor(vec(1, 0, 0, 1));
	current->addChild(n);

	float y = -2;
	if(song->difficulty[0].used.length()) 
	{
		Add(new SongPlayItem("Easy", &song->difficulty[0], false, -2, y, dir.files["cover"]));
		y -= 0.5f;
	}
	if(song->difficulty[1].used.length()) 
	{
		Add(new SongPlayItem("Medium", &song->difficulty[1], false, -2, y, dir.files["cover"]));
		y -= 0.5f;
	}
	if(song->difficulty[2].used.length()) 
	{
		Add(new SongPlayItem("Hard", &song->difficulty[2], false, -2, y, dir.files["cover"]));
		y -= 0.5f;
	}
	if(song->difficulty[3].used.length()) 
	{
		Add(new SongPlayItem("Expert", &song->difficulty[3], false, -2, y, dir.files["cover"]));
		y -= 0.5f;
	}
	 
	Add(new BackItem("Back", -2, -4.5));
}

void SongPlayItem::Select()
{
	if(game)
	{
		delete game;
		game = NULL;
		//Audio::stopMusic();
		parent->Show();
		parent->Close();
	}
	else
	{
		parent->Hide();
		game = new Guitar();
		game->setScale(vec(0.01f,0.01f));
		game->setPosition(vec(-10,2,0));
		game->setRotation(-90);
		game->setGrow(vec(2,2), 0.2f);
		game->setSlide(vec(16,0,0), 0.22f);
		game->setTurn(0, 0.6f);
		parent->addChild(game);
		game->setSong(Track, picture, false);
	}
}
