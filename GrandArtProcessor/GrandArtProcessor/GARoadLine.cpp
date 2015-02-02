#include "GARoadLine.h"

GARoadLine::GARoadLine()
{
	g_roadPointVec.clear();
	g_totalPointCount=0;
}

void GARoadLine::initRoad(std::string path)
{
	std::ifstream ifs;
	GAPMisc misc;
	ifs.open(path.c_str(),std::ios::in|std::ios::binary);
	if(ifs.fail()) return;
	Ogre::DataStreamPtr dataPtr(OGRE_NEW Ogre::FileStreamDataStream(path,&ifs,false));
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
	g_roadPointVec.clear();
	for(UINT i=0;i<lineVec.size();i++)
	{
		std::string line=lineVec[i];
		RoadCell c;
		std::string::size_type separator_pos=line.find_first_of(",",0);//we use ',' to separate the params
		c.x=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(",",0);
		c.y=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		c.type=misc.ConvertToInt(line);
		g_roadPointVec.push_back(c);
		if(g_roadPointVec.size()>1)
		{
			if((i!=lineVec.size()-1))
			{
				i++;
				std::string line2=lineVec[i];
				RoadCell c2;
				std::string::size_type separator_pos2=line2.find_first_of(",",0);//we use ',' to separate the params
				c2.x=misc.ConvertToFloat(line2.substr(0,separator_pos2));
				line2=line2.substr(separator_pos2+1,line2.length()-1);
				separator_pos2=line2.find_first_of(",",0);
				c2.y=misc.ConvertToFloat(line2.substr(0,separator_pos2));
				line2=line2.substr(separator_pos2+1,line2.length()-1);
				c2.type=misc.ConvertToInt(line2);
				int roadSize=g_roadPointVec.size();
				RoadCell cg=generateBeizerControlPoint(g_roadPointVec[roadSize-2],g_roadPointVec[roadSize-1],c2);
				g_roadPointVec.push_back(cg);
				g_roadPointVec.push_back(c2);
			}
			else
			{
				int roadSize=g_roadPointVec.size();
				RoadCell cg=generateBeizerControlPoint(g_roadPointVec[roadSize-2],g_roadPointVec[roadSize-1],g_roadPointVec[0]);
				g_roadPointVec.push_back(cg);
			}
		}
	}

	//生成pathCell，将马路分段
	g_pathCellVec.clear();
	float dx,dy;
	for(UINT i=0;i<g_roadPointVec.size();i++)
	{
		PathCell p;
		p.startPoint=i;
		p.endPoint=i+1;
		p.type=1;
		g_pathCellVec.push_back(p);
		i++;
		PathCell p2;
		p2.startPoint=i;
		p2.endPoint=i+2;
		p2.type=2;
		g_pathCellVec.push_back(p2);
		i++;
	}
	g_pathCellVec.back().endPoint=0;

	for(UINT i=0;i<g_pathCellVec.size();i++)
	{
		PathCell &p=g_pathCellVec[i];
		dx=g_roadPointVec[p.endPoint].x-g_roadPointVec[p.startPoint].x;
		dy=g_roadPointVec[p.endPoint].y-g_roadPointVec[p.startPoint].y;
		p.length=sqrtf(dx*dx+dy*dy);
		if(p.type==2)
		{
			p.length=p.length/1.414*1.57;
		}
	}
	
}

GARoadLine::RoadCell GARoadLine::generateBeizerControlPoint(RoadCell& p1,RoadCell& p2,RoadCell &p3)
{
	float x31,x21,y31,y21;
	x31=p3.x-p1.x;
	x21=p2.x-p1.x;
	y31=p3.y-p1.y;
	y21=p2.y-p1.y;
	float k=(x31*x21+y31*y21)/(y21*y21+x21*x21);
	RoadCell c;
	c.type=2;
	c.x=p1.x+k*x21;
	c.y=p1.y+k*y21;
	return c;
}

void GARoadLine::setOnBornPoint(UINT num,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir)
{
	UINT k1=num%g_pathCellVec.size();
	PathCell &c=g_pathCellVec[k1];
	if(c.type==1)
	{
		getParamOnBeeline(c,ratio,pos,dir);
	}
	else if(c.type==2)
	{
		getParamOnBeizerLine(c,ratio,pos,dir);
	}
}

UINT GARoadLine::getPathCellNum()
{
	return g_pathCellVec.size();
}

UINT GARoadLine::getPathCellType(UINT num)
{
	num=num%g_pathCellVec.size();
	return g_pathCellVec[num].type;
}

float GARoadLine::getPathCellLength(UINT num)
{
	num=num%g_pathCellVec.size();
	return g_pathCellVec[num].length;
}
void GARoadLine::getParam(UINT num,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir)
{
	UINT k1=num%g_pathCellVec.size();
	PathCell &c=g_pathCellVec[k1];
	if(c.type==1)
	{
		getParamOnBeeline(c,ratio,pos,dir);
	}
	else if(c.type==2)
	{
		getParamOnBeizerLine(c,ratio,pos,dir);
	}
}

void GARoadLine::getParamOnBeizerLine(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir)
{
	float t1=(1-ratio);
	float t2=2*ratio;
	float t3=ratio*ratio;
	RoadCell p1,p2,p3;
	p1=g_roadPointVec[p.startPoint];
	p2=g_roadPointVec[p.startPoint+1];
	p3=g_roadPointVec[p.endPoint];
	dir.x=2*t1*(p2.x-p1.x)+t2*(p3.x-p2.x);
	dir.y=2*t1*(p2.y-p1.y)+t2*(p3.y-p2.y);
	dir.z=0;
	dir.normalise();

	t2*=t1;
	t1*=t1;
	pos.x=t1*p1.x+t2*p2.x+t3*p3.x;
	pos.y=t1*p1.y+t2*p2.y+t3*p3.y;
}

void GARoadLine::getParamOnBeeline(PathCell p,float ratio,Ogre::Vector3& pos,Ogre::Vector3& dir)
{
	Ogre::Vector3 d;
	d.x=g_roadPointVec[p.endPoint].x-g_roadPointVec[p.startPoint].x;
	d.y=g_roadPointVec[p.endPoint].y-g_roadPointVec[p.startPoint].y;
	d.z=0;

	pos.x=g_roadPointVec[p.startPoint].x+ratio*d.x;
	pos.y=g_roadPointVec[p.startPoint].y+ratio*d.y;
	dir.x=d.x;
	dir.y=d.y;
	dir.z=0;
	dir.normalise();
}