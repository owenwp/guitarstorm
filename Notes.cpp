#include "Notes.h"

const float Notes::tolerance = 0.2f;

void Notes::hitNote(int key)
{
	bool miss = true;
	if(key == 'r')
		time = new osg::Timer();

	double t = time->time_s();
	NoteChart::iterator itr = chart.lower_bound(t-tolerance);
	while(itr!=chart.end() && itr->first < t + tolerance)
	{
		int k = itr->second->getName()[0];
		if(k == key)
		{
			itr->second->setAllChildrenOff();
			if(multiplier < 4)
			{
				combo++;
				if(combo > 5)
				{
					multiplier++;
					if(multiplier < 4)
						combo = 0;
				}
			}
			score += 10 * multiplier;
			miss = false;
		}

		itr++;
	}
	if(miss)
	{
		combo = 0;
		multiplier = 1;
	}
}

void Notes::Update()
{
	double t = time->time_s() * speed;
	
	// place beat lines
	while(col != tab->c.end() && t + offset > count2)
	{
		PlaceBeat(count2);
		
		count2 += 60 / tempo * speed;
	}

	// place new notes
	while(col != tab->c.end() && t + offset > count)
	{
		for(int s=0; s<6; s++)
		{
			if(col->a[s] != -1)
				PlaceNote(count, s, col->a[s]);
		}
		count += col->l * 240 / tempo * speed;
		col++;
	}

	NoteChart::iterator itr = chart.begin();
	for(;itr!=chart.end();itr++)
	{
		if(!(itr->second->getValue(0) ||
			itr->second->getValue(1)))
		{
			_scene->removeChild(itr->second.get());
			chart.erase(itr);
			break;
		}
	}

	itr = chart.begin();
	while(itr!=chart.end())
	{
		if(!itr->second)
			continue;

		float x = itr->first - t;

		if(itr->second->getValue(0) && x < -tolerance)
		{
			itr->second->setSingleChildOn(1);
			combo = 0;
			multiplier = 1;
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
		else if(x > tolerance)
		{
			itr->second->setSingleChildOn(0);
		}

		if(x < -5)
		{
			itr->second->setAllChildrenOff();
			//_scene->removeChild(itr->second.get());
			//itr->second = NULL;
		}

		itr++;
	}

	// update score text
	std::ostringstream s;
	s	<< "Score:      " << score << "\n"
		<< "Multiplier: " << multiplier << "X ";
	for(int i=0; i< 5; i++)
	{
		if(i<combo)
			s << "+";
		else
			s << "-";
	}
	scoreText->setText(s.str());
}

void Notes::PlaceNote(double t, int s, int f)
{
	osg::Vec3 origin((float)t, 0.0f, (float)s);

	string fret;
	stringstream str;
	str << f;
	str >> fret;

	osg::Geode* geodeUp = new osg::Geode;
	{
		osgText::Text* textUp = new osgText::Text;
		//textUp->setFont("fonts/arial.ttf");
		textUp->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		textUp->setCharacterSize(2.0f);
		textUp->setPosition(origin);
		textUp->setDrawMode(osgText::Text::TEXT |osgText::Text::BOUNDINGBOX);
		textUp->setAlignment(osgText::Text::CENTER_CENTER);
		textUp->setAxisAlignment(osgText::Text::XZ_PLANE);
		textUp->setText(fret);

		geodeUp->setUserData(textUp);
		geodeUp->addDrawable(textUp);
	}
	osg::Geode* geodeDown = new osg::Geode;
	{
		osgText::Text* textDown = new osgText::Text;
		//textDown->setFont("fonts/arial.ttf");
		textDown->setColor(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
		textDown->setCharacterSize(2.0f);
		textDown->setPosition(origin);
		textDown->setDrawMode(osgText::Text::TEXT/*||osgText::Text::BOUNDINGBOX*/);
		textDown->setAlignment(osgText::Text::CENTER_CENTER);
		textDown->setAxisAlignment(osgText::Text::XZ_PLANE);
		textDown->setText(fret);

		geodeDown->setUserData(textDown);
		geodeDown->addDrawable(textDown);
	}
	osg::Switch* model = new osg::Switch;
	model->addChild(geodeUp,true);
	model->addChild(geodeDown,false);
	//model->setName(string);
	_scene->addChild(model);

	chart[t+s*0.00001f] = model;
}


void Notes::PlaceBeat(double t)
{
	for(int s=0; s<6; s++)
	{
		osg::Vec3 origin((float)t, 0.5f, (float)s);

		osg::Geode* geodeUp = new osg::Geode;
		{
			osgText::Text* textUp = new osgText::Text;
			//textUp->setFont("fonts/arial.ttf");
			textUp->setColor(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
			textUp->setCharacterSize(3.0f);
			textUp->setPosition(origin);
			textUp->setDrawMode(osgText::Text::TEXT);
			textUp->setAlignment(osgText::Text::CENTER_CENTER);
			textUp->setAxisAlignment(osgText::Text::XZ_PLANE);
			textUp->setText("|");

			geodeUp->setUserData(textUp);
			geodeUp->addDrawable(textUp);
		}
		osg::Geode* geodeDown = new osg::Geode;
		{
			osgText::Text* textDown = new osgText::Text;
			//textDown->setFont("fonts/arial.ttf");
			textDown->setColor(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
			textDown->setCharacterSize(2.0f);
			textDown->setPosition(origin);
			textDown->setDrawMode(osgText::Text::TEXT/*||osgText::Text::BOUNDINGBOX*/);
			textDown->setAlignment(osgText::Text::CENTER_CENTER);
			textDown->setAxisAlignment(osgText::Text::XZ_PLANE);
			textDown->setText("");

			geodeDown->setUserData(textDown);
			geodeDown->addDrawable(textDown);
		}
		osg::Switch* model = new osg::Switch;
		model->addChild(geodeUp,true);
		model->addChild(geodeDown,false);
		//model->setName(string);
		_scene->addChild(model);

		chart[t+s*0.00001f] = model;
	}
}

void Notes::setSong(std::string name)
{
	
	// load song
	TabSong* song = ConvertGtp::load(name);
	tab = &song->t[0];
	tempo = song->tempo;
	title = song->info["TITLE"];
	artist = song->info["ARTIST"];
	transcription = song->info["TRANSCRIBER"];
	
	// gameplay variables
	speed = 5.0f;
	
	score = 0;
	combo = 0;
	multiplier = 1;

	col = tab->c.begin();
	count2 = count = offset = 25;

	// setup interface
	osg::Geode* geodeTrack = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> trackText = new osgText::Text;
        //scoreText->setFont("fonts/arial.ttf");
        trackText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        trackText->setCharacterSize(1.5f);
        trackText->setPosition(osg::Vec3(2.0f,0.0f,10.0f));
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
        //scoreText->setFont("fonts/arial.ttf");
        scoreText->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        scoreText->setCharacterSize(2.0f);
        scoreText->setPosition(osg::Vec3(6.0f,0.0f,-3.0f));
        scoreText->setDrawMode(osgText::Text::TEXT);
		scoreText->setAlignment(osgText::Text::LEFT_TOP);
        scoreText->setAxisAlignment(osgText::Text::XZ_PLANE);
		scoreText->setText("Score: ");
        
        geodeScore->addDrawable(scoreText.get());
        
        _scene->addChild(geodeScore);
    }

	time = new osg::Timer();
}
