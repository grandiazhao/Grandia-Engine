/*
����дע�͵ĺ�ϰ�ߣ�������ʱ�������Լ����㲻�����������ʲô�õġ���

GACharacterParamRecorder�����ڼ�¼Character��֮ǰһ��ʱ���״̬��
Ŀǰ���Լ�¼
-ǰһ��ʱ���λ�ã����ȡ�
-Ĭ��1���¼һ�Σ���������frequency����frequencyΪ2���������¼һ��
-�Ժ���չ�ù��ܡ�
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