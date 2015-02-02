#ifndef GASCENEHAND_H
#define GASCENEHAND_H

#include "GASceneBase.h"
#include "GACharacterAI\GACharacterAISimpleObject.h"
#include "GAServerSimple.h"
#include "GAPhysicsBaseObject.h"

class GAExampleSceneUI;

class GABone
{
protected:
	GAPMainManager* g_mainMgr;
	Ogre::SceneNode* rootNode;
	float maxPitch,minPitch,currPitch;
	float maxYaw,minYaw,currYaw;
	float maxRoll,minRoll,currRoll;
	GABone* parentBone;
	std::string boneName;
	std::string g_materialName;
public:
	std::vector<GABone*> childBoneVec;
public:
	enum GABoneType
	{
		GABone_Root=1,
		GABone_Child,
	};
	enum GABoneInherit
	{
		GABone_None=0,
		GABone_Cube,
		GABone_Capsule,
	};
	GABoneInherit g_inheritType;
public:
	GABone(GAPMainManager* mainMgr,std::string name,GABoneType type=GABone_Root,GABone* parent=0);
	Ogre::SceneNode* getRootNode();
	virtual GABone* findChildBone(std::string name,bool deep=false);
	virtual void loadPostureFromString(std::string str);
	virtual std::string savePostureToString();
	virtual std::string saveBoneTree();
	virtual bool loadBoneTreePosture(std::string filePath);
	virtual void setMaterials(std::string materialName);
	virtual std::string getMaterialName();

	std::string getName();
	void setPosition(float x,float y,float z);
	void translate(float x,float y,float z);
	void setPitch(float degree);
	void setYaw(float degree);
	void setRoll(float degree);
	void pitch(float degree);
	void yaw(float degree);
	void roll(float degree);
	float getCurrentPitch();
	float getCurrentYaw();
	float getCurrentRoll();
	GAFloat3Struct* getPosition();
	void setMaxMinPitch(float max,float min);
	void setMaxMinYaw(float max,float min);
	void setMaxMinRoll(float max,float min);
};

class GACapsuleBone;

class GACubeBone : public GABone
{
private:
	Ogre::SceneNode *cubeNode;
	Ogre::Entity* cube;
	float g_length_x,g_length_y,g_length_z;
public:
	GASimpleCube* p_cube;
public:
	GACubeBone(GAPMainManager* mainMgr,std::string name,float length_x,float length_y,float length_z,GABoneType type=GABone_Root,GABone* parent=0);
	void setMaterials(std::string materialName);
	void loadPostureFromString(std::string str);
	std::string savePostureToString();
	void resetCube(float x,float y,float z);
	float getLengthX();
	float getLengthY();
	float getLengthZ();
	GACubeBone* createChildCubeBone(std::string name,float length_x,float length_y,float length_z);
	GACapsuleBone* createChildCapsuleBone(std::string name,float cylinderHeight,float radius);
};

class GACapsuleBone : public GABone
{
private:
	Ogre::SceneNode* halfSphere1,*halfSphere2,*cylinder;
	Ogre::Entity* cyl,*sp1,*sp2;
	float g_radius,g_cylinderHeight;
public:
	GACapsuleBone(GAPMainManager* mainMgr,std::string name,float cylinderHeight,float radius,GABoneType type=GABone_Root,GABone* parent=0);
	void setMaterials(std::string materialName);
	void loadPostureFromString(std::string str);
	std::string savePostureToString();
	void resetRadius(float radius);
	void resetCylinderHeight(float height);
	float getRadius();
	float getCylinderHeight();
	GACapsuleBone* createChildCapsuleBone(std::string name,float cylinderHeight,float radius);
	GACubeBone* createChildCubeBone(std::string name,float length_x,float length_y,float length_z);
};


class GASceneHand : public GASceneBase
{
private:
	enum HandControllType
	{
		Controll_None=0,
		Controll_Pitch,
		Controll_Yaw,
		Controll_Roll,
		Controll_Move_X,
		Controll_Move_Y,
		Controll_Move_Z,
		Controll_SetColor_Red,
		Controll_SetColor_Yellow,
		Controll_SetColor_Blue,
		Controll_SetColor_Green,
		Controll_SetColor_Pink,
		Controll_SetColor_Purple,
		Controll_SetColor_Black,
		Controll_SetColor_White,
	};
public:
	
public:
	GASceneHand(GAPMainManager* mainMgr,std::string sceneName);
	~GASceneHand();
	bool createScene();
	void initBones();
	bool reCreateScene();
	bool freezeScene();
	bool reloadScene();
	void createHand();

	void loadInitFile(std::string filePath);
	void saveInitFile(std::string filePath);

	bool keyReleased(const OIS::KeyEvent& evt);
	bool frameStarted(float deltaTime);
	bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& evt,OIS::MouseButtonID id);

	void controllSelectBone();
	void updatePhysics();
private:
	void userFunction(std::string recvedString);
	GACharacterAISimpleObject* womanAI;
	Ogre::Entity* handEnt;
	Ogre::Bone* bone[64];
	UINT boneNum;
	GADataMap* stringMap;
	Ogre::MovableObject* touchedObject;
	GABone* touchedBone;
	GACubeBone* rootBone;
	HandControllType ctrlType;
	std::string lastMaterial;
	GAExampleSceneUI* g_sceneUI;
	float deltaValue;
	GAPMisc misc;

	GASimpleCube* p_cubeToCollide[5];
	Ogre::SceneNode* cubeToCollide[5];
	Ogre::Entity* cubeToCollideEnt[5];

	Ogre::Entity* dynamicEnt;
	Ogre::SceneNode* dynamicEntNode;
};

#endif