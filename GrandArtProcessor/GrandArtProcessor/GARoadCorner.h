#ifndef GAROADCORNER_H
#define GAROADCORNER_H

#include "Common.h"

class GAExport GARoadCorner
{
private:
	struct Point2
	{
		float x;
		float y;
	};
public:
	
	struct LineToCross
	{
		UINT no;
		float x1,y1,x2,y2;
		float z;
		float degree;
		LineToCross* next;
		LineToCross* pre;
	};
	struct Corner
	{
		UINT lineNum;
		LineToCross* lineGroup;
		float xmin,ymin,xmax,ymax;
		float z;
	};
	std::vector<Corner> g_cornerVec;
	GAPMisc misc;
public:
	GARoadCorner(){};

	void addCorner(std::string text);
	void addCornerAutoForm1(std::string text);
	Corner* whetherLineCrossedWithCorner(float x1,float y1,float x2,float y2,float z,UINT& crossedFirstLineNo);
	//0 not crossed,1 crossed innerToOuter,2 crossed outerToInter,3 crossed outerToOuter
	UINT whetherLineCrossedWithCertainCorner(float x1,float y1,float x2,float y2,float z,Corner* corner,UINT& lineNo);
	UINT whetherLineCrossedWithCertainCornerCertainLine(float x1,float y1,float x2,float y2,float z,Corner* corner,UINT lineNo); 
	
};
#endif