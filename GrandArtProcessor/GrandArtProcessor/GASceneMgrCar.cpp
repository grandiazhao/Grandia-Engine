#include "GASceneMgrCar.h"
#include "GAPMainManager.h"
#include "GASceneCar1.h"
#include "GAFrameListener.h"

GASceneMgrCar::GASceneMgrCar(GAPMainManager* mainMgr):GASceneManager(mainMgr)
{

}
void GASceneMgrCar::createScene(Ogre::RenderWindow* mWindow,OIS::Mouse* Mouse)
{
	GASceneManager::createScene(mWindow,Mouse);
	GASceneBase* s=new GASceneCar1(g_mainMgr,"car");
	s->createScene();
	g_sceneVec.push_back(s);
	g_currentScene=s;
	g_previousSceneName="car";
}
void GASceneMgrCar::createUI(Ogre::RenderWindow* win,OIS::Mouse* mouse)
{

}
bool GASceneMgrCar::switchToScene(std::string sceneName,bool reCreate)
{
	//no need to switch
	return true;
}

//GASceneManager* GAFrameListener::GAMain(GAPMainManager* mainMgr)
//{
//	GASceneManager *m=new GASceneMgrCar(mainMgr);
//	return m;
//}