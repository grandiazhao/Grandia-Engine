#include "GARollPoint.h"

GARollPoint::GARollPoint(float timeInterval)
{
	g_timeInterval=timeInterval;
	g_time=0;
	srand((unsigned)time(0));
}

void GARollPoint::setTimeInterval(float timeInterval)
{
	g_timeInterval=timeInterval;
	g_time=0;
}

bool GARollPoint::getRollPoint(float deltaTime,float possibility)
{
	g_time+=deltaTime;
	if(g_time>g_timeInterval)
	{
		g_time=0;
		int t1=rand()%1000;
		if(t1>=possibility*1000) return false;
		else return true;
	}
	return false;
}

bool GARollPoint::getRollPoint2(float deltaTime,float possibility)
{
	g_time+=deltaTime;
	if(g_time>g_timeInterval)
	{
		//g_time=0;
		int t1=rand()%1000;
		if(t1>=possibility*1000) return false;
		else return true;
	}
	return false;
}

bool GARollPoint::getImmediateRollPoint(float possibility)
{
	int t1=rand()%1000;
	if(t1>=possibility*1000) return false;
	else return true;
}

UINT GARollPoint::getSeparateRollPoint(float deltaTime,UINT kindNum)
{
	g_time+=deltaTime;
	if(g_time>g_timeInterval)
	{
		g_time=0;
		return rand()%kindNum+1;
	}
	return 0;
}

UINT GARollPoint::getImmediateSeparateRollPoint(UINT kindNum)
{
	return rand()%kindNum+1;
}

float GARollPoint::getCurrentTimeCount()
{
	return g_time;
}

float GARollPoint::getTimeInterval()
{
	return g_timeInterval;
}

float GARollPoint::getTimeLeft()
{
	return g_timeInterval-g_time;
}