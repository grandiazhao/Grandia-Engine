#include "GASceneMgrHand.h"
#include "GAFrameListener.h"
#include "GASceneHand.h"
#include "GASceneWalk.h"
#include "GAServerSimple.h"
#include "GAExampleSceneUI.h"

GASceneMgrHand::GASceneMgrHand(GAPMainManager* mainMgr):GASceneManager(mainMgr)
{
	g_previousSceneName="dp";
}

void GASceneMgrHand::createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse)
{
	GASceneManager::createScene(mWindow,Mouse);
	GAUIBase* b=new GAExampleSceneUI(mWindow,Mouse,"SceneUI");
	b->createUI();
	b->showAll();
	g_UIMgr->addNewUI(b);
	g_UIMgr->setCurrentUI("SceneUI");

	switchToScene("hand",false);
}

bool GASceneMgrHand::switchToScene(std::string sceneName,bool reCreate)
{
	GASceneBase* s=findScene(sceneName);
	if(s==0)
	{
		if(sceneName=="dp")
		{
			s=new GASceneHand(g_mainMgr,sceneName);
		}
		else if(sceneName=="walk")
		{
			s=new GASceneWalk(g_mainMgr,sceneName);
		}
		else return false;
		if(g_currentScene!=0) g_currentScene->freezeScene();
		s->createScene();
		g_sceneVec.push_back(s);
		g_currentScene=s;
		if(s==0) return false;
	}
	else
	{
		g_currentScene->freezeScene();
		s->reloadScene();
		if(reCreate||s->isSceneEnded()) s->reCreateScene();
		g_currentScene=s;
	}
	g_previousSceneName=sceneName;
	return true;
}

void GASceneMgrHand::frameStarted(float deltaTime)
{
	//processServerData;
	return GASceneManager::frameStarted(deltaTime);
}

//GASceneManager* GAFrameListener::GAMain(GAPMainManager* mainMgr)
//{
//	GASceneManager *m=new GASceneMgrHand(mainMgr);
//	return m;
//}