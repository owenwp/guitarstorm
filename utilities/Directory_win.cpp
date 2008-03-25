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
#include "Directory.h"
#include <windows.h>

// relative location of resource files
string Directory::resdir = "";

// fill the contents map with all of the files in the given directory
Directory::Directory(string n)
{
	WIN32_FIND_DATA FindData;
    HANDLE hFind;

	hFind = FindFirstFile((resdir + n + "\\*.*").c_str(), &FindData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{ 
		do
		{

			_strlwr_s(FindData.cFileName); 
			string szFileName(FindData.cFileName); 
			string name = szFileName.substr(szFileName.find_last_of("\\")+1);

			 // we looking at a directory?? then call this function *again* to look in it 
			if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{ 
				//if (szFileName != "." && szFileName != "..") 
				//	FindFiles(szDirectory + "/" + szFileName); 
			} 
			else 
			{ 
				contents[name] = szFileName;
			} 
		}
		while(FindNextFile(hFind, &FindData));
	}
}
