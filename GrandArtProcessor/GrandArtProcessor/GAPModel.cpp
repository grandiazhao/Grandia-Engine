#include "GAPModel.h"

GAPModel::GAPModel(Ogre::SceneNode* node)
{
	g_time=0;
	g_hasRoad=false;
	g_loopRoad=true;
	g_modelNode=node;
	currentVecPos=0;
}

void GAPModel::addToRoadVec(std::string text)
{
	g_roadVec.clear();
	std::ifstream ifs;
	GAPMisc misc;
	ifs.open(text.c_str(),std::ios::in|std::ios::binary);
	if(ifs.fail()) return;
	Ogre::DataStreamPtr dataPtr(OGRE_NEW Ogre::FileStreamDataStream(text,&ifs,false));
	while(!dataPtr->eof())
	{
		std::string line=dataPtr->getLine(true);
		if(line.length()>0&&line.at(0)!='#'&&line.at(0)!='@')
		{
			RoadCell c;
			std::string::size_type separator_pos=line.find_first_of(",",0);//we use ',' to separate the params
			c.timePos=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			separator_pos=line.find_first_of(",",0);
			c.x=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			separator_pos=line.find_first_of(",",0);
			c.y=misc.ConvertToFloat(line.substr(0,separator_pos));
			line=line.substr(separator_pos+1,line.length()-1);
			c.z=misc.ConvertToFloat(line);
			g_roadVec.push_back(c);
		}
	}
	g_baseTime=g_roadVec[0].timePos;
	for(UINT i=0;i<g_roadVec.size();i++)
	{
		g_roadVec[i].timePos-=g_baseTime;
	}
	g_hasRoad=true;
	g_totalRoadTime=g_roadVec[g_roadVec.size()-1].timePos;
	ifs.close();
}

void GAPModel::updateRidingOnRoad(float dt)
{
	while(g_time>=g_roadVec[currentVecPos].timePos)
	{
		currentVecPos++;
		if(currentVecPos>=g_roadVec.size())
		{
			g_time=0;
			currentVecPos=0;
			if(!g_loopRoad) g_hasRoad=false;
			else
			{
				g_modelNode->setPosition(g_roadVec[0].x,g_roadVec[0].y,g_roadVec[0].z);
				return;
			}
		}
	}
	Ogre::Vector3 vec1=g_modelNode->_getDerivedPosition();
	Ogre::Vector3 vec2(g_roadVec[currentVecPos].x,g_roadVec[currentVecPos].y,g_roadVec[currentVecPos].z);
	float r=dt/(g_roadVec[currentVecPos].timePos-g_time);
	g_time+=dt;
	vec1=(vec2-vec1)*r;
	g_modelNode->translate(vec1,Ogre::Node::TS_WORLD);
}

void GAPModel::updateRidingOnRoadOnCertainTime(float time)
{
	g_time=time-g_baseTime;
	currentVecPos=1;
	while(g_time>g_roadVec[currentVecPos].timePos)
	{
		currentVecPos++;
		if(currentVecPos>=g_roadVec.size())
		{
			g_time=0;
			currentVecPos=0;
			if(!g_loopRoad) g_hasRoad=false;
			else
			{
				g_modelNode->setPosition(g_roadVec[0].x,g_roadVec[0].y,g_roadVec[0].z);
				return;
			}
		}
	}
	if(currentVecPos==0) return;
	Ogre::Vector3 vec1(g_roadVec[currentVecPos-1].x,g_roadVec[currentVecPos-1].y,g_roadVec[currentVecPos-1].z);
	Ogre::Vector3 vec2(g_roadVec[currentVecPos].x,g_roadVec[currentVecPos].y,g_roadVec[currentVecPos].z);
	float r=(g_time-g_roadVec[currentVecPos-1].timePos)/(g_roadVec[currentVecPos].timePos-g_roadVec[currentVecPos-1].timePos);
	vec1+=(vec2-vec1)*r;
	g_modelNode->setPosition(vec1);
}

void GAPModel::update(float dt)
{
	if(g_hasRoad) updateRidingOnRoad(dt);
}

void GAPModel::updateOnCertainTime(float time)
{
	if(g_hasRoad) updateRidingOnRoadOnCertainTime(time);
}
void GAPModel::resetRidingOnRoad()
{
	g_time=0;
	currentVecPos=0;
	if(!g_loopRoad) g_hasRoad=false;
	else
	{
		g_modelNode->setPosition(g_roadVec[0].x,g_roadVec[0].y,g_roadVec[0].z);
		return;
	}
}