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
#ifndef TUNERMENU
#define TUNERMENU

#include "Menu.h"
#include "Tuner.h"

struct Song;

class TunerMenu : public Menu
{	
public:
	TunerMenu(string n, float* s):Menu(n),strings(s),tuner(NULL) {}

protected:
	void OnOpen();

	float* strings;
	Tuner* tuner;
};

#endif
