#include "GASceneHand.h"
#include "GAPCameraController.h"
#include "GACharacterAI\GACharacterAISimpleObject.h"
#include "GAExampleSceneUI.h"
#include "GAOgreUIManager.h"
#include "GAFileIO.h"
#include "GAPMisc.h"

GASceneHand::GASceneHand(GAPMainManager* mainMgr,std::string sceneName) : GASceneBase(mainMgr,sceneName)
{
	touchedBone=0;
	ctrlType=Controll_None;
	deltaValue=0;
}

bool GASceneHand::createScene()
{
	Ogre::SceneManager* sm=Ogre::Root::getSingleton().createSceneManager("BspSceneManager","hand");
	Ogre::Camera* cam=sm->createCamera("hand");
	cam->setPosition(0,0,100);
    cam->lookAt(0,0,-300);
    cam->setNearClipDistance(15);
	g_mainMgr->setNewSceneManager(sm,cam);
	GAPCameraController *newCamc=new GAPCameraController(g_mainMgr);
	newCamc->init();
	newCamc->setCameraStyle(GAPCameraController::CameraStyle_Free);
	g_mainMgr->g_cameraCtrl=newCamc;
	g_mainMgr->mWindow->removeAllViewports();
	Ogre::Viewport* vp=g_mainMgr->mWindow->addViewport(cam);
	cam->setAspectRatio((float)vp->getActualWidth()/(float)vp->getActualHeight());
	cam->setFOVy(Ogre::Degree(60));

	Ogre::Ray ray=cam->getCameraToViewportRay(0.5,0.5);
	GAUIBase* ui=g_mainMgr->g_UIMgr->getCurrentUI();
	g_worldName="house1_6.bsp";
	sm->setSkyBox(true,"Examples/MorningSkyBox",2500);
	sm->getSkyBoxNode()->pitch(Ogre::Degree(90));
	sm->setWorldGeometry(g_worldName);
	handEnt=g_mainMgr->getCurrentSceneMgr()->createEntity("woman_shuibei_get.mesh");
	Ogre::SceneNode* handNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode("handNode");
	handNode->attachObject(handEnt);
	handNode->scale(0.1,0.1,0.1);
	handNode->pitch(Ogre::Degree(90));
	GACharacterSimpleObject *s=new GACharacterSimpleObject(g_mainMgr->getCurrentSceneMgr(),handNode,handNode,handEnt);
	womanAI=new GACharacterAISimpleObject(s);
	womanAI->getCharacter()->setFacingDirection(5);
	stringMap=g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap();
	initBones();
	reCreateScene();
	g_sceneUI=(GAExampleSceneUI*)g_mainMgr->g_UIMgr->getCurrentUI();
	g_sceneUI->progressBar2Visible(false);
	ctrlType=Controll_None;

	for(UINT i=0;i<5;i++)
	{
		cubeToCollide[i]=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
		cubeToCollideEnt[i]=g_mainMgr->getCurrentSceneMgr()->createEntity("cube.mesh");
		cubeToCollideEnt[i]->setMaterialName("Examples/Solid/Pink");
		cubeToCollide[i]->attachObject(cubeToCollideEnt[i]);
		cubeToCollide[i]->scale(10.f/100,10.f/100,10.f/100);
		if(i==0) cubeToCollide[i]->setPosition(0,50,50);
		else if(i==1) cubeToCollide[i]->setPosition(0,50,65);
		else if(i==2) cubeToCollide[i]->setPosition(0,50,35);
		else if(i==3) cubeToCollide[i]->setPosition(15,50,65);
		else if(i==4) cubeToCollide[i]->setPosition(15,50,35);
		p_cubeToCollide[i]=new GASimpleCube();
		p_cubeToCollide[i]->createCube(5,5,5);
		Ogre::Vector3 pos2=cubeToCollide[i]->_getDerivedPosition();
		p_cubeToCollide[i]->origin.setPosition(pos2);
		p_cubeToCollide[i]->update();
	}
	
	dynamicEnt=g_mainMgr->getCurrentSceneMgr()->createEntity("dynamicSqure.mesh");
	dynamicEntNode=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
	dynamicEntNode->attachObject(dynamicEnt);
	dynamicEntNode->setPosition(0,100,100);
	return true;
}

void GASceneHand::createHand()
{
	rootBone=new GACubeBone(g_mainMgr,"rootBone",20,20,5);
	rootBone->translate(80,50,50);
	GACapsuleBone* finger11=rootBone->createChildCapsuleBone("finger1_1",5,2.5);
	GACapsuleBone* finger12=finger11->createChildCapsuleBone("finger1_2",8,2.5);
	GACapsuleBone* finger13=finger12->createChildCapsuleBone("finger1_3",5,2.5);
	finger11->setMaxMinPitch(150,0);finger11->setMaxMinYaw(0.01,-0.01);finger11->setMaxMinRoll(0,-90);
	finger12->setMaxMinPitch(90,0);finger12->setMaxMinYaw(0.01,-0.01);finger12->setMaxMinRoll(0.01,-0.01);
	finger13->setMaxMinPitch(90,0);finger13->setMaxMinYaw(0.01,-0.01);finger13->setMaxMinRoll(0.01,-0.01);

	GACapsuleBone* finger21=rootBone->createChildCapsuleBone("finger2_1",8,2.5);
	GACapsuleBone* finger22=finger21->createChildCapsuleBone("finger2_2",6,2.5);
	GACapsuleBone* finger23=finger22->createChildCapsuleBone("finger2_3",4,2.5);

	GACapsuleBone* finger31=rootBone->createChildCapsuleBone("finger3_1",9,2.5);
	GACapsuleBone* finger32=finger31->createChildCapsuleBone("finger3_2",7,2.5);
	GACapsuleBone* finger33=finger32->createChildCapsuleBone("finger3_3",5,2.5);

	GACapsuleBone* finger41=rootBone->createChildCapsuleBone("finger4_1",8,2.5);
	GACapsuleBone* finger42=finger41->createChildCapsuleBone("finger4_2",6.5,2.5);
	GACapsuleBone* finger43=finger42->createChildCapsuleBone("finger4_3",4,2.5);

	GACapsuleBone* finger51=rootBone->createChildCapsuleBone("finger5_1",6,2.5);
	GACapsuleBone* finger52=finger51->createChildCapsuleBone("finger5_2",5,2.5);
	GACapsuleBone* finger53=finger52->createChildCapsuleBone("finger5_3",4,2.5);
	finger11->translate(7.5,-7.5,0);
	finger11->setRoll(-50);
	finger21->translate(7.5,10,0);
	finger31->translate(2.5,10,0);
	finger41->translate(-2.5,10,0);
	finger51->translate(-7.5,10,0);
	rootBone->setMaterials("Examples/Solid/Red");
}

void GASceneHand::initBones()
{
	bone[0]=handEnt->getSkeleton()->getBone("Bip01 R Hand");
	bone[1]=handEnt->getSkeleton()->getBone("Bip01 R Finger0");
}

bool GASceneHand::reCreateScene()
{
	womanAI->runAnimation("open");
	womanAI->stopAnimation();
	womanAI->pauseAnimation();
	createHand();
	return true;
}

bool GASceneHand::keyReleased(const OIS::KeyEvent& evt)
{
	if(evt.key==OIS::KC_ESCAPE)
	{
		Ogre::Root::getSingleton().queueEndRendering();
	}
	if(evt.key==OIS::KC_D)
	{
		Ogre::SceneNode* node=g_mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode();
		Ogre::Quaternion q1=node->_getDerivedOrientation();
		Ogre::Vector3 vec=q1.zAxis();
	}
	if(evt.key==OIS::KC_F1)
	{
		g_mainMgr->g_cameraCtrl->lock();
	}
	if(evt.key==OIS::KC_F2)
	{
		g_mainMgr->g_cameraCtrl->unlock();
	}
	if(evt.key==OIS::KC_F3)
	{
		saveInitFile("");
	}
	if(evt.key==OIS::KC_F4)
	{
		rootBone->findChildBone("finger1_3",true)->loadPostureFromString("materialName:Examples/Solid/Black	position:7.5,-7.5,0	max_min:150,0,0.01,-0.01,0,-90	quaternion:0,0,-50");
	}
	if(evt.key==OIS::KC_Z)
	{
		ctrlType=Controll_Pitch;
	}
	if(evt.key==OIS::KC_X)
	{
		ctrlType=Controll_Yaw;
	}
	if(evt.key==OIS::KC_C)
	{
		ctrlType=Controll_Roll;
	}
	if(evt.key==OIS::KC_A)
	{
		ctrlType=Controll_Move_X;
	}
	if(evt.key==OIS::KC_S)
	{
		ctrlType=Controll_Move_Y;
	}
	if(evt.key==OIS::KC_D)
	{
		ctrlType=Controll_Move_Z;
	}
	if(evt.key==OIS::KC_1)
	{
		ctrlType=Controll_SetColor_Black;
	}
	if(evt.key==OIS::KC_2)
	{
		ctrlType=Controll_SetColor_White;
	}
	if(evt.key==OIS::KC_3)
	{
		ctrlType=Controll_SetColor_Blue;
	}
	if(evt.key==OIS::KC_4)
	{
		ctrlType=Controll_SetColor_Green;
	}
	if(evt.key==OIS::KC_5)
	{
		ctrlType=Controll_SetColor_Pink;
	}
	if(evt.key==OIS::KC_6)
	{
		ctrlType=Controll_SetColor_Purple;
	}
	if(evt.key==OIS::KC_7)
	{
		ctrlType=Controll_SetColor_Red;
	}
	if(evt.key==OIS::KC_8)
	{
		ctrlType=Controll_SetColor_Yellow;
	}
	if(evt.key==OIS::KC_N)
	{
		ctrlType=Controll_None;
	}
	return true;
}

bool GASceneHand::frameStarted(float deltaTime)
{
	womanAI->update(deltaTime);
	controllSelectBone();
	//updatePhysics();
	return true;
}

void GASceneHand::updatePhysics()
{
	GAVector3 trans=rootBone->p_cube->origin;
	rootBone->p_cube->origin.setPosition(rootBone->getPosition());
	rootBone->p_cube->update();
	GAVector3List list;
	for(UINT i=0;i<5;i++)
	{
		if(rootBone->p_cube->calculateCubeIntersectWithThisCube(p_cubeToCollide[i],&list))
		{
			trans=rootBone->p_cube->origin.minus(trans);
			for(UINT k=0;k<5;k++)
			{
				cubeToCollideEnt[k]->setMaterialName("Examples/Solid/Blue");
				cubeToCollide[k]->translate(trans.x,trans.y,trans.z,Ogre::Node::TS_WORLD);
				Ogre::Vector3 pos2=cubeToCollide[k]->_getDerivedPosition();
				p_cubeToCollide[k]->origin.setPosition(pos2);
				p_cubeToCollide[k]->update();
			}
			break;
		}
		else
		{
			cubeToCollideEnt[i]->setMaterialName("Examples/Solid/Yellow");
		}
	}
	
}

bool GASceneHand::mouseMoved(const OIS::MouseEvent &evt)
{
	float t=evt.state.Z.rel*-0.05f;
	if(t!=0)
	{
		deltaValue+=t*0.1;
	}
	return true;
}

bool GASceneHand::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if(id==OIS::MB_Right)
	{
		if(touchedBone!=0) touchedBone->setMaterials(lastMaterial);
		touchedBone=0;
		ctrlType=Controll_None;
	}
	else if(id==OIS::MB_Left)
	{
		Ogre::RaySceneQuery* rsq=g_mainMgr->getCurrentSceneMgr()->createRayQuery(Ogre::Ray());
		float dx=((float)evt.state.X.abs)/((float)g_mainMgr->mWindow->getWidth());
		float dy=((float)evt.state.Y.abs)/((float)g_mainMgr->mWindow->getHeight());
		Ogre::Ray mouseRay=g_mainMgr->getCurrentCamera()->getCameraToViewportRay(dx,dy);
		rsq->setRay(mouseRay);
		rsq->setSortByDistance(true);
		Ogre::RaySceneQueryResult& res=rsq->execute();
		Ogre::RaySceneQueryResult::iterator iter=res.begin();
		touchedObject=0;
		while(iter!=res.end())
		{
			if(iter->movable&&iter->distance!=0&&iter->movable->getName()!="PlayerCam")
			{
				touchedObject=iter->movable;
				break;
			}
			iter++;
		}
		if(touchedObject==0) return false;
		else
		{
			std::string str=touchedObject->getParentNode()->getName();
			std::string::size_type st=str.find_last_of("_");
			if(st!=str.npos) str=str.substr(0,st);
			if(touchedBone!=0) touchedBone->setMaterials(lastMaterial);

			if(rootBone->getName()==str) touchedBone=rootBone;
			else touchedBone=rootBone->findChildBone(str,true);
			if(touchedBone!=0)
			{
				lastMaterial=touchedBone->getMaterialName();
				touchedBone->setMaterials("Examples/transparent/pink");
			}
		}
	}
	
	return true;
}

bool GASceneHand::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{

	return true;
}


void GASceneHand::controllSelectBone()
{
	switch(ctrlType)
	{
	case Controll_Pitch:
		g_sceneUI->showTips("Pitch");
		if(touchedBone!=0) touchedBone->pitch(deltaValue);
		deltaValue=0;
		break;
	case Controll_Yaw:
		g_sceneUI->showTips("Yaw");
		if(touchedBone!=0) touchedBone->yaw(deltaValue);
		deltaValue=0;
		break;
	case Controll_Roll:
		g_sceneUI->showTips("Roll");
		if(touchedBone!=0) touchedBone->roll(deltaValue);
		deltaValue=0;
		break;
	case Controll_Move_X:
		g_sceneUI->showTips("Move_X");
		if(touchedBone!=0&&touchedBone->getName()=="rootBone") touchedBone->translate(deltaValue,0,0);
		deltaValue=0;
		break;
	case Controll_Move_Y:
		g_sceneUI->showTips("Move_Y");
		if(touchedBone!=0&&touchedBone->getName()=="rootBone") touchedBone->translate(0,deltaValue,0);
		deltaValue=0;
		break;
	case Controll_Move_Z:
		g_sceneUI->showTips("Move_Z");
		if(touchedBone!=0&&touchedBone->getName()=="rootBone") touchedBone->translate(0,0,deltaValue);
		deltaValue=0;
		break;
	case Controll_SetColor_Black:
		g_sceneUI->showTips("Color_Black");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Black");
		lastMaterial="Examples/Solid/Black";
		break;
	case Controll_SetColor_Blue:
		g_sceneUI->showTips("Color_Blue");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Blue");
		lastMaterial="Examples/Solid/Blue";
		break;
	case Controll_SetColor_Green:
		g_sceneUI->showTips("Color_Green");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Green");
		lastMaterial="Examples/Solid/Green";
		break;
	case Controll_SetColor_Pink:
		g_sceneUI->showTips("Color_Pink");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Pink");
		lastMaterial="Examples/Solid/Pink";
		break;
	case Controll_SetColor_Purple:
		g_sceneUI->showTips("Color_Purple");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Purple");
		lastMaterial="Examples/Solid/Purple";
		break;
	case Controll_SetColor_Red:
		g_sceneUI->showTips("Color_Red");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Red");
		lastMaterial="Examples/Solid/Red";
		break;
	case Controll_SetColor_White:
		g_sceneUI->showTips("Color_White");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/White");
		lastMaterial="Examples/Solid/White";
		break;
	case Controll_SetColor_Yellow:
		g_sceneUI->showTips("Color_Yellow");
		if(touchedBone!=0) touchedBone->setMaterials("Examples/Solid/Yellow");
		lastMaterial="Examples/Solid/Yellow";
		break;
	default:
		g_sceneUI->showTips("None");
		deltaValue=0;
		break;
	}
}

void GASceneHand::loadInitFile(std::string filePath)
{
	filePath="sceneHand.txt";
	GAFileIO fileIO;
	if(!fileIO.openFile(filePath)) return;
	std::string line=fileIO.getNextLine();
	while(line!="")
	{
		//...
		line=fileIO.getNextLine();
	}
}

void GASceneHand::saveInitFile(std::string filePath)
{
	filePath="sceneHand.txt";
	GAFileIO fileIO;
	if(!fileIO.createNewFile(filePath)) return;
	std::string line="";
	line=rootBone->saveBoneTree();
	fileIO.writeLine(line);
	fileIO.endOutput();
}




bool GASceneHand::freezeScene()
{
	return GASceneBase::freezeScene();
}

bool GASceneHand::reloadScene()
{
	return GASceneBase::reloadScene();
}

void GASceneHand::userFunction(std::string recvString)
{
	UINT i=0;
	std::string str1,str2;
	stringMap->addData("recv","");
	std::string::size_type st;
	st=recvString.find_first_of(":");
	if(st!=std::string::npos)
	{
		str1=recvString.substr(0,st);
		str2=recvString.substr(st+1);
	}
}















//----------------------------------------------GABone------------------------------------------------------------
GABone::GABone(GAPMainManager* mainMgr,std::string name,GABoneType type,GABone* parent)
{
	boneName=name;
	parentBone=parent;
	g_mainMgr=mainMgr;
	maxPitch=minPitch=currPitch=0;
	maxYaw=minYaw=currYaw=0;
	maxRoll=minRoll=currRoll=0;
	if(type==GABone_Root)
	{
		rootNode=mainMgr->getCurrentSceneMgr()->getRootSceneNode()->createChildSceneNode(name);
	}
	else if(type==GABone_Child)
	{
		rootNode=parentBone->getRootNode()->createChildSceneNode(name);
	}
	childBoneVec.clear();
	g_materialName="None";
	g_inheritType=GABone_None;
}

Ogre::SceneNode* GABone::getRootNode()
{
	return rootNode;
}

std::string GABone::getName()
{
	return boneName;
}

void GABone::setMaterials(std::string materialName)
{

}

std::string GABone::getMaterialName()
{
	return g_materialName;
}
void GABone::setPosition(float x,float y,float z)
{
	rootNode->setPosition(x,y,z);
}
void GABone::translate(float x,float y,float z)
{
	rootNode->translate(x,y,z);
}
void GABone::setPitch(float degree)
{
	if(maxPitch>minPitch&&(degree>maxPitch||degree<minPitch)) return;
	rootNode->pitch(Ogre::Degree(-currPitch));
	rootNode->pitch(Ogre::Degree(degree));
	currPitch=degree;
}
void GABone::pitch(float degree)
{
	if(maxPitch<=minPitch) rootNode->pitch(Ogre::Degree(degree));
	else
	{
		currPitch+=degree;
		if(currPitch>maxPitch||currPitch<minPitch)
		{
			currPitch-=degree;
		}
		else rootNode->pitch(Ogre::Degree(degree));
	}
}
void GABone::setYaw(float degree)
{
	if(maxYaw>minYaw&&(degree>maxYaw||degree<minYaw)) return;
	rootNode->yaw(Ogre::Degree(-currYaw));
	rootNode->yaw(Ogre::Degree(degree));
	currYaw=degree;
}
void GABone::yaw(float degree)
{
	if(maxYaw<=minYaw) rootNode->yaw(Ogre::Degree(degree));
	else
	{
		currYaw+=degree;
		if(currYaw>maxYaw||currYaw<minYaw)
		{
			currYaw-=degree;
		}
		else rootNode->yaw(Ogre::Degree(degree));
	}
}
void GABone::setRoll(float degree)
{
	if(maxRoll>minRoll&&(degree>maxRoll||degree<minRoll)) return;
	rootNode->roll(Ogre::Degree(-currRoll));
	rootNode->roll(Ogre::Degree(degree));
	currRoll=degree;
}
void GABone::roll(float degree)
{
	if(maxRoll<=minRoll) rootNode->roll(Ogre::Degree(degree));
	else
	{
		currRoll+=degree;
		if(currRoll>maxRoll||currRoll<minRoll)
		{
			currRoll-=degree;
		}
		else rootNode->roll(Ogre::Degree(degree));
	}
}

float GABone::getCurrentPitch()
{
	return currPitch;
}
float GABone::getCurrentYaw()
{
	return currYaw;
}
float GABone::getCurrentRoll()
{
	return currRoll;
}
GAFloat3Struct* GABone::getPosition()
{
	GAFloat3Struct *s=new GAFloat3Struct;
	Ogre::Vector3 vec=rootNode->getPosition();
	s->x=vec.x;
	s->y=vec.y;
	s->z=vec.z;
	return s;
}
void GABone::setMaxMinPitch(float max,float min)
{
	maxPitch=max;
	minPitch=min;
}

void GABone::setMaxMinYaw(float max,float min)
{
	maxYaw=max;
	minYaw=min;
}

void GABone::setMaxMinRoll(float max,float min)
{
	maxRoll=max;
	minRoll=min;
}

GABone* GABone::findChildBone(std::string name,bool deep)
{
	std::vector<GABone*>::iterator iter=childBoneVec.begin();
	std::vector<GABone*>::iterator endIter=childBoneVec.end();
	if(deep==false)
	{
		for(;iter!=endIter;iter++)
		{
			if((*iter)->getName()==name) return *iter;
		}
		return 0;
	}
	else
	{
		for(;iter!=endIter;iter++)
		{
			if((*iter)->getName()!=name)
			{
				GABone* b=(*iter)->findChildBone(name,true);
				if(b!=0) return b;
			}
			else return *iter;
		}
		return 0;
	}
}

std::string GABone::savePostureToString()
{
	return "";
}

void GABone::loadPostureFromString(std::string str)
{

}

bool GABone::loadBoneTreePosture(std::string filePath)
{
	filePath="sceneHand.txt";
	GAFileIO fileIO;
	if(!fileIO.openFile(filePath)) return false;
	std::string line=fileIO.getNextLine();
	std::string type,name,parent,dimension,others;
	std::string::size_type st;
	while(line!="")
	{
		st=line.find_first_of('\t');
		type=line.substr(0,st);
		line=line.substr(st+1);
		st=line.find_first_not_of('\t');
		name=line.substr(0,st);
		line=line.substr(st+1);
		st=line.find_first_not_of('\t');
		parent=line.substr(0,st);
		line=line.substr(st+1);
		st=line.find_first_not_of('\t');
		dimension=line.substr(0,st);
		line=line.substr(st+1);
		others=line;
		GABone* bone=findChildBone(parent,true);
		GABone* bone2=0;
		if(bone->g_inheritType==GABone_Cube)
		{
			//if()((GACubeBone*)bone)->createChildCapsuleBone(name,
		}
		line=fileIO.getNextLine();
	}
	return true;
}

std::string GABone::saveBoneTree()
{
	std::string str=savePostureToString()+"\r\n";
	for(UINT i=0;i<childBoneVec.size();i++)
	{
		GABone* bone=childBoneVec.at(i);
		if(bone->childBoneVec.empty()) str+=bone->savePostureToString()+"\r\n";
		else str+=bone->saveBoneTree();
	}
	return str;
}


//-------------------------------------------------GACubeBone------------------------------------------------------
GACubeBone::GACubeBone(GAPMainManager* mainMgr,std::string name,float length_x,float length_y,float length_z,GABoneType type,GABone* parent) : GABone(mainMgr,name,type,parent)
{
	cube=mainMgr->getCurrentSceneMgr()->createEntity("cube.mesh");
	cubeNode=rootNode->createChildSceneNode(name+"_cube");
	cubeNode->attachObject(cube);
	cubeNode->setScale(length_x/100,length_y/100,length_z/100);
	g_length_x=length_x;
	g_length_y=length_y;
	g_length_z=length_z;
	p_cube=new GASimpleCube();
	p_cube->createCube(g_length_x/2,g_length_y/2,g_length_z/2);
	Ogre::Vector3 vec=cubeNode->_getDerivedPosition();
	p_cube->origin.setPosition(vec);
	p_cube->update();
	g_inheritType=GABone_Cube;
}

void GACubeBone::setMaterials(std::string materialName)
{
	cube->setMaterialName(materialName);
	g_materialName=materialName;
}

GACapsuleBone* GACubeBone::createChildCapsuleBone(std::string name,float cylinderHeight,float radius)
{
	GACapsuleBone* bone=new GACapsuleBone(g_mainMgr,name,cylinderHeight,radius,GABone_Child,this);
	childBoneVec.push_back(bone);
	return bone;
}

GACubeBone* GACubeBone::createChildCubeBone(std::string name,float length_x,float length_y,float length_z)
{
	GACubeBone* bone=new GACubeBone(g_mainMgr,name,length_x,length_y,length_z,GABone_Child,this);
	childBoneVec.push_back(bone);
	return bone;
}

float GACubeBone::getLengthX()
{
	return g_length_x;
}

float GACubeBone::getLengthY()
{
	return g_length_y;
}

float GACubeBone::getLengthZ()
{
	return g_length_z;
}

void GACubeBone::resetCube(float x,float y,float z)
{
	if(x>0) g_length_x=x;
	if(y>0) g_length_y=y;
	if(z>0) g_length_z=z;
	cubeNode->setScale(g_length_x/100,g_length_y/100,g_length_z/100);
}

std::string GACubeBone::savePostureToString()
{
	std::string str="";
	GAPMisc misc;
	str+="CubeBone\t";
	str+="name:"+getName()+"\t";
	std::string temp="";
	if(parentBone==0) temp="None";
	else temp=parentBone->getName();
	str+="Parent:"+temp+"\t";
	str+="dimension:"+misc.ConvertToString(g_length_x)+","+misc.ConvertToString(g_length_y)+","+misc.ConvertToString(g_length_z)+"\t";
	str+="materialName:"+getMaterialName()+"\t";
	GAFloat3Struct* pos=getPosition();
	str+="position:"+misc.ConvertToString(pos->x)+","+misc.ConvertToString(pos->y)+","+misc.ConvertToString(pos->z)+"\t";
	delete pos;
	str+="max_min:"+misc.ConvertToString(maxPitch)+","+misc.ConvertToString(minPitch)+","+misc.ConvertToString(maxYaw)+","+misc.ConvertToString(minYaw)+","+misc.ConvertToString(maxRoll)+","+misc.ConvertToString(minRoll);
	str+="quaternion:"+misc.ConvertToString(currPitch)+","+misc.ConvertToString(currYaw)+","+misc.ConvertToString(currRoll)+"\t";
	return str;
}

void GACubeBone::loadPostureFromString(std::string str)
{
	//materialName:None	position:7.5,-7.5,0	quaternion:0,0,-50	max_min:150,0,0.01,-0.01,0,-90	
	std::string str1,str2;
	GAPMisc misc;
	bool canBreak=false;
	std::string::size_type st=str.find_first_of('\t');
	if(st==str.npos) canBreak=true;
	else
	{
		str1=str.substr(0,st);
		str=str.substr(st+1);
	}
	while(true)
	{
		st=str1.find_first_of(':');
		if(st==str1.npos) return;
		str2=str1.substr(0,st);
		str1=str1.substr(st+1);
		if(str2=="materialName")
		{
			if(str1!="None")
			{
				setMaterials(str1);
			}
		}
		else if(str2=="position")
		{
			float x,y,z;
			st=str1.find_first_of(',');
			x=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z=misc.ConvertToFloat(str1);
			setPosition(x,y,z);
		}
		else if(str2=="quaternion")
		{
			float x,y,z;
			st=str1.find_first_of(',');
			x=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z=misc.ConvertToFloat(str1);
			setPitch(x);
			setYaw(y);
			setRoll(z);
		}
		else if(str2=="max_min")
		{
			float x1,y1,z1,x2,y2,z2;
			st=str1.find_first_of(',');
			x1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			x2=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y2=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			z1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z2=misc.ConvertToFloat(str1);
			setMaxMinPitch(x1,x2);
			setMaxMinYaw(y1,y2);
			setMaxMinRoll(z1,z2);
		}
		else
		{
			if(canBreak) break;
			st=str.find_first_of('\t');
			if(st==str.npos) canBreak=true;
			else
			{
				str1=str.substr(0,st);
				str=str.substr(st+1);
			}
		}
	}
}

//------------------------------------------------GACapsuleBone----------------------------------------------------
GACapsuleBone::GACapsuleBone(GAPMainManager* mainMgr,std::string name,float cylinderHeight,float radius,GABoneType type,GABone* parent) : GABone(mainMgr,name,type,parent)
{
	cyl=mainMgr->getCurrentSceneMgr()->createEntity("cylinder_r10_h10.mesh");
	sp1=mainMgr->getCurrentSceneMgr()->createEntity("halfSphere_bottom.mesh");
	sp2=mainMgr->getCurrentSceneMgr()->createEntity("halfSphere_top.mesh");
	cylinder=rootNode->createChildSceneNode(name+"_cylinder");
	halfSphere1=rootNode->createChildSceneNode(name+"_halfSphere1");
	halfSphere2=rootNode->createChildSceneNode(name+"_halfSphere2");
	cylinder->attachObject(cyl);
	halfSphere1->attachObject(sp1);
	halfSphere2->attachObject(sp2);
	cylinder->scale(radius/10,cylinderHeight/10,radius/10);
	halfSphere1->scale(radius/10,radius/10,radius/10);
	halfSphere2->scale(radius/10,radius/10,radius/10);
	halfSphere1->translate(0,0,0);
	halfSphere2->translate(0,cylinderHeight,0);	
	g_cylinderHeight=cylinderHeight;
	g_radius=radius;
	g_inheritType=GABone_Capsule;
}

void GACapsuleBone::setMaterials(std::string materialName)
{
	cyl->setMaterialName(materialName);
	sp1->setMaterialName(materialName);
	sp2->setMaterialName(materialName);
	g_materialName=materialName;
}

GACapsuleBone* GACapsuleBone::createChildCapsuleBone(std::string name,float cylinderHeight,float radius)
{
	GACapsuleBone* bone=new GACapsuleBone(g_mainMgr,name,cylinderHeight,radius,GABone_Child,this);
	bone->setPosition(0,g_cylinderHeight,0);
	childBoneVec.push_back(bone);
	return bone;
}

GACubeBone* GACapsuleBone::createChildCubeBone(std::string name,float length_x,float length_y,float length_z)
{
	GACubeBone* bone=new GACubeBone(g_mainMgr,name,length_x,length_y,length_z,GABone_Child,this);
	childBoneVec.push_back(bone);
	return bone;
}

std::string GACapsuleBone::savePostureToString()
{
	std::string str="";
	GAPMisc misc;
	str+="CapsuleBone\t";
	str+="name:"+getName()+"\t";
	std::string temp="";
	if(parentBone==0) temp="None";
	else temp=parentBone->getName();
	str+="Parent:"+temp+"\t";
	str+="radius:"+misc.ConvertToString(g_radius)+"\t";
	str+="height:"+misc.ConvertToString(g_cylinderHeight)+"\t";
	str+="materialName:"+getMaterialName()+"\t";
	GAFloat3Struct* pos=getPosition();
	str+="position:"+misc.ConvertToString(pos->x)+","+misc.ConvertToString(pos->y)+","+misc.ConvertToString(pos->z)+"\t";
	delete pos;
	str+="max_min:"+misc.ConvertToString(maxPitch)+","+misc.ConvertToString(minPitch)+","+misc.ConvertToString(maxYaw)+","+misc.ConvertToString(minYaw)+","+misc.ConvertToString(maxRoll)+","+misc.ConvertToString(minRoll);
	str+="quaternion:"+misc.ConvertToString(currPitch)+","+misc.ConvertToString(currYaw)+","+misc.ConvertToString(currRoll)+"\t";
	return str;
}
void GACapsuleBone::loadPostureFromString(std::string str)
{
	//materialName:None	position:7.5,-7.5,0	quaternion:0,0,-50	max_min:150,0,0.01,-0.01,0,-90	
	std::string str1,str2;
	GAPMisc misc;
	bool canBreak=false;
	std::string::size_type st=str.find_first_of('\t');
	if(st==str.npos)
	{
		canBreak=true;
		str1=str;
	}
	else
	{
		str1=str.substr(0,st);
		str=str.substr(st+1);
	}
	while(true)
	{
		st=str1.find_first_of(':');
		if(st==str1.npos) return;
		str2=str1.substr(0,st);
		str1=str1.substr(st+1);
		if(str2=="materialName")
		{
			if(str1!="None")
			{
				setMaterials(str1);
			}
		}
		else if(str2=="position")
		{
			float x,y,z;
			st=str1.find_first_of(',');
			x=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z=misc.ConvertToFloat(str1);
			setPosition(x,y,z);
		}
		else if(str2=="quaternion")
		{
			float x,y,z;
			st=str1.find_first_of(',');
			x=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z=misc.ConvertToFloat(str1);
			setPitch(x);
			setYaw(y);
			setRoll(z);
		}
		else if(str2=="max_min")
		{
			float x1,y1,z1,x2,y2,z2;
			st=str1.find_first_of(',');
			x1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			x2=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			y2=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			st=str1.find_first_of(',');
			z1=misc.ConvertToFloat(str1.substr(0,st));
			str1=str1.substr(st+1);
			z2=misc.ConvertToFloat(str1);
			setMaxMinPitch(x1,x2);
			setMaxMinYaw(y1,y2);
			setMaxMinRoll(z1,z2);
		}
		
		if(canBreak) break;
		st=str.find_first_of('\t');
		if(st==str.npos)
		{
			canBreak=true;
			str1=str;
		}
		else
		{
			str1=str.substr(0,st);
			str=str.substr(st+1);
		}
	}
}