/*
养成写注释的好习惯，否则有时候连我自己都搞不清这个类是做什么用的……

GARoadLine类用于通过读取一个路径文件来生成一条路径。
这条路径上
-转弯处用贝塞尔曲线来模拟。
-直线的中点将被自动生成为一个初始点。
-自动生成贝塞尔控制点。
-提供让GAVehicleAI类（也可扩展到其他有需要路径的类里）可以使用的接口。
-按直线-曲线-直线-曲线这样间隔分布，其中直线为第一个（第一个点和最后一个点的连线）
暂时先这么多
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
	autoGenerateBeizerPoint 规则，生成一个贝塞尔控制点，需要先有两个点来描绘一条直线，终点表示开始转向的点，直线的方向为转向的初始方向，
	还需要一个表示要转到的点。贝塞尔控制点为该要转到的点在直线上的垂直点。
	*/
	RoadCell generateBeizerControlPoint(RoadCell& p1,RoadCell& p2,RoadCell &p3);
	void getParamOnBeizerLine(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
	void getParamOnBeeline(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir);
};



#endif