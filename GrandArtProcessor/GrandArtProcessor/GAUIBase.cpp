#include "GAUIBase.h"

GAUIBase::GAUIBase(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name)
{
	g_trayMgr=new OgreBites::SdkTrayManager(name,win,mouse,this);
	mWindow=win;
	hideCursor=false;
	g_inUse=false;
	g_name=name;
	g_switchUI=false;
	g_nextUIName="";
	g_enterScene=false;
}
void GAUIBase::firstShow()
{
	showAll();
}
void GAUIBase::showAll()
{
	g_trayMgr->showAll();
	hideCursor=false;
	g_inUse=true;
}

void GAUIBase::hideAll()
{
	g_trayMgr->hideAll();
	hideCursor=true;
	g_inUse=false;
}

void GAUIBase::frameStarted(float deltaTime)
{
	Ogre::FrameEvent evt;
	evt.timeSinceLastEvent=deltaTime;
	evt.timeSinceLastFrame=deltaTime;
	g_trayMgr->frameRenderingQueued(evt);
}

void GAUIBase::buttonHit(OgreBites::Button *btn){}

void GAUIBase::itemSelected(OgreBites::SelectMenu* menu){}

bool GAUIBase::mouseMoved(const OIS::MouseEvent& evt)
{
	return g_trayMgr->injectMouseMove(evt);
}

bool GAUIBase::keyPressed(const OIS::KeyEvent& evt){return true;}

bool GAUIBase::keyReleased(const OIS::KeyEvent& evt){return true;}

void GAUIBase::mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_trayMgr->injectMouseDown(evt,id);
}

void GAUIBase::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_trayMgr->injectMouseUp(evt,id);
}

void GAUIBase::loadUIFromFile(std::string UIName){}