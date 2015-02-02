#ifndef SCENEPINGHENG_H
#define SCENEPINGHENG_H

#include "GASceneBase.h"
#include "GAFileIO.h"
#include "GAExampleSceneUI.h"
#include "GARollPoint.h"

class ScenePingHeng : public GASceneBase
{
public:
	ScenePingHeng(GAPMainManager* mainMgr,std::string sceneName);

	bool createScene();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();
	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);

	void toNewScene();


private:
	int getLRCommandFromFile(std::string filePath);
	void loadInitFile(std::string filePath);
	void saveCurrentState(std::string filePath);
	void setNextCommand(std::string filePath);
	void ballDrops(float deltaTime);
	GAFileIO fileIO,fileIO2,fileIO3,g_output;

	void calcPingHeng(float deltaTime);
	bool plantRotate(float deltaTime,float forceDegree=0);
	void windBlow(bool toLeft,float speedAdd=1,float lastTime=3);
	void blowWindTrail(bool toLeft);
	void blowWindTrailFrame(float deltaTime);
	void arrowMove(float deltaTime);
	void start();
	void end(bool finished);
	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);
	Ogre::SceneNode* ballNode;
	Ogre::SceneNode* pinghengmuNode,*pinghengNode;
	float ballAcc,ballVel,ballDistance;
	float muDeltaDegree,muTotalDegree;
	float muMass,muHalflength;
	float ballro;
	float forceL,forceR,roacc,rovel;
	float ballWeight;
	float windAcc;
	bool addForceL,addForceR;
	Ogre::SceneNode *plant1,*plant2,*arrowL,*arrowR;
	float plantForceDegree,plantBlowTotalTime,plantBlowDeltaTime;
	bool g_plantYaw,g_ballDrop;
	UINT g_currentDropTime,g_currentTime;
	GAExampleSceneUI* g_sceneUI;
	UINT g_toleranceTime;
	UINT g_limitTime;
	float totalTime;
	GAPMisc misc;
	GARollPoint* newSceneRoll;
	bool g_toNewScene;
	Ogre::RibbonTrail* windTrail;
	Ogre::SceneNode* windTrailNode1,*windTrailNode2;
	float windTime;
	bool windDirection,isWindBlowed;
	Ogre::Vector3 windStartPoint,windEndPoint;
	bool isWindBlowing;

	float g_timeOut;
	GARollPoint* g_timeOutRoll;
private:
	float s_progressRatio;
	float g_vSpeedDown,g_totalBallDis;
	Ogre::Entity *arrowEntL;
	Ogre::Entity *arrowEntR;
	float arrowTransL,arrowTransR,arrowTransDeltaL,arrowTransDeltaR;
	bool arrowDirL,arrowDirR;

	UINT g_trialState;
	bool g_showArrows;

	void playStartSound(UINT leftright);
	UINT g_soundType;
	bool g_soundEnable;
};


#endif