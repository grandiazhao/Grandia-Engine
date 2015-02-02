#include "GARoadCorner.h"

void GARoadCorner::addCorner(std::string text)
{
	std::ifstream ifs;
	ifs.open(text.c_str(),std::ios::in|std::ios::binary);
	if(ifs.fail()) return;
	Ogre::DataStreamPtr dataPtr(OGRE_NEW Ogre::FileStreamDataStream(text,&ifs,false));
	std::vector<std::string> lineVec;
	while(!dataPtr->eof())
	{
		std::string line=dataPtr->getLine(true);
		if(line.length()>0&&line.at(0)!='#'&&line.at(0)!='@')
		{
			lineVec.push_back(line);
		}
	}
	ifs.close();
	bool group=false;
	Corner c;
	UINT groupNo=0;
	for(UINT i=0;i<lineVec.size();i++)
	{
		std::string line=lineVec[i];
		if(line.at(0)=='c'){
			line=line.substr(2,line.length()-1);
			int num=misc.ConvertToInt(line);
			line=line.substr(line.find_first_of(',')+1,line.length()-1);
			c.z=misc.ConvertToFloat(line);
			c.lineGroup=new LineToCross[num];
			c.lineNum=num;
			group=true;
			groupNo=0;
			continue;
		}
		if(group)
		{
			LineToCross &l=c.lineGroup[groupNo];
			l.no=groupNo;
			std::string::size_type separator_pos=line.find_first_of(",",0);//we use ',' to separate the params
			l.x1=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			separator_pos=line.find_first_of(",",0);
			l.y1=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			separator_pos=line.find_first_of(",",0);
			l.x2=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			separator_pos=line.find_first_of(",",0);
			l.y2=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			l.degree=misc.ConvertToInt(line);
			if(groupNo==0)
			{
				c.xmax=l.x1>l.x2?l.x1:l.x2;
				c.xmin=l.x1<l.x2?l.x1:l.x2;
				c.ymax=l.y1>l.y2?l.y1:l.y2;
				c.ymin=l.y1<l.y2?l.y1:l.y2;
			}
			else
			{
				if(l.x1>l.x2)
				{
					if(c.xmax<l.x1) c.xmax=l.x1;
					if(c.xmin>l.x2) c.xmin=l.x2;
				}
				else
				{
					if(c.xmax<l.x2) c.xmax=l.x2;
					if(c.xmin>l.x1) c.xmin=l.x1;
				}
				if(l.y1>l.y2)
				{
					if(c.ymax<l.y1) c.ymax=l.y1;
					if(c.ymin>l.y2) c.ymin=l.y2;
				}
				else
				{
					if(c.ymax<l.y2) c.ymax=l.y2;
					if(c.ymin>l.y1) c.ymin=l.y1;
				}
			}
			groupNo++;
			if(groupNo>=c.lineNum) 
			{
				group=false;
				g_cornerVec.push_back(c);
			}
		}
	}
}

void GARoadCorner::addCornerAutoForm1(std::string text)
{
	std::ifstream ifs;
	ifs.open(text.c_str(),std::ios::in|std::ios::binary);
	if(ifs.fail()) return;
	Ogre::DataStreamPtr dataPtr(OGRE_NEW Ogre::FileStreamDataStream(text,&ifs,false));
	std::vector<std::string> lineVec;
	std::string line;
	while(!dataPtr->eof())
	{
		line=dataPtr->getLine(true);
		if(line.length()>0&&line.at(0)!='#'&&line.at(0)!='@')
		{
			lineVec.push_back(line);
		}
	}
	ifs.close();
	bool group=false;
	Corner c;
	UINT groupNo=0;
	UINT pointCount,streetCount;
	float zHeight;
	std::string::size_type separator_pos;
	std::vector<Point2> pointVec;
	for(UINT i=0;i<lineVec.size();i++)
	{
		line=lineVec[i];
		separator_pos=line.find_first_of(',',0);
		streetCount=misc.ConvertToInt(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(',',0);
		zHeight=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		pointCount=misc.ConvertToInt(line);
		c.lineNum=streetCount;
		c.lineGroup=new LineToCross[streetCount];
		c.z=zHeight;
		pointVec.clear();
		for(UINT j=0;j<pointCount;j++)
		{
			i++;
			line=lineVec[i];
			Point2 p2;
			separator_pos=line.find_first_of(',',0);
			p2.x=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			p2.y=misc.ConvertToFloat(line);
			pointVec.push_back(p2);
		}
		LineToCross* ltc;
		for(UINT j=0;j<streetCount;j++)
		{
			ltc=&c.lineGroup[j];
			ltc->no=j;
			ltc->x1=pointVec[j].x;
			ltc->y1=pointVec[j].y;

			if(j+1==pointCount)
			{
				ltc->x2=pointVec[0].x;
				ltc->y2=pointVec[0].y;
			}
			else
			{
				ltc->x2=pointVec[j+1].x;
				ltc->y2=pointVec[j+1].y;
			}

			if(j==0)
			{
				c.xmax=ltc->x1>ltc->x2?ltc->x1:ltc->x2;
				c.xmin=ltc->x1<ltc->x2?ltc->x1:ltc->x2;
				c.ymax=ltc->y1>ltc->y2?ltc->y1:ltc->y2;
				c.ymin=ltc->y1<ltc->y2?ltc->y1:ltc->y2;
			}
			else
			{
				if(ltc->x1>ltc->x2)
				{
					if(c.xmax<ltc->x1) c.xmax=ltc->x1;
					if(c.xmin>ltc->x2) c.xmin=ltc->x2;
				}
				else
				{
					if(c.xmax<ltc->x2) c.xmax=ltc->x2;
					if(c.xmin>ltc->x1) c.xmin=ltc->x1;
				}
				if(ltc->y1>ltc->y2)
				{
					if(c.ymax<ltc->y1) c.ymax=ltc->y1;
					if(c.ymin>ltc->y2) c.ymin=ltc->y2;
				}
				else
				{
					if(c.ymax<ltc->y2) c.ymax=ltc->y2;
					if(c.ymin>ltc->y1) c.ymin=ltc->y1;
				}
			}

		}
		g_cornerVec.push_back(c);
	}
}




GARoadCorner::Corner* GARoadCorner::whetherLineCrossedWithCorner(float x1,float y1,float x2,float y2,float z,UINT& lineNo)
{
	Corner* c;
	LineToCross* l;
	float temp,vx1,vx2,vy1,vy2,ratio;
	ratio=0;
	bool ret=false;
	for(UINT i=0;i<g_cornerVec.size();i++)
	{
		c=&g_cornerVec[i];
		if(abs(z-c->z)>2) continue;
		else
		{
			//判断是否aabb相交
			if(x1>x2)
			{
				if(x1<c->xmin) continue;
				if(x2>c->xmax) continue;
			}
			else
			{
				if(x2<c->xmin) continue;
				if(x1>c->xmax) continue;
			}
			if(y1>y2)
			{
				if(y1<c->ymin) continue;
				if(y2>c->ymax) continue;
			}
			else
			{
				if(y2<c->ymin) continue;
				if(y1>c->ymax) continue;
			}
			//对corner的每条线进行测试
			for(UINT j=0;j<c->lineNum;j++)
			{
				l=&c->lineGroup[j];
				vx1=l->x2-l->x1;
				vy1=l->y2-l->y1;
				vx2=x2-x1;
				vy2=y2-y1;
				temp=-vy1*vx2+vx1*vy2;
				if(temp>0.0001)
				{
					if(misc.checkTwoLineSegmentCross(l->x1,l->y1,l->x2,l->y2,x1,y1,x2,y2,temp))
					{
						if(ratio<temp)
						{
							ratio=temp;
							lineNo=j;
							ret=true; //检测到一个cross就返回该corner
						}
					}
				}
			}
			if(ret)
			{
				return c;
			}
		}
	}
	return 0;
}

UINT GARoadCorner::whetherLineCrossedWithCertainCorner(float x1,float y1,float x2,float y2,float z,Corner* corner,UINT& lineNo)
{

	return 0;
}


UINT GARoadCorner::whetherLineCrossedWithCertainCornerCertainLine(float x1,float y1,float x2,float y2,float z,Corner* corner,UINT lineNo)
{

	return 0;
}