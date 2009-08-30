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
#include "Song.h"

using namespace YAML;

int vmaj = 0;
int vmin = 0;

Emitter& operator<<(Emitter& stream, Song ob)
{
	stream << BeginMap;

	stream << Key << "Title" << Value << ob.title;
	stream << Key << "Artist" << Value << ob.artist;
	stream << Key << "Backing" << Value << ob.backing;
	stream << Key << "Path" << Value << ob.path;
	stream << Key << "Difficulty" << Value << BeginSeq;
	{
		stream << ob.difficulty[0];
		stream << ob.difficulty[1];
		stream << ob.difficulty[2];
		stream << ob.difficulty[3];
	}
	stream << EndSeq;
	
	stream << EndMap;
	return stream;
}

istream& operator>>(istream &stream, Song &ob)
{
	char str[500];
	stream >> str;
	if(strncmp(str, "Song", 30))
		return stream;

	// do serialization
	stream >> vmaj;
	stream >> vmin;

	if(vmaj == Song::versionMaj && vmin == Song::versionMin)
	{
		stream.ignore();
		stream.getline(str, 500);
		ob.title = str;
		stream.getline(str, 500);
		ob.artist = str;
		stream.getline(str, 500);
		ob.backing = str;
		stream.getline(str, 500);
		ob.path = str;

		stream >> ob.difficulty[0];
		stream >> ob.difficulty[1];
		stream >> ob.difficulty[2];
		stream >> ob.difficulty[3];
	}

	return stream;
}

Emitter& operator<<(Emitter &stream, Difficulty ob)
{
	stream << BeginMap;
	
	if(ob.used.length())
	{
		stream << Key << "Used" << Value << true;
		stream << Key << "Track1" << Value << ob.track1;
		stream << Key << "Track2" << Value << ob.track2;
		stream << Key << "Offset" << Value << ob.offset;
	}
	else
		stream << Key << "Used" << Value << false;

	stream << EndMap;
	
	return stream;
}

istream& operator>>(istream &stream, Difficulty &ob)
{
	char str[30];
	int num;
	stream >> str;
	ob.name = str;

	stream >> num;
	if(!num)
		return stream;
	else
	{
		ob.used = "true";
	}
	stream >> ob.track1;
	stream >> ob.track2;
	stream >> ob.offset;
	
	return stream;
}
