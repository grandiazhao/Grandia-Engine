#include "GAPCameraController.h"
#include "GAPMainManager.h"

GAPCameraController::GAPCameraController(GAPMainManager* mainMgr)
{
	g_mainMgr=mainMgr;
	g_camera=g_mainMgr->getCurrentCamera();
	g_currentStyle=CameraStyle_Free;
	g_aroundStyleMaxPitch=30.f;///180.f*3.1415926f;
	g_aroundStyleMinPitch=-60.f;///180.f*3.1415926f;
	g_aroundStyleCurrentPitch=0.f;
	g_stayStyleCurrentPitch=g_stayStyleCurrentYaw=0.f;
	g_stayStyleMaxPitch=30.f;
	g_stayStyleMaxYaw=40.f;
	g_stayStyleMinPitch=-27.f;
	g_stayStyleMinYaw=-40.f;
	g_zAxisUp=mainMgr->g_zAxisUp;
	g_freeTranslateSpeed=100.f;
	
	g_autoYawSpeed=20.f;
	g_autoPitchSpeed=15.f;
	g_autoZoomSpeed=20.f;
	g_autoPitchTotalDegree=g_autoYawTotalDegree=g_autoZoomTotalLength=0.f;
	g_autoCurrentPitch=g_autoCurrentYaw=g_autoCurrentZoom=0.f;
	g_autoYaw=g_autoPitch=g_autoZoom=false;
	g_staringNodeStaying=false;
	g_maxRotateSpeed=10;

	g_lockCamera=false;
}
void GAPCameraController::init()
{
	g_keyboardInput=g_mainMgr->g_keyboardInput;
	g_translateDir=Ogre::Vector3(0,0,0);
	initCameraNode();
	initKeyMap();
}
void GAPCameraController::initKeyMap()
{
	b_kinto=b_kback=b_kup=b_kdown=b_kleft=b_kright=b_rleft=b_rright=b_rup=b_rdown=false;
	k_into=GAP_KEY_W;
	k_back=GAP_KEY_S;
	k_left=GAP_KEY_A;
	k_right=GAP_KEY_D;
	k_up=GAP_KEY_E;
	k_down=GAP_KEY_C;
	r_left=GAP_KEY_UNDEFINED;
	r_right=GAP_KEY_UNDEFINED;
	r_up=GAP_KEY_UNDEFINED;
	r_down=GAP_KEY_UNDEFINED;
}
void GAPCameraController::initCameraNode()
{
	Ogre::SceneManager* sm=g_mainMgr->getCurrentSceneMgr();
	g_freeNode=sm->getRootSceneNode()->createChildSceneNode();
	g_freeNode->attachObject(g_camera);
	g_camera->setPosition(0,0,0);
	g_camera->lookAt(0,0,-100);
	g_freeNode->setPosition(0,0,100);
	g_aroundNode=0;
	g_cameraGoal=0;
	g_cameraPivot=0;
	g_stayOnNode=0;
	g_staringRootNode=0;
	g_staringTarget=0;
	g_cameraNode=g_freeNode;

	g_lastStayOnNode=0;
	g_lastAroundNode=0;

	if(g_zAxisUp)
	{
		g_freeNode->pitch(Ogre::Degree(90));
		g_freeNode->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
	}
}

Ogre::Vector3 GAPCameraController::getCameraFacingDirection()
{
	return g_camera->getDerivedDirection();
}
void GAPCameraController::freeAllCameraNodes()
{
	if(g_cameraGoal) {g_cameraGoal->detachAllObjects();g_cameraGoal=0;}
	if(g_stayOnNode) {g_stayOnNode->detachAllObjects();g_stayOnNode=0;}
	g_freeNode->detachAllObjects();
}
void GAPCameraController::freeCameraNode()
{
	if(g_cameraNode!=0) g_cameraNode->detachAllObjects();
}
void GAPCameraController::setCameraStyle(GAPCameraController::CameraStyle style,Ogre::SceneNode* requiredNode)
{
	//if(g_currentStyle==style) return;
	g_currentStyle=style;
	if(g_currentStyle==CameraStyle_Static)
	{
		freeCameraNode();
	}
	else if(g_currentStyle==CameraStyle_Free)
	{
		freeCameraNode();
		g_freeNode->attachObject(g_camera);
		g_cameraNode=g_freeNode;
	}
	else if(g_currentStyle==CameraStyle_StayOnNode)
	{
		if(requiredNode==0) return;
		freeCameraNode();
		createStayOnCameraNode(requiredNode);
		g_stayOnNode->attachObject(g_camera);
		g_cameraNode=g_stayOnNode;
	}
	else if(g_currentStyle==CameraStyle_AroundNode)
	{
		if(requiredNode==0) return;
		freeCameraNode();
		createAroundCameraNode(requiredNode);
		g_cameraGoal->attachObject(g_camera);
		g_cameraNode=g_cameraGoal;
	}
	else if(g_currentStyle==CameraStyle_StaringMovable)
	{
		freeCameraNode();
		if(g_staringRootNode==0)
		{
			g_staringRootNode=g_mainMgr->getCurrentSceneMgr()->createSceneNode();
			g_staringRootNode->pitch(Ogre::Degree(90));
			g_staringRootNode->setPosition(100,-100,100);
			g_staringRootNode->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
		}
		g_staringRootNode->attachObject(g_camera);
		g_staringTarget=requiredNode;
		g_cameraNode=g_staringRootNode;
	}
}

void GAPCameraController::setStaringModeParams(Ogre::Vector3 pos,Ogre::SceneNode* target,float speed,bool staying)
{
	g_staringRootNode->setPosition(pos);
	g_staringTarget=target;
	g_maxRotateSpeed=speed;
	g_staringNodeStaying=staying;
}

void GAPCameraController::updateRotate(float dx,float dy,int mode,float ratio)
{
	if(g_lockCamera) return;
	if(mode==0)
	{
	}
	if(g_currentStyle==CameraStyle_Static)
	{
	}
	else if(g_currentStyle==CameraStyle_Free)
	{
		g_freeNode->pitch(Ogre::Degree(dy*ratio));
		g_freeNode->yaw(Ogre::Degree(dx*ratio),Ogre::Node::TS_PARENT);
	}
	else if(g_currentStyle==CameraStyle_StaringMovable)
	{
		if(g_staringNodeStaying) return;
		g_cameraNode->pitch(Ogre::Degree(dy*ratio));
		g_cameraNode->yaw(Ogre::Degree(dx*ratio),Ogre::Node::TS_PARENT);
	}
	else if(g_currentStyle==CameraStyle_StayOnNode)
	{
		Ogre::Radian deltaRadianY=Ogre::Degree(dy*ratio);
		Ogre::Radian deltaRadianX=Ogre::Degree(dx*ratio);
		g_stayStyleCurrentPitch+=deltaRadianY.valueDegrees();
		g_stayStyleCurrentYaw+=deltaRadianX.valueDegrees();
		if((g_stayStyleCurrentPitch>g_stayStyleMaxPitch||g_stayStyleCurrentPitch<g_stayStyleMinPitch)&&g_stayStyleMaxPitch>g_stayStyleMinPitch)
		{
			g_stayStyleCurrentPitch-=deltaRadianY.valueDegrees();
		}
		else
		{
			g_stayOnNode->pitch(deltaRadianY);
		}
		if((g_stayStyleCurrentYaw>g_stayStyleMaxYaw||g_stayStyleCurrentYaw<g_stayStyleMinYaw)&&g_stayStyleMaxYaw>g_stayStyleMinYaw)
		{
			g_stayStyleCurrentYaw-=deltaRadianX.valueDegrees();
		}
		else
		{
			g_stayOnNode->yaw(Ogre::Degree(dx*ratio),Ogre::Node::TS_WORLD);
		}
	}
	else if(g_currentStyle==CameraStyle_AroundNode)
	{
		Ogre::Radian deltaRadian=Ogre::Degree(dy*ratio);
		float valueDegree=deltaRadian.valueDegrees();
		g_aroundStyleCurrentPitch+=valueDegree;
		if(g_aroundStyleCurrentPitch>g_aroundStyleMaxPitch||g_aroundStyleCurrentPitch<g_aroundStyleMinPitch)
		{
			g_aroundStyleCurrentPitch-=valueDegree;
		}
		else
		{
			g_cameraPivot->pitch(deltaRadian);
		}
		g_cameraPivot->yaw(Ogre::Degree(dx*ratio),Ogre::Node::TS_WORLD);
	}
}
void GAPCameraController::updateMoving(float deltaTime,float ratio)
{
	if(g_translateDir.isZeroLength()) return;
	if(g_currentStyle==CameraStyle_Static)
	{
	}
	else if(g_currentStyle==CameraStyle_Free)
	{
		g_freeNode->translate(g_translateDir*g_freeTranslateSpeed*deltaTime*ratio,Ogre::Node::TS_LOCAL);
	}
	else if(g_currentStyle==CameraStyle_StayOnNode)
	{
		
	}
	else if(g_currentStyle==CameraStyle_AroundNode)
	{
		float pz=g_cameraGoal->getPosition().z;
		float delta;
		if(deltaTime>0.1) delta=pz*0.1;
		else delta=pz*deltaTime;
		Ogre::Vector3 vec(0,0,g_translateDir.z*delta*ratio);
		g_cameraGoal->translate(vec);
	}
	else if(g_currentStyle==CameraStyle_StaringMovable)
	{
		if(!g_staringNodeStaying) g_staringRootNode->translate(g_translateDir*g_freeTranslateSpeed*deltaTime*ratio,Ogre::Node::TS_LOCAL);
	}
}
void GAPCameraController::update(float deltaTime)
{
	if(g_lockCamera) return;
	if(g_autoYaw) autoYaw(deltaTime);
	if(g_autoPitch) autoPitch(deltaTime);
	updateMoving(deltaTime);
	lookAt(deltaTime);
}
bool GAPCameraController::injectKeyDown()
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

bool GAPCameraController::injectKeyUp()
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

void GAPCameraController::rotateCameraNode(float degree,int xyz)
{
	if(g_cameraNode==0) return;
	if(xyz==1) g_cameraNode->pitch(Ogre::Degree(degree),Ogre::Node::TS_LOCAL);
	else if(xyz==2) g_cameraNode->yaw(Ogre::Degree(degree),Ogre::Node::TS_WORLD);
	//else if(xyz==3) g_cameraNode->roll(Ogre::Degree(degree),Ogre::Node::TS_PARENT);
}

void GAPCameraController::setStayOnNodeMaxMinPitchYaw(float maxPitch,float minPitch,float maxYaw,float minYaw)
{
	g_stayOnNode->setOrientation(Ogre::Quaternion::IDENTITY);
	//if(g_zAxisUp) g_stayOnNode->pitch(Ogre::Degree(90));
	g_stayStyleMaxPitch=maxPitch;
	g_stayStyleMaxYaw=maxYaw;
	g_stayStyleMinPitch=minPitch;
	g_stayStyleMinYaw=minYaw;
	g_stayStyleCurrentPitch=g_stayStyleCurrentYaw=0;
}

void GAPCameraController::setAutoYaw(float totalDegree,float yawSpeed)
{
	if(totalDegree*yawSpeed<0) return;
	g_autoYaw=true;
	g_autoYawTotalDegree=totalDegree;
	g_autoCurrentYaw=0;
	if(yawSpeed!=0) g_autoYawSpeed=yawSpeed;
}

bool GAPCameraController::autoYaw(float deltaTime)
{
	float deltaYaw=deltaTime*g_autoYawSpeed;
	g_autoCurrentYaw+=deltaYaw;
	if(abs(g_autoCurrentYaw)>=abs(g_autoYawTotalDegree))
	{
		g_autoYaw=false;
		deltaYaw=(g_autoYawTotalDegree-g_autoCurrentYaw+deltaYaw);
		g_autoCurrentYaw=g_autoYawTotalDegree;
		updateRotate(deltaYaw,0,0,1.f);
		return true;
	}
	updateRotate(deltaYaw,0,0,1.f);
	return false;
}

void GAPCameraController::setAutoPitch(float totalDegree,float pitchSpeed)
{
	if(totalDegree*pitchSpeed<0) return;
	g_autoPitch=true;
	g_autoPitchTotalDegree=totalDegree;
	g_autoCurrentPitch=0;
	if(pitchSpeed!=0) g_autoPitchSpeed=pitchSpeed;
}

bool GAPCameraController::autoPitch(float deltaTime)
{
	float deltaPitch=deltaTime*g_autoPitchSpeed;
	g_autoCurrentPitch+=deltaPitch;
	if(abs(g_autoCurrentPitch)>=abs(g_autoPitchTotalDegree))
	{
		g_autoPitch=false;
		deltaPitch=(g_autoPitchTotalDegree-g_autoCurrentPitch+deltaPitch);
		g_autoCurrentPitch=g_autoPitchTotalDegree;
		updateRotate(0,deltaPitch);
		return true;
	}
	updateRotate(0,deltaPitch);
	return false;
}

bool GAPCameraController::createAroundCameraNode(Ogre::SceneNode* roundNode)
{
	if(g_lastAroundNode==roundNode) return false;
	g_lastAroundNode=roundNode;
	if(g_aroundNode!=0)
	{
		g_mainMgr->getCurrentSceneMgr()->destroySceneNode(g_aroundNode);
		g_aroundNode=0;
	}
	if(g_cameraPivot!=0)
	{
		g_mainMgr->getCurrentSceneMgr()->destroySceneNode(g_cameraPivot);
		g_cameraPivot=0;
	}
	if(g_cameraGoal!=0)
	{
		g_mainMgr->getCurrentSceneMgr()->destroySceneNode(g_cameraGoal);
		g_cameraGoal=0;
	}
	g_aroundNode=roundNode->createChildSceneNode();
	g_cameraPivot=g_aroundNode->createChildSceneNode();
	g_cameraGoal=g_cameraPivot->createChildSceneNode(Ogre::Vector3(0,0,100));
	if(g_zAxisUp)
	{
		g_cameraPivot->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
		g_cameraGoal->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
	}
	return true;
}

bool GAPCameraController::createStayOnCameraNode(Ogre::SceneNode* stayOnNode)
{
	if(g_lastStayOnNode==stayOnNode) return false;
	g_lastStayOnNode=stayOnNode;
	if(g_stayOnNode!=0)
	{
		g_mainMgr->getCurrentSceneMgr()->destroySceneNode(g_stayOnNode);
		g_stayOnNode=0;
	}
	g_stayOnNode=stayOnNode->createChildSceneNode();
	if(g_zAxisUp)
	{
		g_stayOnNode->setFixedYawAxis(true,Ogre::Vector3::UNIT_Z);
	}
	return true;
}

void GAPCameraController::lock()
{
	g_lockCamera=true;
}

void GAPCameraController::unlock()
{
	g_lockCamera=false;
}

bool GAPCameraController::lookAt(float deltaTime)
{
	if(g_currentStyle!=CameraStyle_StaringMovable||g_staringTarget==0) return false;
	Ogre::Vector3 curDir=Ogre::Vector3(0,0,0)-g_cameraNode->_getDerivedOrientation().zAxis();
	Ogre::Vector3 curPos=g_cameraNode->_getDerivedPosition();
	Ogre::Vector3 newDir=g_staringTarget->_getDerivedPosition()-curPos;
	Ogre::Quaternion qua=curDir.getRotationTo(newDir);
	Ogre::Degree degree;Ogre::Vector3 axis;
	qua.ToAngleAxis(degree,axis);
	if(g_maxRotateSpeed>0)
	{
		if(degree.valueDegrees()>g_maxRotateSpeed*deltaTime) degree=Ogre::Degree(g_maxRotateSpeed*deltaTime);
	}
	qua.FromAngleAxis(degree,axis);
	g_cameraNode->rotate(qua,Ogre::Node::TS_WORLD);
	//fix
	{
		Ogre::Vector3 yDir=g_cameraNode->_getDerivedOrientation().yAxis();
		Ogre::Vector3 zDir=g_cameraNode->_getDerivedOrientation().zAxis();
		Ogre::Vector3 dd1=zDir.crossProduct(Ogre::Vector3(0,0,1));
		Ogre::Vector3 requaired=dd1.crossProduct(zDir);
		Ogre::Quaternion yqua=yDir.getRotationTo(requaired);
		g_cameraNode->rotate(yqua,Ogre::Node::TS_WORLD);
	}
	return true;
}