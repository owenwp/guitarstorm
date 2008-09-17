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
#include "FilePick.h"

void FilePick::OnSelect()
{
	if(items[select]->name == "Choose")
	{
		*selected = dir->Path();
		done = true;
	}
	else if(select >= fpos && items[select]->name != "Back")
	{
		*selected = dir->files[items[select]->name];
		done = true;
	}
}

void FilePick::OnOpen()
{
	if(firstOpen && parent)
	{
		dir->Load();
		float x = -5;
		float y = 8;
		fpos = 0;
		map<string, Directory>::iterator ditr;
		for(ditr = dir->dirs.begin(); ditr != dir->dirs.end(); ditr++)
		{
			Add(new MenuItem(ditr->first, new FilePick(ditr->first, &ditr->second, selected), x, y)); 
			y -= 2;
			fpos++;
		}
		map<string, string>::iterator fitr;
		for(fitr = dir->files.begin(); fitr != dir->files.end(); fitr++)
		{
			Add(new BackItem(fitr->first, x, y)); 
			y -= 2;
		}
		Add(new BackItem("Back", -5, -11));
	}
	done = false;
}

void DirPick::OnOpen()
{
	if(firstOpen && parent)
	{
		dir->Load();
		float x = -5;
		float y = 8;
		fpos = 0;
		map<string, Directory>::iterator ditr;
		for(ditr = dir->dirs.begin(); ditr != dir->dirs.end(); ditr++)
		{
			Add(new MenuItem(ditr->first, new DirPick(ditr->first, &ditr->second, selected), x, y)); 
			y -= 2;
			fpos++;
		}
		Add(new BackItem("Choose", -5, -11));
		Add(new BackItem("Back", -5, -12));
	}
	done = false;
}

void FilePick::OnValueChange()
{
	if(child)
	{
		done = ((FilePick*)child)->done;
		if(done) 
			Close();
	}
}
