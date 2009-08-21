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

#include <GLUT/GLUT.h>
#include <IL/il.h>

//#include "Audio.h"
#include "Input.h"
#include "Menu.h"
#include "SongListItem.h"
#include "TunerMenu.h"
#include "Options.h"
#include "Sprite.h"
#include "Guitar.h"
#include "Texture.h"
#include "Node.h"
#include "Label.h"

bool useShaders = true;

GLint v, f, p; 
Node* root;

void quit()
{
	//_viewer->setDone(true);
}

void setupView()
{
	int resx = Options::instance->resolutionx[Options::instance->screenResolution];
	int resy = Options::instance->resolutiony[Options::instance->screenResolution];
	if(Options::instance->fullScreen)
	{
	}
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 50); 
	glutInitWindowSize(800, 600); 
	glutCreateWindow("Guitar Storm");
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void deleteScene()
{
	delete root;
	root = NULL;
	
	Texture::UnloadAll();
	
	glDetachShader(p, v);
	glDetachShader(p, f);
	
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
	
	exit(0);
}

void renderScene() 
{	
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int time = glutGet(GLUT_ELAPSED_TIME);
	
	float timeDelta = (time - lastTime) * 0.001f;
	lastTime = time;
	
	if(!root)
		return;
	
	root->update(timeDelta);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0, 0, -11);
	
	root->render(p);
	
	glutSwapBuffers();
}

void openShader(GLint shader, string name)
{
	string folder = "shaders/";
	name = folder + name;
	ifstream in;
	in.open(name.c_str(), ios::in);
	
	char cstr[128];
	
	string str;
	
	do
	{
		in.get(cstr, 128, 0);
		str += string(cstr);
	}
	while(in.gcount() == 127);
	
	cout << endl << "*** " << name << " ***" << endl << str;
	
	const GLchar* c = str.c_str();
	
	glShaderSource(shader, 1, &c, NULL);
}

void loadShaders()
{
	char *vs,*fs;
	
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);	
	
	openShader(v,"Sprite.vert");
	openShader(f,"Sprite.frag");
	
	glCompileShader(v);
	glCompileShader(f);
	
	p = glCreateProgram();
	
	glAttachShader(p,v);
	glAttachShader(p,f);
	
	glLinkProgram(p);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)800 / (float)600, 1, 20);
	
	glMatrixMode(GL_MODELVIEW);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	
	glEnable(GL_TEXTURE_2D);
	
	if(!useShaders)
	{
		glAlphaFunc(GL_GREATER,0.5f);
		glEnable(GL_ALPHA_TEST);
	}
	else
	{
		glEnable (GL_BLEND);
		glUseProgram(p);
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	ilInit();
	
	// setup viewer
	setupView();
	loadShaders();

	// setup audio
	//Audio::init();

	// setup keyboard input
	Input* input = new Input;

	// setup menus
	Menu* menu = new Menu("GUITAR STORM");
	Menu* play = new Menu("Play");
	Menu* manage = new Menu("Manage");
	Menu* tuner = new TunerMenu("Tuner", NULL);
	Menu* options = new Menu("Options");

	Menu* audio = new OptionsMenu("Audio");
	Menu* video = new OptionsMenu("Video");
	Menu* game = new OptionsMenu("Game");
	Menu* player = new OptionsMenu("Player");
	Menu* about = new Menu("About");
	Input::setMenu(menu);

	menu->Add(new MenuItem("Play", play, -2, 1));
	menu->Add(new MenuItem("Manage Songs", manage, -2, 0));
	menu->Add(new MenuItem("Tune Guitar", tuner, -2, -1));
	menu->Add(new MenuItem("Options", options, -2, -2));
	menu->Add(new EventItem("Quit", &quit, -2, -4));

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
	root = new Node;
	root->addChild(menu);

	// background
	Node* clouds = new Node;
	Sprite* spr = new Sprite(new Texture("duskcloudy.jpg"));
	spr->setScroll(vec(0.01, 0));
	clouds->addChild(spr);
	clouds->setPosition(vec(0,0,-5));
	clouds->setScale(vec(14, 14));
	root->addChild(clouds);

	// setup callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutWMCloseFunc(deleteScene);

	// begin audio capture
	try 
	{
#ifndef NOAUDIO
		//Audio::startCapture();
#endif
	}
	catch(string err)
	{
	}

	// run main loop
	glutMainLoop();

	// shutdown
	//Audio::stopCapture();

	return 0;
}
