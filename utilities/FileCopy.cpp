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
#include "FileCopy.h"

bool CaseInsensitiveCompare(string str1, string str2)
{
   for(unsigned int i=0;i<str1.length();i++)
   {
      str1[i] = tolower(str1[i]);
   }
   for(unsigned int j=0;j<str2.length();j++)
   {
      str2[j] = tolower(str2[j]);
   }
   return str1.compare(str2)==0;
} 

int FileCopy(char* source, unsigned long length, string destination)
{
	ofstream out;
	out.open(destination.c_str(), ios_base::binary);

	if(out.fail())
		return -1;

	// do the copy
	out.write(source,length);

	out.close();
	return 0;
}

int FileCopy(string source, string destination)
{
	if(CaseInsensitiveCompare(source, destination))
		return 1;

	ifstream in;
	in.open(source.c_str(), ios_base::binary);
	ofstream out;
	out.open(destination.c_str(), ios_base::binary);

	if(in.fail() || out.fail())
		return -1;

	char *byte = new char;

	// do the copy
	while(true)
	{
		in.read(byte,1);
		if(in.eof())
			break;

		out.write(byte,1);
	}

	in.close();
	out.close();
	return 0;
}
