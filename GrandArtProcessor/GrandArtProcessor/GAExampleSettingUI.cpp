#include "GAExampleSettingUI.h"
#include "GASingleton.h"

GAExampleSettingUI::GAExampleSettingUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name) : GAUIBase(win,mouse,name)
{
	
}

void GAExampleSettingUI::createUI()
{
	g_inUse=true;
	g_trayMgr->destroyAllWidgets();
	g_trayMgr->showBackdrop("BCITrays/BackGround2");
	g_trayMgr->showLogo(OgreBites::TL_RIGHT);
	g_trayMgr->createSeparator(OgreBites::TL_RIGHT,"sep2");
	g_trayMgr->createButton(OgreBites::TL_RIGHT,"apply","Apply");
	g_trayMgr->createButton(OgreBites::TL_RIGHT,"back","Back");

	g_trayMgr->createLabel(OgreBites::TL_LEFT, "ConfigLabel", "Configuration");
	mRendererMenu=g_trayMgr->createLongSelectMenu(OgreBites::TL_LEFT, "RendererMenu", "Render System", 450, 240, 10);
	g_trayMgr->createSeparator(OgreBites::TL_LEFT, "ConfigSeparator");

	// populate render system names
	Ogre::StringVector rsNames;
	Ogre::RenderSystemList rsList = Ogre::Root::getSingleton().getAvailableRenderers();
	for (unsigned int i = 0; i < rsList.size(); i++)
	{
		rsNames.push_back(rsList[i]->getName());
	}
	mRendererMenu->setItems(rsNames);
	mRendererMenu->selectItem(Ogre::Root::getSingleton().getRenderSystem()->getName());

	

}

void GAExampleSettingUI::itemSelected(OgreBites::SelectMenu* smenu)
{
	if(smenu->getName()=="RendererMenu")
	{
		while (g_trayMgr->getNumWidgets(smenu->getTrayLocation()) > 3)
		{
			g_trayMgr->destroyWidget(smenu->getTrayLocation(), 3);
		}

		Ogre::ConfigOptionMap& options = Ogre::Root::getSingleton().getRenderSystemByName(smenu->getSelectedItem())->getConfigOptions();

		unsigned int i = 0;

		// create all the config option select menus
		for (Ogre::ConfigOptionMap::iterator it = options.begin(); it != options.end(); it++)
		{
			i++;
			OgreBites::SelectMenu* optionMenu = g_trayMgr->createLongSelectMenu
				(OgreBites::TL_LEFT, "ConfigOption" + Ogre::StringConverter::toString(i), it->first, 450, 240, 10);
			optionMenu->setItems(it->second.possibleValues);
					
			// if the current config value is not in the menu, add it
			try
			{
				optionMenu->selectItem(it->second.currentValue);
			}
			catch (Ogre::Exception e)
			{
				optionMenu->addItem(it->second.currentValue);
				optionMenu->selectItem(it->second.currentValue);
			}
		}
	}
}
void GAExampleSettingUI::buttonHit(OgreBites::Button *btn)
{
	if(btn->getName()=="apply")
	{
		GAPMisc misc;
		bool reset = false;
		Ogre::Root* mRoot=Ogre::Root::getSingletonPtr();
		Ogre::ConfigOptionMap& options=mRoot->getRenderSystemByName(mRendererMenu->getSelectedItem())->getConfigOptions();
		Ogre::NameValuePairList newOptions;
		if (mRendererMenu->getSelectedItem()!=mRoot->getRenderSystem()->getName()) reset = true;

		for (unsigned int i = 3; i < g_trayMgr->getNumWidgets(mRendererMenu->getTrayLocation()); i++)
		{
			OgreBites::SelectMenu* menu = (OgreBites::SelectMenu*)g_trayMgr->getWidget(mRendererMenu->getTrayLocation(), i);
			if (menu->getSelectedItem() != options[menu->getCaption()].currentValue) reset = true;
			newOptions[menu->getCaption()] = menu->getSelectedItem();
		}

		// reset with new settings if necessary
		if (reset) GASingleton::getSingleton().reConfigure(mRendererMenu->getSelectedItem(), newOptions);
	}
	if(btn->getName()=="back")
	{
		g_switchUI=true;
		g_nextUIName="EnterUI";
	}
}

bool GAExampleSettingUI::mouseMoved(const OIS::MouseEvent& evt)
{
	OIS::MouseState state = evt.state;
	OIS::MouseEvent orientedEvt((OIS::Object*)evt.device, state);
	if(g_trayMgr->injectMouseMove(orientedEvt)) return true;
	else return false;
}

void GAExampleSettingUI::loadUIFromFile(std::string name)
{
	g_trayMgr->destroyAllWidgets();
}
void GAExampleSettingUI::showAll()
{
	g_trayMgr->showAll();
	hideCursor=false;
	g_inUse=true;
}

void GAExampleSettingUI::hideAll()
{
	g_trayMgr->hideAll();
	hideCursor=true;
	g_inUse=false;
}

bool GAExampleSettingUI::keyPressed(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_8)
	{
		hideAll();
	}
	if(evt.key==OIS::KC_9)
	{
		showAll();
	}
	return true;
}

void GAExampleSettingUI::mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_trayMgr->injectMouseDown(evt,id);
}

void GAExampleSettingUI::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_trayMgr->injectMouseUp(evt,id);
}