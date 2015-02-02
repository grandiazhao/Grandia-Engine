#ifndef GAPCHARACTERCONTROLLER_H
#define GAPCHARACTERCONTROLLER_H

#include "Common.h"
#include "GAPKeyboardInput.h"
#include "GAVehicle.h"
#include "GACharacterParamRecorder.h"

class GAPMainManager;

class GAPCharacterController
{
private:
	GAPMainManager* g_mainMgr;
	GAPKeyboardInput* g_keyboardInput;
	Ogre::Vector3 g_translateDir,g_facingDir;
	float g_rotate1,g_rotate2,g_moveSpeed;
	bool g_zAxisUp;
	GAPKeyboardMap k_into,k_back,k_up,k_down,k_left,k_right,r_left,r_right,r_up,r_down;
	bool b_kinto,b_kback,b_kup,b_kdown,b_kleft,b_kright,b_rleft,b_rright,b_rup,b_rdown;
	float g_acc,g_currentSpeed;
public:
	enum GAPCharacterType
	{
		GAPCharacterType_Man=0,
		GAPCharacterType_Vehicle,
		GAPCharacterType_Object,
		GAPCharacterType_SceneNode,
		GAPCharacterType_Undefined
	};
	
	GAPCharacterType g_characterType;
	Ogre::SceneNode* g_objectNode;
	GAVehicle* g_vehicle;

	GACharacterParamRecorder* g_recorder;
public:
	GAPCharacterController(GAPMainManager* mainMgr);
	void init();
	void initKeyMap();
	void setKey(int kNo,GAPKeyboardMap key);
	void setCharacter(Ogre::SceneNode* object);
	void setCharacter(GAVehicle* vehicle);

	float getCurrentSpeed(){return g_currentSpeed;};


	bool injectKeyDown();
	bool injectKeyUp();

	void update(float deltaTime=0.01f);
	bool move(float x,float y,float z);
	bool move(float deltaTime);

private:
	bool moveSceneNode(float deltaTime);
	bool moveVehicle(float deltaTime);
};

#endif