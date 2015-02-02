#ifndef GAEXAMPLESCENEUI_H
#define GAEXAMPLESCENEUI_H

#include "GAUIBase.h"

class GAExampleSceneUI : public GAUIBase
{
public:
	UINT g_currentScore,g_totalSeconds,g_totalScore;
	UINT g_currentTimeSeconds;
	std::string caption;
	Ogre::DisplayString hintString;
	bool g_freezed;
public:
	GAExampleSceneUI(Ogre::RenderWindow* win,OIS::Mouse *mouse,std::string name="SimpleSceneUI");

	void createUI();
	void firstShow();
	void frameStarted(float deltaTime);
	bool keyPressed(const OIS::KeyEvent& evt);
	bool mouseMoved(const OIS::MouseEvent& evt);
	void mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);
	void createIcon(std::string name,UINT i);
	void createLogo();
	void createTextBox(std::string name,UINT i,UINT pos);
	void countBegin();
	void countPause();
	bool isCountOver();
	void countRestart(UINT newTotalTime=0);
	void showYesNoDialog();
	void showFirstLogo();
	void YesNoDialogYesPressed();
	void checkYesNoDialogMouseReleased(const OIS::MouseEvent& evt);
	void showHint();
	void hideHint();
	void showTips(std::string str);

	void progressBar1Visible(bool visible);
	void progressBar2Visible(bool visible);
	void createProgressBar();
	void setProgressBarRatio(float ratio);
	void setProgressBar2Ratio(float ratio);
	float getProgressBarRatio();
	float getProgressBar2Ratio();
	void ProgressBar1Start(float totalTime);
	void ProgressBar2Start(float totalTime);
	void ProgressBar1Pause();
	void ProgressBar2Pause();
	void ProgressBar1UnPause();
	void ProgressBar2UnPause();
	void updateProgressBar(float deltaTime);
	void showProgressBar(bool isShow);

private:
	float totalTime,cellTime;
	GAPMisc misc;
	bool startCount;
	Ogre::BorderPanelOverlayElement* bp[2];
	Ogre::BorderPanelOverlayElement* logo;
	Ogre::BorderPanelOverlayElement* box[2];
	float mProgress,mProgress2;
	float mProgress1Time,mProgress2Time,p1DeltaTime,p2DeltaTime;
	bool mProgress1Enable,mProgress2Enable,mProgress1Pause,mProgress2Pause;
	Ogre::OverlayElement* progressBar,*progressBar2,*mMeter,*mMeter2,*mFill,*mFill2;

	Ogre::BorderPanelOverlayElement* yesNoDialog;
	Ogre::PanelOverlayElement* yesPic,*noPic;
	Ogre::TextAreaOverlayElement* yesNoMessage;

	Ogre::BorderPanelOverlayElement* firstLogoDialog;
	Ogre::PanelOverlayElement* firstLogoPic;
	Ogre::TextAreaOverlayElement* firstLogoMessage;

	Ogre::BorderPanelOverlayElement* hintDialog;
	Ogre::TextAreaOverlayElement* hintMessage,*hintMessage2;
public:
	bool welcome;
};

#endif