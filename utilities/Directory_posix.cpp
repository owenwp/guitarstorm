#include "Directory.h"
#include <dirent.h>

// relative location of resource files
#ifdef MACOSX
string Directory::resdir = "../resources/";
#else
string Directory::resdir = "";
#endif

// fill the contents map with all of the files in the given directory
Directory::Directory(string n)
{
}
