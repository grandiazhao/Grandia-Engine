#ifndef GALOADINGPANELBASE_H
#define GALOADINGPANELBASE_H

#include "Common.h"
#include "SdkTrays.h"

class GALoadingPanelBase : public Ogre::ResourceGroupListener
{
public:
	Ogre::RenderWindow* mWindow;
	float mInitProportion;
	unsigned short mNumGroupsInit;
	unsigned short mNumGroupsLoad;
	float mProgressBarMaxSize;
	float mProgressBarScriptSize;
	float mProgressBarInc;
	OgreBites::ProgressBar* g_loadingBar;
	Ogre::Overlay* g_loadOverlay;
	Ogre::OverlayContainer* g_backGround;
	Ogre::HardwarePixelBufferSharedPtr mTexBuf;
	float fadeTime,fadeTotalTime,lastdt,fadeDeltaTime;
	unsigned char g_trans;
	UINT i1,i2;
	bool fade;
	float ratio;
	UINT count;
	
public:
	GALoadingPanelBase()
	{
		fadeTotalTime=3;
		lastdt=0;
		fade=false;
		ratio=0;
		count=0;
	}

	void createLoadingBar(Ogre::RenderWindow* window)
	{
		mWindow=window;

		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().createManual("thaw2","Essential",
			Ogre::TEX_TYPE_2D, 128, 128, 0, Ogre::PF_L8, Ogre::TU_DYNAMIC_WRITE_ONLY);

		mTexBuf = tex->getBuffer();  // save off the texture buffer

		mTexBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		memset(mTexBuf->getCurrentLock().data, 0xff, mTexBuf->getSizeInBytes());
		mTexBuf->unlock();

		g_loadingBar=new OgreBites::ProgressBar("LoadingBar","Loading",400,308);
		g_loadOverlay=Ogre::OverlayManager::getSingleton().create("LoadingOverlay");
		g_loadOverlay->setZOrder(100);
		g_backGround=(Ogre::OverlayContainer*)Ogre::OverlayManager::getSingleton().createOverlayElement("Panel","LoadingBackGroundPanel");
		g_backGround->setMaterialName("GALogo");
		g_loadOverlay->add2D(g_backGround);
		Ogre::OverlayElement *e=g_loadingBar->getOverlayElement();
		e->setVerticalAlignment(Ogre::GVA_BOTTOM);
		e->setLeft(-(e->getWidth() / 2));
		e->setTop(-(e->getHeight()+30));
		g_backGround->addChild(e);
		
		Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
		g_loadOverlay->show();
	}
	void createFadingLogo(float totalTime,float stayTime)
	{
		g_loadingBar->hide();
		lastdt=GetTickCount();
		float sleepTime=16;
		float lastedTime=0;
		float cTime=0;
		unsigned char trans=0x00;
		mTexBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		memset(mTexBuf->getCurrentLock().data,trans, mTexBuf->getSizeInBytes());
		mTexBuf->unlock();
		mWindow->update();
		float dt=GetTickCount()-lastdt;
		lastdt+=dt;
		while(lastedTime<totalTime)
		{
			lastedTime+=(dt/1000);
			if(lastedTime>totalTime) trans=0xff;
			else trans=0xff*(sqrtf(lastedTime/totalTime));
			mTexBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
			memset(mTexBuf->getCurrentLock().data,trans, mTexBuf->getSizeInBytes());
			mTexBuf->unlock();
			mWindow->update();

			dt=GetTickCount()-lastdt;
			lastdt+=dt;
			if(dt<16)
			{
				Sleep(16-dt);
				dt=16;
			}
		}
		Sleep(stayTime*1000);
		g_loadingBar->show();
		g_backGround->setMaterialName("BCITrays/BackGround1");
	}
	virtual void createDummyScene()
	{
		mWindow->removeAllViewports();
		Ogre::SceneManager* sm = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC, "DummyScene");
		Ogre::Camera* cam = sm->createCamera("DummyCamera");
		mWindow->addViewport(cam);
	}

	virtual void destroyDummyScene()
	{
		mWindow->removeAllViewports();
		Ogre::Root::getSingleton().destroySceneManager(Ogre::Root::getSingleton().getSceneManager("DummyScene"));
	}	
	virtual void start(RenderWindow* window, unsigned short numGroupsInit = 1, unsigned short numGroupsLoad = 1, Real initProportion = 0.70f)
	{

	}

	virtual void finish(void)
	{
		g_loadOverlay->hide();
		ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
	}

	void resourceGroupScriptingStarted(const String& groupName, size_t scriptCount)
	{
		if(scriptCount!=0)
		{
			count=scriptCount;
		}
		mWindow->update();
	}
	void scriptParseStarted(const String& scriptName, bool &skipThisScript)
	{
		g_loadingBar->setComment(scriptName);
		mWindow->update();
	}
	void scriptParseEnded(const String& scriptName, bool skipped)
	{
		if(count==0) ratio=0;
		else ratio+=1.f/count;
		g_loadingBar->setProgress(ratio);
		mWindow->update();
	}
	void resourceGroupScriptingEnded(const String& groupName)
	{
	}
	void resourceGroupLoadStarted(const String& groupName, size_t resourceCount)
	{
		if(resourceCount!=0)
		{
			count=resourceCount;
		}
		mWindow->update();
	}
	void resourceLoadStarted(const ResourcePtr& resource)
	{
		mWindow->update();
	}
	void resourceLoadEnded(void)
	{
		if(count==0) ratio+=0;
		else ratio+=0.5/count;
		g_loadingBar->setProgress(ratio);
		mWindow->update();
	}
	void worldGeometryStageStarted(const String& description)
	{
		mWindow->update();
	}
	void worldGeometryStageEnded(void)
	{
		mWindow->update();
	}
	void resourceGroupLoadEnded(const String& groupName)
	{
	}

	void LogoFade()
	{
		if(fade==false) return;
		//g_loadingBar->hide();
		if(lastdt==0)
		{
			lastdt=GetTickCount();
			fadeDeltaTime=0;
			fadeTotalTime=0;
			i1=i2=0;
			g_trans=0;
		}
		else
		{
			float dt=GetTickCount()-lastdt;
			lastdt=dt+lastdt;
			fadeDeltaTime+=dt;
			fadeDeltaTime/=1000;
			unsigned char d=0x00;
			i1=0;
			i2=g_trans;
			while(fadeDeltaTime>0.03)
			{
				d+=0x01;
				i1+=1;
				fadeDeltaTime-=0.03;
			}
			
			if((i1+i2)>255)
			{
				g_trans=0xff;
				fade=false;
			}
			else
			{
				g_trans+=d;
			}
			mTexBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
			memset(mTexBuf->getCurrentLock().data,g_trans, mTexBuf->getSizeInBytes());
			mTexBuf->unlock();
		}

	}
};

#endif