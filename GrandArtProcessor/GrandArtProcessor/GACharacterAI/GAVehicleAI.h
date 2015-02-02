#ifndef GAVEHICLEAI_H
#define GAVEHICLEAI_H

#include "GACharacterAIBase.h"
#include "../Common.h"
#include "../GA3DMath.h"
#include "../GAVehicle.h"
#include "../GACharacterParamRecorder.h"
#include "../GARoadLine.h"

class GAExport GAVehicleAI : public GACharacterAIBase
{
private:
	Ogre::SceneNode* g_vehicleNode;
	Ogre::Ray g_castRay;
	Ogre::RaySceneQuery* g_raySceneQuery;
	float g_verticalDegree,g_horizontalDegree,g_tempHeight;
	Ogre::Vector3 g_direction,g_scale,g_moveDirection;
	
	UINT g_currentRoadCell;
	bool g_loopRidingOnRoad;
	float g_beizerSpeed,g_beizerTotalTime,g_beizerTimeRatio;
	float g_roadRatio;
	float vTransSpeed;
	float vTransDistance;
	GAVehicle* g_vehicle;
	GARoadLine* g_roadLine;
public:
	GACharacterParamRecorder* g_recorder;
	
public:
	GAVehicleAI(GAVehicle* vehicle,Ogre::SceneManager* sgr);
	bool update(float deltaTime);
	GARoadLine* getRoadLine();
	void setRoadLine(GARoadLine* roadLine);
	bool updateClampToGround();
	void setBornPosition(UINT lineCell,float ratio,bool clamp=true);
	void setVehicleSpeed(float beelineSpeed,float beizerSpeed);
	float getRoadRatio();
	UINT getRoadCell();
	void setPassingTranslate(float deltaTime,Ogre::Vector3 point,Ogre::Vector3 direction);
private:
	bool updateRidingOnRoad(float deltaTime);
};

#endif