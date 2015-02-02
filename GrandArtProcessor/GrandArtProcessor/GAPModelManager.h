#ifndef GAPMODELMANAGER_H
#define GAPMODELMANAGER_H

#include "Common.h"
#include "GAPKeyboardInput.h"

class GAPMainManager;

class GAExport GAPModelManager
{
private:
	GAPMainManager* g_mainMgr;
	Ogre::Entity *g_planeXY,*g_planeYZ,*g_planeXZ,*g_planeXY2,*g_planeYZ2,*g_planeXZ2;
	Ogre::SceneNode* g_planeNode;
	bool g_hasPlane;
	Ogre::SceneManager* g_worldMgr;
	GAPKeyboardMap k_planeXY,k_planeYZ,k_planeXZ;
	int g_showPlane; //1_XY,2_YZ,3_XZ;
	Ogre::AnimationState* g_aniState1;
	bool g_animationPaused;
public:

public:
	GAPModelManager(GAPMainManager* mainMgr);
	void init();

	void setKey(GAPKeyboardMap showPlaneXY,GAPKeyboardMap showPlaneXZ,GAPKeyboardMap showPlaneYZ);
	bool injectKeyDown();
	bool setMoveOnPlane(Ogre::Vector3 origin);
	void freeMoveOnPlane();
	bool setModelPosition(Ogre::SceneNode* modelNode,Ogre::Vector3 vec,int mode=0);
	void moveModel(Ogre::SceneNode* modelNode,float x,float y,float z);
	bool setModelOrientation(Ogre::SceneNode* modelNode,float xdelta,float ydelta,Ogre::Vector3 xAxis,Ogre::Vector3 yAxis,float ratio=0.01f);
	bool setModelScale(Ogre::SceneNode* modelNode,float scalex,float scaley,float scalez);
	bool setModelMaterial(std::string modelName,std::string materialName);
	void setSkeleton(std::string meshName,std::string skeletonName);
	bool runModelAnimation(std::string modelName,std::string aniName="",bool loop=false);
	bool pauseModelAnimation();
	bool restartModelAnimation();
	bool stopModelAnimation();
	void setAnimationTimePos(float time);
	float getAnimationTimePos();
	UINT runningAnimation();

	void showPlaneXY(Ogre::Vector3 origin);
	void showPlaneYZ(Ogre::Vector3 origin);
	void showPlaneXZ(Ogre::Vector3 origin);
	bool update(float deltaTime);
	/*void attachModelToNearestObjOnPointRay();
	void setModelOritation();*/
private:
	void initKeyMap();
};

#endif