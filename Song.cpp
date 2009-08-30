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

void operator>>(const YAML::Node &stream, Song &ob)
{
	stream["Title"] >> ob.title;
	stream["Artist"] >> ob.artist;
	stream["Backing"] >> ob.backing;
	stream["Path"] >> ob.path;

	stream["Difficulty"][0] >> ob.difficulty[0];
	stream["Difficulty"][1] >> ob.difficulty[1];
	stream["Difficulty"][2] >> ob.difficulty[2];
	stream["Difficulty"][3] >> ob.difficulty[3];
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

void operator>>(const YAML::Node &stream, Difficulty &ob)
{
	bool used;
	
	stream["Used"] >> used;
	
	if(used)
	{
		ob.used = "true";
		
		stream["Track1"] >> ob.track1;
		stream["Track2"] >> ob.track2;
		stream["Offset"] >> ob.offset;
	}
}
