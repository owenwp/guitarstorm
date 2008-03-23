#ifndef KEYBOARD
#define KEYBOARD

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/io_utils>

#include <osg/MatrixTransform>
#include <osg/Geode>
#include <osg/Group>
#include <osg/Switch>
#include <osg/Notify>
#include <osg/Geometry>

#include <osgText/Text>

#include "Notes.h"
#include "SongPick.h"

class KeyboardModel : public osg::Referenced
{
public:
        osg::Group* getScene() { return _scene.get(); }
        
        void keyChange(int key,int value);

		void setNotes(osg::ref_ptr<Notes> n) {notes = n;}
		void setPicker(osg::ref_ptr<SongPick> p) {picker = p;}
        
protected:

		osg::ref_ptr<Notes> notes;
		osg::ref_ptr<SongPick> picker;
        
        ~KeyboardModel() {}

        typedef std::map<int, osg::ref_ptr<osg::Switch> > KeyModelMap;
        
        osg::ref_ptr<osg::Group>    _scene;
        KeyModelMap                 _keyModelMap;
        osg::ref_ptr<osgText::Text> _inputText;

};

class KeyboardEventHandler : public osgGA::GUIEventHandler
{
public:
    
        KeyboardEventHandler(KeyboardModel* keyboardModel):
            _keyboardModel(keyboardModel) {}
    
        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter&)
        {

#if 1

//            osg::notify(osg::NOTICE)<<"Mouse "<<ea.getButtonMask()<<std::endl;

            #define PRINT(mask) osg::notify(osg::NOTICE)<<#mask<<" ="<<(ea.getModKeyMask() & mask)<<std::endl;
            switch(ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::KEYDOWN):
                case(osgGA::GUIEventAdapter::KEYUP):
                {
                    osg::notify(osg::NOTICE)<<std::endl;
                    PRINT(osgGA::GUIEventAdapter::MODKEY_LEFT_SHIFT);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_RIGHT_SHIFT);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_LEFT_ALT);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_RIGHT_ALT);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_LEFT_CTRL);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_RIGHT_CTRL);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_LEFT_META);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_RIGHT_META);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_NUM_LOCK);
                    PRINT(osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK);
                    break;
                }
                default:
                    break;
            }
#endif
            switch(ea.getEventType())
            {
                case(osgGA::GUIEventAdapter::KEYDOWN):
                {
                    _keyboardModel->keyChange(ea.getKey(),1);
                    return true;
                }
                case(osgGA::GUIEventAdapter::KEYUP):
                {
                    _keyboardModel->keyChange(ea.getKey(),0);
                    return true;
                }

                default:
                    return false;
            }
        }
        
        osg::ref_ptr<KeyboardModel> _keyboardModel;
        
};

#endif
