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
#include "Notes.h"

const float Notes::tolerance = 0.2f;

Notes::Notes()
{
	_scene = new osg::Group;
	_scene->setUserData(this); 
	_scene->setUpdateCallback(new notesCallback);

	origin = osg::Vec3(-12.0, 0.0, 0.0);

	running = false; 
	finished = false;
	visible(false);

	// create fret textures
	for(int i=0; i<25; i++)
	{	
		string fret;
		stringstream str;
		str << i;
		str >> fret;

		tnote[i] = new osgText::Text;
		tnote[i]->setFont("fonts/arial.ttf");
		tnote[i]->setDataVariance(osg::Object::DYNAMIC);
		//tnote[i]->setFontResolution(10,10);
		tnote[i]->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		tnote[i]->setCharacterSize(1.5f);
		tnote[i]->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		tnote[i]->setDrawMode(osgText::Text::TEXT |osgText::Text::BOUNDINGBOX);
		tnote[i]->setAlignment(osgText::Text::CENTER_CENTER);
		tnote[i]->setAxisAlignment(osgText::Text::XZ_PLANE);
		tnote[i]->setText(fret);
	}
	tbeat = new osgText::Text;
	tbeat->setFont("fonts/arial.ttf");
	tbeat->setDataVariance(osg::Object::DYNAMIC);
	//tbeat->setFontResolution(10,10);
	tbeat->setColor(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
	tbeat->setCharacterSize(3.0f);
	tbeat->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
	tbeat->setDrawMode(osgText::Text::TEXT);
	tbeat->setAlignment(osgText::Text::CENTER_CENTER);
	tbeat->setAxisAlignment(osgText::Text::XZ_PLANE);
	tbeat->setText("|");
}

void Notes::Update()
{
	if(!running)
		return;

	double t = time->time_s();
	double delta = t - last;
	if(stopped && delta < 0.2)
		return;

	last = t;
	if(speed != 0)
	{
		current += delta * spacing * speed;
	}
	
	else
	{
		if(stopped)
			current = stopped;
		else
			current += delta * spacing;
	}

	NoteChart::iterator itr;
	if(!stopped)
	{
		// place beat lines
		while(bar != tab->b.end() && current + offset > count3)
		{
			if(bar->tempo != -1)
				tempo = bar->tempo;

			count3 += 240 / tempo * spacing;
			bar++;

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
			count += col->l * 240 / tempo * spacing;
			col++;
		}

		for(itr = chart.begin();itr!=chart.end();itr++)
		{
			if(!(itr->second->getValue(0) ||
				itr->second->getValue(1)))
			{
				_scene->removeChild(itr->second.get());
				chart.erase(itr);
				break;
			}
		}
	}

	// update fret positions
	list<Fret> frets;

	itr = chart.begin();
	while(itr!=chart.end())
	{
		if(!itr->second)
			continue;

		float x = itr->first - current;

		if(itr->second->getValue(0) && !itr->second->getUserData() && x < origin.x()+tolerance)
		{
			// beat hit
			itr->second->setAllChildrenOff();
			scorer->tick();
		}
		else if(itr->second->getValue(0) && x < origin.x()-tolerance)
		{
			Fret* fr = static_cast<Fret*>(itr->second->getUserData());
			if(fr)
				frets.push_back(*fr);
		}

		if(itr->second->getValue(1))
		{
			osgText::Text* n = dynamic_cast<osgText::Text*> (itr->second->getChild(1)->getUserData());
			osg::Vec3 pos = n->getPosition();
			pos.x() = x;
			n->setPosition(pos);
		}
		if(itr->second->getValue(0))
		{
			osgText::Text* n = dynamic_cast<osgText::Text*> (itr->second->getChild(0)->getUserData());
			osg::Vec3 pos = n->getPosition();
			pos.x() = x;
			n->setPosition(pos);
		}
		else if(x > origin.x()+tolerance)
		{
			itr->second->setSingleChildOn(0);
		}

		if(x < origin.x()-5)
		{
			itr->second->setAllChildrenOff();
		}

		itr++;
	}

	// fret reached
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
				if(!stopped)
					fi->m->setSingleChildOn(1);
	
				combo = 0;
				multiplier = 1;
			}
			else
			{
				fi->m->setAllChildrenOff();
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
	s << "\nNote: " << scorer->lastnote << " Volume: " << scorer->getVolume();
	scoreText->setText(s.str());
}

void Notes::PlaceNote(double t, int s, int f)
{
	osg::Vec3 pos = origin + osg::Vec3((float)t, -0.1f, 2.0f*(s-2.5f));

	osg::Geode* geodeUp = new osg::Geode;
	{
		osg::ref_ptr<osgText::Text> textUp = (osgText::Text*)tnote[f]->clone(osg::CopyOp::SHALLOW_COPY);
		textUp->setPosition(pos);

		geodeUp->setUserData(textUp.get());
		geodeUp->addDrawable(textUp.get());
	}
	osg::Geode* geodeDown = new osg::Geode;
	{
		osg::ref_ptr<osgText::Text> textDown = (osgText::Text*)tnote[f]->clone(osg::CopyOp::SHALLOW_COPY);
		textDown->setColor(osg::Vec4(1.0f,0.0f,0.0f,1.0f));
		textDown->setPosition(pos);

		geodeDown->setUserData(textDown.get());
		geodeDown->addDrawable(textDown.get());
	}
	osg::Switch* model = new osg::Switch;
	model->addChild(geodeUp,true);
	model->addChild(geodeDown,false);
	_scene->addChild(model);
	Fret* fr = new Fret;
	fr->hit = false;
	fr->s = s;
	fr->f = f;
	model->setUserData(fr);
	fr->m = model;

	chart[t+s*0.000011f] = model;
}


void Notes::PlaceBeat(double t)
{
	for(int s=0; s<6; s++)
	{
		osg::Vec3 pos = origin + osg::Vec3((float)t, 0.1f, 2.0f*(s-2.5f));

		osg::Geode* geodeUp = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> textUp = (osgText::Text*)tbeat->clone(osg::CopyOp::SHALLOW_COPY);
			textUp->setPosition(pos);

			geodeUp->setUserData(textUp.get());
			geodeUp->addDrawable(textUp.get());
		}
		osg::Geode* geodeDown = new osg::Geode;
		{
			osg::ref_ptr<osgText::Text> textDown = (osgText::Text*)tbeat->clone(osg::CopyOp::SHALLOW_COPY);
			textDown->setPosition(pos);

			geodeDown->setUserData(textDown.get());
		}
		osg::Switch* model = new osg::Switch;
		model->addChild(geodeUp,true);
		model->addChild(geodeDown,false);
		_scene->addChild(model);

		chart[t+s*0.00001f] = model;
	}
}

void Notes::addString(osg::Vec3& pos,const std::string& text, float height)
{

    osg::Geode* geodeUp = new osg::Geode;
    {
        osgText::Text* textUp = new osgText::Text;
        textUp->setFont("fonts/arial.ttf");
        textUp->setColor(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
        textUp->setCharacterSize(height);
        textUp->setPosition(pos);
		textUp->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        textUp->setDrawMode(osgText::Text::TEXT);
        textUp->setAlignment(osgText::Text::LEFT_CENTER);
        textUp->setAxisAlignment(osgText::Text::XZ_PLANE);
        textUp->setText(text);
        
        geodeUp->addDrawable(textUp);
    }
    
    osg::Geode* geodeDown = new osg::Geode;
    {
        osgText::Text* textDown = new osgText::Text;
        textDown->setFont("fonts/arial.ttf");
        textDown->setColor(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
        textDown->setCharacterSize(height);
        textDown->setPosition(pos);
		textDown->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        textDown->setDrawMode(osgText::Text::TEXT/*||osgText::Text::BOUNDINGBOX*/);
        textDown->setAlignment(osgText::Text::LEFT_CENTER);
        textDown->setAxisAlignment(osgText::Text::XZ_PLANE);
        textDown->setText(text);
        
        geodeDown->addDrawable(textDown);
    }

    osg::Switch* model = new osg::Switch;
    model->addChild(geodeUp,true);
    model->addChild(geodeDown,false);
    
    _scene->addChild(model);  
}

void Notes::createStrings()
{    
    osg::Vec3 pos=origin;
	float ox = origin.x();

	pos.x() =ox;
	pos.z() -= 5.0f;
	addString(pos,"E============================================================",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
	addString(pos,"A------------------------------------------------------------------------------------------------------------------------",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
	addString(pos,"D------------------------------------------------------------------------------------------------------------------------",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
	addString(pos,"G------------------------------------------------------------------------------------------------------------------------",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
	addString(pos,"B------------------------------------------------------------------------------------------------------------------------",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
	addString(pos,"e------------------------------------------------------------------------------------------------------------------------",1.0f);
	pos.x() = ox;
    pos.z() += 2.0f;
}

void Notes::setSpeed(int percent)
{
	stopped = 0;
	speed = percent / 100.0f;
}

void Notes::setSong(std::string name)
{
	_scene->removeChildren(0, _scene->getNumChildren());

	createStrings();
	
	// load song
	TabSong* song = ConvertGtp::load(name);
	tab = &song->t[0];
	tempo = song->tempo;
	title = song->info["TITLE"];
	artist = song->info["ARTIST"];
	transcription = song->info["TRANSCRIBER"];
	
	// gameplay variables
	speed = 1.0f;
	spacing = 8.0f;
	
	score = 0;
	combo = 0;
	multiplier = 1;
	stopped = 0;

	last = 0;
	current = 0;

	col = tab->c.begin();
	bar = tab->b.begin();
	count3 = count2 = count = offset = 17;

	// setup interface
	osg::Geode* geodeTrack = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
        trackText->setFont("fonts/arial.ttf");
        trackText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        trackText->setCharacterSize(1.0f);
        trackText->setPosition(osg::Vec3(-16.0f,0.0f,12.0f));
		trackText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        trackText->setDrawMode(osgText::Text::TEXT);
		trackText->setAlignment(osgText::Text::LEFT_TOP);
        trackText->setAxisAlignment(osgText::Text::XZ_PLANE);
		trackText->setText(title + "\n" + artist);
        
        geodeTrack->addDrawable(trackText.get());
        
        _scene->addChild(geodeTrack);
    }

	osg::Geode* geodeScore = new osg::Geode;
    {
        scoreText = new osgText::Text;
        scoreText->setFont("fonts/arial.ttf");
        scoreText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        scoreText->setCharacterSize(1.5f);
        scoreText->setPosition(osg::Vec3(3.0f,0.0f,-8.0f));
		scoreText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
        scoreText->setDrawMode(osgText::Text::TEXT);
		scoreText->setAlignment(osgText::Text::LEFT_TOP);
        scoreText->setAxisAlignment(osgText::Text::XZ_PLANE);
		scoreText->setText("Score: \nCombo: \nInput Frequency:");
        
        geodeScore->addDrawable(scoreText.get());
        
        _scene->addChild(geodeScore);
    }

	running = true;

	visible(true);

	time = new osg::Timer();
}
