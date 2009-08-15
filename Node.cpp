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

Node::Node()
{
	children = 0;
	next = 0;
}

Node::~Node()
{
	if(next) delete next;
	if(children) delete children;
}

void Node::update()
{
	// update
	
	if(children) children->update();
	if(next) next->update();
}

void Node::render()
{
	// render
	
	if(children) children->render();
	if(next) next->render();
}

void Node::addChild(Node* c)
{
	if(children)
		c->next = children;
	
	children = c;
}

void Node::removeChild(Node* c)
{
}

void Node::removeChild(int i)
{
}
