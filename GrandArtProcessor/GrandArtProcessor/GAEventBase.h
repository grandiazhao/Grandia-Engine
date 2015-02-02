#ifndef GAEVENTBASE_H
#define GAEVENTBASE_H

#include "Common.h"
#include "GAPMisc.h"
#include "GARoadCorner.h"

class GAExport GAEventBase
{
private:
	//p2-p1得到的vec，其逆时针90°的vec为其正cross方向
	struct lineToCrossed
	{
		float x1,x2,y1,y2;
		float z;//决定在z轴上是否碰撞
		UINT type;//1左右，2左中，3右中，4左中右
	};
	std::vector<lineToCrossed> g_lineToCrossedVec;
	GAPMisc misc;
	float g_zTolerance;
public:
	GAEventBase();

	void addLineToVector(std::string textPath);
	void addLineToVector(float x1,float y1,float x2,float y2,float z,UINT type);

	//方向为p2-p1;
	UINT checkWhetherLineCrossed(float x1,float y1,float x2,float y2,float z);
};


#endif