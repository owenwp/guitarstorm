#include "Keyboard.h"

void KeyboardModel::keyChange(int key,int value)
{
    osg::notify(osg::INFO) << "key value change, code="<<std::hex << key << "\t value="<< value << std::dec  << std::endl;
    
    if (value)
    {    
		// handle speed events
		if(key >= '0' && key <= '9')
		{
			notes->setSpeed((key - '0') * 10);
		}
		else if (key==osgGA::GUIEventAdapter::KEY_BackSpace || key==osgGA::GUIEventAdapter::KEY_Delete) 
        {
			notes->setSpeed(100);
            if (!_inputText->getText().empty())
            {
                _inputText->getText().pop_back();
                _inputText->update();
            }
        }
        
		// handle menu events
		else if(key == osgGA::GUIEventAdapter::KEY_Up)
		{
			if(!notes->running) 
			{
				picker->up();
			}
		}
		else if(key == osgGA::GUIEventAdapter::KEY_Down)
		{
			if(!notes->running) 
			{
				picker->down();
			}
		}
        else if (key==osgGA::GUIEventAdapter::KEY_Return)
        {
			string n = picker->get();
			if(!notes->running) 
			{
				visible(true);
				picker->visible(false);
				notes->setSong(n);
			}
			else if(notes->finished)
			{
				notes->running = false;
				notes->finished = false;
				notes->visible(false);
				visible(false);

				picker->visible(true);
			}
        }        
    }
    
}

osg::Switch* KeyboardModel::addKey(osg::Vec3& pos, int key,const std::string& text,float width, float height)
{

    osg::Geode* geodeUp = new osg::Geode;
    {
        osgText::Text* textUp = new osgText::Text;
        //textUp->setFont("fonts/arial.ttf");
        textUp->setColor(osg::Vec4(0.3f,0.3f,0.3f,1.0f));
        textUp->setCharacterSize(height);
        textUp->setPosition(pos);
        textUp->setDrawMode(osgText::Text::TEXT/*|osgText::Text::BOUNDINGBOX*/);
        textUp->setAlignment(osgText::Text::LEFT_CENTER);
        textUp->setAxisAlignment(osgText::Text::XZ_PLANE);
        textUp->setText(text);
        
        geodeUp->addDrawable(textUp);
    }
    
    osg::Geode* geodeDown = new osg::Geode;
    {
        osgText::Text* textDown = new osgText::Text;
        //textDown->setFont("fonts/arial.ttf");
        textDown->setColor(osg::Vec4(1.0f,0.0f,1.0f,1.0f));
        textDown->setCharacterSize(height);
        textDown->setPosition(pos);
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

    _keyModelMap[key] = model;
    
    pos.x() += width;
    
    return model;
    
}

osg::Switch* KeyboardModel::addKey(int key,osg::Switch* sw)
{
    _keyModelMap[key] = sw;
    return sw;
}

void KeyboardModel::createKeyboard()
{
    _scene = new osg::Group;
    
    osg::Vec3 origin(0.0f,0.0f,0.0f);
    osg::Vec3 pos=origin;

	pos.x() = 0.0f;
	addKey('E',addKey(pos,'e',"E------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
	addKey('A',addKey(pos,'a',"A------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
	addKey('D',addKey(pos,'d',"D------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
	addKey('G',addKey(pos,'g',"G------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
	addKey('B',addKey(pos,'b',"B------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
	addKey('e',addKey(pos,'e',"e------------------------------------------------------------",1.0f,1.0f));
	pos.x() = 0.0f;
    pos.z() += 1.0f;
    
    float totalWidth = pos.x()-origin.x();
    pos = origin;
    pos.z() += -1.5f;

	visible(false);
}
