#include "GACharacterAISimpleObject.h"

GACharacterAISimpleObject::GACharacterAISimpleObject(GACharacterSimpleObject* simObj) : GACharacterAIBase(simObj)
{
	g_simpleObj=simObj;
	g_move=false;
	g_speed=10;
	g_totalMoveTime=0;
	g_currentMoveTime=0;
	g_moveDir=Ogre::Vector3(1,0,0);
}

void GACharacterAISimpleObject::moveToNewPlaceByTime(float x,float y,float z,float totalTime)
{
	g_originPos=g_simpleObj->getCharacterSceneNode()->_getDerivedPosition();
	g_newPos=Ogre::Vector3(x,y,z);
	g_moveDir=g_newPos-g_originPos;
	if(g_moveDir.isZeroLength()) return;
	float length=g_moveDir.length();
	g_speed=length/totalTime;
	g_totalMoveTime=totalTime;
	g_moveDir.normalise();
	g_currentMoveTime=0;
	g_move=true;
}

void GACharacterAISimpleObject::moveToNewPlaceBySpeed(float x,float y,float z,float newSpeed)
{
	if(newSpeed>0) g_speed=newSpeed;
	g_originPos=g_simpleObj->getCharacterSceneNode()->_getDerivedPosition();
	g_newPos=Ogre::Vector3(x,y,z);
	g_moveDir=g_newPos-g_originPos;
	if(g_moveDir.isZeroLength()) return;
	float length=g_moveDir.length();
	g_totalMoveTime=length/g_speed;
	g_moveDir.normalise();
	g_currentMoveTime=0;
	g_move=true;

}

bool GACharacterAISimpleObject::update(float deltaTime)
{
	if(g_move)
	{
		g_currentMoveTime+=deltaTime;
		if(!g_simpleObj->getFacingDirection().directionEquals(g_moveDir,Ogre::Degree(0.1)))
		{
			rollAlongZAxis(g_simpleObj->getTurnDegreesXY(g_moveDir));
		}
		if(g_currentMoveTime<g_totalMoveTime)
		{
			Ogre::Vector3 currPos=g_originPos+(g_moveDir*g_currentMoveTime*g_speed);
			g_simpleObj->getCharacterSceneNode()->setPosition(currPos);
			//g_simpleObj->getCharacterSceneNode()->translate(g_speed*deltaTime*g_moveDir,Ogre::Node::TS_WORLD);
		}
		else
		{
			g_simpleObj->getCharacterSceneNode()->setPosition(g_newPos);
			g_move=false;
			g_currentMoveTime=0;
		}
	}
	return GACharacterAIBase::update(deltaTime);
}

void GACharacterAISimpleObject::rollAlongZAxis(float degrees)
{
	g_simpleObj->getRotateNode()->roll(Ogre::Degree(degrees),Ogre::Node::TS_WORLD);
}