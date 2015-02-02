#ifndef GRANDARTPROCESSOR_H
#define GRANDARTPROCESSOR_H

#include "ogre.h"
#include "ExampleApplication.h"
#include "GAFrameListener.h"
#include "GAPMainManager.h"
#include "GAPCameraController.h"
#include "CarFrameListener.h"

#include "ExampleLoadingBar.h"
#include "GALoadingUIBase.h"
#include "GASingleton.h"
#include "GADataManager.h"
#include "GAFileIO.h"

class GAExport GrandArtProcessor : public ExampleApplication
{
private:
	GAPMainManager* g_mainMgr;
	bool g_firstRun;
	std::string g_renderer;
	GASingleton* s;
protected:
	//GALoadingPanelBase* g_loadingPanel;
	Ogre::Camera* cam2;
	Ogre::SceneManager* sm2;
	

public:
	GrandArtProcessor()
	{
		s=GASingleton::getSingletonPtr();
		g_firstRun=true;
		s->g_renderer="";
		//g_loadingPanel=0;
	}

	void go()
    {
		while(!s->g_exit)
		{
			s->g_exit=true;
			createRoot();
			if(!g_firstRun)
			{
				mRoot->setRenderSystem(mRoot->getRenderSystemByName(s->g_renderer));
			}
			if (!setup()) return;
			mRoot->startRendering();
			std::string name=mRoot->getRenderSystem()->getName();
			if(name==s->g_renderer)
			{
				if(name=="Direct3D9 Rendering Subsystem") name="OpenGL Rendering Subsystem";
				else name="Direct3D9 Rendering Subsystem";
			}
			//mRoot->setRenderSystem(mRoot->getRenderSystemByName(s->g_renderer));
			//mRoot->saveConfig();
			
			//mRoot->setRenderSystem(mRoot->getRenderSystemByName(name));
			shutdown();
			if(!s->g_exit)
			{
				saveConfig(s->g_renderer,name);
				if(mRoot) OGRE_DELETE mRoot;
				/*if(g_loadingPanel)
				{
					OGRE_DELETE g_loadingPanel;
					g_loadingPanel=0;
				}*/
				g_firstRun=false;
			}
		}
        destroyScene();
    }
	
	void saveConfig(std::string renderer,std::string renderer2)
	{
		GAFileIO fileio;
		fileio.createNewFile("ogre.cfg");
		std::string line="Render System="+renderer;
		fileio.writeLine(line);
		line="";
		fileio.writeLine(line);
		line="["+renderer+"]";
		fileio.writeLine(line);
		Ogre::RenderSystem* rs=mRoot->getRenderSystemByName(renderer);
		Ogre::ConfigOptionMap& options =rs->getConfigOptions();
		for(Ogre::ConfigOptionMap::iterator it = options.begin(); it != options.end(); it++)
		{
			line=it->first+"="+it->second.currentValue;
			fileio.writeLine(line);
		}
		line="";
		fileio.writeLine(line);
		line="["+renderer2+"]";
		fileio.writeLine(line);
		rs=mRoot->getRenderSystemByName(renderer2);
		options =rs->getConfigOptions();
		for(Ogre::ConfigOptionMap::iterator it = options.begin(); it != options.end(); it++)
		{
			line=it->first+"="+it->second.currentValue;
			fileio.writeLine(line);
		}
		fileio.endOutput();
	}
	void createRoot()
	{
		String pluginsPath;
	#ifndef OGRE_STATIC_LIB
	#if OGRE_DEBUG_MODE
			pluginsPath = mResourcePath + "plugins_d.cfg";
	#else
			pluginsPath = mResourcePath + "plugins.cfg";
	#endif
	#endif
		
			mRoot = OGRE_NEW Root(pluginsPath, 
				mConfigPath + "ogre.cfg", mResourcePath + "Ogre.log");
	#ifdef OGRE_STATIC_LIB
			mStaticPluginLoader.load();
	#endif
	}

	bool setup()
    {
        setupResources();

		 bool carryOn = configure();
        if (!carryOn) 
			return false;

		createResourceListener();
		// Load resources
		loadResources();

        chooseSceneManager();
        createCamera();
        createViewports();
        // Set default mipmap level (NB some APIs ignore this)
        TextureManager::getSingleton().setDefaultNumMipmaps(5);
		// Create the scene
        createScene();

        createFrameListener();

        return true;

    }

	bool configure()
    {
		if(!mRoot->restoreConfig())
        {
			if(mRoot->showConfigDialog())
			{
				mWindow = mRoot->initialise(true,"GA Render Window");
				return true;
			}
			else
			{
				return false;
			}
        }
		else
		{
			mWindow = mRoot->initialise(true,"GA Render Window");
			return true;
		}
    }

	void loadResources()
    {
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Essential");
		GALoadingPanelBase g_loadingPanel;
		g_loadingPanel.createLoadingBar(mWindow);
		g_loadingPanel.createDummyScene();
		g_loadingPanel.createFadingLogo(0,0);
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		g_loadingPanel.destroyDummyScene();
		g_loadingPanel.finish();
    }


	void chooseSceneManager()
	{
		mSceneMgr = mRoot->createSceneManager("BspSceneManager");
	}
	
	void createCamera()
	{
		mCamera = mSceneMgr->createCamera("PlayerCam");
        mCamera->setPosition(Vector3(0,0,500));
        mCamera->lookAt(Vector3(0,0,-300));
        mCamera->setNearClipDistance(5);
	}

	void createViewports()
	{
		mWindow->removeAllViewports();
		Viewport* vp = mWindow->addViewport(mCamera);
        vp->setBackgroundColour(ColourValue(0,0,0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
	}
	void createScene()
	{
		mSceneMgr->setSkyBox(true,"Examples/MorningSkyBox",2500);
		mSceneMgr->getSkyBoxNode()->pitch(Ogre::Degree(90));
		g_mainMgr=new GAPMainManager(mSceneMgr,mCamera);
		g_mainMgr->mWindow=mWindow;
		g_mainMgr->g_zAxisUp=true;
		g_mainMgr->initManagers();
		g_mainMgr->g_dataMgr->loadDataTree("dataTree.txt");
	}
	void shutdown()
	{
		Ogre::WindowEventUtilities::removeWindowEventListener(mWindow,mFrameListener);
		shutdownInput();
	}
	void shutdownInput()
	{
		if (mFrameListener->mInputManager)
		{
			mFrameListener->mInputManager->destroyInputObject(mFrameListener->mKeyboard);
			mFrameListener->mInputManager->destroyInputObject(mFrameListener->mMouse);
			OIS::InputManager::destroyInputSystem(mFrameListener->mInputManager);
			mFrameListener->mInputManager = 0;
		}
	}
	void destroyScene()
	{
		g_mainMgr->g_dataMgr->saveDataTree("dataTree.txt");
	}
	void createFrameListener()
	{
		mFrameListener= new GAFrameListener(mWindow,mCamera,g_mainMgr);
        mFrameListener->showDebugOverlay(false);
        mRoot->addFrameListener(mFrameListener);
	}
};

#endif