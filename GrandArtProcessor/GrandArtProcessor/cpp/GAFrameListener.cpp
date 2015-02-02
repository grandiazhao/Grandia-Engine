#include "GAFrameListener.h"
#include "GAPCameraController.h"

#include "GASceneCar1.h"
#include "GASceneKangFu0.h"
#include "GAOgreUIManager.h"
#include "GASceneManager.h"

GAFrameListener::GAFrameListener(Ogre::RenderWindow *win,Ogre::Camera* cam,GAPMainManager* mainMgr)
	: ExampleFrameListener(win,cam,true,true,true)
{
	g_mainMgr=mainMgr;
	mKeyboard->setEventCallback(this);
	mMouse->setEventCallback(this);
	if(mJoy)
	{
		try{
			mJoy->setEventCallback(this);
		}catch(...)
		{
			mJoy=0;
		}
	}
	mCamera->setNearClipDistance(0.1);
	g_exit=false;
	g_totalTime=g_lastTime=0;
	wtf=new MyWriteToFile("frameListenerOutput.txt");
	g_mainMgr->g_cameraCtrl->setCameraStyle(GAPCameraController::CameraStyle_Free);
	g_mainMgr->g_sceneMgr->createScene(mWindow,mMouse);
	
}

bool GAFrameListener::frameStarted(const FrameEvent &evt)
{
	g_lastTime=16-(evt.timeSinceLastFrame*1000-g_lastTime);
	int lastt=g_lastTime;
	if(lastt>=0)  Sleep(lastt);
	else g_lastTime=0;

	if(mJoy!=0)
	{
		mJoy->capture();
	}
	mMouse->capture();
	mKeyboard->capture();
	g_mainMgr->preUpdate(evt.timeSinceLastFrame);
	g_mainMgr->g_sceneMgr->frameStarted(evt.timeSinceLastFrame);
	return ExampleFrameListener::frameStarted(evt);
}

bool GAFrameListener::frameEnded(const FrameEvent& evt)
{
	if(g_exit) return false;
	g_totalTime+=evt.timeSinceLastFrame;
	g_mainMgr->g_sceneMgr->frameEnded(evt.timeSinceLastFrame);
	g_mainMgr->afterUpdate(evt.timeSinceLastFrame);
	static bool firstRender=true;
	if(firstRender)
	{
		firstRender=false;
	}
	return ExampleFrameListener::frameEnded(evt);
}

bool GAFrameListener::keyPressed(const OIS::KeyEvent& evt)
{
	g_mainMgr->injectKeyDown(evt.key);
	g_mainMgr->g_sceneMgr->keyPressed(evt);
	return true;
}

bool GAFrameListener::keyReleased(const OIS::KeyEvent& evt)
{
	/*if(evt.key==OIS::KC_ESCAPE)
	{
		g_exit=true;
	}*/
	g_mainMgr->injectKeyUp(evt.key);
	g_mainMgr->g_sceneMgr->keyReleased(evt);
	if(evt.key==OIS::KC_J)
	{
	/*	Ogre::OverlayElement* ol=Ogre::OverlayManager::getSingleton().getOverlayElement("l1/LabelCaption");
		((Ogre::TextAreaOverlayElement*)ol)->setCaption(L"ÎÒºÜºÃ");*/
	}
	
	return true;
}

bool GAFrameListener::mouseMoved(const OIS::MouseEvent& evt)
{
	g_mainMgr->g_cameraCtrl->updateRotate(evt.state.X.rel*(-0.1f),evt.state.Y.rel*(-0.1f));
	g_mainMgr->g_sceneMgr->mouseMoved(evt);
	return true;
}

bool GAFrameListener::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	g_mainMgr->g_sceneMgr->mousePressed(evt,id);
	return true;
}
bool GAFrameListener::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_mainMgr->g_sceneMgr->mouseReleased(evt,id);
	return true;
}
bool GAFrameListener::buttonPressed(const OIS::JoyStickEvent &evt,int num)
{
	return g_mainMgr->g_sceneMgr->buttonPressed(evt,num);
}
bool GAFrameListener::buttonReleased(const OIS::JoyStickEvent &evt,int num)
{
	return g_mainMgr->g_sceneMgr->buttonReleased(evt,num);
}
bool GAFrameListener::axisMoved(const OIS::JoyStickEvent &evt,int num)
{
	return g_mainMgr->g_sceneMgr->axisMoved(evt,num);
}
bool GAFrameListener::povMoved(const OIS::JoyStickEvent &evt,int num)
{
	return g_mainMgr->g_sceneMgr->povMoved(evt,num);
}
bool GAFrameListener::sliderMoved(const OIS::JoyStickEvent &evt,int num)
{
	return g_mainMgr->g_sceneMgr->sliderMoved(evt,num);;
}