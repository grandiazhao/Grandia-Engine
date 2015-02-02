/*
养成写注释的好习惯，否则有时候连我自己都搞不清这个类是做什么用的……

GACharacterParamRecorder类用于记录Character在之前一段时间的状态。
目前可以记录
-前一段时间的位置，旋度。
-默认1秒记录一次，可以设置frequency，如frequency为2就是两秒记录一次
-以后扩展该功能。
*/

#ifndef GACHARACTERPARAMRECORDER_H
#define GACHARACTERPARAMRECORDER_H

#include "Common.h"

class GAExport GACharacterParamRecorder
{
private:
	struct ParamCell
	{
		Ogre::Vector3 pos;
		Ogre::Quaternion qua;
		ParamCell* nextCell;
		ParamCell* previousCell;
	};
	ParamCell* p_head,*p_tail,*p_group;
	float g_time;
	float g_frequency;
	UINT g_stepNum;
public:
	GACharacterParamRecorder(UINT stepNum,Ogre::Vector3 originPos=Ogre::Vector3(0,0,0),Ogre::Quaternion originQua=Ogre::Quaternion::IDENTITY);
	void setTimeFrequency(float frequency);
	void update(float deltaTime,Ogre::Vector3 pos,Ogre::Quaternion qua);
	Ogre::Vector3 getFormerPosition(UINT step);
	Ogre::Quaternion getFormerQuaternion(UINT step);
};

#endif