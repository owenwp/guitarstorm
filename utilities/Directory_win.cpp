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
