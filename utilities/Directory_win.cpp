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
#include <direct.h>

// relative location of resource files
const string res = "";

// fill the contents map with all of the files in the given directory
void Directory::Load()
{
	WIN32_FIND_DATA FindData;
    HANDLE hFind;

	if(path.length())
	{
		if(path[0] != '/')
			resdir = res;

		hFind = FindFirstFile((resdir+ path + "\\*.*").c_str(), &FindData);

		if (hFind != INVALID_HANDLE_VALUE) 
		{ 
			do
			{
				_strlwr_s(FindData.cFileName); 
				string fileName(FindData.cFileName); 
				string p = path+"/"+fileName;

				if(FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				{
					// do nothing
				}
				else if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{ 
					// create the folder structure, but do not load its contents
					if (fileName[0] != '.') 
						dirs[fileName] = new Directory(p, false);
				} 
				else 
				{ 
					files[fileName.substr(0, fileName.find("."))] = resdir + p;
				} 
				
			}
			while(FindNextFile(hFind, &FindData));
		}
		else
		{
			_mkdir(Path().c_str());
		}
	}
	else
	{
		// windows root, display list of disk volumes
		const int bufsize = 512;
		TCHAR buffer[bufsize];
		TCHAR* drives = buffer;
        drives[0] = '\0';
		GetLogicalDriveStrings(bufsize, buffer);

		while(drives[0])
		{
			char letter = tolower(drives[0]);
			TCHAR namebuf[bufsize];
			namebuf[0] = '\0';
			// volume info call is really slow, not worth it
			//GetVolumeInformation(drives, namebuf, bufsize, NULL, NULL, NULL, NULL, 0);
			string name(namebuf);
			dirs[string(&letter,1)+string(": ")+name] = Directory(string(&letter,1)+string(":"), false);
			drives = drives + 4;
		}
	}
}
