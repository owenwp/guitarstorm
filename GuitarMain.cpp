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
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <OpenThreads/Thread>

#include <osgText/Text>

#include "Capture.h"
#include "Notes.h"
#include "Keyboard.h"
#include "SongPick.h"

int main(int argc, char **argv)
{
	osg::ArgumentParser arguments(&argc, argv);

    // construct the viewer.
    osgViewer::Viewer viewer(arguments);
	
	// parse command line
	int samples = 128;
    arguments.read("-samples", samples);

	// setup viewer
	viewer.setUpViewInWindow(10, 50, 1024, 768, 0);

	// setup keyboard input
	osg::ref_ptr<KeyboardModel> keyboardModel = new KeyboardModel;

	// setup song picker
	osg::ref_ptr<SongPick> pick = new SongPick;
	keyboardModel->setPicker(pick.get());

	// setup note chart
	osg::ref_ptr<Notes> notes = new Notes;
	keyboardModel->setNotes(notes.get());

	// setup scene
	osg::Group* root = NULL;
    osg::ClearNode* backdrop = new osg::ClearNode;
    backdrop->setClearColor(osg::Vec4(0.1f,0.1f,0.1f,1.0f));
	root = new osg::Group();
	root->addChild(keyboardModel->getScene());
	root->addChild(pick->getScene());
    root->addChild(notes->getScene());
    root->addChild(backdrop);
	viewer.setSceneData( root );

	// setup callbacks
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler());
    viewer.addEventHandler(new KeyboardEventHandler(keyboardModel.get()));

	// begin audio capture
	Capture* cap = new Capture(samples);
	notes->scorer = new Scorer(cap);
	try 
	{
		//cap->start();
	}
	catch(string err)
	{
		// setup interface
		osg::Geode* geodeErr = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> errText = new osgText::Text;
			errText->setFont("fonts/arial.ttf");
			errText->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
			errText->setCharacterSize(2.0f);
			errText->setPosition(osg::Vec3(0.0f,-1.0f,6.0f));
			errText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
			errText->setDrawMode(osgText::Text::TEXT);
			errText->setAlignment(osgText::Text::CENTER_TOP);
			errText->setAxisAlignment(osgText::Text::XZ_PLANE);
			errText->setText(err);
	        
			geodeErr->addDrawable(errText.get());
	        
			root->addChild(geodeErr);
		}
	}

	// setup camera
	viewer.realize();
	viewer.getCamera()->setViewMatrixAsLookAt(
		osg::Vec3(0,-50,0),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1));

	// run main loop
	while (!viewer.done()) 
	{  
		viewer.frame(); 
	}

	// shutdown
	cap->stop();

	return 0;
}
