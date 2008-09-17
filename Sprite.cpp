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
#include "Sprite.h"

#include <osgDB/Registry>
#include <osgDB/ReadFile>

const string Images = "images/";

Timer* Sprite::time = new Timer;

void Sprite::createSquare(Image* image)
{
    // set up the Geometry.
    geom = new Geometry;

	last = time->time_s();
	float aspect = 1;

	if (image)
    {
        stateset = new osg::StateSet;
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage(image);
		texture->setWrap(Texture::WRAP_R, Texture::REPEAT);
		texture->setWrap(Texture::WRAP_S, Texture::REPEAT);
		texture->setWrap(Texture::WRAP_T, Texture::REPEAT);
		aspect = image->s() / (float)image->t();

		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
		stateset->setDataVariance(Object::DYNAMIC);
		osg::BlendEquation* blendEquation = new BlendEquation(BlendEquation::FUNC_ADD);
		blendEquation->setDataVariance(Object::DYNAMIC);
		stateset->setAttributeAndModes(blendEquation,StateAttribute::OVERRIDE|StateAttribute::ON);    
		stateset->setRenderingHint(StateSet::TRANSPARENT_BIN);
		stateset->setMode(GL_LIGHTING, StateAttribute::OFF);
        
		geom->setStateSet(stateset);
    }

    Vec3Array* coords = new Vec3Array(4);
	(*coords)[0].set(-aspect, 0, -1);
    (*coords)[1].set(aspect, 0, -1);
    (*coords)[2].set(aspect, 0, 1);
    (*coords)[3].set(-aspect, 0, 1);

    geom->setVertexArray(coords);

    Vec3Array* norms = new osg::Vec3Array(1);
    (*norms)[0].set(0, 0, -1);
    
    geom->setNormalArray(norms);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    tcoords = new osg::Vec2Array(4);
    (*tcoords)[0].set(0.0f,0.0f);
    (*tcoords)[1].set(1.0f,0.0f);
    (*tcoords)[2].set(1.0f,1.0f);
    (*tcoords)[3].set(0.0f,1.0f);
    geom->setTexCoordArray(0,tcoords);

	colora = new Vec4Array(1);
	(*colora)[0].set(1,1,1,1);

	geom->setColorArray(colora);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

	geode = new Geode;
	geode->addDrawable(geom);
    
    addChild(geode);
}

Sprite::Sprite(Sprite* sprite)
{ 
	setUpdateCallback(new spriteCallback);

    // set up the Geometry.
	geom = (Geometry*)sprite->geom->clone(CopyOp::SHALLOW_COPY);

	last = time->time_s();

	position = sprite->position;
	scale = sprite->scale;
	rotation = sprite->rotation;
	spin = sprite->spin;
	center = sprite->center;   
	color = sprite->color;

	slides = 0;
	turns = 0;
	grows = 0;
	turns = 0;

	colora = new Vec4Array(1);
	tcoords = (Vec2Array*)geom->getTexCoordArray(0);

	geom->setColorArray(colora);
	
	Geode* geode = new Geode;
	geode->addDrawable(geom);
    
    addChild(geode);
}

Sprite::Sprite(string filename, bool absolute)
{
	setUpdateCallback(new spriteCallback);

	scale = Vec2(1,1);
	color = Vec4(1,1,1,1);
	rotation = 0;
	spin = 0;

	slides = 0;
	turns = 0;
	grows = 0;
	turns = 0;

	if(absolute)
		createSquare(osgDB::readImageFile(filename));
	else
		createSquare(osgDB::readImageFile(Images+filename));
}

void Sprite::Update()
{
	double t = time->time_s();
	double delta = min(0.1, t - last);
	last = t;

	rotation += spin * delta;

	if(scroll != Vec2(0,0))
	{
		for(int i=0; i<4; i++)
			(*tcoords)[i] = (*tcoords)[i] + scroll * delta;

		geom->setTexCoordArray(0,tcoords);
	}

	position += (slide - position) * min(1.0, slides * delta);
	rotation += (turn - rotation) * min(1.0, turns * delta);
	scale += (grow - scale) * min(1.0, grows * delta);
	color += (tint - color) * min(1.0, tints * delta);

	if(color != (*colora)[0])
	{
		(*colora)[0] = color;
		geom->setColorArray(colora);
	}

	Matrix mat = 
		Matrix::translate(-center) *
		Matrix::scale(scale.x(), 1, scale.y()) *
		Matrix::rotate(rotation, 0, 1, 0) *
		Matrix::translate(position);

	setMatrix(mat);
}
