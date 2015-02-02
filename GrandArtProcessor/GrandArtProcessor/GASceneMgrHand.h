#ifndef GASCENEMGRHAND_H
#define GASCENEMGRHAND_H

#include "Common.h"
#include "GASceneManager.h"

class GASceneMgrHand : public GASceneManager
{
private:

public:
	GASceneMgrHand(GAPMainManager* mainMgr);
	void createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse);
	//void createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse);
	bool switchToScene(std::string sceneName,bool reCreate);
	void frameStarted(float deltaTime);

};

#endif