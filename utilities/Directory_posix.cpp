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
#include <dirent.h>
#include <sys/stat.h>
#include <iostream>
#include <errno.h>

// relative location of resource files
#if MACOSX
//string Directory::resdir = "guitarstorm.app/Contents/Resources/";
#else
//string Directory::resdir = "";
#endif

void Directory::Load()
{
}

// fill the contents map with all of the files in the given directory
/*Directory::Directory(string n)
{
	long size = pathconf(".", _PC_PATH_MAX);
	char *buf;
	if ((buf = (char *)malloc((size_t)size)) != NULL) {
		getcwd(buf, (size_t)size);
		cout << buf << endl;
		free(buf);
	}


	cout << "Directory: " << (resdir + n) << endl; 
	DIR *dirp = opendir((resdir + n).c_str());
	if(dirp != NULL)
	{
		struct dirent *dp = NULL;
		
		while((dp = readdir(dirp)) != NULL)
		{
			struct stat file_stats;
			
			string name(dp->d_name);
			
			cout << (resdir + n + "/" + name) << endl;
			
			if(name == "." || name == "..")
				continue;
			
			if(stat((resdir + n + "/" + name).c_str(), &file_stats) == 0)
			{
				if(S_ISDIR(file_stats.st_mode))
				{
					string path = n + "/" + name;
					Directory dir(path);
					dirs[name] = dir;
				}
				else
				{
					cout << "Adding " << name << endl;
					files[name] = resdir + n + "/" + name;
				}
			}
			
		}
		
		closedir(dirp);
	}
}*/
