/* OpenSceneGraph example, osgkeyboard.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
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
		cap->start();
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
