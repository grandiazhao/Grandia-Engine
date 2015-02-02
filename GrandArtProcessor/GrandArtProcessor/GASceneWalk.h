#ifndef GASCENEWALK_H
#define GASCENEWALK_H

#include "Common.h"
#include "GASceneBase.h"
#include "GACharacterAI\GACharacterAISimpleObject.h"
#include "GAExampleSceneUI.h"

class GASceneWalk : public GASceneBase
{
public:
	GASceneWalk(GAPMainManager* mainMgr,std::string sceneName);
	bool createScene();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
	bool mouseMoved(const OIS::MouseEvent& evt);

	void end();
	void setWalkTo(UINT startGame,UINT endGame);
private:
	bool processServerData(std::string recvString,GAServerSimple* server,UINT socketNum=0);
	void setPersonOnPoint(UINT point);
	void setPersonDirection(UINT dir);
	void updateWalkTo(float deltaTime);
	Ogre::SceneNode* g_personNode;
	Ogre::SceneNode* g_manNode,*g_womanNode;
	GACharacterAISimpleObject* g_walkMan,*g_walkWoman,*g_currentPerson;

	UINT g_startGame,g_endGame;
	bool g_walking,g_needRotate,g_needRotate2;
	Ogre::Vector3 g_rotateDirection;
	float g_rotateDegrees,g_rotatingTotalDegrees;
	bool keyUpDown,keyDownDown,keyLeftDown,keyRightDown;
	UINT g_nextPos,g_finalPos;
	Ogre::Vector3 g_gamePos[6];
	GAPMisc misc;
	GAExampleSceneUI *g_sceneUI;
};




#endif