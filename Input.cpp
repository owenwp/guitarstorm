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
#include "Input.h"

Guitar* Input::guitar = NULL;
Menu* Input::menu = NULL;

void Input::keyChange(int key,int value)
{
    /*if (value)
    {    
		if(guitar)
		{
			// handle speed events
			if(key >= '0' && key <= '9')
			{
				guitar->setSpeed((key - '0') * 10);
			}
			else if (key==osgGA::GUIEventAdapter::KEY_BackSpace || key==osgGA::GUIEventAdapter::KEY_Delete) 
			{
				guitar->setSpeed(100);
			}
		}
		
		if(menu)
		{
			// handle menu events
			if(key == osgGA::GUIEventAdapter::KEY_Up)
			{
				menu->Up();
			}
			else if(key == osgGA::GUIEventAdapter::KEY_Down)
			{
				menu->Down();
			}
			else if(key == osgGA::GUIEventAdapter::KEY_Left)
			{
				menu->Left();
			}
			else if(key == osgGA::GUIEventAdapter::KEY_Right)
			{
				menu->Right();
			}
			else if (key==osgGA::GUIEventAdapter::KEY_Return)
			{
				menu->Select();
			} 
			else if(key < 256)
			{
				menu->Key((char)key);
			}
		}
    }*/
}

void Input::mouseMove(float x, float y)
{
	if(menu)// && fabs(x - lastx) < 500 && fabs(x - lasty) < 500) 
		menu->Mouse(x, y);
	//lastx = x;
	//lasty = y;
}

void Input::mouseClick(int b)
{
	if(menu) menu->Select();
}
