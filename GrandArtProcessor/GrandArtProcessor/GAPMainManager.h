#ifndef GAPMAINMANAGER_H
#define GAPMAINMANAGER_H

#include "Ogre.h"
#include "OIS.h"
#include "GAMainManagerStates.h"

class GAPCameraController;
class GAPWorldManager;
class GAPKeyboardInput;
class GAPAudioManager;
class GAPPhysicsManager;
class GAPCharacterController;
class GAPModelManager;
class GASceneManager;
class GAOgreUIManager;
class GADataManager;

class GAExport GAPMainManager
{
private:
public:
	GAPCameraController* g_cameraCtrl;
	GAPWorldManager* g_worldMgr;
	GAPKeyboardInput* g_keyboardInput;
	GAPAudioManager* g_audioMgr;
	GAPPhysicsManager* g_physicsMgr;
	GAPCharacterController* g_characterCtrl;
	GAPModelManager* g_modelMgr;
	GASceneManager* g_sceneMgr;
	GAOgreUIManager* g_UIMgr;
	GADataManager* g_dataMgr;

	bool g_zAxisUp;
	Ogre::Camera* mCamera;
	Ogre::SceneManager* mSceneMgr;
	Ogre::RenderWindow* mWindow;
public:
	GAPMainManager(Ogre::SceneManager* sceneMgr,Ogre::Camera* cam);
	void initManagers();
	void setSceneManager(GASceneManager* sceneMgr);

	void setNewSceneManager(Ogre::SceneManager* sm,Ogre::Camera* cam);
	GAMainManagerStates* saveCurrentStates();
	void loadMainManagerStates(GAMainManagerStates* states);

	bool injectKeyDown(OIS::KeyCode key);
	bool injectKeyUp(OIS::KeyCode key);

	bool preUpdate(float deltaTime);
	bool afterUpdate(float deltaTime);

	Ogre::SceneNode* getSceneNode(std::string nodeName);
	Ogre::Camera* getCurrentCamera();
	Ogre::SceneManager* getCurrentSceneMgr();

private:
	void initManagerWiring();
};



#endif