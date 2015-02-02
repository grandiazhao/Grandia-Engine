#ifndef GAOGREUIMANAGER_H
#define GAOGREUIMANAGER_H

#include "Common.h"
#include "GAUIBase.h"

class GAPMainManager;

class GAExport GAOgreUIManager
{
private:
	std::vector<GAUIBase*> g_UIVec;
	GAUIBase* g_currentUI;
	GAPMainManager* g_mainMgr;
public:
	GAOgreUIManager(GAPMainManager* mainMgr);

	void init();
	void setCurrentUI(std::string uiName);
	void addNewUI(GAUIBase* newUI);
	GAUIBase* findUI(std::string uiName);
	void switchToUI(std::string uiName);
	void frameStarted(float deltaTime);
	GAUIBase* getCurrentUI();

	bool mouseMoved(const OIS::MouseEvent& evt);
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
	void mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
private:
	void _checkToSwitchUI();
};


#endif