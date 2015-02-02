#ifndef GACHARACTERAISIMPLEOBJECT_H
#define GACHARACTERAISIMPLEOBJECT_H

#include "GACharacterAIBase.h"
#include "../GACharacterSimpleObject.h"

class GAExport GACharacterAISimpleObject : public GACharacterAIBase
{
protected:
	GACharacterSimpleObject* g_simpleObj;
	float g_totalMoveTime,g_currentMoveTime;
	Ogre::Vector3 g_moveDir,g_originPos,g_newPos;
public:
	bool g_move;
public:
	GACharacterAISimpleObject(GACharacterSimpleObject* characterSimpleObject);

	void moveToNewPlaceByTime(float x,float y,float z,float totalTime);
	void moveToNewPlaceBySpeed(float x,float y,float z,float newSpeed=-1);
	void rollAlongZAxis(float degrees);
	bool update(float deltaTime);
};

#endif
