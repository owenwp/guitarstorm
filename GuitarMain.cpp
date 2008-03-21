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

int main(int argc, char *argv[])
{
	int samples = 128;
	if(argc == 2)
	{
		samples = atoi(argv[1]);
	}
	// setup viewer
    osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(10, 50, 1024, 768, 0);

	// setup keyboard input
	osg::ref_ptr<KeyboardModel> keyboardModel = new KeyboardModel;
	//keyboardModel->visible(true);

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
	cap->start();

	viewer.realize();

	viewer.getCamera()->setViewMatrixAsLookAt(
		osg::Vec3(0,5,0),
		osg::Vec3(0,0,0),
		osg::Vec3(0,0,1));

	// run main loop
    viewer.run();

	// shutdown
	cap->stop();

	return 0;
}
