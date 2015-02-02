#include "GASceneManager.h"
#include "GAPMainManager.h"
#include "GASceneBase.h"
#include "GASceneKangFu0.h"
#include "GAExampleSettingUI.h"
#include "GAExampleEnterUI.h"
#include "GABlankScene.h"
#include "SceneMiGong.h"
#include "SceneChabei.h"
#include "GAExampleSceneUI.h"
#include "SceneYunDong.h"
#include "ScenePingHeng.h"
#include "SceneWuPin.h"

GASceneManager::GASceneManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	g_currentScene=0;
	g_altPressed=false;
}

void GASceneManager::init()
{
	g_UIMgr=g_mainMgr->g_UIMgr;
}

void GASceneManager::frameStarted(float deltaTime)
{
	g_UIMgr->frameStarted(deltaTime);
	if(g_UIMgr->getCurrentUI()!=0&&g_UIMgr->getCurrentUI()->g_enterScene)
	{
		g_UIMgr->getCurrentUI()->g_enterScene=false;
		bool reCreate=g_UIMgr->getCurrentUI()->g_reCreateScene;
		g_UIMgr->getCurrentUI()->g_reCreateScene=false;
		if(g_UIMgr->getCurrentUI()->g_sceneName=="")
		{
			switchToScene(g_previousSceneName,reCreate);	
		}
		else switchToScene(g_UIMgr->getCurrentUI()->g_sceneName,reCreate);
	}
	if(g_currentScene) g_currentScene->frameStarted(deltaTime);
}

void GASceneManager::frameEnded(float deltaTime)
{
	if(g_currentScene) g_currentScene->frameEnded(deltaTime);
}
void GASceneManager::createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* mMouse)
{
	GASceneBase* s=new GABlankScene(g_mainMgr,"blank");
	s->createScene();
	g_sceneVec.push_back(s);
	g_currentScene=s;
}
void GASceneManager::createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse)
{
}

void GASceneManager::UIKeyPressed(const OIS::KeyEvent& evt)
{
	g_UIMgr->keyPressed(evt);
}

void GASceneManager::UIKeyReleased(const OIS::KeyEvent& evt)
{
	g_UIMgr->keyReleased(evt);
	/*bool hit;
	std::string ques;
	g_UIMgr->getCurrentUI()->g_trayMgr->yesNoDialogClosed(ques,hit);*/
}

void GASceneManager::UIMouseMoved(const OIS::MouseEvent& evt)
{
	g_UIMgr->mouseMoved(evt);
}

void GASceneManager::UIMousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_UIMgr->mousePressed(evt,id);
}

void GASceneManager::UIMouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_UIMgr->mouseReleased(evt,id);
}

void GASceneManager::sceneKeyPressed(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_LMENU||evt.key==OIS::KC_RMENU) g_altPressed=true;
	if(g_altPressed&&evt.key==OIS::KC_F4) Ogre::Root::getSingleton().queueEndRendering();
	if(g_currentScene) g_currentScene->keyPressed(evt);
}
void GASceneManager::sceneKeyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_LMENU||evt.key==OIS::KC_RMENU) g_altPressed=false;
	if(g_currentScene) g_currentScene->keyReleased(evt);
}
void GASceneManager::sceneMouseMoved(const OIS::MouseEvent& evt)
{
	if(g_currentScene) g_currentScene->mouseMoved(evt);
}
void GASceneManager::sceneMousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	if(g_currentScene) g_currentScene->mousePressed(evt,id);
}
void GASceneManager::sceneMouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	if(g_currentScene) g_currentScene->mouseReleased(evt,id);
}

void GASceneManager::keyPressed(const OIS::KeyEvent& evt)
{
	UIKeyPressed(evt);
	sceneKeyPressed(evt);
}
void GASceneManager::keyReleased(const OIS::KeyEvent& evt)
{
	UIKeyReleased(evt);
	sceneKeyReleased(evt);
}
void GASceneManager::mouseMoved(const OIS::MouseEvent& evt)
{
	UIMouseMoved(evt);
	sceneMouseMoved(evt);
}
void GASceneManager::mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	UIMousePressed(evt,id);
	sceneMousePressed(evt,id);
}
void GASceneManager::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	UIMouseReleased(evt,id);
	sceneMouseReleased(evt,id);
}

bool GASceneManager::buttonPressed(const OIS::JoyStickEvent &evt,int num)
{
	if(g_currentScene)
	{
		return g_currentScene->buttonPressed(evt,num);
	}
	else return false;
	
}
bool GASceneManager::buttonReleased(const OIS::JoyStickEvent &evt,int num)
{
	if(g_currentScene)
	{
		return g_currentScene->buttonReleased(evt,num);
	}
	else return false;
}
bool GASceneManager::axisMoved(const OIS::JoyStickEvent &evt,int num)
{
	if(g_currentScene)
	{
		return g_currentScene->axisMoved(evt,num);
	}
	else return false;
}
bool GASceneManager::povMoved(const OIS::JoyStickEvent &evt,int num)
{
	if(g_currentScene)
	{
		return g_currentScene->povMoved(evt,num);
	}
	else return false;
}
bool GASceneManager::sliderMoved(const OIS::JoyStickEvent &evt,int num)
{
	if(g_currentScene)
	{
		return g_currentScene->sliderMoved(evt,num);
	}
	else return false;
}

void GASceneManager::addNewScene(GASceneBase* scene)
{
	g_sceneVec.push_back(scene);
}
GASceneBase* GASceneManager::findScene(std::string sceneName)
{
	for(UINT i=0;i<g_sceneVec.size();i++)
	{
		if(g_sceneVec[i]->getSceneName()==sceneName)
		{
			return g_sceneVec[i];
		}
	}
	return 0;
}

void GASceneManager::processServerData()
{

}