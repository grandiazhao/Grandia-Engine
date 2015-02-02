#ifndef GAUIBASE_H
#define GAUIBASE_H

#include "Common.h"
#include "OIS.h"
#include "SdkTrays.h"

class GAUIBase : public OgreBites::SdkTrayListener
{
	friend class GAOgreUIManager;
protected:
	bool hideCursor;
	std::string g_nextUIName;
	bool g_switchUI;
public:
	bool g_enterScene;
	bool g_reCreateScene;
	std::string g_sceneName;
public:
	OgreBites::SdkTrayManager* g_trayMgr;
	Ogre::RenderWindow* mWindow;
	bool g_inUse;
	std::string g_name;
public:
	GAUIBase(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name);

	virtual void createUI()=0;
	virtual void firstShow();
	void loadUIFromFile(std::string UIName);

	virtual void showAll();
	virtual void hideAll();

	virtual void frameStarted(float deltaTime);

	virtual void buttonHit(OgreBites::Button *btn);
	virtual void itemSelected(OgreBites::SelectMenu* smenu);
	
	virtual bool mouseMoved(const OIS::MouseEvent& evt);
	virtual bool keyPressed(const OIS::KeyEvent& evt);
	virtual bool keyReleased(const OIS::KeyEvent& evt);
	virtual void mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	virtual void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	bool isCursorOver(Ogre::OverlayElement* element, const Ogre::Vector2& cursorPos, Ogre::Real voidBorder = 0)
	{
		Ogre::OverlayManager& om = Ogre::OverlayManager::getSingleton();
        Ogre::Real l = element->_getDerivedLeft() * om.getViewportWidth();
        Ogre::Real t = element->_getDerivedTop() * om.getViewportHeight();
        Ogre::Real r = l + element->getWidth();
        Ogre::Real b = t + element->getHeight();

		return (cursorPos.x >= l + voidBorder && cursorPos.x <= r - voidBorder &&
			cursorPos.y >= t + voidBorder && cursorPos.y <= b - voidBorder);
	}
};

#endif