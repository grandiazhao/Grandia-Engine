#include "GACharacterAIBase.h"

GACharacterAIBase::GACharacterAIBase(GACharacterBase *character)
{
	g_character=character;
	g_aniState1=0;
	g_classType=0;
}

void GACharacterAIBase::setSpeed(float newSpeed)
{
	g_speed=newSpeed;
}

void GACharacterAIBase::rollAlongZAxis(float degrees)
{
	g_character->getCharacterSceneNode()->roll(Ogre::Degree(degrees),Ogre::Node::TS_WORLD);
}

std::string GACharacterAIBase::getName()
{
	return g_name;
}

bool GACharacterAIBase::runAnimation(std::string aniName,bool loop)
{
	Ogre::Entity* ent;
	if(g_aniState1!=0)
	{
		g_aniState1->setTimePosition(0);
		g_aniState1->setWeight(0);
	}
	g_aniState1=0;
	g_animationPaused=false;
	ent=g_character->getCharacterEntity();
	if(ent==0) return false;
	if(!ent->hasVertexAnimation()&&!ent->hasSkeleton()) return false;
	g_aniState1=ent->getAnimationState(aniName);
	g_aniState1->setLoop(loop);
	g_aniState1->setEnabled(true);
	g_aniState1->setTimePosition(0.f);
	g_aniState1->setWeight(1);
	return true;
}

UINT GACharacterAIBase::getAnimationState()
{
	if(g_aniState1==0)
	{
		return 0;
	}
	else if(g_animationPaused)
	{
		return 1;
	}
	else if(g_aniState1->hasEnded()) return 3;
	else return 2;
}

bool GACharacterAIBase::pauseAnimation()
{
	if(g_aniState1!=0)
	{
		g_animationPaused=true;
	}
	return true;
}

bool GACharacterAIBase::unpauseAnimation()
{
	if(g_aniState1!=0)
	{
		g_animationPaused=false;
	}
	return true;
}

bool GACharacterAIBase::stopAnimation()
{
	if(g_aniState1!=0)
	{
		g_aniState1->setTimePosition(0);
		g_aniState1=0;
	}
	g_animationPaused=true;
	return true;
}

bool GACharacterAIBase::resetAnimation()
{
	g_aniState1->setTimePosition(0);
	return true;
}
bool GACharacterAIBase::destroyAnimation()
{
	if(g_aniState1!=0)
	{
		g_aniState1->setTimePosition(0);
		g_aniState1->setEnabled(false);
		g_aniState1=0;
	}
	return true;
}

bool GACharacterAIBase::updateAnimation(float deltaTime)
{
	if(g_aniState1!=0&&g_animationPaused==false)
	{
		g_aniState1->addTime(deltaTime);
		return true;
	}
	return false;
}
GACharacterBase* GACharacterAIBase::getCharacter()
{
	return g_character;
}
bool GACharacterAIBase::update(float deltaTime)
{
	updateAnimation(deltaTime);
	return true;
}

float GACharacterAIBase::getAnimationTimePos()
{
	if(g_aniState1!=0) return g_aniState1->getTimePosition();
	else return 0;
}

void GACharacterAIBase::setAnimationTimePos(float time)
{
	if(g_aniState1!=0) g_aniState1->setTimePosition(time);
}

void GACharacterAIBase::rollAlongUpDirection(Ogre::Quaternion qua)
{
	g_character->getCharacterSceneNode()->rotate(qua);
}