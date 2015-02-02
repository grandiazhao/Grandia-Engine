#ifndef SCENEYUNDONG_H
#define SCENEYUNDONG_H

#include "GASceneBase.h"
#include "GACharacterAI\GACharacterAIBase.h"
#include "GAExampleSceneUI.h"
#include "GAFileIO.h"
#include "GARollPoint.h"

class SceneYunDong : public GASceneBase
{
public:
	SceneYunDong(GAPMainManager* mainMgr,std::string sceneName);

	bool createScene();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();
	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
	bool frameEnded(float deltaTime);
private:
	void loadInitFile(std::string filePath);
	void saveCurrentState(std::string filePath);
	void leftPressed();
	void rightPressed();
	void determinAnimation(UINT leftright);
	void checkAnimation();
	UINT translateType(std::string typeName);
	std::string translateString(UINT type);
	GACharacterAIBase* g_manAI;
	Ogre::Bone* boneR,*boneL;
	Ogre::SceneNode* yalingRNode,*yalingLNode,*ballRNode,*ballLNode;
	UINT g_leftRight,g_moveType;
	std::string moveName;
	GAExampleSceneUI* g_sceneUI;

	GAFileIO fileIO,fileIO2,fileIO3,g_output;
	std::vector<float> timeCellVec;
	std::vector<UINT> typeVec; //1-5 front_side_up_grub_flat,11-15 left(front_side_up_grub_flat) 21-25(right(front_side_up_grub_flat)
	UINT totalScore;
	UINT currentScore;
	UINT timeLimit;
	UINT currentCell;
	float move_ratio;
	GAPMisc misc;
	bool g_runningAni;
	UINT g_currentType;
	bool g_toNewScene;
	void toNewScene();
	GARollPoint* newSceneRoll;

	float g_timeOut;
	GARollPoint* g_timeOutRoll;


	void aniLeftFront();
	void aniLeftSide();
	void aniLeftUp();
	void aniLeftGrub();
	void aniLeftFlat();
	void aniRightFront();
	void aniRightSide();
	void aniRightUp();
	void aniRightGrub();
	void aniRightFlat();
	void aniRunning();
	void aniFinished();
	void showHint(UINT type);
	void start();
	void end(bool finished);
	void ballShape(float ratio);
	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);

private:
	float progressBar1Ratio;
	UINT timeRunned;
	bool g_oneTurnFinished;
private:
	UINT g_trialState;

	void playStartSound(UINT leftright);
	UINT g_soundType;
	bool g_soundEnable;
};


#endif