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
#include "Node.h"
#include <GLUT/GLUT.h>
#include <iostream>

using namespace std;

Node::Node()
{
	position = vec(0,0,0);
	scale = vec(1,1);
	rotation = 0;
	spin = 0;
	
	slides = 0;
	turns = 0;
	grows = 0;
	turns = 0;
	
	children = NULL;
	next = NULL;
}

Node::~Node()
{
	if(next) delete next;
	if(children) delete children;
}

void Node::update(float timeDelta)
{
	// update
	rotation += spin * timeDelta;
	
	position += (slide - position) * min(1.0f, slides * timeDelta);
	rotation += (turn - rotation) * min(1.0f, turns * timeDelta);
	scale += (grow - scale) * min(1.0f, grows * timeDelta);
	
	if(children) children->update(timeDelta);
	if(next) next->update(timeDelta);
}

void Node::render(GLint program)
{
	glPushMatrix();
	
	// transform
	glTranslatef(position.x, position.y, position.z);
	glScalef(scale.x, scale.y, 1);
	glRotatef(rotation, 0, 0, 1);
	glTranslatef(-center.x, -center.y, -center.z);
	
	if(children) children->render(program);
	
	glPopMatrix();
	
	if(next) next->render(program);
}

void Node::addChild(Renderable* c)
{
	if(children)
		c->next = children;
	
	children = c;
}

void Node::insertChild(Renderable* c, int i)
{
	if(!i-- || !children)
	{
		c->next = children;
		children = c;
	}
	else if(!i-- || !children->next)
	{
		c->next = children->next;
		children->next = c;
	}
	else
	{
		Renderable* n = children;
		while(n->next)
		{
			if(!i-- || n->next->next)
			{
				c->next = n->next->next;
				n->next = c;
				return;
			}
			n = n->next;
		}
	}
}

void Node::removeChild(Renderable* c)
{
	if(children == c)
	{
		children = children->next;
		c->next = NULL;
		return;
	}
	
	for(Renderable* n=children; n; n=n->next)
	{
		if(n->next == c)
		{
			n->next = c->next;
			c->next = NULL;
			return;
		}
	}
}

void Node::removeChild(int i)
{
	if(!i--)
	{
		Renderable* c = children;
		children = children->next;
		c->next = NULL;
		delete c;
		return;
	}
	
	for(Renderable* n=children; n; n=n->next)
	{
		if(!i--)
		{
			Renderable* c = n->next;
			n->next = c->next;
			c->next = NULL;
			delete c;
			return;
		}
	}
}
