#ifndef GAEXAMPLEENTERUI_H
#define GAEXAMPLEENTERUI_H

#include "GAUIBase.h"

class GAExampleEnterUI : public GAUIBase
{
public:
	GAExampleEnterUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name);

	virtual void createUI();
	void loadUIFromFile(std::string UIName);

	void showAll();
	void hideAll();
	void buttonHit(OgreBites::Button *btn);

	bool mouseMoved(const OIS::MouseEvent& evt);

	bool keyPressed(const OIS::KeyEvent& evt);

	void mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
};

#endif