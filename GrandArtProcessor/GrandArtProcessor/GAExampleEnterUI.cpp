#include "GAExampleEnterUI.h"

Ogre::BorderPanelOverlayElement* bp[5];
GAExampleEnterUI::GAExampleEnterUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name):GAUIBase(win,mouse,name)
{
	g_reCreateScene=false;
}

void createSection(OgreBites::SdkTrayManager* g_trayMgr,int posx,int posy,int num)
{
	GAPMisc misc;
	std::string name="bp"+misc.ConvertToString(num);
	bp[num] = (Ogre::BorderPanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/PicWithDesc", "BorderPanel",name);
	bp[num]->setHorizontalAlignment(Ogre::GHA_LEFT);
	bp[num]->setVerticalAlignment(Ogre::GVA_CENTER);

	bp[num]->setBorderMaterialName("BCITrays/Frame");
	bp[num]->setDimensions(400,90);
	bp[num]->setPosition(posx,posy);
	Ogre::PanelOverlayElement* pic=(Ogre::PanelOverlayElement*)bp[num]->getChild(name+"/Picture");
	pic->setDimensions(128,64);
	pic->setPosition(13,13);
	if(num==0) pic->setMaterialName("SdkTrays/Scene1Logo");
	else if(num==1) pic->setMaterialName("SdkTrays/Scene2Logo");
	else if(num==2) pic->setMaterialName("SdkTrays/Scene3Logo");
	else if(num==3) pic->setMaterialName("SdkTrays/Scene4Logo");
	else if(num==4) pic->setMaterialName("SdkTrays/Scene5Logo");
	Ogre::TextAreaOverlayElement* text=(Ogre::TextAreaOverlayElement*)bp[num]->getChild(name+"/Describe");
	text->setPosition(-140,40);
	text->setHorizontalAlignment(Ogre::GHA_RIGHT);
	if(num==0) text->setCaption("Get The Cup");
	else if(num==1) text->setCaption("Rehablitation Excicise");
	else if(num==2) text->setCaption("Balance The Ball");
	else if(num==3) text->setCaption("Daily Traning");
	else if(num==4) text->setCaption("Lost In The Maze");
	g_trayMgr->getTraysLayer()->add2D(bp[num]);
}
void GAExampleEnterUI::createUI()
{
	g_inUse=true;
	g_trayMgr->destroyAllWidgets();
	g_trayMgr->showBackdrop("BCITrays/BackGround2");
	g_trayMgr->showLogo(OgreBites::TL_RIGHT);
	g_trayMgr->createSeparator(OgreBites::TL_RIGHT, "LogoSep");
	g_trayMgr->createButton(OgreBites::TL_RIGHT, "createScene", "Create Scene");
	g_trayMgr->createButton(OgreBites::TL_RIGHT, "settings", "Settings");
	g_trayMgr->createButton(OgreBites::TL_RIGHT, "info", "Information");
	g_trayMgr->createButton(OgreBites::TL_RIGHT, "quit", "Quit");
	g_trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	g_trayMgr->toggleAdvancedFrameStats();
	
	createSection(g_trayMgr,40,40-305,0);
	createSection(g_trayMgr,35,140-305,1);
	createSection(g_trayMgr,30,240-305,2);
	createSection(g_trayMgr,35,340-305,3);
	createSection(g_trayMgr,40,440-305,4);
}

void GAExampleEnterUI::buttonHit(OgreBites::Button *btn)
{
	if(btn->getName()=="createScene")
	{
		hideAll();
		g_reCreateScene=true;
		g_enterScene=true;
		g_sceneName="";
	}
	else if(btn->getName()=="settings")
	{
		g_switchUI=true;
		g_nextUIName="SettingUI";
	}
	else if(btn->getName()=="info")
	{
		std::string message="This application is created by GrandiaZhao. Used by laboratory of Shanghai Jiao Tong University. I will change ";
		message+="all the appearance later on with my assistant for beauty.";
		g_trayMgr->showOkDialog("Information",message);
	}
	else if(btn->getName()=="quit") Ogre::Root::getSingleton().queueEndRendering();
}

bool GAExampleEnterUI::mouseMoved(const OIS::MouseEvent& evt)
{
	OIS::MouseState state = evt.state;
	OIS::MouseEvent orientedEvt((OIS::Object*)evt.device, state);
	if(!g_trayMgr->isDialogVisible())
	{
		Ogre::Vector2 cursorPos(evt.state.X.abs, evt.state.Y.abs);
		for(UINT i=0;i<5;i++)
		{
			if(isCursorOver(bp[i],cursorPos))
			{
				bp[i]->setBorderMaterialName("SdkTrays/Frame/Over");
			}
			else bp[i]->setBorderMaterialName("BCITrays/Frame");
		}
	}
	if(g_trayMgr->injectMouseMove(orientedEvt)) return true;
	else return false;	
}

void GAExampleEnterUI::loadUIFromFile(std::string name)
{
	g_trayMgr->destroyAllWidgets();
}
void GAExampleEnterUI::showAll()
{
	g_trayMgr->showAll();
	hideCursor=false;
	g_inUse=true;
}

void GAExampleEnterUI::hideAll()
{
	g_trayMgr->hideAll();
	hideCursor=true;
	g_inUse=false;
}

bool GAExampleEnterUI::keyPressed(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_C)
	{
		buttonHit((OgreBites::Button*)g_trayMgr->getWidget("createScene"));
	}
	else if(evt.key==OIS::KC_S)
	{
		buttonHit((OgreBites::Button*)g_trayMgr->getWidget("settings"));
	}
	else if(evt.key==OIS::KC_I)
	{
		buttonHit((OgreBites::Button*)g_trayMgr->getWidget("info"));
	}
	else if(evt.key==OIS::KC_Q)
	{
		buttonHit((OgreBites::Button*)g_trayMgr->getWidget("quit"));
	}
	else if(evt.key==OIS::KC_1)
	{
		hideAll();
		g_enterScene=true;
		g_reCreateScene=false;
		g_sceneName="chabei";
	}
	else if(evt.key==OIS::KC_2)
	{
		hideAll();
		g_enterScene=true;
		g_reCreateScene=false;
		g_sceneName="yundong";
	}
	else if(evt.key==OIS::KC_3)
	{
		hideAll();
		g_enterScene=true;
		g_reCreateScene=false;
		g_sceneName="pingheng";
	}
	else if(evt.key==OIS::KC_4)
	{
		hideAll();
		g_enterScene=true;
		g_reCreateScene=false;
		g_sceneName="wupin";
	}
	else if(evt.key==OIS::KC_5)
	{
		hideAll();
		g_enterScene=true;
		g_reCreateScene=false;
		g_sceneName="migong";
	}
	else if(evt.key==OIS::KC_SPACE)
	{
		if(g_trayMgr->isDialogVisible()) g_trayMgr->closeDialog();
	}
	return true;
}

void GAExampleEnterUI::mousePressed(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	g_trayMgr->injectMouseDown(evt,id);
}

void GAExampleEnterUI::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	
	if(!g_trayMgr->isDialogVisible())
	{
		Ogre::Vector2 cursorPos(evt.state.X.abs, evt.state.Y.abs);
		for(UINT i=0;i<5;i++)
		{
			if(isCursorOver(bp[i],cursorPos))
			{
				hideAll();
				g_enterScene=true;
				g_reCreateScene=false;
				if(i==0) g_sceneName="chabei";
				else if(i==1) g_sceneName="yundong";
				else if(i==2) g_sceneName="pingheng";
				else if(i==3) g_sceneName="wupin";
				else if(i==4) g_sceneName="migong";
				else
				{
					showAll();
					g_enterScene=false;
				}
			}
		}
	}
	g_trayMgr->injectMouseUp(evt,id);
}