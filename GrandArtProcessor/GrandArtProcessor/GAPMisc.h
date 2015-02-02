#ifndef GAPMISC_H
#define GAPMISC_H

#include <string>
#include "Ogre.h"

class GAPMisc
{
public:
	GAPMisc(){}
	std::string WChar2Ansi(LPCWSTR pwszSrc)
	{
		int nLen=WideCharToMultiByte(CP_ACP,0,pwszSrc,-1,NULL,0,NULL,NULL);
		if(nLen<=0) return std::string("");
		char* pszDst=new char[nLen];
		if(NULL==pszDst) return std::string("");
		WideCharToMultiByte(CP_ACP,0,pwszSrc,-1,pszDst,nLen,NULL,NULL);
		pszDst[nLen-1]=0;
		std::string strTemp(pszDst);
		delete[] pszDst;
		return strTemp;
	}
	wchar_t* Ansi2WChar(const char* ansiString)
	{
		int nLen=MultiByteToWideChar(CP_ACP,0,ansiString, -1, NULL, 0);
		if(nLen<0)
		{
			return 0;
		}
		wchar_t* wc=new wchar_t[nLen];
		MultiByteToWideChar (CP_ACP,0,ansiString,-1,wc,nLen);  
		wc[nLen-1]=0;
		return wc;
	}
	bool setEntityScaleByWorldLengthXYZ(Ogre::Entity* ent,float x,float y,float z,float ratiox=1,float ratioy=1,float ratioz=1)
	{
		Ogre::Vector3 size=ent->getWorldBoundingBox(true).getSize();
		Ogre::SceneNode* entNode=ent->getParentSceneNode();
		float scalez=z/size.z*ratioz;
		float scalex=x/size.x*ratiox;
		float scaley=y/size.y*ratioy;
		entNode->setScale(scalex,scaley,scalez);
		return true;
	}

	template <class T> 
	std::string ConvertToString(T value)
	{
	  std::stringstream ss;
	  ss << value;
	  return ss.str();
	}

	int ConvertToInt(std::string str)
	{
		return atoi(str.c_str());
	}

	float ConvertToFloat(std::string str)
	{
		return (float)atof(str.c_str());
	}

	std::string stringTrip(std::string str)
	{
		while((str.find_first_of(" ")==0||str.find_first_of("\t")==0)&&str.length()!=0)
		{
			str=str.substr(1,str.length()-1);
		}
		while((str.find_last_of(" ")==str.length()-1||str.find_last_of("\t")==str.length()-1)&&str.length()!=0)
		{
			str=str.substr(0,str.length()-2);
		}
		return str;
	}

	bool checkTwoLineSegmentCross(float p1x,float p1y,float p2x,float p2y,float p3x,float p3y,float p4x,float p4y,float& ratio)
	{
		float bx=p4x-p3x;
		float by=p4y-p3y;
		float cx=p1x-p3x;
		float cy=p1y-p3y;

		float dx=p1y-p2y;
		float dy=p2x-p1x;
		float x=bx*dx+by*dy;
		if(x>-0.0001&&x<0.0001) return false;
		cy=cx*dx+cy*dy;
		cy=cy/x;
		if(cy>0&&cy<1)
		{
			ratio=cy;
			x=p3x+cy*bx;
			float y=(p1x-x)*(p2x-x);
			if(y<0) return true;
			else if(y>0) return false;
			else
			{
				y=p3y+cy*by;
				x=(p1y-y)*(p2y-y);
				if(x<0) return true;
				else return false;
			}
		}
		else return false;
	}

	void setFullScreen(Ogre::RenderWindow *win,bool fullScreen)
	{
		const Ogre::ConfigOptionMap::iterator opti =Ogre::Root::getSingleton().getRenderSystem()->getConfigOptions().find("Video Mode");
		Ogre::StringVector vmopts = Ogre::StringUtil::split(opti->second.currentValue, " x");
		unsigned int w = Ogre::StringConverter::parseUnsignedInt(vmopts[0]);
		unsigned int h = Ogre::StringConverter::parseUnsignedInt(vmopts[1]);
		win->setFullscreen(fullScreen, w, h);
	}
};
class GASettingValues;

class GASettingValues
{
private:
	static GASettingValues* ptr;
public:
	GASettingValues()
	{
		if(ptr==0) ptr=this;
		else OGRE_EXCEPT(Ogre::Exception::ERR_INVALID_STATE, "GASettingValues already exist.", "GASettingValues already exist");

		g_exit=true;
	}
	static GASettingValues* getSingletonPtr()
	{
		return ptr;
	}
public:
	bool g_exit;
};
#endif