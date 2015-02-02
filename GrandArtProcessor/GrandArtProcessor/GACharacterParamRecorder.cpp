#include "GACharacterParamRecorder.h"

GACharacterParamRecorder::GACharacterParamRecorder(UINT stepNum,Ogre::Vector3 originPos,Ogre::Quaternion originQua)
{
	p_group=new ParamCell[stepNum];
	ParamCell* p;
	p_head=&p_group[0];
	p_head->pos=originPos;
	p_head->qua=originQua;
	p_head->previousCell=&p_group[stepNum-1];
	p_head->nextCell=&p_group[1];

	for(UINT i=1;i<stepNum-1;i++)
	{
		p=&p_group[i];
		p->pos=originPos;
		p->qua=originQua;
		p->previousCell=&p_group[i-1];
		p->nextCell=&p_group[i+1];
	}

	p_tail=&p_group[stepNum-1];
	p_tail->previousCell=&p_group[stepNum-2];
	p_tail->nextCell=p_head;
	p_tail->pos=originPos;
	p_tail->qua=originQua;
	
	g_time=0;
	g_frequency=1;
	g_stepNum=stepNum;
}

void GACharacterParamRecorder::setTimeFrequency(float frequency)
{
	g_frequency=frequency;
}

void GACharacterParamRecorder::update(float deltaTime,Ogre::Vector3 pos,Ogre::Quaternion qua)
{
	g_time+=deltaTime*g_frequency;
	if(g_time>1)
	{
		g_time=0;
		p_tail->pos=pos;
		p_tail->qua=qua;
		p_head=p_tail;
		p_tail=p_tail->previousCell;
	}
}

Ogre::Vector3 GACharacterParamRecorder::getFormerPosition(UINT step)
{
	if(step==0) return Ogre::Vector3(0,0,0);
	ParamCell* p=p_head;
	for(UINT i=1;i<step;i++)
	{
		p=p->nextCell;
	}
	return p->pos;
}

Ogre::Quaternion GACharacterParamRecorder::getFormerQuaternion(UINT step)
{
	if(step==0) return Ogre::Quaternion::IDENTITY;
	ParamCell* p=p_head;
	for(UINT i=1;i<step;i++)
	{
		p=p->nextCell;
	}
	return p->qua;
}