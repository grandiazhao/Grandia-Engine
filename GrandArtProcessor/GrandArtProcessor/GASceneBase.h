#ifndef GASCENEBASE_H
#define GASCENEBASE_H

#include "Common.h"
#include "GAPMainManager.h"
#include "GAServerSimple.h"

class GAExport GASceneBase
{
protected:
	GAPMainManager* g_mainMgr;
	std::string g_sceneName;
	std::string g_worldName;
	GAMainManagerStates* g_mainMgrStates;
	bool g_freezed;
	bool g_sceneEnd;
	bool g_started;
	float g_sceneRunningTime;
public:
	GASceneBase(GAPMainManager* mainMgr,std::string sceneName);
	
	std::string getSceneName();
	bool isSceneEnded();

	virtual bool createScene();
	virtual bool reCreateScene();
	virtual bool freezeScene();
	virtual bool reloadScene();
	virtual bool destroyScene();

	virtual bool frameStarted(float deltaTime);
	virtual bool frameEnded(float deltaTime);

	virtual bool keyPressed(const OIS::KeyEvent& evt);
	virtual bool keyReleased(const OIS::KeyEvent& evt);

	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	virtual bool buttonPressed(const OIS::JoyStickEvent &evt,int num);
	virtual bool buttonReleased(const OIS::JoyStickEvent &evt,int num);
	virtual bool axisMoved(const OIS::JoyStickEvent &evt,int num);
	virtual bool povMoved(const OIS::JoyStickEvent &evt,int num);
	virtual bool sliderMoved(const OIS::JoyStickEvent &evt,int num);

	virtual bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);
};


#endif