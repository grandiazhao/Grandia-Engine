#ifndef GASCENEMGRCAR_H
#define GASCENEMGRCAR_H

#include "GASceneManager.h"

class GASceneMgrCar : public GASceneManager
{
public:
	GASceneMgrCar(GAPMainManager* mainMgr);
	void createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse);
	void createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse);
	bool switchToScene(std::string sceneName,bool reCreate);
};


#endif