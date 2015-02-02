#include "GASceneMgrRecover.h"
#include "GAPMainManager.h"
#include "GASceneBase.h"
#include "GASceneKangFu0.h"
#include "GAExampleSettingUI.h"
#include "GAExampleEnterUI.h"
#include "GABlankScene.h"
#include "SceneMiGong.h"
#include "SceneChabei.h"
#include "GAExampleSceneUI.h"
#include "SceneYunDong.h"
#include "ScenePingHeng.h"
#include "SceneWuPin.h"
#include "GASceneWalk.h"
#include "GAFrameListener.h"
#include "GAPMisc.h"
#include "GAPAudioManager.h"
#include "GAServerSimple.h"

GASceneMgrRecover::GASceneMgrRecover(GAPMainManager* mainMgr):GASceneManager(mainMgr)
{
	g_previousSceneName="chabei";
	server=new GAServerSimple(mainMgr);
	GAFileIO file;
	file.openFile("serverport.txt");
	string str=file.getNextLine();
	int port=misc.ConvertToInt(str);
	file.endInput();
	server->initServer(port);
	stringMap=g_mainMgr->g_dataMgr->getRootNode()->getChildLeafNode("recvString")->getDataMap();
	output=new GAFileIO();
	output->createNewFile("InternetDataRecord.txt");
	g_startQuit=false;
}
GASceneMgrRecover::~GASceneMgrRecover()
{
	output->endOutput();
}
void GASceneMgrRecover::createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse)
{
	GASceneManager::createScene(mWindow,Mouse);
	switchToScene("walk",false);
	switchToScene("migong",false);
	switchToScene("yundong",false);
	switchToScene("pingheng",false);
	switchToScene("wupin",false);
	switchToScene("chabei",false);
	g_currentScene->processServerData("esc",0,0);
}

void GASceneMgrRecover::createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse)
{
	GAUIBase* b1=new GAExampleEnterUI(win,mouse,"EnterUI");
	GAUIBase* b2=new GAExampleSettingUI(win,mouse,"SettingUI");
	GAUIBase* b3=new GAExampleSceneUI(win,mouse,"SceneUI");
	b1->createUI();
	b2->createUI();
	b3->createUI();
	b1->hideAll();
	b2->hideAll();
	b3->hideAll();
	g_UIMgr->addNewUI(b1);
	g_UIMgr->addNewUI(b2);
	g_UIMgr->addNewUI(b3);
	g_UIMgr->setCurrentUI("EnterUI");
}

bool GASceneMgrRecover::switchToScene(std::string sceneName,bool reCreate)
{
	GASceneBase* s=findScene(sceneName);
	if(s==0)
	{
		if(sceneName=="migong")
		{
			s=new SceneMiGong(g_mainMgr,sceneName);
		}
		else if(sceneName=="chabei")
		{
			s=new SceneChabei(g_mainMgr,sceneName);
		}
		else if(sceneName=="yundong")
		{
			s=new SceneYunDong(g_mainMgr,sceneName);
		}
		else if(sceneName=="pingheng")
		{
			s=new ScenePingHeng(g_mainMgr,sceneName);
		}
		else if(sceneName=="wupin")
		{
			s=new SceneWuPin(g_mainMgr,sceneName);
		}
		else if(sceneName=="walk")
		{
			s=new GASceneWalk(g_mainMgr,sceneName);
		}
		else return false;
		if(g_currentScene!=0) g_currentScene->freezeScene();
		s->createScene();
		g_sceneVec.push_back(s);
		g_currentScene=s;
		if(s==0) return false;
	}
	else
	{
		g_currentScene->freezeScene();
		s->reloadScene();
		if(reCreate||s->isSceneEnded()) s->reCreateScene();
		g_currentScene=s;
	}
	g_previousSceneName=sceneName;
	return true;
}

void GASceneMgrRecover::frameStarted(float deltaTime)
{
	if(g_startQuit)
	{
		startQuit(deltaTime);
	}
	else
	{
		processServerData();
		GASceneManager::frameStarted(deltaTime);
	}
}
void GASceneMgrRecover::startQuit(float deltaTime)
{
	static float quitTime=3;
	static float delta=0;
	if(g_UIMgr->getCurrentUI()->g_name!="SceneUI")
	{
		Ogre::Root::getSingleton().queueEndRendering();
	}
	else
	{
		delta+=deltaTime;
		((GAExampleSceneUI*)g_UIMgr->getCurrentUI())->hintString=L"»¶Ó­¼ÌÐøÊ¹ÓÃ½»´óÐéÄâ¿µ¸´ÑµÁ·Æ½Ì¨";
		((GAExampleSceneUI*)g_UIMgr->getCurrentUI())->showHint();
		if(delta>quitTime)
		{
			Ogre::Root::getSingleton().queueEndRendering();
		}
	}
}
void GASceneMgrRecover::processServerData()
{
	UINT i=0;
	bool processed=true;
	std::string recvString=stringMap->getStringData("recv");
	if(recvString=="") return;
	stringMap->addData("recv","");
	//µ¥ÃüÁî
	UINT socketNum=0;
	std::string::size_type sst=recvString.find_first_of('-');
	if(sst==recvString.npos) return;
	output->writeLine(recvString);
	socketNum=misc.ConvertToInt(recvString.substr(0,sst));
	recvString=recvString.substr(sst+1);
	if(recvString=="quit") g_startQuit=true;
	else if(recvString=="soundLeft")
	{
		g_mainMgr->g_audioMgr->playShortSound("../sounds/tryLeft.wav");
	}
	else if(recvString=="soundRight")
	{
		g_mainMgr->g_audioMgr->playShortSound("../sounds/tryRight.wav");
	}
	else if(recvString=="jiayou")
	{
		g_mainMgr->g_audioMgr->playShortSound("../sounds/fight.wav");
	}
	else
	{
		//¸´ºÏÃüÁî
		std::string::size_type st;
		std::string str1,str2;
		st=recvString.find_first_of(":");
		if(st!=std::string::npos)
		{
			str1=recvString.substr(0,st);
			str2=recvString.substr(st+1);
			if(str1=="switchGame")
			{	
				if(str2=="1") str2="chabei";
				else if(str2=="2") str2="yundong";
				else if(str2=="3") str2="pingheng";
				else if(str2=="4") str2="wupin";
				else if(str2=="5") str2="migong";
				switchToScene(str2,true);
			}
			else if(str1=="switchGameByAni")
			{
				UINT t1=0;
				if(g_currentScene->getSceneName()=="chabei") t1=1;
				else if(g_currentScene->getSceneName()=="yundong") t1=2;
				else if(g_currentScene->getSceneName()=="pingheng") t1=3;
				else if(g_currentScene->getSceneName()=="wupin") t1=4;
				else if(g_currentScene->getSceneName()=="migong") t1=5;
				if(t1>0&&t1<6)
				{
					UINT t2=misc.ConvertToInt(str2);
					if(t1!=t2)
					{
						if(t2>0&&t2<6)
						{
							switchToScene("walk",true);
							((GASceneWalk*)g_currentScene)->setWalkTo(t1,t2);
						}
					}
					else
					{
						if(str2=="1") str2="chabei";
						else if(str2=="2") str2="yundong";
						else if(str2=="3") str2="pingheng";
						else if(str2=="4") str2="wupin";
						else if(str2=="5") str2="migong";
						switchToScene(str2,true);
					}
				}
			}
			else processed=false;
		}
		else processed=false;
	}
	if(processed==false) g_currentScene->processServerData(recvString,server,socketNum);
}

#include "GALogic.h"
void tryLogic()
{
	GALogic logic;
	logic.addLogic("l1","1:l1(0)#0:l1(1)",0);
	int i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
	i=logic.updateAndGetValue("l1");
}


GASceneManager* GAFrameListener::GAMain(GAPMainManager* mainMgr)
{
	tryLogic();
	GASceneManager *m=new GASceneMgrRecover(mainMgr);
	return m;
}