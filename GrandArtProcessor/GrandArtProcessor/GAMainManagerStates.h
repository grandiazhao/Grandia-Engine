#ifndef GAMAINMANAGERSTATES_H
#define GAMAINMANAGERSTATES_H

#include "Common.h"

class GAPCameraController;
class GAPWorldManager;
class GAPAudioManager;
class GAPPhysicsManager;
class GAPCharacterController;
class GAPModelManager;
class GASceneManager;
class GAOgreUIManager;
class GAPMainManager;

class GAExport GAMainManagerStates
{
	friend class GAPMainManager;
private:
	GAPCameraController* g_cameraCtrl;
	GAPCharacterController* g_characterCtrl;
	struct CameraStates
	{
	};
	struct CharacterStates
	{
	};
	Ogre::SceneManager* g_ogreSceneMgr;
	Ogre::Camera* g_camera;
public:
	GAMainManagerStates();
	void createMainManagerStates(GAPMainManager* mainMgr);
};


#endif