#include "GASceneBase.h"
#include "GAPCameraController.h"

GASceneBase::GASceneBase(GAPMainManager* mainMgr,std::string sceneName)
{
	g_mainMgr=mainMgr;
	g_sceneName=sceneName;
	g_mainMgrStates=0;
	g_worldName="";
	g_freezed=false;
	g_sceneEnd=false;
	g_started=false;
	g_sceneRunningTime=0;
}
std::string GASceneBase::getSceneName()
{
	return g_sceneName;
}
bool GASceneBase::isSceneEnded()
{
	return g_sceneEnd;
}
bool GASceneBase::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager",g_sceneName);
	Ogre::Camera* cam=sm->createCamera(g_sceneName+"Cam");
	cam->setPosition(0,0,100);
    cam->lookAt(0,0,-300);
    cam->setNearClipDistance(5);
	g_mainMgr->setNewSceneManager(sm,cam);
	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
	newCamc->init();
	g_mainMgr->g_cameraCtrl=newCamc;
	g_mainMgr->mWindow->removeAllViewports();
	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
	return true;
}
bool GASceneBase::reCreateScene()
{
	return true;
}
bool GASceneBase::freezeScene()
{
	if(g_mainMgrStates!=0) delete g_mainMgrStates;
	g_mainMgrStates=g_mainMgr->saveCurrentStates();
	g_mainMgr->mWindow->removeViewport(0);
	g_freezed=true;
	return true;
}
bool GASceneBase::reloadScene()
{
	if(g_mainMgrStates!=0)
	{
		g_mainMgr->loadMainManagerStates(g_mainMgrStates);
		g_mainMgr->mWindow->removeAllViewports();
		g_mainMgr->mWindow->addViewport(g_mainMgr->getCurrentCamera());
		g_freezed=false;
	}
	return true;
}
bool GASceneBase::destroyScene()
{
	return true;
}

bool GASceneBase::frameStarted(float deltaTime)
{
	return true;
}
bool GASceneBase::frameEnded(float deltaTime)
{
	g_sceneRunningTime+=deltaTime;
	return true;
}

bool GASceneBase::keyPressed(const OIS::KeyEvent& evt)
{
	return true;
}
bool GASceneBase::keyReleased(const OIS::KeyEvent& evt)
{
	return true;
}

bool GASceneBase::mouseMoved(const OIS::MouseEvent& evt)
{
	return true;
}
bool GASceneBase::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}
bool GASceneBase::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	return true;
}

bool GASceneBase::buttonPressed(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GASceneBase::buttonReleased(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GASceneBase::axisMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GASceneBase::povMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}
bool GASceneBase::sliderMoved(const OIS::JoyStickEvent &evt,int num)
{
	return true;
}

bool GASceneBase::processServerData(std::string recvString,GAServerSimple* server,UINT socketNum)
{
	return true;
}