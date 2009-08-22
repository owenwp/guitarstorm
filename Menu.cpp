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
#include "Menu.h"

Node* Menu::cursor = NULL;
bool Menu::cursorActive = false;

void Item::Setup()
{
	/*osg::Geode* geodeItem = new osg::Geode;
    {
		itemText = new osgText::Text;
        itemText->setFont(prefix+"fonts/arial.ttf");
        if(enabled) itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
        else itemText->setColor(osg::Vec4(0.5f,0.5f,0.5f,1.0f));
		itemText->setCharacterSize(1.0f);
		itemText->setPosition(osg::Vec3(X,0.0f,Y));
		itemText->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		itemText->setDrawMode(osgText::Text::TEXT);
		itemText->setBackdropType(osgText::Text::OUTLINE);
		itemText->setBackdropOffset(-0.05f, 0.0f);
		itemText->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		itemText->setAlignment(osgText::Text::LEFT_CENTER);
        itemText->setAxisAlignment(osgText::Text::XZ_PLANE);
		itemText->setText(name);
		itemWidth = itemText->getBound().radius();
        
        geodeItem->addDrawable(itemText.get());
        
        _scene->addChild(geodeItem);
	 }*/
	
	Node* header = new Node;
	Label* l = new Label("arial", name, alignLeft);
	l->setColor(vec(1,1,1));
	header->addChild(l);
	header->setPosition(vec(0, 0));
	header->setScale(vec(0.5,0.5));
	addChild(header);
}

void Menu::Setup()
{	
	current = new Node;
	addChild(current);
	
	Node* header = new Node;
	Label* l = new Label("arial", name, alignCenter);
	l->setColor(vec(1,1,0));
	header->addChild(l);
	header->setPosition(vec(0, 3));
	header->setScale(vec(1,1));
	current->addChild(header);

	// add cursor
	if(cursor == NULL)
	{
		cursor = new Node;
		cursor->addChild(new Sprite(new Texture("pick.tga")));
		cursor->setCenter(vec(0.6f,0));
	}

	cursorOffset = -0.1;
}

void Menu::Highlight() 
{
	if(!cursorActive && cursor != NULL && items.size()>select) 
	{
		cursor->setTurn(0, 5);
		cursor->setSlide(vec(items[select]->getPosition().x + cursorOffset, items[select]->getPosition().y+0.25), 5.0f);
	}
}

void Menu::OnOpen()
{
	if(firstOpen && parent)
	{
		Add(new BackItem("Back", -2, -4));
	}
}

void Menu::Up()
{
	if(child)
	{
		child->Up();
	}
	else if(open && !hidden)
	{
		int i = select - 1;
		while(i >= 0)
		{
			if(items[i]->IsEnabled())
			{
				select = i;
				cursorActive = false;
				OnHighlight();
				break;
			}
			i--;
		}
		Highlight();
	}
}

void Menu::Down()
{
	if(child)
	{
		child->Down();
	}
	else if(open && !hidden)
	{
		int i = select + 1;
		while(i < items.size())
		{
			if(items[i]->IsEnabled())
			{
				select = i;
				cursorActive = false;
				OnHighlight();
				break;
			}
			i++;
		}
		Highlight();
	}
}

void Menu::Mouse(float x, float y)
{
	if(child)
	{
		child->Mouse(x,y);
	}
	else if(open && cursor)
	{
		/*Vec3 pos = cursor->getPosition();
		pos.x() = x * 0.04f;
		pos.z() = y * 0.04f;
		cursor->setSlide(pos, 10);
		cursor->setTurn(PI * -3 / 4, 5);
		cursorActive = true;

		vector<Item*>::iterator it;
		int index = 0;
		for(it=items.begin(); it!=items.end(); it++)
		{
			if(pos.z() > (*it)->Y - 0.5f && pos.z() < (*it)->Y + 0.5f)
			{
				if((*it)->IsEnabled() && pos.x() > (*it)->X)// && pos.x() < (*it)->X+(*it)->GetWidth())	
				{
					select = index;
					(*it)->Mouse(pos.x());
					OnHighlight();
					return;
				}
			}
			index++;
		}
		//select = -1;*/
	}
}

void Menu::Select()
{
	if(child)
	{
		child->Select();
	}
	else if(open)
	{
		if(select == -1)
		{
			return;
		}
		else if(items.size()) 
		{
			OnSelect();
			items[select]->Select();
			OnPostSelect();
		}
	}
}

void Menu::Left()
{
	if(child)
	{
		child->Left();
	}
	else if(open)
	{
		if(items.size()) 
		{
			items[select]->Left();
			OnValueChange();
		}
	}
}

void Menu::Right()
{
	if(child)
	{
		child->Right();
	}
	else if(open)
	{
		if(items.size()) 
		{
			items[select]->Right();
			OnValueChange();
		}
	}
}

void Menu::Key(char k)
{
	if(child)
	{
		child->Key(k);
	}
	else if(open)
	{
		if(items.size()) 
		{
			items[select]->Key(k);
			OnValueChange();
		}
	}
}

void Menu::Backspace()
{
	if(child)
	{
		child->Backspace();
	}
	else if(open)
	{
		if(items.size()) 
		{
			items[select]->Backspace();
			OnValueChange();
		}
	}
}

void Menu::Delete()
{
	if(child)
	{
		child->Delete();
	}
	else if(open)
	{
		if(items.size()) 
		{
			items[select]->Delete();
			OnValueChange();
		}
	}
}

void Menu::Add(Item* i)
{
	// sorted by Y position
	vector<Item*>::iterator it;
	for(it=items.begin(); it!=items.end(); it++)
	{
		if((*it)->getPosition().y < i->getPosition().y)
		{
			items.insert(it, i);
			i->SetParent(this);
			current->addChild(i);
			return;
		}
	}
	items.push_back(i);
	i->SetParent(this);
	current->addChild(i);
	Highlight();
}

void Menu::Open(Menu* c)
{
	if(c && !child)
	{
		// hide the current menu
		current->setHidden(true);

		// add the next menu
		c->parent = this;
		addChild(c);
		child = c;
		c->open = true;
		c->select = 0;
		c->Highlight();
		c->OnOpen();
		c->firstOpen = false;
	}
	else
	{
		OnOpen();
		Highlight();
		firstOpen = false;
		open = true;
		addChild(cursor);
		current->setHidden(false);
	}
}

void Menu::Close()
{
	OnClose();
	open = false;
	if(parent)
	{
		parent->OnValueChange();
		parent->Highlight();
		parent->removeChild(this);
		parent->current->setHidden(false);
		parent->child = NULL;
	}
	else
	{
		// this is the bottom of the stack, just hide the menu
		current->setHidden(true);
	}
}

void Menu::Hide()
{
	if(child)
		child->Hide();
	else
	{
		hidden = true;
		current->setHidden(true);
	}
}

void Menu::Show()
{
	if(child)
		child->Show();
	else
	{
		hidden = false;
		current->setHidden(false);
	}
}

Menu::~Menu()
{
	if(child) delete child;

	vector<Item*>::iterator it;
	for(it==items.begin(); it!=items.end(); it++)
	{
		delete (*it);
	}
}

// number item
void NumberItem::Setup()
{
	if(vstrings)
	{
		int i=0;
		while(vstrings[i].length()) i++;
		maximum = max(0, i-1);
	}

	/*osg::Geode* geodeNum = new osg::Geode;
    {
		vtext = new osgText::Text;
        vtext->setFont(prefix+"fonts/arial.ttf");
        vtext->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        vtext->setCharacterSize(1.0f);
		vtext->setPosition(osg::Vec3(X+12.0f,0.0f,Y));
		vtext->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		vtext->setDrawMode(osgText::Text::TEXT);
		vtext->setBackdropType(osgText::Text::OUTLINE);
		vtext->setBackdropOffset(-0.05f, 0.0f);
		vtext->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		vtext->setAlignment(osgText::Text::LEFT_CENTER);
        vtext->setAxisAlignment(osgText::Text::XZ_PLANE);
		vtext->setText(": ");
		Change();
        
        geodeNum->addDrawable(vtext.get());
        
        _scene->addChild(geodeNum);
    }*/
}

void NumberItem::Change()
{
	if(*value < 0)
		(*value)=0;
	if(*value > maximum)
		(*value)=maximum;

	std::ostringstream s;
	s << ": ";
	if(vstrings)
	{
		s << vstrings[*value];
	}
	else
	{
		s << *value;
	}
	//vtext->setText(s.str());
}

// real item
void RealItem::Setup()
{
	/*osg::Geode* geodeNum = new osg::Geode;
    {
		vtext = new osgText::Text;
        vtext->setFont(prefix+"fonts/arial.ttf");
        vtext->setColor(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        vtext->setCharacterSize(1.0f);
		vtext->setPosition(osg::Vec3(X+12.0f,0.0f,Y));
		vtext->setCharacterSizeMode(osgText::Text::OBJECT_COORDS);
		vtext->setDrawMode(osgText::Text::TEXT);
		vtext->setBackdropType(osgText::Text::OUTLINE);
		vtext->setBackdropOffset(-0.05f, 0.0f);
		vtext->setBackdropImplementation(osgText::Text::STENCIL_BUFFER);
		vtext->setAlignment(osgText::Text::LEFT_CENTER);
        vtext->setAxisAlignment(osgText::Text::XZ_PLANE);
		vtext->setText(": ");
		Change();
        
        geodeNum->addDrawable(vtext.get());
        
        _scene->addChild(geodeNum);
    }*/
}

void RealItem::Change()
{
	if(show)
	{
		std::ostringstream s;
		s << ": ";
		s << *value;
		//vtext->setText(s.str());
	}
	//else
		//vtext->setText("< >");
}
