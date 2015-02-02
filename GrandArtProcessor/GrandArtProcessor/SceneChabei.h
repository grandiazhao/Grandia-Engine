#ifndef SCENECHABEI_H
#define SCENECHABEI_H

#include "Common.h"
#include "GAOgreUIManager.h"
#include "GASceneBase.h"
#include "GAFileIO.h"
#include "GACharacterAI\GACharacterAIBase.h"
#include "GAExampleSceneUI.h"
#include "GARollPoint.h"

class SceneChabei : public GASceneBase
{
public:
	SceneChabei(GAPMainManager* mainMgr,std::string sceneName);
	bool createScene();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();

	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
private:
	int getLRCommandFromFile(std::string filePath);
	void loadInitFile(std::string filePath);
	void saveCurrentState(std::string filePath);

	void setNextCommand(std::string filePath);
	void checkAniState();
	GAFileIO fileIO,fileIO2,fileIO3,g_output;
	
	void manAniLeft();
	void manAniRight();
	void womanAniLeft();
	void womanAniRight();

	void oneTurnFinished();
	void womanAniStart();
	void showHint(UINT type);
	void start();
	void end();
	void changeCup(UINT kind);
	void toNewScene();
	void playStartSound(UINT leftright);
	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);

	Ogre::Matrix4 moveCup();
	Ogre::Matrix4 moveCupWoman();
	std::string moveName;
	Ogre::Bone* boneR,*boneL,*boneWomanR,*boneWomanL;
	UINT g_leftRight,g_womanLeftRight;
	GACharacterAIBase* g_womanAI,*g_manAI;
	std::vector<float> timeVecLeft,timeVecRight,womanRatioVec;
	float womanRatio,manRatio;
	GAPMisc misc;
	UINT currentState;
	bool g_manInAni;
	std::vector<UINT> womanForceLeftRightVec,manForceLeftRightVec;
	UINT g_currentScore;
	UINT g_totalScore;
	UINT g_currentTime,g_totalTime;
	bool g_isWomanFinished,g_womanStart;

	GARollPoint* newSceneRoll;
	bool g_toNewScene;
	bool welcome;
	float totalTime;
	GAExampleSceneUI* g_sceneUI;

private:
	UINT s_timeLeft;

private:
	GARollPoint* g_deltaWaitRoll;
	bool g_deltaWaitRollBegin;
	UINT g_trialState; //0-end_failed,1-started,2-end_success 
	UINT g_soundType;
	bool g_soundEnable;
};


#endif