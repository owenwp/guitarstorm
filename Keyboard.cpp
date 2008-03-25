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
#include "Keyboard.h"

void KeyboardModel::keyChange(int key,int value)
{
    osg::notify(osg::INFO) << "key value change, code="<<std::hex << key << "\t value="<< value << std::dec  << std::endl;
    
    if (value)
    {    
		// handle speed events
		if(key >= '0' && key <= '9')
		{
			notes->setSpeed((key - '0') * 10);
		}
		else if (key==osgGA::GUIEventAdapter::KEY_BackSpace || key==osgGA::GUIEventAdapter::KEY_Delete) 
        {
			notes->setSpeed(100);
        }
        
		// handle menu events
		else if(key == osgGA::GUIEventAdapter::KEY_Up)
		{
			if(!notes->running) 
			{
				picker->up();
			}
		}
		else if(key == osgGA::GUIEventAdapter::KEY_Down)
		{
			if(!notes->running) 
			{
				picker->down();
			}
		}
        else if (key==osgGA::GUIEventAdapter::KEY_Return)
        {
			string n = picker->get();
			if(!notes->running) 
			{
				picker->visible(false);
				notes->setSong(n);
			}
			else if(notes->finished)
			{
				notes->running = false;
				notes->finished = false;
				notes->visible(false);

				picker->visible(true);
			}
        }        
    }
    
}
