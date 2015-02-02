#include "GAPMainManager.h"
#include "GAPCameraController.h"
#include "GAPWorldManager.h"
#include "GAPKeyboardInput.h"
#include "GAPAudioManager.h"
#include "GAPPhysicsManager.h"
#include "GAPCharacterController.h"
#include "GAPModelManager.h"
#include "GAOgreUIManager.h"
#include "GASceneManager.h"
#include "GADataManager.h"

GAPMainManager::GAPMainManager(Ogre::SceneManager* sceneMgr,Ogre::Camera* cam)
{
	mSceneMgr=sceneMgr;
	mCamera=cam;
	g_zAxisUp=true;
	g_keyboardInput=new GAPKeyboardInput();
	g_cameraCtrl=new GAPCameraController(this);
	g_worldMgr=new GAPWorldManager(this);
	g_audioMgr=new GAPAudioManager(this);
	g_physicsMgr=new GAPPhysicsManager(this);
	g_characterCtrl=new GAPCharacterController(this);
	g_modelMgr=new GAPModelManager(this);
	g_sceneMgr=new GASceneManager(this);
	g_UIMgr=new GAOgreUIManager(this);
	g_dataMgr=new GADataManager(this);
}

Ogre::SceneNode* GAPMainManager::getSceneNode(std::string nodeName)
{
	return mSceneMgr->getSceneNode(nodeName);
}

Ogre::Camera* GAPMainManager::getCurrentCamera()
{
	return mCamera;
}

Ogre::SceneManager* GAPMainManager::getCurrentSceneMgr()
{
	return mSceneMgr;
}

void GAPMainManager::setNewSceneManager(Ogre::SceneManager* sm,Ogre::Camera* cam)
{
	mSceneMgr=sm;
	mCamera=cam;
}

GAMainManagerStates* GAPMainManager::saveCurrentStates()
{
	GAMainManagerStates* s=new GAMainManagerStates();
	s->createMainManagerStates(this);
	return s;
}

void GAPMainManager::loadMainManagerStates(GAMainManagerStates* states)
{
	mSceneMgr=states->g_ogreSceneMgr;
	mCamera=states->g_camera;
	g_characterCtrl=states->g_characterCtrl;
	g_cameraCtrl=states->g_cameraCtrl;
}

void GAPMainManager::initManagers()
{
	g_cameraCtrl->init();
	g_characterCtrl->init();
	g_worldMgr->init();
	g_physicsMgr->init();
	g_modelMgr->init();
	g_UIMgr->init();
	g_sceneMgr->init();
	g_dataMgr->init();
	//g_physicsMgr->setBspWorld("house1_4.bsp");
	initManagerWiring();
}

void GAPMainManager::initManagerWiring()
{
}

bool GAPMainManager::injectKeyDown(OIS::KeyCode key)
{
	g_keyboardInput->updateKeyDown(key);
	g_characterCtrl->injectKeyDown();
	g_cameraCtrl->injectKeyDown();
	return true;
}

bool GAPMainManager::injectKeyUp(OIS::KeyCode key)
{
	g_keyboardInput->updateKeyUp(key);
	g_characterCtrl->injectKeyDown();
	g_cameraCtrl->injectKeyUp();
	return true;
}

bool GAPMainManager::preUpdate(float deltaTime)
{
	g_audioMgr->update();
	g_modelMgr->update(deltaTime);
	g_characterCtrl->update(deltaTime);
	g_cameraCtrl->update(deltaTime);
	return true;
}

bool GAPMainManager::afterUpdate(float deltaTime)
{
	return true;
}