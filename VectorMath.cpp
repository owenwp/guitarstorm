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

#include "VectorMath.h"

vec::vec()
{
	x = 0;
	y = 0;
	z = 0;
	w = 1;
}

vec::vec(float _x, float _y)
{
	x = _x;
	y = _y;
	z = 0;
	w = 1;
}

vec::vec(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
	w = 1;
}

vec::vec(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

const vec vec::operator+(const vec &v) const
{
	return vec(x+v.x, y+v.y, z+v.z, w);
}

const vec vec::operator-(const vec &v) const
{
	return vec(x-v.x, y-v.y, z-v.z, w);
}

const vec vec::operator*(const float &s) const
{
	return vec(x*s, y*s, z*s);
}

const vec vec::operator/(const float &s) const
{
	return vec(x/s, y/s, z/s);
}

const float vec::operator*(const vec &v) const
{
	return x*v.x + y*v.y + z*v.z;
}

const vec vec::operator%(const vec &v) const
{
	return vec(y * v.z - z * v.y,
			   z * v.x - x * v.z,
			   x * v.y - y * v.x);
}

vec& vec::operator+=(const vec &v)
{	
	x += v.x;
	y += v.y;
	z += v.z;
	
	return *this;
}

vec& vec::operator-=(const vec &v)
{	
	x -= v.x;
	y -= v.y;
	z -= v.z;
	
	return *this;
}

vec& vec::operator*=(const float &s)
{	
	x *= s;
	y *= s;
	z *= s;
	
	return *this;
}

vec& vec::operator/=(const float &s)
{	
	x /= s;
	y /= s;
	z /= s;
	
	return *this;
}

// cross product
vec& vec::operator%=(const vec &v)
{	
	x = y * v.z - z * v.y;
	y = z * v.x - x * v.z;
	z = x * v.y - y * v.x;
	
	return *this;
}
