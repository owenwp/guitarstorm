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
#ifndef DIRECTORY_
#define DIRECTORY_

#include <string>
#include <map>

using namespace std;

class Directory
{
public:
	Directory(){}
	Directory(string n, bool load = true):path(n) {if(load) Load();}

	void Load();
	string Path(){return resdir + path;}

	map<string, Directory*> dirs;
	map<string, string> files;

	Directory& operator=(Directory &d) 
	{ 
		if(this != &d) 
		{
			dirs = d.dirs; 
			files = d.files; 
			path = d.path;
		}
		return *this; 
	}

private:
	string resdir;
	string path;
};

#endif
