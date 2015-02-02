#include "GAMainManagerStates.h"
#include "GAPMainManager.h"
#include "GAPCameraController.h"
#include "GAPCharacterController.h"


GAMainManagerStates::GAMainManagerStates()
{
	g_camera=0;
	g_cameraCtrl=0;
	g_characterCtrl=0;
	g_ogreSceneMgr=0;
}

void GAMainManagerStates::createMainManagerStates(GAPMainManager* mainMgr)
{
	g_camera=mainMgr->getCurrentCamera();
	g_ogreSceneMgr=mainMgr->getCurrentSceneMgr();
	g_cameraCtrl=mainMgr->g_cameraCtrl;
	g_characterCtrl=mainMgr->g_characterCtrl;
}
