#include "GAEventBase.h"

GAEventBase::GAEventBase()
{
	g_zTolerance=2;
}

void GAEventBase::addLineToVector(std::string text)
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
	for(UINT i=0;i<lineVec.size();i++)
	{
		std::string line=lineVec[i];
		lineToCrossed c;
		std::string::size_type separator_pos=line.find_first_of(",",0);//we use ',' to separate the params
		c.x1=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(",",0);
		c.y1=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(",",0);
		c.x2=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(",",0);
		c.y2=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		separator_pos=line.find_first_of(",",0);
		c.z=misc.ConvertToFloat(line.substr(0,separator_pos));
		line=line.substr(separator_pos+1,line.length()-1);
		c.type=misc.ConvertToInt(line);
		g_lineToCrossedVec.push_back(c);
	}
}

void GAEventBase::addLineToVector(float x1,float y1,float x2,float y2,float z,UINT type)
{
	lineToCrossed l;
	l.x1=x1;l.x2=x2;l.y1=y1;l.y2=y2;l.z=z;l.type=type;
	g_lineToCrossedVec.push_back(l);
}

UINT GAEventBase::checkWhetherLineCrossed(float x1,float y1,float x2,float y2,float z)
{
	lineToCrossed* l;
	float temp,vx1,vx2,vy1,vy2,ratio;
	for(UINT i=0;i<g_lineToCrossedVec.size();i++)
	{
		l=&g_lineToCrossedVec[i];
		if(abs(z-l->z)>g_zTolerance) continue;
		else
		{
			vx1=l->x2-l->x1;
			vy1=l->y2-l->y1;
			vx2=x2-x1;
			vy2=y2-y1;
			temp=-vy1*vx2+vx1*vy2;
			if(temp>0.0001)
			{
				if(misc.checkTwoLineSegmentCross(l->x1,l->y1,l->x2,l->y2,x1,y1,x2,y2,ratio)) return l->type;
			}
		}
	}
	return 0;
}