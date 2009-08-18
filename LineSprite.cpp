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
#include "LineSprite.h"

LineSprite::LineSprite(LineSprite* sprite) : Sprite(sprite)
{ 
	line = sprite->line;
	makeline = true;
}

LineSprite::LineSprite(string filename, bool absolute) : Sprite(new Texture(0))
{
	line = vec(0, 1, 0);
	makeline = true;
}

void LineSprite::Update()
{
	/*if(makeline)
	{
		makeline = false;

		float len = line.length();
		float ang = acos((line/len) * Vec3(0,0,1));
		if(line.x() < 0)
			ang = -ang;

		Vec3Array* coords = new Vec3Array(4);
		(*coords)[0].set(-0.5f, 0, 0);
		(*coords)[1].set(0.5f, 0, 0);
		(*coords)[2].set(0.5f, 0, len);
		(*coords)[3].set(-0.5f, 0, len);
		geom->setVertexArray(coords);

		float tile = scale.y();

		(*tcoords)[0].set(0.0f,0.0f);
		(*tcoords)[1].set(1.0f,0.0f);
		(*tcoords)[2].set(1.0f,len/tile);
		(*tcoords)[3].set(0.0f,len/tile);
		geom->setTexCoordArray(0,tcoords);

		rotation = ang;
	}

	if(scale.y() != 1)
		scale.y() = 1;

	Sprite::Update();*/
}
