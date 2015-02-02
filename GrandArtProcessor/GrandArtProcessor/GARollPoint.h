#ifndef GAROLLPOINT_H
#define GAROLLPOINT_H

#include "Common.h"

class GAExport GARollPoint
{
private:
    float g_timeInterval;
	float g_time;
public:
	GARollPoint(float timeInterval);
	void setTimeInterval(float timeInterval);
	bool getRollPoint(float deltaTime,float possibility);
	bool getRollPoint2(float deltaTime,float possibility);
	bool getImmediateRollPoint(float possibility);
	float getCurrentTimeCount();
	float getTimeInterval();
	float getTimeLeft();
	UINT getSeparateRollPoint(float deltaTime,UINT kindNum);
	UINT getImmediateSeparateRollPoint(UINT kindNum);
};

#endif