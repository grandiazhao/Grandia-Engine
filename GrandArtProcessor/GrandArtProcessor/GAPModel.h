#ifndef GAPMODEL_H
#define GAPMODEL_H

#include "Common.h"

class GAExport GAPModel
{
private:
	bool g_hasRoad,g_loopRoad;
	float g_time,g_totalRoadTime,g_baseTime;
	Ogre::SceneNode* g_modelNode;
	UINT currentVecPos;
	struct RoadCell
	{
		float timePos;
		float x;
		float y;
		float z;
	};
	std::vector<RoadCell> g_roadVec;
public:
	GAPModel(Ogre::SceneNode* nodeToSimulate);
	void addToRoadVec(std::string text);
	void resetRidingOnRoad();
	void updateRidingOnRoad(float deltaTime);
	void update(float deltaTime);
	void updateRidingOnRoadOnCertainTime(float time);
	void updateOnCertainTime(float time);
};


#endif