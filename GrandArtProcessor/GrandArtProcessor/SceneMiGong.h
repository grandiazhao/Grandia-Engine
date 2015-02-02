#ifndef SCENEMIGONG_H
#define SCENEMIGONG_H

#include "Common.h"
#include "GAOgreUIManager.h"
#include "GASceneBase.h"
#include "GACharacterAI\GACharacterAISimpleObject.h"
#include "GADataManager.h"
#include "GAFileIO.h"
#include "GAExampleSceneUI.h"
#include "GARollPoint.h"

class SceneMiGong : public GASceneBase
{
public:
	SceneMiGong(GAPMainManager* mainMgr,std::string sceneName);
	~SceneMiGong();
	bool createScene();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();

	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
	bool mouseMoved(const OIS::MouseEvent &evt);

private:
	int getLRCommandFromFile(std::string filePath);
	void loadInitFile(std::string filePath);
	void saveCurrentState(std::string filePath);

	void setData1();
	void setData2();
	void setData3();
	void setData4();
	void setScene1();
	void setScene2();
	void setScene3();
	void setScene4();
	void goRoadLeft();
	void goRoadRight();
	bool rotateLeft();
	bool rotateRight();
	void doMoving(float deltaTime);
	void setRotating();
	void setRotating2();
	void setArrows();

	void oneSceneEnd(bool finished);
	void toNewScene();
	void start();
	void end();
	void rotateCamera(float degrees,float speed);

	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);



	float totalTime;
	float cellTime;
	float currDegrees;
	float g_timeOut;
	GARollPoint* g_timeOutRoll;
	bool material1;
	GACharacterAISimpleObject* g_ballAI,*g_endManAI;
	GADataLeafNode* g_cornerLeaf;
	GADataLeafNode* g_leftRightLeaf;
	std::vector<void*>* g_cornerStruct;
	std::vector<void*>* g_leftRightStruct;
	GADataContainerNode* g_dataNode;
	UINT nextPosL,nextPosR,currPos,prePos;
	int arrowLDir,arrowRDir;
	bool g_moving,g_showArrow,g_rotateCamera,g_processing;
	Ogre::SceneNode* arrow1Pivot,*arrow2Pivot;
	UINT finalPos;
	GAFileIO fileIO,fileIO2,fileIO3,g_output;
	int fileControllSign;
	Ogre::SceneNode* g_endNode;
	UINT g_currentScene;
	GAPMisc misc;
	GAExampleSceneUI* g_sceneUI;
	float g_height,g_rotateDgrees;
	UINT g_currentScore,g_currentSeconds,g_totalSeconds;
	UINT g_timeLimit[4];
	Ogre::SceneNode* waitressNode;
	float g_degreeSpeed,g_currentDegree;
	bool g_leftRotating,g_45Degrees,g_setRotating;

private:
	void setWomanRotate(float degrees);
	GAFloat3Struct getRunningTime(bool leftRotating);
	float getRotateDegreeOfNextPoint(UINT pre,UINT pos,Ogre::Vector3 roadDir);

private:
	float s_progressBar1Ratio;
	UINT s_countTime;

private:
	UINT g_trialState;
	bool g_showArrows;
	UINT g_showArrowsState;

	void playStartSound(UINT leftright);
	UINT g_soundType;
	bool g_soundEnable;
};


#endif