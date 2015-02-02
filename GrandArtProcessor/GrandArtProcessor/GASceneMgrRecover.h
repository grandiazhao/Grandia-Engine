#ifndef GASCENEMGRRECOVER_H
#define GASCENEMGRRECOVER_H

#include "GASceneManager.h"
#include "GADataManager.h"
#include "GAFileIO.h"

class GASceneMgrRecover : public GASceneManager
{
public:
	GASceneMgrRecover(GAPMainManager* mainMgr);
	~GASceneMgrRecover();
	void createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse);
	void createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse);
	bool switchToScene(std::string sceneName,bool reCreate);

	void frameStarted(float deltaTime);
	void startQuit(float deltaTime);
private:
	void processServerData();
	GADataMap* stringMap;
	GAServerSimple* server;
	GAPMisc misc;
	GAFileIO* output;
	bool g_startQuit;
};


#endif