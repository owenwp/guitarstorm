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

#include "Audio.h"
#include "Input.h"
#include "Menu.h"
#include "SongListItem.h"
#include "TunerMenu.h"
#include "Options.h"
#include "Sprite.h"
#include "Guitar.h"

osgViewer::Viewer *_viewer = NULL;

void quit()
{
	_viewer->setDone(true);
}

void setupView()
{
	int resx = Options::instance->resolutionx[Options::instance->screenResolution];
	int resy = Options::instance->resolutiony[Options::instance->screenResolution];
	if(Options::instance->fullScreen)
	{
		//_viewer->setUpViewOnSingleScreen(0);
		osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
		if (!wsi) 
		{
			//osg::notify(osg::NOTICE)<<"View::setUpViewOnSingleScreen() : Error, no WindowSystemInterface available, cannot create windows."<<std::endl;
			return;
		}

		osg::GraphicsContext::ScreenIdentifier si;
		si.readDISPLAY();
	    
		// displayNum has not been set so reset it to 0.
		if (si.displayNum<0) si.displayNum = 0;

		si.screenNum = 0;

		unsigned int width, height;
		wsi->getScreenResolution(si, width, height);

		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->hostName = si.hostName;
		traits->displayNum = si.displayNum;
		traits->screenNum = si.screenNum;
		traits->x = 0;
        traits->y = 0;
        traits->width = width;
        traits->height = height;
        traits->windowDecoration = false;
        traits->doubleBuffer = true;
		traits->useCursor = false;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
		camera->setViewMatrixAsLookAt(
			osg::Vec3(0,-28,0),
			osg::Vec3(0,0,0),
			osg::Vec3(0,0,1));
		camera->setProjectionMatrixAsPerspective(45, traits->width / (float)traits->height, 1, 100);

		osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
		if (gw)
		{
			gw->getEventQueue()->getCurrentEventState()->setWindowRectangle(traits->x, traits->y, traits->width, traits->height );
		}

        // add this slave camera to the viewer, with a shift left of the projection matrix
		_viewer->setCamera(camera.get());
	}
	else
	{
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
        traits->x = 50;
        traits->y = 50;
        traits->width = resx;
        traits->height = resy;
        traits->windowDecoration = true;
        traits->doubleBuffer = true;
		traits->useCursor = false;
        traits->sharedContext = 0;

        osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

        osg::ref_ptr<osg::Camera> camera = new Camera;
        camera->setGraphicsContext(gc.get());
        camera->setViewport(new Viewport(0,0, traits->width, traits->height));
        GLenum buffer = traits->doubleBuffer ? GL_BACK : GL_FRONT;
        camera->setDrawBuffer(buffer);
        camera->setReadBuffer(buffer);
		camera->setViewMatrixAsLookAt(
			osg::Vec3(0,-28,0),
			osg::Vec3(0,0,0),
			osg::Vec3(0,0,1));
		camera->setProjectionMatrixAsPerspective(45, traits->width / (float)traits->height, 1, 100);

		osgViewer::GraphicsWindow* gw = dynamic_cast<osgViewer::GraphicsWindow*>(gc.get());
		if (gw)
		{
			gw->getEventQueue()->getCurrentEventState()->setWindowRectangle(traits->x, traits->y, traits->width, traits->height );
		}

        // add this slave camera to the viewer, with a shift left of the projection matrix
		_viewer->setCamera(camera.get());
	}
}

int main(int argc, char **argv)
{
	osg::ArgumentParser arguments(&argc, argv);

    // construct the viewer.
    osgViewer::Viewer viewer(arguments);
	_viewer = &viewer;

	// setup viewer
	setupView();

	// setup audio
	Audio::init();

	// setup keyboard input
	osg::ref_ptr<Input> input = new Input;

	// setup menus
	osg::ref_ptr<Menu> menu = new Menu("GUITAR STORM");
	osg::ref_ptr<Menu> play = new Menu("Play");
	osg::ref_ptr<Menu> manage = new Menu("Manage");
	osg::ref_ptr<Menu> tuner = new TunerMenu("Tuner", NULL);
	osg::ref_ptr<Menu> options = new Menu("Options");

	osg::ref_ptr<Menu> audio = new OptionsMenu("Audio");
	osg::ref_ptr<Menu> video = new OptionsMenu("Video");
	osg::ref_ptr<Menu> game = new OptionsMenu("Game");
	osg::ref_ptr<Menu> player = new OptionsMenu("Player");
	osg::ref_ptr<Menu> about = new Menu("About");
	Input::setMenu(menu.get());

	menu->Add(new MenuItem("Play", play, -5, 0));
	menu->Add(new MenuItem("Manage Songs", manage, -5, -2));
	menu->Add(new MenuItem("Tune Guitar", tuner, -5, -4));
	menu->Add(new MenuItem("Options", options, -5, -6));
	menu->Add(new EventItem("Quit", &quit, -5, -10));

	play->Add(new SongListItem("<- Play ->", -5, -4, true));
	manage->Add(new SongListItem("<- Manage ->", -5, -4, false));

	options->Add(new MenuItem("Audio Settings", audio, -5, 2));
	options->Add(new MenuItem("Video Settings", video, -5, 0));
	options->Add(new MenuItem("Game Settings", game, -5, -2));
	options->Add(new MenuItem("Player Settings", player, -5, -4));
	options->Add(new MenuItem("About", about, -5, -6));

	audio->Add(new NumberItem("Guitar Volume", &Options::instance->guitarVolume, 100, -10, 8));
	audio->Add(new NumberItem("Backing Volume", &Options::instance->backingVolume, 100, -10, 6));
	audio->Add(new NumberItem("Metronome Volume", &Options::instance->metronomeVolume, 100, -10, 4));
	audio->Add(new NumberItem("Input Device", &Options::instance->inputDevice, Options::instance->devices, -10, 2));
	audio->Add(new NumberItem("Output Device", &Options::instance->outputDevice, Options::instance->devices, -10, 0));
	audio->Add(new NumberItem("Buffer Size", &Options::instance->bufferSize, Options::instance->bsizes, -10, -2));
	audio->Add(new NumberItem("Sample Rate", &Options::instance->sampleRate, Options::instance->srates, -10, -4));

	video->Add(new NumberItem("Video Resolution", &Options::instance->screenResolution, Options::instance->resolutions, -10, 8));
	video->Add(new NumberItem("Fullscreen", &Options::instance->fullScreen, Options::instance->noyes, -10, 6));
	video->Add(new NumberItem("Detail Level", &Options::instance->detailLevel, Options::instance->details, -10, 4));

	game->Add(new NullItem("Guitar String Colors", -10, 8, false));
	game->Add(new NullItem("Default Guitar Distortion", -10, 6, false));
	game->Add(new NullItem("Default Stomp Box Effect", -10, 4, false));
	game->Add(new MenuItem("Song Folder", new DirPick("Directory for Songs", new Directory, &Options::instance->songDir), -10, 2));
	game->Add(new MenuItem("Backing Track Folder", new DirPick("Directory for MP3s", new Directory, &Options::instance->backingDir), -10, 0));
	game->Add(new MenuItem("Effect Folder", new DirPick("Directory for VST Plugins", new Directory, &Options::instance->fxDir), -10, -2));
	game->Add(new MenuItem("Tab Folder", new DirPick("Directory for Guitar Pro Tabs", new Directory, &Options::instance->tabDir), -10, -4));

	player->Add(new NullItem("Add Friend", -10, 8, false));
	player->Add(new NullItem("Manage Friends", -10, 6, false));
	player->Add(new NullItem("Edit Profile", -10, 4, false));
	player->Add(new NullItem("Change Password", -10, 2, false));
	player->Add(new NullItem("Change Email Address", -10, 0, false));
	player->Add(new NullItem("Delete Account", -10, -2, false));

	about->Add(new NullItem("GUITAR STORM", -5, 4, false));
	about->Add(new NullItem("Prototype 0.1", -5, 2, false));
	about->Add(new NullItem("Copyright 2008 Zombie Process, Owen Pedrotti", -15, -2, false));
	about->Add(new NullItem("http://guitarstormgame.com/", -8, -4, false));
	about->Add(new NullItem("http://guitarstorm.googlecode.com/", -8, -6, false));

	menu->Open();

	// setup scene
	osg::Group* root = NULL;
    osg::ClearNode* backdrop = new osg::ClearNode;
    backdrop->setClearColor(osg::Vec4(0.1f,0.1f,0.1f,1.0f));
	root = new osg::Group();
	root->addChild(menu->getScene());
    root->addChild(backdrop);
	viewer.setSceneData( root );

	// background
	Sprite* clouds = new Sprite("duskcloudy.jpg");
	clouds->setScale(Vec2(20, 20));
	clouds->setPosition(Vec3(0,20,0));
	clouds->setColor(Vec4(0.5f, 0.5f, 0.5f, 1));
	clouds->setScroll(Vec2(-0.02f,0));
	root->addChild(clouds);

	// setup callbacks
    viewer.addEventHandler(new osgViewer::StatsHandler);
    viewer.addEventHandler(new osgViewer::WindowSizeHandler());
    viewer.addEventHandler(new InputEventHandler(input.get()));

	// begin audio capture
	try 
	{
#ifndef NOAUDIO
		Audio::startCapture();
#endif
	}
	catch(string err)
	{
		// setup interface
		osg::Geode* geodeErr = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> errText = new osgText::Text;
			errText->setFont("fonts/arial.ttf");
			errText->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
			errText->setCharacterSize(1.5f);
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

	// run main loop
	while (!viewer.done()) 
	{  
		viewer.frame(); 
	}

	// shutdown
	Audio::stopCapture();

	return 0;
}
