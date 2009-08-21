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
#ifndef MENU
#define MENU

#include <iostream>
#include <sstream>
#include <vector>

#include "Sprite.h"
#include "Node.h"
#include "Label.h"

#if MACOSX
#define prefix string("/library/")
#else 
#define prefix string("")
#endif

using namespace std;

class Menu;
class Item : public Node
{
public:
	const string name;
	const float X, Y;

	void Enable(bool e) {enabled = e; }//if(enabled) itemText->setColor(osg::Vec4(1.0f,1.0f,1.0f,1.0f)); else itemText->setColor(osg::Vec4(0.5f,0.5f,0.5f,1.0f));}
	bool IsEnabled() {return enabled;}
	float GetWidth() {return itemWidth;}

	// override these methods for menu item functionality
	virtual void Highlight() {}
	virtual void Mouse(float x) {}
	virtual void Select() {}
	virtual void Left() {}
	virtual void Right() {}
	virtual void Key(char k) {}
	virtual void Backspace() {}
	virtual void Delete() {}

	void SetParent(Menu* p) {parent = p;}

protected:
	Item(string n, float x, float y, bool e = true):name(n),X(x),Y(y),enabled(e),parent(NULL) {Setup();}
	
	// method for initializing the menu item
	virtual void Setup();

	float itemWidth;
	Menu* parent;
	bool enabled;

private:
	Item():X(0),Y(0),name(""){}
};

class Menu : public Node
{
public:
	Menu(string n):open(false),firstOpen(true),name(n),select(0),parent(NULL),child(NULL),hidden(false) {Setup();}

	void Up();
	void Down();
	void Mouse(float x, float y);
	void Select();
	void Left();
	void Right();
	void Key(char k);
	void Backspace();
	void Delete();

	void Add(Item* i);

	void Open(Menu* c = NULL);
	void Close();
	void Hide();
	void Show();
	bool IsOpen() {return open;}

protected:
	bool firstOpen;

	// override to implement menu events
	virtual void Setup();
	virtual void OnHighlight() {}
	virtual void OnValueChange() {}
	virtual void OnSelect() {}
	virtual void OnPostSelect() {}
	virtual void OnClose() {}
	virtual void OnOpen();

	void Highlight();

	float cursorOffset;
	string name;
	int select;
	vector<Item*> items;
	Menu* parent;
	Menu* child;
	bool open;
	bool hidden;
	
	static Sprite* cursor;
	static bool cursorActive;

	~Menu();
};

// menu item that does nothing
class NullItem : public Item
{
public:
	NullItem(string n, float x, float y, bool e = true):Item(n,x,y,e){}
};

// opens a new menu
class MenuItem : public Item
{
public:
	MenuItem(string n, Menu* m, float x, float y):Item(n,x,y),menu(m){}
	void Select() {parent->Open(menu);}

protected:
	Menu* menu;
};

// goes back to the previous menu
class BackItem : public Item
{
public:
	BackItem(string n, float x, float y):Item(n,x,y){}
	void Select() {parent->Close();}
};

// calls the given event when selected
class EventItem : public Item
{
public:
	EventItem(string n, void (*e)(), float x, float y):Item(n,x,y),eventMethod(e){}
	void Select() {eventMethod();}

protected:
	void (*eventMethod)();
};

class NumberItem : public Item
{
public:
	NumberItem(string n, int *val, int max, float x, float y):Item(n,x,y),value(val),maximum(max),vstrings(NULL){Setup();}
	NumberItem(string n, int *val, string *vstrs, float x, float y):Item(n,x,y),value(val),maximum(0),vstrings(vstrs){Setup();}
	void Left() {(*value)--; Change();}
	void Right() {(*value)++; Change();}
	virtual void Select() {(*value)++; if(*value > maximum) (*value)=0; Change();}

protected:
	int *value;
	string *vstrings;
	int maximum;

	virtual void Setup();
	virtual void Change();
};

class RealItem : public Item
{
public:
	RealItem(string n, float *val, float inc, bool s, float x, float y):Item(n,x,y),value(val),increment(inc),show(s){Setup();}
	void Left() {(*value)-=increment; Change();}
	void Right() {(*value)+=increment; Change();}
protected:
	float *value;
	float increment;
	bool show;

	virtual void Setup();
	virtual void Change();
};

#endif
