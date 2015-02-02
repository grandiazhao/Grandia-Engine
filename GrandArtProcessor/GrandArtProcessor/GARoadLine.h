/*
����дע�͵ĺ�ϰ�ߣ�������ʱ�������Լ����㲻�����������ʲô�õġ���

GARoadLine������ͨ����ȡһ��·���ļ�������һ��·����
����·����
-ת�䴦�ñ�����������ģ�⡣
-ֱ�ߵ��е㽫���Զ�����Ϊһ����ʼ�㡣
-�Զ����ɱ��������Ƶ㡣
-�ṩ��GAVehicleAI�ࣨҲ����չ����������Ҫ·�����������ʹ�õĽӿڡ�
-��ֱ��-����-ֱ��-������������ֲ�������ֱ��Ϊ��һ������һ��������һ��������ߣ�
��ʱ����ô��
*/

#ifndef GAROADLINE_H
#define GAROADLINE_H

#include "Common.h"

class GAExport GARoadLine
{
private:
	struct RoadCell
	{
		UINT type;
		float x;
		float y;
	};
	struct BornCell
	{
		Ogre::Vector3 pos;
		Ogre::Vector3 dir;
	};
	struct PathCell
	{
		UINT startPoint;
		UINT endPoint;
		UINT type;
		float length;
	};
public:
	std::vector<RoadCell> g_roadPointVec;
	std::vector<PathCell> g_pathCellVec;
	UINT g_totalPointCount;
public:
	GARoadLine();
	void initRoad(std::string fileName);
	void setOnBornPoint(UINT num,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
	UINT getPathCellType(UINT num);
	float getPathCellLength(UINT num);
	UINT getPathCellNum();
	void getParam(UINT num,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
private:
	/*
	autoGenerateBeizerPoint ��������һ�����������Ƶ㣬��Ҫ���������������һ��ֱ�ߣ��յ��ʾ��ʼת��ĵ㣬ֱ�ߵķ���Ϊת��ĳ�ʼ����
	����Ҫһ����ʾҪת���ĵ㡣���������Ƶ�Ϊ��Ҫת���ĵ���ֱ���ϵĴ�ֱ�㡣
	*/
	RoadCell generateBeizerControlPoint(RoadCell& p1,RoadCell& p2,RoadCell &p3);
	void getParamOnBeizerLine(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
	void getParamOnBeeline(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
};



#endif