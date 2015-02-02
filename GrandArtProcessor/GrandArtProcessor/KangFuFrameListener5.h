#ifndef KANGFUFRAMELISTENER_H
#define KANGFUFRAMELISTENER_H

#include "Common.h"
#include "ExampleFrameListener.h"
#include "GAPMainManager.h"
#include "MyWriteToFile.h"

class KangFuFrameListener : public ExampleFrameListener,OIS::KeyListener,OIS::MouseListener,OIS::JoyStickListener
{
private:
	bool g_exit;
	float g_totalTime;
	float g_lastTime;
	GAPMainManager* g_mainMgr;
	MyWriteToFile *wtf;
public:
	KangFuFrameListener(Ogre::RenderWindow *win,Ogre::Camera* cam,GAPMainManager* mainMgr);
	bool frameStarted(const FrameEvent &evt);
	bool frameEnded(const FrameEvent& evt);
	bool OIS::KeyListener::keyPressed(const OIS::KeyEvent& evt);
	bool OIS::KeyListener::keyReleased(const OIS::KeyEvent& evt);
	bool mouseMoved(const OIS::MouseEvent& evt);
	bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	bool OIS::JoyStickListener::buttonPressed(const OIS::JoyStickEvent &evt,int num);
	bool OIS::JoyStickListener::buttonReleased(const OIS::JoyStickEvent &evt,int num);
	bool OIS::JoyStickListener::axisMoved(const OIS::JoyStickEvent &evt,int num);
	bool povMoved(const OIS::JoyStickEvent &evt,int num);
	bool sliderMoved(const OIS::JoyStickEvent &evt,int num);
};

#endif