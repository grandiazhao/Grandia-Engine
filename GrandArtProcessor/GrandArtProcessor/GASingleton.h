#ifndef GASINGLETON_H
#define GASINGLETON_H

#include <string>
#include "Ogre.h"

class GASingleton
{
private:
	GASingleton()
	{
		g_exit=false;
	}
public:
	static GASingleton& getSingleton()
	{
		if(s==0) s=new GASingleton();
		return *s;
	}
	static GASingleton* getSingletonPtr()
	{
		if(s==0) s=new GASingleton();
		return s;
	}

	void reConfigure(const std::string& renderer, Ogre::NameValuePairList& options)
	{
		g_renderer=renderer;
		Ogre::RenderSystem* rs=Ogre::Root::getSingleton().getRenderSystemByName(renderer);
		for (Ogre::NameValuePairList::iterator it = options.begin(); it != options.end(); it++)
		{
			rs->setConfigOption(it->first, it->second);
		}
		g_exit=false;
		Ogre::Root::getSingleton().queueEndRendering();
	}
	bool g_exit;
	std::string g_renderer;
private:
	static GASingleton* s;
};
#endif