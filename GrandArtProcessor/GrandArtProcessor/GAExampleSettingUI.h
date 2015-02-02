#ifndef GAEXAMPLESETTINGUI_H
#define GAEXAMPLESETTINGUI_H

#include "GAUIBase.h"

class GAExampleSettingUI : public GAUIBase
{
public:
	GAExampleSettingUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name="ExampleSettings");

	virtual void createUI();

	void loadUIFromFile(std::string UIName);

	void showAll();
	void hideAll();

	void buttonHit(OgreBites::Button *btn);
	void itemSelected(OgreBites::SelectMenu* smenu);
	bool mouseMoved(const OIS::MouseEvent& evt);

	bool keyPressed(const OIS::KeyEvent& evt);

	void mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
private:
	OgreBites::SelectMenu* mRendererMenu;
};

#endif