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

#include "Audio.h"
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

bool isFullscreen;

GLint v, f, p; 
Node* root;
Menu* menu;
Label* fps;

float ftimes[20];
int findex = 0;
bool fready = false;

void deleteScene()
{
	delete root;
	root = NULL;
	
	Texture::UnloadAll();
	Font::UnloadAll();
	
	glDetachShader(p, v);
	glDetachShader(p, f);
	
	glDeleteShader(v);
	glDeleteShader(f);
	glDeleteProgram(p);
	
	exit(0);
}

void setupView()
{	
	int resx = Options::instance->resolutionx[Options::instance->screenResolution];
	int resy = Options::instance->resolutiony[Options::instance->screenResolution];
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	
	if(isFullscreen = Options::instance->fullScreen)
	{
		glutGameModeString(string(Options::instance->resolutions[Options::instance->screenResolution]+":32").c_str());
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
		{
			glutEnterGameMode();
		}
		else
		{
			glutGameModeString(string(Options::instance->resolutions[0]+":32").c_str());
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) 
			{
				Options::instance->screenResolution = 0;
				Options::instance->Save();
				glutEnterGameMode();
			}
			else
			{
				deleteScene();
			}
		}
	}
	else
	{
		glutInitWindowPosition(100, 50); 
		glutInitWindowSize(resx, resy); 
		glutCreateWindow("Guitar Storm");
	}
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45, (float)resx / (float)resy, 1, 20);
	
	glMatrixMode(GL_MODELVIEW);
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void makeMenus()
{
	float left = -2;
	float left2 = -4;
	
	menu = new Menu("GUITAR STORM");
	Menu* play = new Menu("Play");
	Menu* manage = new Menu("Manage");
	Menu* tuner = new TunerMenu("Tuner", NULL);
	Menu* options = new Menu("Options");
	
	Menu* audio = new OptionsMenu("Audio");
	Menu* video = new OptionsMenu("Video");
	Menu* game = new OptionsMenu("Game");
	Menu* player = new OptionsMenu("Player");
	Menu* about = new Menu("About");
	
	menu->Add(new MenuItem("Play", play, left, 1));
	menu->Add(new MenuItem("Manage Songs", manage, left, 0));
	menu->Add(new MenuItem("Tune Guitar", tuner, left, -1));
	menu->Add(new MenuItem("Options", options, left, -2));
	menu->Add(new EventItem("Quit", deleteScene, left, -4));
	
	play->Add(new SongListItem("<- Play ->", 0, -2, true));
	manage->Add(new SongListItem("<- Manage ->", 0, -2, false));
	
	options->Add(new MenuItem("Audio Settings", audio, left, 2));
	options->Add(new MenuItem("Video Settings", video, left, 1));
	options->Add(new MenuItem("Game Settings", game, left, 0));
	options->Add(new MenuItem("Player Settings", player, left, -1));
	options->Add(new MenuItem("About", about, left, -2));
	
	audio->Add(new NumberItem("Guitar Volume", &Options::instance->guitarVolume, 100, left2, 2));
	audio->Add(new NumberItem("Backing Volume", &Options::instance->backingVolume, 100, left2, 1));
	audio->Add(new NumberItem("Metronome Volume", &Options::instance->metronomeVolume, 100, left2, 0));
	audio->Add(new NumberItem("Input Device", &Options::instance->inputDevice, Options::instance->devices, left2, -1));
	audio->Add(new NumberItem("Output Device", &Options::instance->outputDevice, Options::instance->devices, left2, -2));
	audio->Add(new NumberItem("Sample Rate", &Options::instance->sampleRate, Options::instance->srates, left2, -3));
	
	video->Add(new NumberItem("Video Resolution", &Options::instance->screenResolution, Options::instance->resolutions, left2, 2));
	video->Add(new NumberItem("Fullscreen", &Options::instance->fullScreen, Options::instance->noyes, left2, 1));
	video->Add(new NumberItem("Detail Level", &Options::instance->detailLevel, Options::instance->details, left2, 0));
	
	game->Add(new NullItem("Default Guitar Distortion", left2, 2, false));
	game->Add(new NullItem("Default Stomp Box Effect", left2, 1, false));
	game->Add(new MenuItem("Song Folder", new DirPick("Directory for Songs", new Directory, &Options::instance->songDir), left2, 0));
	game->Add(new MenuItem("Backing Track Folder", new DirPick("Directory for MP3s", new Directory, &Options::instance->backingDir), left2, -1));
	game->Add(new MenuItem("Effect Folder", new DirPick("Directory for VST Plugins", new Directory, &Options::instance->fxDir), left2, -2));
	game->Add(new MenuItem("Tab Folder", new DirPick("Directory for Guitar Pro Tabs", new Directory, &Options::instance->tabDir), left2, -3));
	
	player->Add(new NullItem("Add Friend", left2, 2, false));
	player->Add(new NullItem("Manage Friends", left2, 1, false));
	player->Add(new NullItem("Edit Profile", left2, 0, false));
	player->Add(new NullItem("Change Password", left2, -1, false));
	player->Add(new NullItem("Change Email Address", left2, -2, false));
	player->Add(new NullItem("Delete Account", left2, -3, false));
	
	about->Add(new NullItem("GUITAR STORM", left, 2, false));
	about->Add(new NullItem("Prototype 0.1", left, 1, false));
	about->Add(new NullItem("Copyright 2008 Zombie Process, Owen Pedrotti", left2, -1, false));
	about->Add(new NullItem("http://guitarstormgame.com/", left2, -2, false));
	about->Add(new NullItem("http://guitarstorm.googlecode.com/", left2, -3, false));
}

void keyPressed(unsigned char key, int x, int y) 
{
	if (key == 27) 
		deleteScene();
	else if (key == 13)
		menu->Select();
	else
		menu->Key((char)key);

}

void arrowPressed(int key, int x, int y) 
{	
	switch(key) 
	{
		case GLUT_KEY_UP:
			menu->Up();
			break;
		case GLUT_KEY_DOWN:
			menu->Down();
			break;
		case GLUT_KEY_LEFT:
			menu->Left();
			break;
		case GLUT_KEY_RIGHT:
			menu->Right();
			break;
	};
}

void renderScene() 
{	
	static int lastTime = glutGet(GLUT_ELAPSED_TIME);
	int time = glutGet(GLUT_ELAPSED_TIME);
	
	float timeDelta = min(0.1f, (time - lastTime) * 0.001f);
	lastTime = time;
	
	if(!root)
		return;
	
	// fps display
	ftimes[findex++] = 1 / timeDelta;
	if(findex >= 20)
	{
		fready = true;
		findex = 0;
	}
	if(fready)
	{
		fready = false;
		int ftotal = 0;
		for(int i=0; i<20; i++)
			ftotal += ftimes[i];
		
		std::ostringstream s;
		s << "FPS: ";
		s << ftotal / 20;
		fps->printf(s.str());
	}
	
	root->update(timeDelta);
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	glTranslatef(0, 0, -11);
	
	switch (Options::instance->detailLevel) 
	{
		case 0:
			glUseProgram(0);
			root->render(0);
			break;
		default:
			glEnable(GL_BLEND);
			glDisable(GL_ALPHA_TEST);
			glUseProgram(p);
			root->render(p);
			break;
	}
	
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
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	
	glEnable(GL_TEXTURE_2D);
	
	glAlphaFunc(GL_GREATER,0.5f);

	glUseProgram(p);
}

void blah()
{
	cout << "blah";
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	ilInit();
	
	Options::instance->Load();
	
	// setup viewer
	setupView();
	loadShaders();

	// setup audio
	Audio::init();

	// setup scene
	root = new Node;
	
	// background
	Node* clouds = new Node;
	Sprite* spr = new Sprite(new Texture("duskcloudy.jpg"));
	spr->setScroll(vec(0.01, 0));
	spr->setColor(vec(0.5, 0.5, 0.5, 1));
	clouds->addChild(spr);
	clouds->setPosition(vec(0,0,-5));
	clouds->setScale(vec(14, 14));
	root->addChild(clouds);
	
	// fps display
	Node* display = new Node;
	fps = new Label("arial", "FPS: ");
	display->addChild(fps);
	display->setScale(vec(0.25, 0.25));
	display->setPosition(vec(-6, 4.25));
	root->addChild(display);

	// setup menus
	makeMenus();
	menu->setScale(vec(20,20,1));
	menu->setGrow(vec(1,1,1), 2.0);
	menu->setPosition(vec(0,-30));
	menu->setSlide(vec(0,0), 1.0);
	menu->Open();
	root->addChild(menu);

	// setup callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutKeyboardFunc(keyPressed);
	glutSpecialFunc(arrowPressed);
	glutWMCloseFunc(deleteScene);

	// begin audio capture
	try 
	{
#ifndef NOAUDIO
		Audio::startCapture();
#endif
	}
	catch(string err)
	{
	}
	
	// cache textures
	Texture::Cache("body");
	Texture::Cache("neck");
	Texture::Cache("head");
	Texture::Cache("nut");

	// run main loop
	glutMainLoop();

	// shutdown
	Audio::stopCapture();

	return 0;
}
