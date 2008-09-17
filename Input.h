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
#ifndef INPUT
#define INPUT

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <osgText/Text>

#include "Guitar.h"
#include "Menu.h"

class Input : public osg::Referenced
{
public:        
        void keyChange(int key,int value);
		void mouseMove(float x, float y);
		void mouseClick(int b);

		static void setMenu(Menu* m) {menu = m;}
		static void setGuitar(Guitar* g) {guitar = g;}
        
protected:

		static Guitar* guitar;
		static Menu* menu;
        
		float lastx;
		float lasty;

        ~Input() {}
};

class InputEventHandler : public osgGA::GUIEventHandler
{
public:
    
        InputEventHandler(Input* input):
            _input(input) {}
    
		virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
        {
			osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
			if (!viewer) return false;

            switch(ea.getEventType())
            {
				case(osgGA::GUIEventAdapter::RELEASE):
				{
					_input->mouseMove(ea.getX(), ea.getY());
					_input->mouseClick(ea.getButton());
					return true;
				}
				case(osgGA::GUIEventAdapter::MOVE):
				case(osgGA::GUIEventAdapter::PUSH):
				{
					osg::Viewport* viewport = viewer->getCamera()->getViewport();
					float mx = viewport->x() + (int)((float)viewport->width()*(ea.getXnormalized()*0.5f+0.0f));
					float my = viewport->y() + (int)((float)viewport->height()*(ea.getYnormalized()*0.5f+0.0f));

					_input->mouseMove(mx, my);
					return true;
				}
                case(osgGA::GUIEventAdapter::KEYDOWN):
                {
                    _input->keyChange(ea.getKey(),1);
                    return true;
                }
                case(osgGA::GUIEventAdapter::KEYUP):
                {
                    _input->keyChange(ea.getKey(),0);
                    return true;
                }

                default:
                    return false;
            }
        }
        
        osg::ref_ptr<Input> _input; 
};

#endif
