#include "GAPCharacterController.h"
#include "GAPMainManager.h"

GAPCharacterController::GAPCharacterController(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	g_zAxisUp=mainMgr->g_zAxisUp;
	g_rotate1=g_rotate2=0;
	g_moveSpeed=50;
	g_translateDir=Ogre::Vector3(0,0,0);
	g_facingDir=Ogre::Vector3(0,0,0);
	g_objectNode=0;
	g_characterType=GAPCharacterType_Undefined;
	g_recorder=new GACharacterParamRecorder(5);
	g_acc=5;
	g_currentSpeed=0;
}

void GAPCharacterController::init()
{
	g_keyboardInput=g_mainMgr->g_keyboardInput;
	initKeyMap();
}

void GAPCharacterController::initKeyMap()
{
	k_into=GAP_KEY_I;
	k_back=GAP_KEY_K;
	k_left=GAP_KEY_J;
	k_right=GAP_KEY_L;
	k_up=GAP_KEY_UNDEFINED;
	k_down=GAP_KEY_UNDEFINED;
	r_up=GAP_KEY_UNDEFINED;
	r_down=GAP_KEY_UNDEFINED;
	r_left=GAP_KEY_UNDEFINED;
	r_right=GAP_KEY_UNDEFINED;
}

void GAPCharacterController::setCharacter(Ogre::SceneNode* node)
{
	g_objectNode=node;
	g_characterType=GAPCharacterType_SceneNode;
}

void GAPCharacterController::setCharacter(GAVehicle* vehicle)
{
	g_vehicle=vehicle;
	g_objectNode=g_vehicle->getCharacterSceneNode();
	g_characterType=GAPCharacterType_Vehicle;
}

bool GAPCharacterController::injectKeyDown()
{
	g_translateDir=Ogre::Vector3(0,0,0);
	b_kinto=g_keyboardInput->isKeyDown(k_into);
	b_kback=g_keyboardInput->isKeyDown(k_back);
	b_kleft=g_keyboardInput->isKeyDown(k_left);
	b_kright=g_keyboardInput->isKeyDown(k_right);
	b_kup=g_keyboardInput->isKeyDown(k_up);
	b_kdown=g_keyboardInput->isKeyDown(k_down);
	b_rleft=g_keyboardInput->isKeyDown(r_left);
	b_rright=g_keyboardInput->isKeyDown(r_right);
	b_rup=g_keyboardInput->isKeyDown(r_up);
	b_rdown=g_keyboardInput->isKeyDown(r_down);
	if(b_kinto)
	{
		g_translateDir.z-=1;
	}
	if(b_kback)
	{
		g_translateDir.z+=1;
	}
	if(b_kleft)
	{
		g_translateDir.x-=1;
	}
	if(b_kright)
	{
		g_translateDir.x+=1;
	}
	if(b_kup)
	{
		g_translateDir.y+=1;
	}
	if(b_kdown)
	{
		g_translateDir.y-=1;
	}
	if(g_translateDir.isZeroLength()) return false;
	else return true;
}
bool GAPCharacterController::injectKeyUp()
{
	//b_kinto=b_kback=b_kup=b_kdown=b_kleft=b_kright=b_rleft=b_rright=b_rup=b_rdown=false;
	return true;
}

bool GAPCharacterController::move(float deltaTime)
{
	switch(g_characterType)
	{
	case GAPCharacterType_SceneNode:
		return moveSceneNode(deltaTime);
	case GAPCharacterType_Vehicle:
		return moveVehicle(deltaTime);
	default:
		return false;
	}
	return true;
}

void GAPCharacterController::update(float deltaTime)
{
	if(g_characterType==GAPCharacterType_Undefined) return;
	move(deltaTime);
}

void GAPCharacterController::setKey(int keyNo,GAPKeyboardMap key)
{
	switch(keyNo)
	{
	case 1:
		k_into=key;
		break;
	case 2:
		k_back=key;
		break;
	case 3:
		k_left=key;
		break;
	case 4:
		k_right=key;
		break;
	case 5:
		k_up=key;
		break;
	case 6:
		k_down=key;
		break;
	case 7:
		r_left=key;
		break;
	case 8:
		r_right=key;
		break;
	case 9:
		r_up=key;
		break;
	case 10:
		r_down=key;
		break;
	default:
		break;
	}
}

bool GAPCharacterController::moveSceneNode(float deltaTime)
{
	if(!g_translateDir.isZeroLength())
	{
		Ogre::Vector3 facing=g_mainMgr->getCurrentCamera()->getDerivedDirection();
		facing.z=0;
		Ogre::Vector3 facingLeft=Ogre::Vector3(-facing.y,facing.x,0);
		Ogre::Vector3 trans=facing*(-g_translateDir.z)-facingLeft*g_translateDir.x;
		trans.normalise();
		trans=trans*200*deltaTime;
		g_objectNode->translate(trans,Ogre::Node::TS_WORLD);
		return true;
	}
	return false;
}

bool GAPCharacterController::moveVehicle(float deltaTime)
{
	if(!g_translateDir.isZeroLength())
	{
		if(g_translateDir.z!=0)
		{
			g_currentSpeed+=g_acc*deltaTime*(-g_translateDir.z);
			if(g_currentSpeed>g_moveSpeed) g_currentSpeed=g_moveSpeed;
			Ogre::Vector3 len=g_currentSpeed*deltaTime*g_vehicle->getFacingDirection();
			g_vehicle->getCharacterSceneNode()->translate(len,Ogre::Node::TS_WORLD);
		}
		if(g_translateDir.x!=0)
		{
			float turnDegree=-90*g_translateDir.x*deltaTime;
			g_vehicle->getCharacterSceneNode()->roll(Ogre::Degree(turnDegree),Ogre::Node::TS_WORLD);
		}
		if(g_translateDir.y!=0)
		{
			g_currentSpeed=0;
		}
	}

	float dh1,dh2;
	if(!g_vehicle->getHeightFromNode(dh1,1)) return false;
	if(!g_vehicle->getHeightFromNode(dh2,2)) return false;

	dh1=0-dh1;
	dh2=0-dh2;
	float d1=dh1-dh2;
	float d2=dh1+dh2;
	d1/=2;d2/=2;
	g_vehicle->getCharacterSceneNode()->translate(0,0,d2+d1,Ogre::Node::TS_WORLD);
	float theta=atan((float)d1/g_vehicle->g_halfLength);
	g_vehicle->getCharacterSceneNode()->roll(Ogre::Radian(theta),Ogre::Node::TS_LOCAL);
	g_recorder->update(deltaTime,g_vehicle->getCharacterSceneNode()->getPosition(),g_vehicle->getCharacterSceneNode()->getOrientation());
	return true;
}