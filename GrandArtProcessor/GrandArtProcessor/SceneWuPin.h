#ifndef SCENEWUPIN_H
#define SCENEWUPIN_H

#include "GASceneBase.h"
#include "GACharacterAI\GACharacterAISimpleObject.h"
#include "GAExampleSceneUI.h"
#include "GAFileIO.h"
#include "GARollPoint.h"

class SceneWuPin : public GASceneBase
{
public:
	SceneWuPin(GAPMainManager* mainMgr,std::string sceneName);

	bool createScene();
	bool reCreateScene();
	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
	bool frameEnded(float deltaTime);
	bool freezeScene();
	bool reloadScene();
private:
	void start();
	void end(bool finished);
	int getLRCommandFromFile(std::string filePath);
	void loadInitFile(std::string filePath);
	void saveCurrentState(std::string filePath);
	void setNextCommand(std::string filePath);
	void goLeft();
	void goRight();
	void moveWuPin();
	void hideAllWoman();
	void showWoman(UINT No);
	void showSingleWoman(UINT No);
	
	void setRotate(UINT type);
	bool setWalkTo(UINT type);
	void slowlyRotation(float deltaTime);
	void slowlyWalk(float deltaTime);
	bool womanRotate,womanWalk;
	float rollDegree,deltaRollDegree;
	void addWoman(UINT count);
	void arrowMove(float deltaTime);
	void selectObject(UINT type);

	void runGetWuPin(UINT No);
	void runAni1();
	void runAni2();
	void runAni3();
	void runAni4();
	void runAni5();
	void runAni6();
	void runAni7();
	void runAni8();

	void turnBody(Ogre::Vector3 goal);
	void turnBody2(float degree);
	void walkToPlace(Ogre::Vector3 goal,float walkRatio=1);
	void getObject(UINT No,float deltaTime);
	void turnLeft();
	void turnRight();
	void oneTurnEnd();
	void showWuPin();
	void hideWuPinOnHand();
	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);

	GACharacterAISimpleObject* g_womanAI[9];

	UINT currentTurn;
	bool g_chooseDir,g_oneTurnLastWalk;
	float transDelta;
	std::string moveName;
	Ogre::Bone* rightHand[9];
	Ogre::SceneNode* womanRootNode;
	UINT currentAni,formerAni,placeWalkTo;
	GAPMisc misc;
	Ogre::SceneNode* arrowLeftNode,*arrowRightNode,*arrowGoNode;
	bool g_bodyTurning,g_walking,g_getting,g_bodyTurning2;
	bool g_bodyTurningFinished,g_startRotating;
	bool g_aniEnd,g_temp1;
	Ogre::Vector3 g_bornPlace[4];
	Ogre::Vector3 g_goalPlace[7];
	Ogre::Vector3 g_objectPlace[4];
	UINT SceneQueue[10];
	UINT g_totalScore;
	UINT g_currentScore,g_currentObject;
	UINT g_limitTime;
	GAExampleSceneUI* g_sceneUI;
	GAFileIO g_input,g_output;
	float g_timeOut;
	GARollPoint* g_timeOutRoll;
	GARollPoint* newSceneRoll;
	bool g_toNewScene;
	
private:
	float s_progressBar1Ratio;
	UINT s_countTimeLeft;
	float g_deltaRollDegree;
private:
	bool isCurrentDirectionRight();
	UINT getLeftRight();

private:
	UINT g_trialState;
	bool g_oneTurnFinishedSign;
	bool g_showArrow;

	void playStartSound(UINT leftright);
	UINT g_soundType;
	bool g_soundEnable;
};



#endif