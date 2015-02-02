#include "GAVehicleAI.h"

GAVehicleAI::GAVehicleAI(GAVehicle* vehicle,Ogre::SceneManager* sgr) : GACharacterAIBase(vehicle)
{
	g_vehicle=vehicle;
	g_vehicleNode=vehicle->getCharacterSceneNode();
	g_vehicleNode->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
	g_raySceneQuery=sgr->createRayQuery(Ogre::Ray());
	g_raySceneQuery->setWorldFragmentType(Ogre::SceneQuery::WFT_SINGLE_INTERSECTION);
	g_heightFromGround=0.f;
	g_loopRidingOnRoad=true;
	g_speed=30;
	g_beizerSpeed=g_speed/4;
	g_beizerTotalTime=0;
	g_currentRoadCell=0;
	g_beizerTimeRatio=0;
	g_name=vehicle->getName();
	g_recorder=new GACharacterParamRecorder(10);
	g_roadRatio=0;
	g_roadLine=0;
	vTransSpeed=3;
	vTransDistance=0;
	g_classType=12345;
}

bool GAVehicleAI::update(float deltaTime)
{
	updateRidingOnRoad(deltaTime);
	g_recorder->update(deltaTime,g_vehicleNode->_getDerivedPosition(),g_vehicleNode->_getDerivedOrientation());
	return updateClampToGround();
}

void GAVehicleAI::setPassingTranslate(float deltaTime,Ogre::Vector3 point,Ogre::Vector3 direction)
{
	Ogre::Vector3 facing=g_vehicle->getFacingDirection();
	if(facing.dotProduct(direction)<0) return;
	Ogre::Vector3 tempLength=point-g_vehicleNode->getPosition();
	tempLength.z=0;
	if(tempLength.squaredLength()>40000)
	{
		vTransDistance=0;
	}
	else
	{
		Ogre::Vector3 pDir=facing.crossProduct(Ogre::Vector3(0,0,1));
		pDir.normalise();
		float cos=facing.dotProduct(tempLength);
		if(cos<0)
		{
			if(vTransDistance>0)
			{
				vTransDistance-=vTransSpeed*deltaTime;
				if(vTransDistance<0) vTransDistance=0;
			}
			else vTransDistance=0;
		}
		else
		{
			float dis=pDir.dotProduct(tempLength);
			if(dis>4)
			{
				if(vTransDistance>0)
				{
					vTransDistance-=vTransSpeed*deltaTime;
					if(vTransDistance<0) vTransDistance=0;
				}
				else vTransDistance=0;
			}
			else
			{
				if(vTransDistance<8)
				{
					vTransDistance+=vTransSpeed*deltaTime;
					if(vTransDistance>8) vTransDistance=8;
				}
				else vTransDistance=8;
			}
		}
		g_vehicleNode->translate(pDir*vTransDistance,Ogre::Node::TS_WORLD);
	}
}

bool GAVehicleAI::updateClampToGround()
{
	float dh1,dh2;
	if(!g_vehicle->getHeightFromNode(dh1,1)) return false;
	if(!g_vehicle->getHeightFromNode(dh2,2)) return false;

	dh1=g_heightFromGround-dh1;
	dh2=g_heightFromGround-dh2;
	float d1=dh1-dh2;
	float d2=dh1+dh2;
	d1/=2;d2/=2;
	g_vehicleNode->translate(0,0,d2+d1,Ogre::Node::TS_WORLD);
	float theta=atan((float)d1/g_vehicle->g_halfLength);
	g_vehicleNode->roll(Ogre::Radian(theta),Ogre::Node::TS_LOCAL);
	return true;
}

void GAVehicleAI::setRoadLine(GARoadLine* roadLine)
{
	g_roadLine=roadLine;
}


bool GAVehicleAI::updateRidingOnRoad(float deltaTime)
{
	if(g_roadLine==0) return false;
	if(g_currentRoadCell>=g_roadLine->getPathCellNum())
	{
		if(g_loopRidingOnRoad)
		{
			g_currentRoadCell=0;
			g_roadRatio=0;
		}
		else return false;
	}
	Ogre::Vector3 pos,dir;
	g_roadLine->getParam(g_currentRoadCell,g_roadRatio,pos,dir);
	if(g_roadLine->getPathCellType(g_currentRoadCell)==1)
	{
		g_roadRatio+=g_speed*deltaTime/g_roadLine->getPathCellLength(g_currentRoadCell);
		if(g_roadRatio>=1)
		{
			g_currentRoadCell++;
			g_roadRatio=0;
		}
	}
	else if(g_roadLine->getPathCellType(g_currentRoadCell)==2)
	{
		g_roadRatio+=g_beizerSpeed*deltaTime/g_roadLine->getPathCellLength(g_currentRoadCell);
		if(g_roadRatio>=1)
		{
			g_currentRoadCell++;
			g_roadRatio=0;
		}
	}
	g_vehicleNode->setPosition(pos.x,pos.y,g_vehicleNode->_getDerivedPosition().z);
	float degrees=g_vehicle->getTurnDegreesXY(dir);
	if(!Delta(degrees)) g_vehicleNode->yaw(Ogre::Degree(degrees),Ogre::Node::TS_WORLD);
	return true;
}

void GAVehicleAI::setBornPosition(UINT lineCell,float ratio,bool clamp)
{
	if(g_roadLine==0) return;
	g_currentRoadCell=lineCell;
	g_roadRatio=ratio;
	Ogre::Vector3 pos,dir;
	g_roadLine->getParam(lineCell,ratio,pos,dir);
	g_vehicleNode->setPosition(pos.x,pos.y,g_vehicleNode->_getDerivedPosition().z);
	float degrees=g_vehicle->getTurnDegreesXY(dir);
	if(!Delta(degrees)) g_vehicleNode->yaw(Ogre::Degree(degrees),Ogre::Node::TS_WORLD);
	if(clamp) updateClampToGround();
}

void GAVehicleAI::setVehicleSpeed(float beeline,float beizerline)
{
	g_speed=beeline;
	g_beizerSpeed=beizerline;
}

float GAVehicleAI::getRoadRatio()
{
	return g_roadRatio;
}
UINT GAVehicleAI::getRoadCell()
{
	return g_currentRoadCell;
}