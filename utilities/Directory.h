#ifndef DIRECTORY_
#define DIRECTORY_

#include <string>
#include <map>

using namespace std;

class Directory
{
public:
	Directory(string n);

	map<string, string> contents;

private:
	static string resdir;
};

#endif
