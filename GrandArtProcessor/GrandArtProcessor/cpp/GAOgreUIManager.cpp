#include "GAOgreUIManager.h"
#include "GAPMainManager.h"

GAOgreUIManager::GAOgreUIManager(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	g_currentUI=0;
	g_UIVec.clear();
}

void GAOgreUIManager::init()
{
	
}

GAUIBase* GAOgreUIManager::getCurrentUI()
{
	return g_currentUI;
}
void GAOgreUIManager::addNewUI(GAUIBase* newUI)
{
	g_UIVec.push_back(newUI);
}

void GAOgreUIManager::setCurrentUI(std::string uiName)
{
	g_currentUI=findUI(uiName);
	g_currentUI->showAll();
}
GAUIBase* GAOgreUIManager::findUI(std::string name)
{
	for(UINT i=0;i<g_UIVec.size();i++)
	{
		if(g_UIVec[i]->g_name==name) return g_UIVec[i];
	}
	return 0;
}

void GAOgreUIManager::switchToUI(std::string name)
{
	GAUIBase* newUI=findUI(name);
	if(newUI==0) return;
	if(g_currentUI) g_currentUI->hideAll();
	newUI->showAll();
	g_currentUI=newUI;
}

bool GAOgreUIManager::mouseMoved(const OIS::MouseEvent& evt)
{
	if(g_currentUI&&g_currentUI->g_inUse) g_currentUI->mouseMoved(evt);
	return true;
}
bool GAOgreUIManager::keyPressed(const OIS::KeyEvent& evt)
{
	if(g_currentUI&&g_currentUI->g_inUse) g_currentUI->keyPressed(evt);
	return true;
}
void GAOgreUIManager::mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	if(g_currentUI&&g_currentUI->g_inUse) g_currentUI->mousePressed(evt,id);
}
void GAOgreUIManager::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	if(g_currentUI&&g_currentUI->g_inUse) g_currentUI->mouseReleased(evt,id);
}

void GAOgreUIManager::frameStarted(float deltaTime)
{
	if(g_currentUI&&g_currentUI->g_inUse) g_currentUI->frameStarted(deltaTime);
	_checkToSwitchUI();
}

void GAOgreUIManager::_checkToSwitchUI()
{
	if(g_currentUI&&g_currentUI->g_switchUI)
	{
		std::string str=g_currentUI->g_nextUIName;
		g_currentUI->g_switchUI=false;
		g_currentUI->g_nextUIName="";
		switchToUI(str);
	}
}