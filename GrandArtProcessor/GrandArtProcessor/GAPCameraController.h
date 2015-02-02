#ifndef GAPCAMERACONTROLLER_H
#define GAPCAMERACONTROLLER_H

#include "Common.h"
#include "GAPKeyboardInput.h"

class GAPMainManager;

class GAPCameraController
{
private:
	GAPMainManager* g_mainMgr;
	Ogre::Camera* g_camera;
	GAPKeyboardMap k_into,k_back,k_up,k_down,k_left,k_right,r_left,r_right,r_up,r_down;
	bool b_kinto,b_kback,b_kup,b_kdown,b_kleft,b_kright,b_rleft,b_rright,b_rup,b_rdown;
	GAPKeyboardInput* g_keyboardInput;
	Ogre::Vector3 g_translateDir;
	float g_freeTranslateSpeed;
	float g_aroundStyleMaxPitch,g_aroundStyleMinPitch,g_aroundStyleCurrentPitch;
	float g_stayStyleMaxYaw,g_stayStyleMinYaw,g_stayStyleCurrentYaw;
	float g_stayStyleMaxPitch,g_stayStyleMinPitch,g_stayStyleCurrentPitch;
	float g_maxRotateSpeed;
	bool g_zAxisUp;

	float g_autoYawSpeed,g_autoYawTotalDegree,g_autoPitchSpeed,g_autoPitchTotalDegree,g_autoZoomSpeed,g_autoZoomTotalLength;
	float g_autoCurrentYaw,g_autoCurrentPitch,g_autoCurrentZoom;
	bool g_autoYaw,g_autoPitch,g_autoZoom;

	bool g_lockCamera,g_staringNodeStaying;
public:
	enum CameraStyle
	{
		CameraStyle_Static=0,
		CameraStyle_Free,
		CameraStyle_AroundNode,
		CameraStyle_StayOnNode,
		CameraStyle_StaringMovable,
	};

	CameraStyle g_currentStyle;
	Ogre::SceneNode *g_cameraNode;
	Ogre::SceneNode *g_cameraPivot;
	Ogre::SceneNode *g_cameraGoal;
	Ogre::SceneNode *g_aroundNode;
	Ogre::SceneNode *g_stayOnNode;
	Ogre::SceneNode *g_lastStayOnNode;
	Ogre::SceneNode *g_lastAroundNode;
	Ogre::SceneNode *g_freeNode;
	Ogre::SceneNode *g_staringRootNode;
	Ogre::SceneNode *g_staringTarget;
	bool g_fixRotateOnNode;
public:
	GAPCameraController(GAPMainManager* mainMgr);
	void init();
	void updateRotate(float dx,float dy,int mode=0,float ratio=1.f);
	void updateMoving(float deltaTime,float ratio=1.f);
	void updateCameraNode();
	void update(float deltaTime);
	bool injectKeyDown();
	bool injectKeyUp();
	void setCameraStyle(CameraStyle style,Ogre::SceneNode* requiredNode=0);
	inline void setFreeTranslateSpeed(float newSpeed){ g_freeTranslateSpeed=newSpeed;}
	inline CameraStyle getCameraStyle(){return g_currentStyle;}
	void rotateCameraNode(float degree,int xyz);
	void setStayOnNodeMaxMinPitchYaw(float maxPitch,float minPitch,float maxYaw,float minYaw);

	void lock();
	void unlock();

	void setAutoPitch(float totalDegree,float pitchSpeed=0.f);
	bool autoPitch(float deltaTime);
	void setAutoYaw(float totalDegree,float yawSpeed=0.f);
	bool autoYaw(float deltaTime);
	void setAutoZoom(float totalDegree,float zoomSpeed=0.f);
	bool autoZoom(float deltaTime);

	Ogre::Vector3 getCameraFacingDirection();
	bool lookAt(float deltaTime);
	void setStaringModeParams(Ogre::Vector3 pos,Ogre::SceneNode* staringTarget,float maxRotate,bool staying=false);
private:
	void initCameraNode();
	void initKeyMap();
	void freeAllCameraNodes();
	void freeCameraNode();
	bool createAroundCameraNode(Ogre::SceneNode* roundNode);
	bool createStayOnCameraNode(Ogre::SceneNode* stayNode);
};

#endif