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
	Audio::openMusic(song->backing);

	Sprite* art;
	if(dir.files["cover"] != "")
		art = new Sprite(dir.files["cover"], true);
	else
		art = new Sprite("cd.tga", false);
	art->setPosition(Vec3(0, 0.5f, 3));
	art->setScale(Vec2(4,4));
	if(song->backing == "none")
		art->setColor(Vec4(1, 0, 0, 1));
	_mscene->addChild(art);

	float y = -2;
	if(song->difficulty[0].used.length()) 
	{
		Add(new SongPlayItem("Easy", &song->difficulty[0], false, -5, y, dir.files["cover"]));
		y -= 2;
	}
	if(song->difficulty[1].used.length()) 
	{
		Add(new SongPlayItem("Medium", &song->difficulty[1], false, -5, y, dir.files["cover"]));
		y -= 2;
	}
	if(song->difficulty[2].used.length()) 
	{
		Add(new SongPlayItem("Hard", &song->difficulty[2], false, -5, y, dir.files["cover"]));
		y -= 2;
	}
	if(song->difficulty[3].used.length()) 
	{
		Add(new SongPlayItem("Expert", &song->difficulty[3], false, -5, y, dir.files["cover"]));
		y -= 2;
	}

	Add(new BackItem("Back", -5, -11));
}

void SongPlayItem::Select()
{
	if(game)
	{
		delete game;
		game = NULL;
		Audio::stopMusic();
		Input::setGuitar(NULL);
		parent->Close();
	}
	else
	{
		parent->Hide();
		game = new Guitar();
		game->getSprite()->setScale(Vec2(0.1f,0.1f));
		game->getSprite()->setPosition(Vec3(-20,-5,0));
		game->getSprite()->setRotation(PI / 2);
		game->getSprite()->setGrow(Vec2(9,10), 0.2f);
		game->getSprite()->setSlide(Vec3(48,0,0), 0.22f);
		game->getSprite()->setTurn(0, 0.6f);
		parent->getScene()->addChild(game->getScene());
		Input::setGuitar(game);
		game->setSong(Track, picture, false);
	}
}
