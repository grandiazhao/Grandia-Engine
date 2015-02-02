#ifndef GASCENEMANAGER_H
#define GASCENEMANAGER_H

#include "GASceneBase.h"
#include "GAOgreUIManager.h"

class GAPMainManager;

class GAExport GASceneManager
{
protected:
	GAOgreUIManager* g_UIMgr;
	GAPMainManager* g_mainMgr;
	std::string g_previousSceneName;
	std::vector<GASceneBase*> g_sceneVec;
	GASceneBase* g_currentScene;

	bool g_altPressed;
public:

public:
	GASceneManager(GAPMainManager* mainMgr);
	virtual void init();

	virtual void createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse);
	virtual void createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse);

	void addNewScene(GASceneBase* scene);
	GASceneBase* findScene(std::string sceneName);
	virtual bool switchToScene(std::string sceneName,bool reCreate=false)=0;

	virtual void frameStarted(float deltaTime);
	virtual void frameEnded(float deltaTime);

	virtual void sceneKeyPressed(const OIS::KeyEvent& evt);
	virtual void UIKeyPressed(const OIS::KeyEvent& evt);
	virtual void sceneKeyReleased(const OIS::KeyEvent& evt);
	virtual void UIKeyReleased(const OIS::KeyEvent& evt);
	virtual void sceneMouseMoved(const OIS::MouseEvent& evt);
	virtual void UIMouseMoved(const OIS::MouseEvent& evt);
	virtual void sceneMousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	virtual void UIMousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	virtual void sceneMouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	virtual void UIMouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	virtual void keyPressed(const OIS::KeyEvent& evt);
	virtual void keyReleased(const OIS::KeyEvent& evt);
	virtual void mouseMoved(const OIS::MouseEvent& evt);
	virtual void mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	virtual bool buttonPressed(const OIS::JoyStickEvent &evt,int num);
	virtual bool buttonReleased(const OIS::JoyStickEvent &evt,int num);
	virtual bool axisMoved(const OIS::JoyStickEvent &evt,int num);
	virtual bool povMoved(const OIS::JoyStickEvent &evt,int num);
	virtual bool sliderMoved(const OIS::JoyStickEvent &evt,int num);

	virtual void processServerData();
};

#endif