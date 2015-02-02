#ifndef GACHARACTERAIBASE_H
#define GACHARACTERAIBASE_H

#include "../Common.h"
#include "../GACharacterBase.h"

class GAExport GACharacterAIBase
{
protected:
	float g_heightFromGround,g_speed;
	GACharacterBase* g_character;
	std::string g_name;

	Ogre::AnimationState* g_aniState1;
	bool g_animationPaused;
public:
	UINT g_classType;
public:
	GACharacterAIBase(GACharacterBase* character);
	virtual bool update(float deltaTime);

public:
	virtual void setSpeed(float newSpeed);
	virtual void rollAlongZAxis(float degrees);
	virtual void rollAlongUpDirection(Ogre::Quaternion qua);

	virtual bool runAnimation(std::string aniName,bool loop=true);
	virtual UINT getAnimationState();
	virtual bool unpauseAnimation();
	virtual bool destroyAnimation();
	virtual bool resetAnimation();
	virtual bool pauseAnimation();
	virtual bool stopAnimation();
	virtual float getAnimationTimePos();
	virtual void setAnimationTimePos(float time);
	virtual bool updateAnimation(float deltaTime);
	virtual GACharacterBase* getCharacter();
	std::string getName();
};

#endif