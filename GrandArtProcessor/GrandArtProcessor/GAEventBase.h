#ifndef GAEVENTBASE_H
#define GAEVENTBASE_H

#include "Common.h"
#include "GAPMisc.h"
#include "GARoadCorner.h"

class GAExport GAEventBase
{
private:
	//p2-p1�õ���vec������ʱ��90���vecΪ����cross����
	struct lineToCrossed
	{
		float x1,x2,y1,y2;
		float z;//������z�����Ƿ���ײ
		UINT type;//1���ң�2���У�3���У�4������
	};
	std::vector<lineToCrossed> g_lineToCrossedVec;
	GAPMisc misc;
	float g_zTolerance;
public:
	GAEventBase();

	void addLineToVector(std::string textPath);
	void addLineToVector(float x1,float y1,float x2,float y2,float z,UINT type);

	//����Ϊp2-p1;
	UINT checkWhetherLineCrossed(float x1,float y1,float x2,float y2,float z);
};


#endif