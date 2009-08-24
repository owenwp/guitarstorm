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
const float Guitar::nutpos = -7.1f;
const float Guitar::neckscale = 0.03f;
const float Guitar::neckpos = -0.07f;
const float Guitar::bridgepos = 13.36f;
const float Guitar::bridgeheight = -0.09f;
const float Guitar::neckspace = 0.23f;
const float Guitar::bridgespace = 0.31f;

Guitar::Guitar()
{
	MakeGuitar();

	running = false; 
	finished = false;

	// create fret textures
	snote = new Sprite(new Texture(shapeCircle));
	
	for(int i=0; i<25; i++)
	{	
		string fret;
		stringstream str;
		str << i;
		str >> fret;

		tnote[i] = new Label("arial", str.str(), alignCenter);
		if(i == 3 || i == 5 || i == 7 || i == 9 || i == 12 || i == 15 || i == 17 || i == 19 || i == 21 || i == 23) 
		{
			tnote[i]->setColor(vec(1,1,1,1));
		}
		else
		{
			tnote[i]->setColor(vec(0,0,0,1));
		}
	}

	sbeat = new Sprite(new Texture("fret.tga"));
}

void Guitar::MakeGuitar()
{
	Node* neck = new Node;
	neck->addChild(new Sprite(new Texture("neck")));
	neck->setScale(vec(4,4));

	Node* head = new Node;
	head->addChild(new Sprite(new Texture("head")));
	head->setScale(vec(4,4));
	head->setPosition(vec(-9, 0));

	Node* nut = new Node;
	nut->addChild(new Sprite(new Texture("nut")));
	nut->setPosition(vec(nutpos, neckpos));
	nut->setScale(vec(2, 2));

	body = new Node;
	body->addChild(new Sprite(new Texture("body")));
	body->setPosition(vec(11.5f, -0.02));
	body->setScale(vec(16,16));

	chart = new Node;
	beats = new Node;

	addChild(body);
	addChild(neck);
	addChild(head);
	addChild(beats);
	addChild(nut);

	Node* s = new Node;
	strings[0] = new LineSprite(new Texture("bstring.tga"));
	s->addChild(strings[0]);
	nstring[0] = vec(nutpos - 0.05f, neckpos - neckspace * 2.5f);
	bstring[0] = vec(bridgepos, bridgeheight - bridgespace * 2.5f);
	s->setPosition(nstring[0]);
	strings[0]->setThickness(0.08);
	strings[0]->setLine(bstring[0] - nstring[0]);
	LineSprite* nstr = new LineSprite(new Texture("bstring.tga"));
	nstr->setThickness(0.08);
	nstr->setLine(vec(-1.9, -0.04));
	s->addChild(nstr);
	addChild(s);
	
	s = new Node;
	strings[1] = new LineSprite(new Texture("bstring.tga"));
	s->addChild(strings[1]);
	nstring[1] = vec(nutpos - 0.05f, neckpos - neckspace * 1.5f);
	bstring[1] = vec(bridgepos, bridgeheight - bridgespace * 1.5f);
	s->setPosition(nstring[1]);
	strings[1]->setThickness(0.06);
	strings[1]->setLine(bstring[1] - nstring[1]);
	nstr = new LineSprite(new Texture("bstring.tga"));
	nstr->setThickness(0.06);
	nstr->setLine(vec(-2.9, -0.26));
	s->addChild(nstr);
	addChild(s);
	
	s = new Node;
	strings[2] = new LineSprite(new Texture("bstring.tga"));
	s->addChild(strings[2]);
	nstring[2] = vec(nutpos - 0.05f, neckpos - neckspace * 0.5f);
	bstring[2] = vec(bridgepos, bridgeheight - bridgespace * 0.5f);
	s->setPosition(nstring[2]);
	strings[2]->setThickness(0.05);
	strings[2]->setLine(bstring[2] - nstring[2]);
	nstr = new LineSprite(new Texture("bstring.tga"));
	nstr->setThickness(0.05);
	nstr->setLine(vec(-3.9, -0.54));
	s->addChild(nstr);
	addChild(s);
	
	s = new Node;
	strings[3] = new LineSprite(new Texture("tstring.tga"));
	s->addChild(strings[3]);
	nstring[3] = vec(nutpos - 0.05f, neckpos + neckspace * 0.5f);
	bstring[3] = vec(bridgepos, bridgeheight + bridgespace * 0.5f);
	s->setPosition(nstring[3]);
	strings[3]->setThickness(0.04);
	strings[3]->setLine(bstring[3] - nstring[3]);
	nstr = new LineSprite(new Texture("tstring.tga"));
	nstr->setThickness(0.04);
	nstr->setLine(vec(-3.9, 0.54));
	s->addChild(nstr);
	addChild(s);
	
	s = new Node;
	strings[4] = new LineSprite(new Texture("tstring.tga"));
	s->addChild(strings[4]);
	nstring[4] = vec(nutpos - 0.05f, neckpos + neckspace * 1.5f);
	bstring[4] = vec(bridgepos, bridgeheight + bridgespace * 1.5f);
	s->setPosition(nstring[4]);
	strings[4]->setThickness(0.03);
	strings[4]->setLine(bstring[4] - nstring[4]);
	nstr = new LineSprite(new Texture("tstring.tga"));
	nstr->setThickness(0.03);
	nstr->setLine(vec(-2.9, 0.26));
	s->addChild(nstr);
	addChild(s);
	
	s = new Node;
	strings[5] = new LineSprite(new Texture("tstring.tga"));
	s->addChild(strings[5]);
	nstring[5] = vec(nutpos - 0.05f, neckpos + neckspace * 2.5f);
	bstring[5] = vec(bridgepos, bridgeheight + bridgespace * 2.5f);
	s->setPosition(nstring[5]);
	strings[5]->setThickness(0.02);
	strings[5]->setLine(bstring[5] - nstring[5]);
	nstr = new LineSprite(new Texture("tstring.tga"));
	nstr->setThickness(0.02);
	nstr->setLine(vec(-1.9, 0.04));
	s->addChild(nstr);
	addChild(s);
	
	addChild(chart);
	
	setCenter(vec(3,0,0));
}

void Guitar::update(float timeDelta)
{
	Node::update(timeDelta);
	
	if(!running)
		return;

	if(stopped && timeDelta < 0.2)
		return;

	float d2 = 0;
	double cdelta;
	if(speed != 0)
	{
		d2 = timeDelta * speed;
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
			d2 = timeDelta;
			realtime += d2;
			cdelta = d2 * spacing;
		}
	}
	current += cdelta;

	if(!playing && realtime > backdelay)
	{
		//if(speed == 1)
		//	playing = !Audio::playMusic();
		//else if(d2 > 0)	
		//	Audio::advanceMusic(d2);
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
							t += col->bend[i].x / 60.0;
							PlaceNote(count + dur * t, s, 
								col->a[s] + col->bend[i].y/50);
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

	for(Renderable* r = chart->children; r; r=r->next)
	{
		Node* note = (Node*)r;
		Fret* fr = static_cast<Fret*>(note->getUserData());

		float x = fr->t - current;

		vec pos = note->getPosition();
		pos.x = x;
		vec str = bstring[fr->s] - nstring[fr->s];
		
		pos.y = (nstring[fr->s] + (str * ((x - nstring[fr->s].x)/str.x))).y;
		
		note->setPosition(pos);
		if(!scorer && pos.x < nutpos)
		{
			chart->removeChild(note);	// autoplay
		}
		else if(pos.x < nutpos - tolerance)
		{
            if(fr)
				frets.push_back(*fr);

			float a = 1 + (pos.x - nutpos - tolerance) / 0.75f;
			if(a <= 0)
				chart->removeChild(note);
			else
				fr->m->setColor(vec(0.5f, 0.5f, 0.5f, 1));

		}
	}
	
	for(Renderable* r = beats->children; r; r=r->next)
	{
		Node* beat = (Node*)r;
		Fret* fr = static_cast<Fret*>(beat->getUserData());

		float x = fr->t - current;

		vec pos = beat->getPosition();
		vec scal = beat->getScale(); // in progress
		pos.x = x;
		scal.y = scal.y - cdelta * neckscale;
		beat->setPosition(pos);
		beat->setScale(scal);
		if(pos.x < nutpos)
		{
			beats->removeChild(beat);
			//if(!playing || !scorer)
				//Audio::tick();
		}
	}
/*
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
	 */

		// update score text
		/*std::ostringstream s;
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
		scoreText->setText(s.str());*/
	//}
}

void Guitar::PlaceNote(double t, int s, int f)
{
	if(f < 0 || f > 24)
		return;

	Node* n = new Node;
	
	Sprite* spr = new Sprite(snote);
	n->addChild(spr);
	
	Node* num = new Node;
	num->addChild(new Label(tnote[f]));
	num->setPosition(vec(0,-0.35f));
	num->setScale(vec(0.7f, 0.7f));
	n->addChild(num);
	
	n->setPosition(bstring[s]);
	n->setScale(vec(0.4f, 0.4f));

	float brightness = 0.7f;
	float dark = 0.1f;
	switch(s)
	{
	case 0:
		spr->setColor(vec(brightness, dark, dark, 1));
		break;
	case 1:
		spr->setColor(vec(brightness, brightness, dark, 1));
		break;
	case 2:
		spr->setColor(vec(dark, brightness, dark, 1));
		break;
	case 3:
		spr->setColor(vec(dark, brightness, brightness, 1));
		break;
	case 4:
		spr->setColor(vec(dark, dark, brightness, 1));
		break;
	case 5:
		spr->setColor(vec(brightness, dark, brightness, 1));
		break;
	}

	Fret* fr = new Fret;
	fr->hit = false;
	fr->s = s;
	fr->f = f;
	fr->t = t;
	n->setUserData(fr);
	fr->m = spr;
	
	chart->addChild(n);
}


void Guitar::PlaceBeat(double t)
{
	Node* n = new Node;
	Sprite* spr = new Sprite(sbeat);
	n->addChild(spr);
	
	n->setPosition(vec(bridgepos, bridgeheight));
	n->setScale(vec(2,2));
	
	Fret* fr = new Fret;
	fr->t = t;
	n->setUserData(fr);

	beats->addChild(n);
}

void Guitar::setSpeed(int percent)
{
	stopped = 0;
	speed = percent / 100.0f;

	if(percent != 100)
	{
		//Audio::pauseMusic();
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
	
	chart->removeChildren(0, chart->numChildren());
	beats->removeChildren(0, beats->numChildren());

	string name = t->tab;

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
	spacing = 1.5f;
	
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
	count3 = count2 = count = offset = 7.1f;

	if(!a)
	{
		// setup interface
		/*if(pic != "")
		{
			Sprite* p = new Sprite(pic, true);
			p->setScale(Vec2(2,2));
			p->setPosition(Vec3(-13.0f, 0.0f, 9.0f));
			scene->addChild(p);
		}

		osg::Geode* geodeTrack = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
			trackText->setFont(prefix+"fonts/arial.ttf");
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
			scoreText->setFont(prefix+"fonts/arial.ttf");
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
		}*/
	}

	running = true;
	playing = false;

	hidden = false;

	return true;
}
