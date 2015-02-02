#include "GAExampleSceneUI.h"

GAExampleSceneUI::GAExampleSceneUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name):GAUIBase(win,mouse,name)
{
	totalTime=0;
	cellTime=0;
	g_currentTimeSeconds=0;
	startCount=false;
	yesNoDialog=0;
	hintDialog=0;
	firstLogoDialog=0;
	g_totalScore=5;
	g_totalSeconds=200;
	g_currentScore=0;
	caption="score";
	progressBar=0;
	mProgress=mProgress2=0;
	mProgress1Enable=mProgress2Enable=false;
	mProgress1Pause=mProgress2Pause=false;
	g_freezed=false;
	welcome=true;
}

void GAExampleSceneUI::createIcon(std::string name,UINT i)
{
	bp[i] = (Ogre::BorderPanelOverlayElement*)
		Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/Pic", "BorderPanel", name);
	bp[i]->setHorizontalAlignment(Ogre::GHA_RIGHT);
	bp[i]->setVerticalAlignment(Ogre::GVA_TOP);
	bp[i]->setBorderMaterialName("GATrays/EmptyFrame");
	bp[i]->setDimensions(40,40);
	bp[i]->setPosition(-40*((float)i+1),0);
	Ogre::PanelOverlayElement* pic=(Ogre::PanelOverlayElement*)bp[i]->getChild(name+"/Pic");
	pic->setDimensions(40,40);
	pic->setPosition(0,0);
	if(i==0) pic->setMaterialName("GATrays/Exit1");
	else if(i==1) pic->setMaterialName("GATrays/Help1");
	g_trayMgr->getTraysLayer()->add2D(bp[i]);
}

void GAExampleSceneUI::createLogo()
{
	logo = (Ogre::BorderPanelOverlayElement*)
		Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/Pic", "BorderPanel", "jiaodalogo");
	logo->setHorizontalAlignment(Ogre::GHA_RIGHT);
	logo->setVerticalAlignment(Ogre::GVA_TOP);
	logo->setBorderMaterialName("GATrays/EmptyFrame");
	logo->setDimensions(80,80);
	logo->setPosition(-80,0);
	Ogre::PanelOverlayElement* pic=(Ogre::PanelOverlayElement*)logo->getChild("jiaodalogo/Pic");
	pic->setDimensions(80,80);
	pic->setPosition(0,0);
	pic->setMaterialName("GATrays/jiaodalogo1");
	g_trayMgr->getTraysLayer()->add2D(logo);
}

void GAExampleSceneUI::createTextBox(std::string name,UINT i,UINT pos)
{
	box[i]=(Ogre::BorderPanelOverlayElement*)
		Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/TextBox", "BorderPanel", name);
	//if(pos==2) box[i]->setHorizontalAlignment(Ogre::GHA_CENTER);

	box[i]->setBorderMaterialName("GATrays/EmptyFrame");
	box[i]->setDimensions(200,50);
	box[i]->setPosition(0,0);
	if(pos==2) box[i]->setPosition(100,0);
	Ogre::TextAreaOverlayElement* text=(Ogre::TextAreaOverlayElement*)box[i]->getChild(name+"/textbox");
	text->setPosition(0,15);
	text->setHorizontalAlignment(Ogre::GHA_LEFT);
	if(pos==1)
	{
		text->setPosition(50,15);
		text->setCaption("score:0/5");
	}
	if(pos==2)
	{
		text->setPosition(50,15);
		text->setCaption("seconds:24");
	}
	g_trayMgr->getTraysLayer()->add2D(box[i]);
}

void GAExampleSceneUI::firstShow()
{
	showYesNoDialog();
	g_trayMgr->showAll();
	g_trayMgr->hideBackdrop();
	g_inUse=true;
}
void GAExampleSceneUI::showYesNoDialog()
{
	g_trayMgr->hideTrays();
	g_trayMgr->showBackdrop("SdkTrays/Shade");
	if(yesNoDialog!=0) return;
	yesNoDialog=(Ogre::BorderPanelOverlayElement*)
		Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/YesNoDialog", "BorderPanel", "yesnodialog");
	yesNoDialog->setHorizontalAlignment(Ogre::GHA_CENTER);
	yesNoDialog->setVerticalAlignment(Ogre::GVA_CENTER);
	yesNoDialog->setDimensions(500,100);
	yesNoDialog->setPosition(-250,-100);
	yesPic=(Ogre::PanelOverlayElement*)yesNoDialog->getChild("yesnodialog/PicYes");
	noPic=(Ogre::PanelOverlayElement*)yesNoDialog->getChild("yesnodialog/PicNo");
	yesPic->setDimensions(80,80);
	yesPic->setPosition(60,60);
	yesPic->setMaterialName("GATrays/Exit1");
	noPic->setDimensions(80,80);
	noPic->setPosition(360,60);
	noPic->setMaterialName("GATrays/Exit2");
	yesNoMessage=(Ogre::TextAreaOverlayElement*)yesNoDialog->getChild("yesnodialog/YesNoMessage");
	yesNoMessage->setPosition(0,-10);
	yesNoMessage->setCaption("this is my first yesno message");
	g_trayMgr->getBackdropLayer()->add2D(yesNoDialog);
}

void GAExampleSceneUI::showFirstLogo()
{
	if(firstLogoDialog!=0)
	{
		firstLogoDialog->show();
		return;
	}
	firstLogoDialog=(Ogre::BorderPanelOverlayElement*)Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/PicWithDesc", "BorderPanel","firstLogo");
	firstLogoDialog->setHorizontalAlignment(Ogre::GHA_CENTER);
	firstLogoDialog->setVerticalAlignment(Ogre::GVA_CENTER);
	firstLogoDialog->setDimensions(600,90);
	firstLogoPic=(Ogre::PanelOverlayElement*)firstLogoDialog->getChild("firstLogo/Picture");
	firstLogoPic->setDimensions(128,64);
	firstLogoPic->setPosition(13,13);
	firstLogoPic->setMaterialName("SdkTrays/Scene1Logo");
	firstLogoMessage=(Ogre::TextAreaOverlayElement*)firstLogoDialog->getChild("firstLogo/Describe");
	firstLogoMessage->setPosition(-140,40);
	firstLogoMessage->setHorizontalAlignment(Ogre::GHA_RIGHT);
	firstLogoMessage->setCaption("welcome");
	g_trayMgr->getTraysLayer()->add2D(firstLogoDialog);
}


void GAExampleSceneUI::showHint()
{
	if(hintDialog!=0)
	{
		hintMessage->setCaption(hintString);
		hintDialog->show();
		return;
	}
	hintDialog=(Ogre::BorderPanelOverlayElement*)
		Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate("GATrays/HintDialog", "BorderPanel", "hintdialog");
	hintDialog->setHorizontalAlignment(Ogre::GHA_CENTER);
	hintDialog->setVerticalAlignment(Ogre::GVA_CENTER);
	hintDialog->setDimensions(500,100);
	hintDialog->setPosition(-250,-100);
	hintMessage=(Ogre::TextAreaOverlayElement*)hintDialog->getChild("hintdialog/HintMessage");
	hintMessage->setPosition(0,-15);
	hintMessage->setCaption(hintString);
	g_trayMgr->getTraysLayer()->add2D(hintDialog);
}

void GAExampleSceneUI::hideHint()
{
	if(hintDialog!=0) hintDialog->hide();
}

void GAExampleSceneUI::checkYesNoDialogMouseReleased(const OIS::MouseEvent& evt)
{
	if(yesNoDialog==0||!g_trayMgr->isBackdropVisible()) return;
	if(isCursorOver(yesPic,Ogre::Vector2(evt.state.X.abs,evt.state.Y.abs)))
	{
		g_trayMgr->hideBackdrop();
		g_trayMgr->showTrays();
		YesNoDialogYesPressed();
	}
	else if(isCursorOver(noPic,Ogre::Vector2(evt.state.X.abs,evt.state.Y.abs)))
	{
		g_trayMgr->hideBackdrop();
		g_trayMgr->showTrays();
	}
}

void GAExampleSceneUI::YesNoDialogYesPressed()
{
	countBegin();
}
void GAExampleSceneUI::createUI()
{
	g_inUse=true;
	g_trayMgr->destroyAllWidgets();
	g_trayMgr->showFrameStats(OgreBites::TL_BOTTOMLEFT);
	g_trayMgr->toggleAdvancedFrameStats();
	g_trayMgr->hideCursor();
	hideCursor=false;
	/*std::string name="score";
	std::string caption="score:0/6";
	g_trayMgr->createLabel(OgreBites::TL_TOPLEFT,name,caption,120)->getOverlayElement()->setMaterialName("GATrays/EmptyFrame");
	((OgreBites::Label*)g_trayMgr->getWidget(name))->setCaption("score:0/7");
	g_trayMgr->createLabel(OgreBites::TL_TOP,"time","seconds:0/200",170);*/
	//createIcon("icon1",0);
	//createIcon("icon2",1);
	createLogo();
	createTextBox("box1",0,1);
	createTextBox("box2",1,2);
	hintString=L"ÄãºÃ";
	showHint();
	hideHint();
	createProgressBar();
}

void GAExampleSceneUI::createProgressBar()
{
	progressBar = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate
		("GATrays/ProgressBar", "BorderPanel", "progressBar");
	progressBar->setWidth(450);
	Ogre::OverlayContainer* c = (Ogre::OverlayContainer*)progressBar;
	c->setVerticalAlignment(Ogre::GVA_TOP);
	c->setHorizontalAlignment(Ogre::GHA_CENTER);
	c->setPosition(-200,7);
	mMeter = c->getChild("progressBar/GAProgressMeter");
	mMeter->setWidth(450 - 10);
	mFill = ((Ogre::OverlayContainer*)mMeter)->getChild(mMeter->getName() + "/GAProgressFill");
	g_trayMgr->getTraysLayer()->add2D(c);

	progressBar2 = Ogre::OverlayManager::getSingleton().createOverlayElementFromTemplate
		("GATrays/ProgressBar", "BorderPanel", "progressBar2");
	progressBar2->setWidth(450);
	Ogre::OverlayContainer* c2 = (Ogre::OverlayContainer*)progressBar2;
	c2->setVerticalAlignment(Ogre::GVA_BOTTOM);
	c2->setHorizontalAlignment(Ogre::GHA_CENTER);
	c2->setPosition(-200,-45);
	mMeter2 = c2->getChild("progressBar2/GAProgressMeter");
	mMeter2->setWidth(450 - 10);
	mFill2 = ((Ogre::OverlayContainer*)mMeter2)->getChild(mMeter2->getName() + "/GAProgressFill");
	g_trayMgr->getTraysLayer()->add2D(c2);

}

void GAExampleSceneUI::showProgressBar(bool isShow)
{
	if(progressBar==0) createProgressBar();
	if(isShow) progressBar->show();
	else progressBar->hide();
}

void GAExampleSceneUI::progressBar1Visible(bool visible)
{
	if(visible) progressBar->show();
	else progressBar->hide();
}

void GAExampleSceneUI::progressBar2Visible(bool visible)
{
	if(visible) progressBar2->show();
	else progressBar2->hide();
}

void GAExampleSceneUI::setProgressBarRatio(float ratio)
{
	mProgress = Ogre::Math::Clamp<Ogre::Real>(ratio, 0, 1);
	p1DeltaTime=mProgress1Time*(1-mProgress);
	mFill->setWidth(std::max<int>(17,(int)(mProgress * (mMeter->getWidth() - 2 * mFill->getLeft()))));
}

void GAExampleSceneUI::setProgressBar2Ratio(float ratio)
{
	mProgress2 = Ogre::Math::Clamp<Ogre::Real>(ratio, 0, 1);
	p2DeltaTime=mProgress2Time*(1-mProgress2);
	mFill2->setWidth(std::max<int>(17,(int)(mProgress2 * (mMeter->getWidth() - 2 * mFill2->getLeft()))));
}
void GAExampleSceneUI::ProgressBar1Start(float totalTime)
{
	mProgress1Enable=true;
	mProgress1Time=totalTime;
	p1DeltaTime=0;
	setProgressBarRatio(1);
	mProgress1Pause=false;
}
void GAExampleSceneUI::ProgressBar2Start(float totalTime)
{
	mProgress2Enable=true;
	mProgress2Time=totalTime;
	p2DeltaTime=0;
	setProgressBar2Ratio(1);
	mProgress2Pause=false;
}
void GAExampleSceneUI::ProgressBar1Pause()
{
	mProgress1Pause=true;
}
void GAExampleSceneUI::ProgressBar1UnPause()
{
	mProgress1Pause=false;
}
void GAExampleSceneUI::ProgressBar2Pause()
{
	mProgress2Pause=true;
}
void GAExampleSceneUI::ProgressBar2UnPause()
{
	mProgress2Pause=false;
}
void GAExampleSceneUI::updateProgressBar(float deltaTime)
{
	if(mProgress1Enable)
	{
		p1DeltaTime+=deltaTime;
		if(p1DeltaTime>=mProgress1Time)
		{
			mProgress1Enable=false;
			setProgressBarRatio(0);
		}
		else
		{
			if(mProgress1Pause)
			{
				p1DeltaTime-=deltaTime;
			}
			else setProgressBarRatio((mProgress1Time-p1DeltaTime)/mProgress1Time);
		}
	}
	if(mProgress2Enable)
	{
		p2DeltaTime+=deltaTime;
		if(p2DeltaTime>=mProgress2Time)
		{
			mProgress2Enable=false;
			setProgressBar2Ratio(0);
		}
		else
		{
			if(mProgress2Pause)
			{
				p2DeltaTime-=deltaTime;
			}
			else setProgressBar2Ratio((mProgress2Time-p2DeltaTime)/mProgress2Time);
		}
	}
}
float GAExampleSceneUI::getProgressBarRatio()
{
	return mProgress;
}
float GAExampleSceneUI::getProgressBar2Ratio()
{
	return mProgress2;
}
void GAExampleSceneUI::frameStarted(float deltaTime)
{
	GAUIBase::frameStarted(deltaTime);
	if(g_freezed) return;
	g_trayMgr->showCursor();
	((Ogre::TextAreaOverlayElement*)box[0]->getChild("box1/textbox"))->setCaption(caption+":"+misc.ConvertToString(g_currentScore)+"/"+misc.ConvertToString(g_totalScore));
	if(startCount)
	{
		totalTime+=deltaTime;
		cellTime+=deltaTime;
		while(cellTime>1&&g_currentTimeSeconds<=200)
		{
			g_currentTimeSeconds++;
			cellTime-=1;
		}
		UINT sec=g_totalSeconds-g_currentTimeSeconds;
		if(g_currentTimeSeconds>=g_totalSeconds) sec=0;
		((Ogre::TextAreaOverlayElement*)box[1]->getChild("box2/textbox"))->setCaption(misc.ConvertToString(sec)+"/"+misc.ConvertToString(g_totalSeconds));
	}
	hintMessage->setCaption(hintString);
	updateProgressBar(deltaTime);
}

void GAExampleSceneUI::showTips(std::string str)
{
	((Ogre::TextAreaOverlayElement*)box[1]->getChild("box2/textbox"))->setCaption(str);
}
void GAExampleSceneUI::countBegin()
{
	startCount=true;
	//totalTime=0;cellTime=0;g_currentTimeSeconds=0;
}
void GAExampleSceneUI::countPause()
{
	startCount=false;
}
bool GAExampleSceneUI::isCountOver()
{
	if(g_currentTimeSeconds>=g_totalSeconds) return true;
	else return false;
}
void GAExampleSceneUI::countRestart(UINT newTotalTime)
{
	startCount=true;
	if(newTotalTime>0) g_totalSeconds=newTotalTime;
	totalTime=0;cellTime=0;g_currentTimeSeconds=0;
}

bool GAExampleSceneUI::mouseMoved(const OIS::MouseEvent& evt)
{
	OIS::MouseState state = evt.state;
	OIS::MouseEvent orientedEvt((OIS::Object*)evt.device, state);
	Ogre::Vector2 cursorPos(evt.state.X.abs, evt.state.Y.abs);
	if(isCursorOver(logo,cursorPos))
	{
		((Ogre::PanelOverlayElement*)logo->getChild("jiaodalogo/Pic"))->setMaterialName("GATrays/jiaodalogo2");
	}
	else ((Ogre::PanelOverlayElement*)logo->getChild("jiaodalogo/Pic"))->setMaterialName("GATrays/jiaodalogo1");
	/*if(isCursorOver(bp[1],cursorPos))
	{
		((Ogre::PanelOverlayElement*)bp[1]->getChild("icon2/Pic"))->setMaterialName("GATrays/Help2");
	}
	else ((Ogre::PanelOverlayElement*)bp[1]->getChild("icon2/Pic"))->setMaterialName("GATrays/Help1");*/
	if(g_trayMgr->injectMouseMove(evt)) return true;
	else return false;	
}

void GAExampleSceneUI::mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id)
{
	/*g_trayMgr->injectMouseUp(evt,id);
	checkYesNoDialogMouseReleased(evt);
	Ogre::Vector2 cursorPos(evt.state.X.abs, evt.state.Y.abs);
	for(UINT i=0;i<2;i++)
	{
		if(isCursorOver(logo,cursorPos))
		{
			if(i==0)
			{
				g_switchUI=true;
				g_nextUIName="EnterUI";
			}
			else if(i==1)
			{
				g_trayMgr->showOkDialog("OK","this is help message");
			}
		}
	}*/
}

bool GAExampleSceneUI::keyPressed(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_B) showYesNoDialog();
	return true;
}