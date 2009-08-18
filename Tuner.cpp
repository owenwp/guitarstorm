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
#include "Tuner.h"

/*
License for gTune (c) 1999 by Michael Celli and Harold C.L. Baur.

The following are the terms which the program "gTune" including all its source
files and the resultant binary files fall under:

	- You may distribute gTune and all or any piece of it so long as this 
license is included and remains unchanged and credit to all the authors 
remains.  A fee may be charged for distribution of gTune so long as the above 
terms are met.  Distribution of gTune or any piece of gTune in any way shape or
form must be under this license.

	- You may modify gTune and distribute any modifications as long as the 
source code to your modifications are included in any distribution of these 
modifications, and the terms outlined in the above section are met.  This 
means that any modified distribution of gTune or any piece of gTune must 
contain this license with no modifications to the license, be released under 
this license, include the source code to the modifications, and credit to the 
original authors must be stated explicitly.

	- gTune has no warranty whatsoever.  gTune is provided "as is" without 
warranty of any kind.  Should gTune malfunction or cause dammage, you assume 
the cost of servicing or dammages caused.  The authors of gTune or any 
party who may distribute or modify gTune cannot be held accountable for any of
its actions as all accountability is assumed by the user.  Liability for 
dammages caused by this program is assumed entirely by the user.

	- If distribution or usage of this program under this license is 
restricted in any country for any reason including patent or copyright 
interferences, then this software may not be distributed or used in such a 
country.

End of Terms and Restrictions.
*/

Tuner::Tuner()
{
	//scene = new Group();
	MakeTuner();
	//scene->addChild(sprite);

	//scene->setUserData(this); 
	//scene->setUpdateCallback(new tunerCallback);
}

void Tuner::MakeTuner()
{
	Sprite* tuner = new Sprite(new Texture("tuner.tga"));

	/*Sprite* back = new Sprite("");
	back->setColor(Vec4(0,0,0,1));
	back->setScale(Vec2(1.8f, 0.9f));
	back->setPosition(Vec3(0,1,0));
	tuner->addChild(back);

	Sprite* tick = new Sprite("tick.tga");
	Vec3 dpos = Vec3(-0.59f, 0.5f, -0.85);
	float minorDelta = (PI/2.0) / 50.0;
    for (int i = 0; i <= 50; i++)
    {
        Sprite* t = new Sprite(tick);
		t->setRotation(-PI/4.0 + minorDelta * i);
		tuner->addChild(t);

		float p = -65;
		if (i % 25 == 0)
        {
			t->setCenter(Vec3(0, 0, (p+5)/8));
			t->setScale(Vec2(0.08f, 0.16f));
        }
        else if (i % 5 == 0)
        {
			t->setCenter(Vec3(0, 0, (p+3)/4));
			t->setScale(Vec2(0.08f, 0.08f));
        }
        else
        {
			t->setCenter(Vec3(0, 0, p));
			t->setScale(Vec2(0.02f, 0.02f));
        }

		t->setPosition(dpos);
    }

	arm = new Sprite("arm.tga");
	arm->setScale(Vec2(0.25f,0.7f));
	arm->setCenter(Vec3(0, 0.1f, -0.8f));
	arm->setPosition(dpos);
	arm->setColor(Vec4(1,0,0,1));
	tuner->addChild(arm);

	absolute = new Sprite("circle.tga");
	absolute->setScale(Vec2(0.075f,0.075f));
	absolute->setPosition(dpos + Vec3(0, -0.75f, 0.25f));
	absolute->setColor(Vec4(0,1,0,1));
	tuner->addChild(absolute);

	flat = new Sprite("circle.tga");
	flat->setScale(Vec2(0.075f,0.075f));
	flat->setPosition(dpos + Vec3(-0.4, -0.75f, 0.25f));
	flat->setColor(Vec4(1,0,0,1));
	tuner->addChild(flat);

	sharp = new Sprite("circle.tga");
	sharp->setScale(Vec2(0.075f,0.075f));
	sharp->setPosition(dpos + Vec3(0.4f, -0.75f, 0.25f));
	sharp->setColor(Vec4(1,0,0,1));
	tuner->addChild(sharp);

	osg::Geode* geodeName = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> itemText = new osgText::Text;
        itemText->setFont(prefix+"fonts/arial.ttf");
        itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		itemText->setCharacterSize(0.125f);
		itemText->setPosition(Vec3(1.25f,-0.25f,-0.4f));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setAlignment(osgText::Text::CENTER_CENTER);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText("VIRTUAL\nCHROMATIC\nTUNER");
        
        geodeName->addDrawable(itemText.get());
        
        tuner->addChild(geodeName);
    }

	osg::Geode* geodeFT = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> itemText = new osgText::Text;
        itemText->setFont(prefix+"fonts/arial.ttf");
        itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		itemText->setCharacterSize(0.1f);
		itemText->setPosition(dpos + Vec3(-0.55f, -0.75f, 0.25f));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setAlignment(osgText::Text::CENTER_CENTER);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText("b");
        
        geodeFT->addDrawable(itemText.get());
        
        tuner->addChild(geodeFT);
    }

	osg::Geode* geodeST = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> itemText = new osgText::Text;
        itemText->setFont(prefix+"fonts/arial.ttf");
        itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		itemText->setCharacterSize(0.1f);
		itemText->setPosition(dpos + Vec3(0.55f, -0.75f, 0.25f));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setAlignment(osgText::Text::CENTER_CENTER);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText("#");
        
        geodeST->addDrawable(itemText.get());
        
        tuner->addChild(geodeST);
    }

	osg::Geode* geodeAT = new osg::Geode;
    {
		osg::ref_ptr<osgText::Text> itemText = new osgText::Text;
        itemText->setFont(prefix+"fonts/arial.ttf");
        itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
		itemText->setCharacterSize(0.2f);
		itemText->setPosition(dpos + Vec3(0, -0.75f, 0.1f));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setAlignment(osgText::Text::CENTER_CENTER);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText("^");
        
        geodeAT->addDrawable(itemText.get());
        
        tuner->addChild(geodeAT);
    }

	Geode* geodeNote = new osg::Geode;
    {
		ntext = new osgText::Text;
		ntext->setColor(Vec4(1.0f,0.0f,0.0f,1.0f));
		ntext->setCharacterSize(0.5f);
		ntext->setPosition(Vec3(1.175f,0.5f,0.4f));
		ntext->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		ntext->setDrawMode(osgText::Text::TEXT);
		ntext->setAlignment(osgText::Text::CENTER_CENTER);
		ntext->setAxisAlignment(osgText::Text::XZ_PLANE);
		ntext->setText("G");

		geodeNote->addDrawable(ntext.get());
		tuner->addChild(geodeNote);
	}
	
	Geode* geodeSharp = new osg::Geode;
    {
		stext = new osgText::Text;
		stext->setFont(prefix+"fonts/arial.ttf");
		stext->setColor(Vec4(1.0f,0.0f,0.0f,1.0f));
		stext->setCharacterSize(0.25f);
		stext->setPosition(Vec3(1.45f,0.5f,0.5f));
		stext->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		stext->setDrawMode(osgText::Text::TEXT);
		stext->setAlignment(osgText::Text::CENTER_CENTER);
		stext->setAxisAlignment(osgText::Text::XZ_PLANE);
		stext->setText("#");

		geodeSharp->addDrawable(stext.get());
		tuner->addChild(geodeSharp);
	}
	
	Geode* geodeCN = new osg::Geode;
    {
		ref_ptr<osgText::Text> label = new osgText::Text;
		label->setFont(prefix+"fonts/arial.ttf");
		label->setColor(Vec4(1.0f,1.0f,1.0f,1.0f));
		label->setCharacterSize(0.1f);
		label->setPosition(dpos + Vec3(-1.0f,0,1.4f));
		label->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		label->setDrawMode(osgText::Text::TEXT);
		label->setAlignment(osgText::Text::LEFT_TOP);
		label->setAxisAlignment(osgText::Text::XZ_PLANE);
		label->setText("CENT\n-50");

		geodeCN->addDrawable(label.get());
		tuner->addChild(geodeCN);
	}
	
	Geode* geodeCP = new osg::Geode;
    {
		ref_ptr<osgText::Text> label = new osgText::Text;
		label->setFont(prefix+"fonts/arial.ttf");
		label->setColor(Vec4(1.0f,1.0f,1.0f,1.0f));
		label->setCharacterSize(0.1f);
		label->setPosition(dpos + Vec3(1.0f,0,1.4f));
		label->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		label->setDrawMode(osgText::Text::TEXT);
		label->setAlignment(osgText::Text::RIGHT_TOP);
		label->setAxisAlignment(osgText::Text::XZ_PLANE);
		label->setText("CENT\n+50");

		geodeCP->addDrawable(label.get());
		tuner->addChild(geodeCP);
	}
	
	Geode* geodeCZ = new osg::Geode;
    {
		ref_ptr<osgText::Text> label = new osgText::Text;
		label->setFont(prefix+"fonts/arial.ttf");
		label->setColor(Vec4(1.0f,1.0f,1.0f,1.0f));
		label->setCharacterSize(0.1f);
		label->setPosition(dpos + Vec3(0.0f,0,1.5f));
		label->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		label->setDrawMode(osgText::Text::TEXT);
		label->setAlignment(osgText::Text::CENTER_TOP);
		label->setAxisAlignment(osgText::Text::XZ_PLANE);
		label->setText("> 0 <");

		geodeCZ->addDrawable(label.get());
		tuner->addChild(geodeCZ);
	}

	sprite = tuner;*/
}

void Tuner::Update()
{
	int samples = Options::instance->sampleRate;
	float dark = 0.2f;
	
	/*if(Audio::readVol() < 500)
	{		
		note = -1;
		arm->setTurn(0, 0.5f);
		stext->setText("");
		ntext->setText("");
		
		flat->setTint(Vec4(dark,0,0,1), 1);
		absolute->setTint(Vec4(0,dark,0,1), 1);
		sharp->setTint(Vec4(dark,0,0,1), 1);

		return;
	}

	double* buf = Audio::readLast(samples);
	double* spec = new double[samples];

	// compute frequency spectrum
	realfft (buf, samples, spec);

	// for now, just check the bass note
	double ffreq = 0.0; 
	double norm;
	double maxnorm = -100.0; 
	float li = (int)(samples - 1)/2;
	double frequency;

	// find the dominant frequency
	for (int i=1; i<li; i+=2) 
	{
		norm = (spec[i] * spec[i]) + (spec[i+1] * spec[i+1]);

		if (norm>maxnorm)
		{
			maxnorm = norm; 
			ffreq = i;
		}
	}
	frequency = 2.0 * ffreq;

	double a,r; 
	int o,n;

	// determine note
	a=log(frequency/440.0)/log(2.0); 
	o=(int)floor(a); 
	r=(a-o)*12.0; 
	n=(int)(r+0.5);
	if(n >= 12) n -= 12;

	delete[] spec;
	delete[] buf;
	
	note = n;
	cents = (r-n) * 100;

	if(cents > 1)
	{
		flat->setTint(Vec4(dark,0,0,1), 10);
		absolute->setTint(Vec4(0,dark,0,1), 10);
		sharp->setTint(Vec4(1,0,0,1), 10);
	}
	else if(cents < -1)
	{
		flat->setTint(Vec4(1,0,0,1), 10);
		absolute->setTint(Vec4(0,dark,0,1), 10);
		sharp->setTint(Vec4(dark,0,0,1), 10);
	}
	else
	{
		flat->setTint(Vec4(dark,0,0,1), 10);
		absolute->setTint(Vec4(0,1,0,1), 10);
		sharp->setTint(Vec4(dark,0,0,1), 10);
	}

	arm->setTurn((r-n) * PI/2.0, 10);

	stext->setText("");
	switch(note)
	{
	case 1:
		stext->setText("#");
	case 0:
	case 12:
		ntext->setText("A");
		break;
	case 2:
		ntext->setText("B");
		break;
	case 4:
		stext->setText("#");
	case 3:
		ntext->setText("C");
		break;
	case 6:
		stext->setText("#");
	case 5:
		ntext->setText("D");
		break;
	case 7:
		ntext->setText("E");
		break;
	case 9:
		stext->setText("#");
	case 8:
		ntext->setText("F");
		break;
	case 11:
		stext->setText("#");
	case 10:
		ntext->setText("G");
		break;
	}*/
}
