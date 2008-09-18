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
#include "Guitar.h"

const float Guitar::tolerance = 0.03f;
const float Guitar::nutpos = -3.55f;
const float Guitar::neckscale = 0.015f;
const float Guitar::neckpos = -0.035f;
const float Guitar::bridgepos = 6.67f;
const float Guitar::bridgeheight = -0.05f;
const float Guitar::neckspace = 0.115f;
const float Guitar::bridgespace = 0.17f;

Guitar::Guitar()
{
	scene = new Group();
	MakeGuitar();
	scene->addChild(sprite);

	scene->setUserData(this); 
	scene->setUpdateCallback(new guitarCallback);

	running = false; 
	finished = false;

	// create fret textures
	snote = new Sprite("circle.tga");
	snote->setScale(Vec2(0.1f, 0.1f));
	for(int i=0; i<25; i++)
	{	
		string fret;
		stringstream str;
		str << i;
		str >> fret;

		tnote[i] = new osgText::Text;
		tnote[i]->setFont("fonts/arial.ttf");
		//tnote[i]->setDataVariance(Object::DYNAMIC);
		if(i == 3 || i == 5 || i == 7 || i == 9 || i == 12 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23) 
		{
			tnote[i]->setColor(Vec4(1,1,1,1));
			tnote[i]->setBackdropColor(Vec4(0,0,0,1));
		}
		else
		{
			tnote[i]->setColor(Vec4(0,0,0,1));
			tnote[i]->setBackdropColor(Vec4(1,1,1,1));
		}
		if(i < 10)
			tnote[i]->setCharacterSize(1.5f);
		else
			tnote[i]->setCharacterSize(1.1f);
		tnote[i]->setPosition(Vec3(0,-0.1f,0));
		tnote[i]->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		tnote[i]->setBackdropType(osgText::Text::OUTLINE);
		tnote[i]->setBackdropOffset(-0.05f, 0.0f);
		tnote[i]->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		tnote[i]->setDrawMode(osgText::Text::TEXT);
		tnote[i]->setAlignment(osgText::Text::CENTER_CENTER);
		tnote[i]->setAxisAlignment(osgText::Text::XZ_PLANE);
		tnote[i]->setText(fret);
	}

	sbeat = new Sprite("fret.tga");
	sbeat->setScale(Vec2(0.5f, 0.5f));
}

void Guitar::MakeGuitar()
{
	Sprite* neck = new Sprite("neck.tga");
	neck->setCenter(Vec3(3,0,0));

	Sprite* head = new Sprite("head.tga");
	head->setPosition(Vec3(-4.5, -0.1f, 0));

	Sprite* nut = new Sprite("nut.tga");
	nut->setPosition(Vec3(nutpos, -0.2f, neckpos));
	nut->setScale(Vec2(0.5f, 0.5f));

	Sprite* body = new Sprite("body.tga");
	body->setPosition(Vec3(5.75f, 0.1f, 0));
	body->setScale(Vec2(4,4));

	chart = new Group();
	beats = new Group();

	neck->addChild(chart);
	neck->addChild(beats);

	neck->addChild(body);
	neck->addChild(head);
	neck->addChild(nut);

	strings[0] = new LineSprite("bstring.tga");
	nstring[0] = Vec3(nutpos - 0.025f, -0.25f, neckpos - neckspace * 2.5f);
	bstring[0] = Vec3(bridgepos, -0.25f, bridgeheight - bridgespace * 2.5f);
	strings[0]->setPosition(nstring[0]);
	strings[0]->setScale(Vec2(0.03f,1));
	strings[0]->setLine(bstring[0] - nstring[0]);
	neck->addChild(strings[0]);

	strings[1] = new LineSprite("bstring.tga");
	nstring[1] = Vec3(nutpos - 0.025f, -0.25f, neckpos - neckspace * 1.5f);
	bstring[1] = Vec3(bridgepos, -0.25f, bridgeheight - bridgespace * 1.5f);
	strings[1]->setPosition(nstring[1]);
	strings[1]->setScale(Vec2(0.025f,1));
	strings[1]->setLine(bstring[1] - nstring[1]);
	neck->addChild(strings[1]);

	strings[2] = new LineSprite("bstring.tga");
	nstring[2] = Vec3(nutpos - 0.025f, -0.25f, neckpos - neckspace * 0.5f);
	bstring[2] = Vec3(bridgepos, -0.25f, bridgeheight - bridgespace * 0.5f);
	strings[2]->setPosition(nstring[2]);
	strings[2]->setScale(Vec2(0.02f,1));
	strings[2]->setLine(bstring[2] - nstring[2]);
	neck->addChild(strings[2]);

	strings[3] = new LineSprite("tstring.tga");
	nstring[3] = Vec3(nutpos - 0.025f, -0.25f, neckpos + neckspace * 0.5f);
	bstring[3] = Vec3(bridgepos, -0.25f, bridgeheight + bridgespace * 0.5f);
	strings[3]->setPosition(nstring[3]);
	strings[3]->setScale(Vec2(0.015f,1));
	strings[3]->setLine(bstring[3] - nstring[3]);
	neck->addChild(strings[3]);

	strings[4] = new LineSprite("tstring.tga");
	nstring[4] = Vec3(nutpos - 0.025f, -0.25f, neckpos + neckspace * 1.5f);
	bstring[4] = Vec3(bridgepos, -0.25f, bridgeheight + bridgespace * 1.5f);
	strings[4]->setPosition(nstring[4]);
	strings[4]->setScale(Vec2(0.01f,1));
	strings[4]->setLine(bstring[4] - nstring[4]);
	neck->addChild(strings[4]);

	strings[5] = new LineSprite("tstring.tga");
	nstring[5] = Vec3(nutpos - 0.025f, -0.25f, neckpos + neckspace * 2.5f);
	bstring[5] = Vec3(bridgepos, -0.25f, bridgeheight + bridgespace * 2.5f);
	strings[5]->setPosition(nstring[5]);
	strings[5]->setScale(Vec2(0.008f,1));
	strings[5]->setLine(bstring[5] - nstring[5]);
	neck->addChild(strings[5]);

	LineSprite* nstr = new LineSprite("bstring.tga");
	nstr->setPosition(nstring[0]);
	nstr->setScale(Vec2(0.03f,1));
	nstr->setLine(Vec3(-0.95f, 0, -0.02f));
	neck->addChild(nstr);

	nstr = new LineSprite("bstring.tga");
	nstr->setPosition(nstring[1]);
	nstr->setScale(Vec2(0.025f,1));
	nstr->setLine(Vec3(-1.45f, 0, -0.13f));
	neck->addChild(nstr);

	nstr = new LineSprite("bstring.tga");
	nstr->setPosition(nstring[2]);
	nstr->setScale(Vec2(0.02f,1));
	nstr->setLine(Vec3(-1.95f, 0, -0.27f));
	neck->addChild(nstr);

	nstr = new LineSprite("tstring.tga");
	nstr->setPosition(nstring[3]);
	nstr->setScale(Vec2(0.015f,1));
	nstr->setLine(Vec3(-1.95f, 0, 0.27f));
	neck->addChild(nstr);

	nstr = new LineSprite("tstring.tga");
	nstr->setPosition(nstring[4]);
	nstr->setScale(Vec2(0.01f,1));
	nstr->setLine(Vec3(-1.45f, 0, 0.13f));
	neck->addChild(nstr);

	nstr = new LineSprite("tstring.tga");
	nstr->setPosition(nstring[5]);
	nstr->setScale(Vec2(0.008f,1));
	nstr->setLine(Vec3(-0.95f, 0, 0.02f));
	neck->addChild(nstr);
	
	sprite = neck;
}

void Guitar::Update()
{
	if(!running)
		return;

	double t = time->time_s();
	double delta = t - last;
	if(stopped && delta < 0.2)
		return;

	last = t;
	float d2 = 0;
	double cdelta;
	if(speed != 0)
	{
		d2 = delta * speed;
		realtime += d2;
		cdelta = d2 * spacing;
	}
	else
	{
		if(stopped)
		{
			cdelta = 0;
			//current = stopped;
		}
		else
		{
			d2 = delta;
			realtime += d2;
			cdelta = d2 * spacing;
		}
	}
	current += cdelta;

	if(!playing && realtime > backdelay)
	{
		if(speed == 1)
			playing = !Audio::playMusic();
		else if(d2 > 0)	
			Audio::advanceMusic(d2);
	}

	if(!stopped)
	{
		// place beat lines
		while(bar != tab->b.end() && current + offset > count3)
		{
			if(bar->end > repcount)
			{
				repcount++;
				bar = repb;
				col = repc;
			}

			count3 += 240 / tempo * spacing;
			bar++;

			if(bar->tempo != -1)
				tempo = bar->tempo;

			if(bar->begin)
			{
				repb = bar;
				repc = col;
				repcount = 0;
			}

			if(bar == tab->b.end())
			{
				finished = true;
			}
		}
		while(col != tab->c.end() && current + offset > count2)
		{
			PlaceBeat(count2);
			
			count2 += 60 / tempo * spacing;
		}

		// place new notes
		while(col != tab->c.end() && current + offset > count)
		{
			for(int s=0; s<6; s++)
			{
				if(col->a[s] != -1)
				{
					PlaceNote(count, s, col->a[s]);
					if(col->e[s] & EFFECT_BEND)
					{
						double dur = col->l * 240 / tempo * spacing;
						double t = 0.0;
						for(int i=0; i<col->bend.size(); i++)
						{
							t += col->bend[i].x() / 60.0;
							PlaceNote(count + dur * t, s, 
								col->a[s] + col->bend[i].y()/50);
						}
					}
					break;
				}
			}
			if(col->flags & FLAG_DOT)
				count += 1.5f * col->l * 240 / tempo * spacing;
			else
				count += col->l * 240 / tempo * spacing;
			col++;
		}
	}

	// update positions
	list<Fret> frets;

	for(int i=chart->getNumChildren()-1; i >= 0; i--)
	{
		Sprite* note = ((Sprite*)chart->getChild(i));
		Fret* fr = static_cast<Fret*>(note->getUserData());

		float x = fr->t - current;

		Vec3 pos = note->getPosition();
		pos.x() = x;
		Vec3 str = bstring[fr->s] - nstring[fr->s];
		pos.z() = (nstring[fr->s] + (str * ((x - nstring[fr->s].x())/str.x()))).z();
		//pos.z() = pos.z() - pos.z() * 2 * cdelta * neckscale;
		note->setPosition(pos);
		if(!scorer && pos.x() < nutpos)
		{
			chart->removeChild(i);	// autoplay
		}
		else if(pos.x() < nutpos - tolerance)
		{
            if(fr)
				frets.push_back(*fr);

			float a = 1 + (pos.x() - nutpos - tolerance) / 0.75f;
			if(a <= 0)
				chart->removeChild(i);
			else
				note->setColor(Vec4(0.5f, 0.5f, 0.5f, a));

		}
	}
	
	for(int i=beats->getNumChildren()-1; i >= 0; i--)
	{
		Sprite* beat = ((Sprite*)beats->getChild(i));
		Fret* fr = static_cast<Fret*>(beat->getUserData());

		float x = fr->t - current;

		Vec3 pos = beat->getPosition();
		Vec2 scal = beat->getScale(); // in progress
		pos.x() = x;
		scal.y() = scal.y() - cdelta * neckscale;
		beat->setPosition(pos);
		beat->setScale(scal);
		if(pos.x() < nutpos)
		{
			beats->removeChild(i);
			if(!playing || !scorer)
				Audio::tick();
		}
	}

	// note reached
	if(scorer)
	{
		if(frets.size() > 0)
		{
			scorer->Test(frets);
		}
		if(scorer->HasResults())
		{
			if(speed == 0)
				  stopped = current;

			list<Fret> f;
			f = scorer->GetResult();

			list<Fret>::iterator fi = f.begin();
			for(fi = f.begin(); fi != f.end(); fi++)
			{
				if(!fi->hit)
				{
					combo = 0;
					multiplier = 1;
				}
				else
				{
					chart->removeChild(fi->m);
					if(multiplier < 4)
					{
						combo++;
						if(combo > 8)
						{
							multiplier++;
							if(multiplier < 4)
								combo = 0;
						}
					}
					if(stopped)
						 frets.clear();
					stopped = 0;
					score += 10 * speed * multiplier * f.size();
				}
			}
		}

		// update score text
		std::ostringstream s;
		s	<< "Score:      " << score << "\n"
			<< "Multiplier: " << multiplier << "X ";
		for(int i=0; i< 8; i++)
		{
			if(i<combo)
				s << "+";
			else
				s << "-";
		}
		s << "\nNote: " << scorer->lastnote << " Volume: " << Audio::readVol();
		scoreText->setText(s.str());
	}
}

void Guitar::PlaceNote(double t, int s, int f)
{
	if(f < 0 || f > 24)
		return;

	osg::Vec3 pos = osg::Vec3(3.5f - t, -0.3f, 0.13f*(s-3.0f));

	Sprite* spr = new Sprite(snote);

	Geode* geode = new Geode();
	geode->addDrawable((osgText::Text*)tnote[f]->clone(CopyOp::SHALLOW_COPY));
	spr->addChild(geode);

	spr->setPosition(pos);
	float brightness = 0.7f;
	float dark = 0.1f;
	switch(s)
	{
	case 0:
		spr->setColor(Vec4(brightness, dark, dark, 1));
		break;
	case 1:
		spr->setColor(Vec4(brightness, brightness, dark, 1));
		break;
	case 2:
		spr->setColor(Vec4(dark, brightness, dark, 1));
		break;
	case 3:
		spr->setColor(Vec4(dark, brightness, brightness, 1));
		break;
	case 4:
		spr->setColor(Vec4(dark, dark, brightness, 1));
		break;
	case 5:
		spr->setColor(Vec4(brightness, dark, brightness, 1));
		break;
	}

	Fret* fr = new Fret;
	fr->hit = false;
	fr->s = s;
	fr->f = f;
	fr->t = t;
	spr->setUserData(fr);
	fr->m = spr;
	
	chart->addChild(spr);
}


void Guitar::PlaceBeat(double t)
{
	osg::Vec3 pos = osg::Vec3(3.5f - t, -0.1f, -0.04f);

	Sprite* spr = new Sprite(sbeat);
	spr->setPosition(pos);
	
	Fret* fr = new Fret;
	fr->t = t;
	spr->setUserData(fr);

	beats->addChild(spr);
}

void Guitar::setSpeed(int percent)
{
	stopped = 0;
	speed = percent / 100.0f;

	if(percent != 100)
	{
		Audio::pauseMusic();
		playing = false;
	}
}

bool Guitar::setSong(Difficulty *t, string pic, bool a)
{
	if(!t)
	{
		running = false;
		return true;
	}

	chart->removeChildren(0, chart->getNumChildren());
	beats->removeChildren(0, beats->getNumChildren());

	std::string name = t->tab;

	backdelay = -min(0.0f, t->offset);
	tabdelay = max(0.0f, t->offset);
	scorer = NULL;

	// load song
	TabSong* song = ConvertGtp::load(name);
	if(!song)
	{
		return false;
	}

	if(!a)
	{
		scorer = new Scorer();
	}
	
	tab = &song->t[max(0,t->track1-1)];
	tempo = song->tempo;
	title = song->info["TITLE"];
	artist = song->info["ARTIST"];
	transcription = song->info["TRANSCRIBER"];
	
	// gameplay variables
	speed = 1.0f;
	spacing = 0.9f;
	
	score = 0;
	combo = 0;
	multiplier = 1;
	stopped = 0;

	last = 0;
	current = -tabdelay;
	realtime = 0;

	col = tab->c.begin();
	bar = tab->b.begin();
	repcount = 0;
	count3 = count2 = count = offset = 3.5f;

	if(!a)
	{
		// setup interface
		if(pic != "")
		{
			Sprite* p = new Sprite(pic, true);
			p->setScale(Vec2(2,2));
			p->setPosition(Vec3(-13.0f, 0.0f, 9.0f));
			scene->addChild(p);
		}

		osg::Geode* geodeTrack = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
			trackText->setFont("fonts/arial.ttf");
			trackText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
			trackText->setCharacterSize(1.0f);
			if(pic == "")
				trackText->setPosition(osg::Vec3(-14.0f,0.0f,10.0f));
			else
				trackText->setPosition(osg::Vec3(-10.0f,0.0f,10.0f));
			trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
			trackText->setDrawMode(osgText::Text::TEXT);
			trackText->setAlignment(osgText::Text::LEFT_TOP);
			trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
			trackText->setText(title + "\n" + artist);
	        
			geodeTrack->addDrawable(trackText.get());
	        
			scene->addChild(geodeTrack);
		}

		osg::Geode* geodeScore = new osg::Geode;
		{
			scoreText = new osgText::Text;
			scoreText->setFont("fonts/arial.ttf");
			scoreText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
			scoreText->setCharacterSize(1.0f);
			scoreText->setPosition(osg::Vec3(0.0f,0.0f,-7.0f));
			scoreText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
			scoreText->setDrawMode(osgText::Text::TEXT);
			scoreText->setAlignment(osgText::Text::LEFT_TOP);
			scoreText->setAxisAlignment(osgText::Text::XZ_PLANE);
			scoreText->setText("Score: \nCombo: \nInput Frequency:");
	        
			geodeScore->addDrawable(scoreText.get());
	        
			scene->addChild(geodeScore);
		}
	}

	running = true;
	playing = false;

	visible(true);

	time = new osg::Timer();

	return true;
}
